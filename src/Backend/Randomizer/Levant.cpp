#include "Backend/Randomizer.hpp"

#include "Backend/File.hpp"
#include "Backend/Item.hpp"
#include "Backend/Mips.hpp"
#include "Backend/MipsFn.hpp"
#include "Backend/Resource.hpp"

#include <array>
#include <utility>

void Randomizer::levantBaseStats() const
{
	static constexpr u8 
		totalStats{ 106 }, 
		minStat{ 18 }, 
		iteration{ totalStats - minStat * 5 };

	// Attack - Defense - Magic Attack - Magic Defense - Speed
	std::array<u8, 5> levantStats;
	levantStats.fill(minStat);

	for (u8 i{}; i < iteration; ++i)
	{
		++levantStats[m_game->random()->generate(levantStats.size() - 1)];
	}

	m_game->file(File::OVER_TITLE_BIN)->write(m_game->offset().file.over_title_bin.levantStats, levantStats);
}

void Randomizer::levantAnimation(Randomizer::LevantAnimation_t state) const
{
	const auto over_game_bin{ m_game->file(File::OVER_GAME_BIN) };

	if (state & Randomizer::LEVANT_ANIMATION_BETA_USING_ITEM)
	{
		static constexpr s32 betaMagicAnimationId{ 0x19 };
		static constexpr std::array<Mips_t, 2> b_2A_nop{ 0x1000002A, 0x00000000 };

		over_game_bin->write(m_game->offset().file.over_game_bin.chooseMagicLevantAnim, b_2A_nop);
		over_game_bin->write(m_game->offset().file.over_game_bin.levantCastingItemId, betaMagicAnimationId);
	}

	if (state & Randomizer::LEVANT_ANIMATION_BETA_SUMMON)
	{
		const auto summon3{ m_game->file(File::EFFECT_SUMMON3_EFD) };
		summon3->write(0, Resource::bSummon1Efd);
		summon3->resize(Resource::bSummon1Efd.size());

		const auto summon4{ m_game->file(File::EFFECT_SUMMON4_EFD) };
		summon4->write(0, Resource::bSummon2Efd);
		summon4->resize(Resource::bSummon2Efd.size());

		const auto& game{ m_game->offset().game };
		const auto behaviorOffset{ m_game->customCodeOffset(sizeof(MipsFn::BetaSummonBehavior)) };

		const u16
			schedHi{ static_cast<u16>((game.effectMdecSchedule + 0x8000u) >> 16) },
			vlcHi{ static_cast<u16>((game.effectMdecVlcBuffer + 0x8000u) >> 16) },
			scratchHi{ static_cast<u16>((game.effectMdecScratchBuffer + 0x8000u) >> 16) };

		const MipsFn::BetaSummonBehavior behaviorFn
		{
			0x8663000C, // lh v1, 0xC(s3)
			0x8E620000, // lw v0, 0(s3)
			0x8E64001C, // lw a0, 0x1C(s3)
			0x14430015, // bne v0, v1, 0x15
			Mips::li(Mips::Register::a1, 32),

			// Spawn the SUMMON2 beams mesh, returns the animation duration
			0x00003021, // move a2, zero
			Mips::jal(game.spawnEffectComponentFn),
			0x00003821, // move a3, zero
			0x00408021, // move s0, v0

			// Effect sound channel 6 on
			Mips::li(Mips::Register::a0, 6),
			Mips::jal(game.playOstFn),
			Mips::li(Mips::Register::a1, 1),

			// Skip the video if the sec4 frames schedule is not built
			Mips::lui(Mips::Register::t0, schedHi),
			0x8D020000 | static_cast<u16>(game.effectMdecSchedule), // lw v0, LO(t0)
			0x8D030000 | static_cast<u16>(game.effectMdecSchedule + 4u), // lw v1, LO + 4(t0)
			0x10400009, // beqz v0, 0x9
			0x8D040000 | static_cast<u16>(game.effectMdecSchedule + 8u), // lw a0, LO + 8(t0)

			// Publish the decoder buffers, as the magics FSM does
			Mips::lui(Mips::Register::t1, vlcHi),
			0xAD230000 | static_cast<u16>(game.effectMdecVlcBuffer), // sw v1, LO(t1)
			Mips::lui(Mips::Register::t2, scratchHi),
			0xAD440000 | static_cast<u16>(game.effectMdecScratchBuffer), // sw a0, LO(t2)

			// Start the MDEC video player, duration = mesh animation duration
			0x06000003, // bltz s0, 0x3
			0x00000000, // nop
			Mips::jal(game.spawnEffectMdecFn),
			0x02002821, // move a1, s0

			// Hold the sound channel open during the beams (trigger + 84 frames)
			0x8663000C, // lh v1, 0xC(s3)
			0x8E620000, // lw v0, 0(s3)
			Mips::addiu(Mips::Register::v1, 84),
			0x0043102A, // slt v0, v0, v1
			0x10400003, // beqz v0, 0x3
			0x00000000, // nop
			Mips::j(game.effectBehaviorContinue),
			0x00000000, // nop

			// Original retail end path (sound channel off + task kill)
			Mips::j(game.effectBehaviorSummonEnd),
			0x00000000  // nop
		};

		auto executable{ m_game->executable() };
		executable.write(behaviorOffset.file, behaviorFn);
		executable.write(m_game->offset().file.executable.tableOfEffectBehaviorFns + 19u * sizeof(u32), behaviorOffset.game);
	}

	if (state & Randomizer::LEVANT_ANIMATION_RANDOM_WEAPONS_POSTURE)
	{
		std::array<u32, WEAPON_SKIN_COUNT> weaponAnimationsId;

		for (auto& id : weaponAnimationsId)
		{
			id = m_game->random()->generate(4u);
		}

		over_game_bin->write(m_game->offset().file.over_game_bin.tableOfWeaponAnimationsId, weaponAnimationsId);
	}
}

void Randomizer::levantFluteStyling() const
{
	std::array<u16, 10> stylings;
	auto* const stylingsPtr{ stylings.data() };

	auto executable{ m_game->executable() };

	const auto 
		offsetSummon{ m_game->offset().file.executable.tableOfLevantFluteStylingsId },
		offsetCapture{ static_cast<u32>(offsetSummon + sizeof(u16) * 6) };

	executable.read(offsetSummon, stylingsPtr, sizeof(u16) * 5);
	executable.read(offsetCapture, stylingsPtr + 5, sizeof(u16) * 5);

	m_game->random()->shuffle(&stylings);

	executable.write(offsetSummon, *stylingsPtr, sizeof(u16) * 5);
	executable.write(offsetCapture, *(stylingsPtr + 5), sizeof(u16) * 5);
}