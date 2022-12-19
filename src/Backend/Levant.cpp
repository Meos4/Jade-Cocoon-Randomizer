#include "Levant.hpp"

#include "Backend/File.hpp"
#include "Backend/Item.hpp"
#include "Backend/Mips.hpp"
#include "Backend/Random.hpp"

#include <array>
#include <utility>

Levant::Levant(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
	: m_game(std::move(game)), m_sharedData(std::move(sharedData))
{
}

void Levant::setBaseStats() const
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
		++levantStats[Random::get().generate(levantStats.size() - 1)];
	}

	m_game->file(File::OVER_TITLE_BIN)->write(m_game->offset().file.over_title_bin.levantStats, levantStats);
}

void Levant::setAnimation(Levant::Animation_t state) const
{
	const auto over_game_bin{ m_game->file(File::OVER_GAME_BIN) };

	if (state & Levant::ANIMATION_BETA_USING_ITEM)
	{
		static constexpr s32 betaMagicAnimationId{ 0x19 };
		static constexpr std::array<Mips_t, 2> b_2A_nop{ 0x1000002A, 0x00000000 };

		over_game_bin->write(m_game->offset().file.over_game_bin.chooseMagicLevantAnim, b_2A_nop);
		over_game_bin->write(m_game->offset().file.over_game_bin.levantCastingItemId, betaMagicAnimationId);
	}

	if (state & Levant::ANIMATION_RANDOM_WEAPONS_POSTURE)
	{
		std::array<u32, WEAPON_SKIN_COUNT> weaponAnimationsId;

		for (auto& id : weaponAnimationsId)
		{
			id = Random::get().generate(4u);
		}

		over_game_bin->write(m_game->offset().file.over_game_bin.tableOfWeaponAnimationsId, weaponAnimationsId);
	}
}