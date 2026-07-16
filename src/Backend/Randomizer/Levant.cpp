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

	if (state & Randomizer::LEVANT_ANIMATION_BETA_CAPTURE)
	{
		const auto seal2{ m_game->file(File::EFFECT_SEAL2_EFD) };
		seal2->write(0, Resource::bCaptureEfd);
		seal2->resize(Resource::bCaptureEfd.size());

		// The screen halo around the seal ball
		static constexpr std::array<u8, 3> paleGreenHalo{ 0x80, 0xE8, 0x80 };
		const auto battle{ m_game->file(File::OVER_BATTLE_BIN) };
		battle->write(m_game->offset().file.over_battle_bin.captureHaloColor, paleGreenHalo);

		// The slot 3 and 4 records only spawn if the capture flute animations
		// give them a timer window, copy the envelopment beat (slot 7 trigger)
		// into the slot 3 pair and open the slot 4 watcher window in the anim
		// records (anims 0x14 and 0x15). The battle timers come from the anim
		// table of the caster model in use, so both Levant battle models must
		// be patched (LEBANT.MDL and LEB2.MDL)
		static constexpr std::array<File, 2> levantModels{ File::MODEL_LEBANT_MDL, File::MODEL_LEB2_MDL };

		for (const auto model : levantModels)
		{
			const auto lebant{ m_game->file(model) };
			const auto animRecords{ lebant->read<u32>(4) + 8u };

			for (u32 i{}; i < 48u; ++i)
			{
				const auto record{ animRecords + i * 64u };
				const auto animId{ lebant->read<u16>(record) };

				if (animId != 0x14u && animId != 0x15u)
				{
					continue;
				}

				std::array<s16, 20> timers;
				lebant->read(record + 0xCu, &timers);

				const auto envelopmentTrigger{ timers[7 * 2] };
				const bool isCaptureShaped
				{
					envelopmentTrigger != -1 &&
					timers[3 * 2] == -1 && timers[3 * 2 + 1] == -1 &&
					timers[0] == timers[1 * 2] && timers[0] != -1
				};

				if (isCaptureShaped)
				{
					static constexpr std::array<s16, 2> slot4Pair{ 0, 0 };
					lebant->write(record + 0xCu + 4u * 4u, slot4Pair);
				}
			}
		}

		const auto& game{ m_game->offset().game };
		const auto blockOffset{ m_game->customCodeOffset(sizeof(MipsFn::BetaCaptureFailBurst)) };
		const auto flagVa{ blockOffset.game + 77u * static_cast<u32>(sizeof(Mips_t)) };
		const auto snapshotVa{ blockOffset.game + 78u * static_cast<u32>(sizeof(Mips_t)) };

		const u16
			flagHi{ static_cast<u16>((flagVa + 0x8000u) >> 16) },
			flagLo{ static_cast<u16>(flagVa) },
			outcomeHi{ static_cast<u16>((game.effectOutcomeFlag + 0x8000u) >> 16) },
			outcomeLo{ static_cast<u16>(game.effectOutcomeFlag) };

		const auto
			li32_snapshotT3{ Mips::li32(Mips::Register::t3, snapshotVa) },
			li32_snapshotA0{ Mips::li32(Mips::Register::a0, snapshotVa) };

		const u16
			optionsHi{ static_cast<u16>((game.gameOptions + 0x8000u) >> 16) },
			optionsLo{ static_cast<u16>(game.gameOptions) };

		const bool isNtscJ1{ m_game->isVersion(Version::NtscJ1) };
		const auto captureFile{ m_game->file(isNtscJ1 ? File::OVER_BTLEXEC_BIN : File::OVER_BATTLE_BIN) };
		const auto captureSuccessOffset{ isNtscJ1 ?
			m_game->offset().file.over_btlexec_bin.captureSuccess :
			m_game->offset().file.over_battle_bin.captureSuccess };

		const auto currentSuccessJal{ captureFile->read<Mips_t>(captureSuccessOffset) };
		const auto chainTarget{ 0x80000000u | ((currentSuccessJal & 0x03FFFFFFu) << 2) };
		const auto orbiterBranchVa{ game.captureOrbiterOutcomeBranch };
		const auto orbiterDeathVa{ orbiterBranchVa + (isNtscJ1 ? 0x360u : 0x364u) };

		const MipsFn::BetaCaptureFailBurst failBurstFn
		{
			0x8E620000, // lw v0, 0(s3)
			Mips::lui(Mips::Register::t1, flagHi),
			0x14400003, // bne v0, zero, 0x3
			Mips::lui(Mips::Register::t0, outcomeHi),
			static_cast<Mips_t>(0xA1200000u | flagLo), // sb zero, flagLo(t1)
			0x00000000, // nop
			0x284A0BB8, // slti t2, v0, 3000
			0x1140002D, // beqz t2, 0x2D
			static_cast<Mips_t>(0x85080000u | outcomeLo), // lh t0, outcomeLo(t0)
			0x00000000, // nop
			0x15000013, // bne t0, zero, 0x13
			0x00000000, // nop
			Mips::lui(Mips::Register::t2, optionsHi),
			static_cast<Mips_t>(0x8D4A0000u | optionsLo), // lw t2, optionsLo(t2)
			0x00000000, // nop
			0x314A0020, // andi t2, t2, 0x20
			0x15400024, // bne t2, zero, 0x24
			0x00000000, // nop
			0x8E6A001C, // lw t2, 0x1C(s3)
			li32_snapshotT3[0], // lui t3, 0xXXXX
			0x11400022, // beqz t2, 0x22
			li32_snapshotT3[1], // ori t3, 0xXXXX
			0x8D4C0000, // lw t4, 0(t2)
			0x8D4D0004, // lw t5, 4(t2)
			0x8D4E0008, // lw t6, 8(t2)
			0xAD6C0000, // sw t4, 0(t3)
			0xAD6D0004, // sw t5, 4(t3)
			0xAD6E0008, // sw t6, 8(t3)
			Mips::j(game.effectBehaviorContinue),
			0x00000000, // nop
			Mips::lui(Mips::Register::t1, flagHi),
			static_cast<Mips_t>(0x91290000u | flagLo), // lbu t1, flagLo(t1)
			0x00000000, // nop
			0x15200013, // bne t1, zero, 0x13
			0x00000000, // nop
			li32_snapshotA0[0], // lui a0, 0xXXXX
			li32_snapshotA0[1], // ori a0, 0xXXXX
			Mips::li(Mips::Register::a1, 23),
			0x00003021, // move a2, zero
			Mips::jal(game.spawnEffectComponentFn),
			0x00003821, // move a3, zero
			li32_snapshotA0[0], // lui a0, 0xXXXX
			li32_snapshotA0[1], // ori a0, 0xXXXX
			Mips::li(Mips::Register::a1, 23),
			0x00003021, // move a2, zero
			Mips::jal(game.spawnEffectComponentFn),
			Mips::li(Mips::Register::a3, 1365),
			li32_snapshotA0[0], // lui a0, 0xXXXX
			li32_snapshotA0[1], // ori a0, 0xXXXX
			Mips::li(Mips::Register::a1, 23),
			0x00003021, // move a2, zero
			Mips::jal(game.spawnEffectComponentFn),
			Mips::li(Mips::Register::a3, 2730),
			Mips::j(game.effectBehaviorKill),
			0x00000000, // nop
			Mips::j(game.effectBehaviorContinue),
			0x00000000, // nop
			Mips::li(Mips::Register::v0, 1),
			Mips::lui(Mips::Register::at, flagHi),
			static_cast<Mips_t>(0xA0220000u | flagLo), // sb v0, flagLo(at)
			Mips::j(chainTarget),
			0x00000000, // nop
			0x14400003, // bne v0, zero, 0x3
			0x00000000, // nop
			Mips::j(orbiterBranchVa + 8u),
			0x00000000, // nop
			Mips::lui(Mips::Register::v1, flagHi),
			static_cast<Mips_t>(0x90630000u | flagLo), // lbu v1, flagLo(v1)
			0x00000000, // nop
			0x10600003, // beqz v1, 0x3
			0x00000000, // nop
			Mips::j(orbiterBranchVa + 0x1B8u),
			0x00000000, // nop
			Mips::jal(game.playSfxFn),
			Mips::li(Mips::Register::a0, 63),
			Mips::j(orbiterDeathVa),
			0x00000000, // nop

			// Flag storage / anchor snapshot { x, y, z }
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000
		};

		auto executable{ m_game->executable() };
		executable.write(blockOffset.file, failBurstFn);
		executable.write(m_game->offset().file.executable.tableOfEffectBehaviorFns + 45u * sizeof(u32), blockOffset.game);
		captureFile->write(captureSuccessOffset, Mips::jal(blockOffset.game + 57u * static_cast<u32>(sizeof(Mips_t))));
		battle->write(m_game->offset().file.over_battle_bin.captureOrbiterOutcomeBranch,
			Mips::j(blockOffset.game + 62u * static_cast<u32>(sizeof(Mips_t))));

		if (isNtscJ1)
		{
			const auto orbTargetOffset{ m_game->customCodeOffset(sizeof(MipsFn::BetaCaptureOrbTarget)) };
			const auto targetVa{ orbTargetOffset.game + 23u * static_cast<u32>(sizeof(Mips_t)) };
			const auto li32_targetA0{ Mips::li32(Mips::Register::a0, targetVa) };
			const auto orbSpawnJalOffset{ m_game->offset().file.over_battle_bin.captureOrbiterOutcomeBranch + 0x1C8u };
			const auto orbSpawnJal{ battle->read<Mips_t>(orbSpawnJalOffset) };
			const auto orbSpawnTarget{ 0x80000000u | ((orbSpawnJal & 0x03FFFFFFu) << 2) };

			const MipsFn::BetaCaptureOrbTarget orbTargetFn
			{
				0x8E220098, // lw v0, 0x98(s1)
				0x8E230020, // lw v1, 0x20(s1)
				0x8E25001C, // lw a1, 0x1C(s1)
				0x00022040, // sll a0, v0, 1
				0x00822021, // addu a0, a0, v0
				0x00042080, // sll a0, a0, 2
				0x00832021, // addu a0, a0, v1
				0x8C880000, // lw t0, 0(a0)
				0x8C890004, // lw t1, 4(a0)
				0x8C8A0008, // lw t2, 8(a0)
				0x8CAB0000, // lw t3, 0(a1)
				0x8CAC0004, // lw t4, 4(a1)
				0x8CAD0008, // lw t5, 8(a1)
				0x010B4021, // addu t0, t0, t3
				0x012C4821, // addu t1, t1, t4
				0x014D5021, // addu t2, t2, t5
				li32_targetA0[0], // lui a0, 0xXXXX
				li32_targetA0[1], // ori a0, 0xXXXX
				0xAC880000, // sw t0, 0(a0)
				0xAC890004, // sw t1, 4(a0)
				0xAC8A0008, // sw t2, 8(a0)
				Mips::j(orbSpawnTarget),
				Mips::li(Mips::Register::a1, 37),

				// Target position { x, y, z }
				0x00000000,
				0x00000000,
				0x00000000
			};

			executable.write(orbTargetOffset.file, orbTargetFn);
			battle->write(orbSpawnJalOffset, Mips::jal(orbTargetOffset.game));
		}

		static constexpr std::array<u32, 4> flareGateShifts{ 0x0, 0xD8, 0x190, 0x294 };

		for (const auto shift : flareGateShifts)
		{
			const auto gate{ m_game->offset().file.executable.captureChargeFlareGates + shift };
			executable.write(gate, static_cast<Mips_t>(executable.read<Mips_t>(gate) & ~0x03E00000u));
		}
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