#include "Backend/Randomizer.hpp"

#include "Backend/File.hpp"
#include "Backend/Item.hpp"
#include "Backend/Mips.hpp"

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