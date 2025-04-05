#include "Equipment.hpp"

#include "Backend/File.hpp"
#include "Backend/Item.hpp"
#include "Backend/MipsFn.hpp"
#include "Backend/Path.hpp"
#include "Common/JcrException.hpp"

#include <array>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <type_traits>
#include <utility>

struct EquipmentStatsPoint
{
	s8 attack, defense, magicAttack, magicDefense, speed;

	s32 totalStatsPoint() const
	{
		return std::abs(attack) + std::abs(defense) + std::abs(magicAttack) + std::abs(magicDefense) + std::abs(speed);
	}

	constexpr bool hasNegativePoint() const
	{
		return attack < 0 || defense < 0 || magicAttack < 0 || magicDefense < 0 || speed < 0;
	}

	constexpr auto& operator=(const EquipmentStats& stats)
	{
		attack = stats.attack;
		defense = stats.defense;
		magicAttack = stats.magicAttack;
		magicDefense = stats.magicDefense;
		speed = stats.speed;
		return *this;
	}
};

static void setEquipmentStatsByPattern(EquipmentStats* stats, const EquipmentStatsPoint& pattern, Random* random)
{
	const s32 totalStats
	{
		pattern.hasNegativePoint() ? 
		stats->totalStatsPoint() : 
		stats->totalStatsPoint() - stats->totalNegativePoint()
	};

	if (totalStats < 0)
	{
		throw JcrException{ "Equipment stats are less than 0" };
	}

	const s32
		step1{ std::abs(pattern.attack) },
		step2{ step1 + std::abs(pattern.defense) },
		step3{ step2 + std::abs(pattern.magicAttack) },
		step4{ step3 + std::abs(pattern.magicDefense) };

	const s32 totalPattern{ pattern.totalStatsPoint() };

	stats->attack = 0;
	stats->defense = 0;
	stats->magicAttack = 0;
	stats->magicDefense = 0;
	stats->speed = 0;

	for (s32 i{}; i < totalStats; ++i)
	{
		const auto rng{ random->generate(totalPattern) };

		if (rng < step1)
		{
			pattern.attack >= 0 ? ++stats->attack : --stats->attack;
		}
		else if (rng < step2)
		{
			pattern.defense >= 0 ? ++stats->defense : --stats->defense;
		}
		else if (rng < step3)
		{
			pattern.magicAttack >= 0 ? ++stats->magicAttack : --stats->magicAttack;
		}
		else if (rng < step4)
		{
			pattern.magicDefense >= 0 ? ++stats->magicDefense : --stats->magicDefense;
		}
		else
		{
			pattern.speed >= 0 ? ++stats->speed : --stats->speed;
		}
	}
}

Equipment::Equipment(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
	: m_game(std::move(game)), m_sharedData(std::move(sharedData))
{
}

void Equipment::setWeapons(Equipment::Weapons_t state) const
{
	auto executable{ m_game->executable() };

	const auto statsOffset{ m_game->offset().file.executable.tableOfWeaponStats };
	auto stats{ executable.read<std::array<EquipmentStats, WEAPON_COUNT>>(statsOffset) };

	if (state & Equipment::WEAPONS_RANDOM_STATS_AND_ELEMENT)
	{
		std::array<EquipmentStatsPoint, WEAPON_COUNT> patterns;

		for (Item_t i{}; i < WEAPON_COUNT; ++i)
		{
			patterns[i] = stats[i];
		}

		const auto characteristicOffset{ m_game->offset().file.executable.tableOfWeaponCharacteristic };
		const auto characteristicConst{ executable.read<std::array<WeaponCharacteristic, WEAPON_COUNT>>(characteristicOffset) };
		std::remove_const_t<decltype(characteristicConst)> characteristic;

		for (Item_t i{}; i < WEAPON_COUNT; ++i)
		{
			const auto rngItem{ m_game->random()->generate(patterns.size() - 1) };
			setEquipmentStatsByPattern(&stats[i], patterns[rngItem], m_game->random());

			characteristic[i] = characteristicConst[rngItem];

			if (characteristic[i].bonus == WEAPON_BONUS_DRAIN_MP || 
				characteristic[i].bonus == WEAPON_BONUS_CRITICAL_1HP)
			{
				continue;
			}

			switch (characteristic[i].element)
			{
			case WEAPON_ELEMENT_FIRE:
			case WEAPON_ELEMENT_AIR:
			case WEAPON_ELEMENT_EARTH:
			case WEAPON_ELEMENT_WATER:
				characteristic[i].element = m_game->random()->generate<WeaponElement_t>(WEAPON_ELEMENT_FIRE, WEAPON_ELEMENT_WATER);
				break;
			case WEAPON_ELEMENT_POISON:
			case WEAPON_ELEMENT_SLEEP:
			case WEAPON_ELEMENT_STONE:
				characteristic[i].element = m_game->random()->generate<WeaponElement_t>(WEAPON_ELEMENT_POISON, WEAPON_ELEMENT_STONE);
				break;
			}

			switch (characteristic[i].element)
			{
			case WEAPON_ELEMENT_POISON:
				characteristic[i].status = STATUS_EFFECT_POISON;
				break;
			case WEAPON_ELEMENT_SLEEP:
				characteristic[i].status = STATUS_EFFECT_SLEEP;
				break;
			case WEAPON_ELEMENT_STONE:
				characteristic[i].status = STATUS_EFFECT_STONE;
				break;
			}
		}

		executable.write(characteristicOffset, characteristic);
	}

	if (state & Equipment::WEAPONS_RANDOM_APPEARANCE)
	{
		static constexpr std::array<EquipmentIcon_t, WEAPON_SKIN_COUNT> skinsIcon
		{
			EQUIPMENT_ICON_DAGGER, EQUIPMENT_ICON_DAGGER, EQUIPMENT_ICON_DAGGER, EQUIPMENT_ICON_SWORD,
			EQUIPMENT_ICON_SWORD_2_HAND, EQUIPMENT_ICON_SWORD, EQUIPMENT_ICON_SWORD, EQUIPMENT_ICON_SWORD_2_HAND,
			EQUIPMENT_ICON_SWORD_2_HAND, EQUIPMENT_ICON_SWORD_2_HAND, EQUIPMENT_ICON_SWORD_2_HAND, EQUIPMENT_ICON_AXE,
			EQUIPMENT_ICON_AXE, EQUIPMENT_ICON_AXE, EQUIPMENT_ICON_SPEAR, EQUIPMENT_ICON_SPEAR,
			EQUIPMENT_ICON_SPEAR, EQUIPMENT_ICON_SPEAR, EQUIPMENT_ICON_KATANA
		};

		for (auto& id : stats)
		{
			id.skin = m_game->random()->generate(WeaponSkin_t(WEAPON_SKIN_COUNT - 1));
			id.icon = skinsIcon[id.skin];
		}
	}

	executable.write(statsOffset, stats);
	setDamageEffectFromWeaponIdFn(*m_game, false);
}

void Equipment::setArmors(Equipment::Armors_t state) const
{
	auto executable{ m_game->executable() };

	if (state & Equipment::ARMORS_RANDOM_STATS)
	{
		const auto statsOffset{ m_game->offset().file.executable.tableOfArmorStats };
		auto stats{ executable.read<std::array<EquipmentStats, ARMOR_COUNT>>(statsOffset) };

		std::array<EquipmentStatsPoint, ARMOR_COUNT> patterns;

		for (Item_t i{}; i < ARMOR_COUNT; ++i)
		{
			patterns[i] = stats[i];
		}

		for (auto& armor : stats)
		{
			setEquipmentStatsByPattern(&armor, patterns[m_game->random()->generate(patterns.size() - 1)], m_game->random());
		}

		executable.write(statsOffset, stats);
	}

	if (state & Equipment::ARMORS_RANDOM_APPEARANCE)
	{
		std::array<u8, ARMOR_COUNT> armorsAppearanceId;

		for (auto& id : armorsAppearanceId)
		{
			id = m_game->random()->generate<u8>(1, 8);
		}

		executable.write(m_game->offset().file.executable.tableOfArmorsAppearanceId, armorsAppearanceId);
	}
}

void Equipment::setOthers() const
{
	auto executable{ m_game->executable() };

	const auto statsOffset{ m_game->offset().file.executable.tableOfOtherStats };
	auto stats{ executable.read<std::array<EquipmentStats, OTHER_COUNT>>(statsOffset) };

	std::array<EquipmentStatsPoint, OTHER_COUNT> patterns;

	for (Item_t i{}; i < OTHER_COUNT; ++i)
	{
		patterns[i] = stats[i];
	}

	for (auto& other : stats)
	{
		setEquipmentStatsByPattern(&other, patterns[m_game->random()->generate(patterns.size() - 1)], m_game->random());
	}

	executable.write(statsOffset, stats);
}

void Equipment::setDamageEffectFromWeaponIdFn(const Game& game, bool setAutumnMoonEffect)
{
	const auto over_battle_bin{ game.file(File::OVER_BATTLE_BIN) };

	using SetDamageEffectFromWeaponId = std::array<Mips_t, 33>;
	auto function
	{ 
		over_battle_bin->read<SetDamageEffectFromWeaponId>
		(game.offset().file.over_battle_bin.setBattleWeaponsEffectFn)
	};

	const std::array<Mips_t, 2> laPtrTable{ function[5], function[6] };

	for (s32 i{ 1 }; i < 28; ++i)
	{
		function[i] = Mips_t(0);
	}
	
	// a0 = weapon id
	function[1] = 0xAFBF0010; // sw ra, 0x10(sp)
	function[5] = laPtrTable[0]; // lui v0, 0xXXXX
	function[6] = laPtrTable[1]; // addiu v0, 0xXXXX
	function[7] = 0x00441021; // addu v0, a0
	function[8] = 0x90440000; // lbu a0, 0(v0)

	over_battle_bin->write(game.offset().file.over_battle_bin.setBattleWeaponsEffectFn, function);

	using TablePtr = std::array<u32, WEAPON_COUNT - 2>; // -2 No Dagger and Knife
	over_battle_bin->write(game.offset().file.over_battle_bin.tableOfBattleWeaponsEffectPtr, TablePtr{});

	auto executable{ game.executable() };

	const auto weaponsCharacteristic
	{ 
		executable.read<std::array<WeaponCharacteristic, WEAPON_COUNT>>
		(game.offset().file.executable.tableOfWeaponCharacteristic)
	};

	std::array<WeaponAnimation_t, WEAPON_COUNT> animationsId;

	for (Item_t i{}; i < WEAPON_COUNT; ++i)
	{
		if (setAutumnMoonEffect && weaponsCharacteristic[i].critical == 90)
		{
			animationsId[i] = WEAPON_ANIMATION_CRITICAL;
		}
		else if (weaponsCharacteristic[i].bonus == WEAPON_BONUS_DAMAGE_DESTROY_MP ||
			weaponsCharacteristic[i].bonus == WEAPON_BONUS_DRAIN_MP ||
			weaponsCharacteristic[i].element == WEAPON_ELEMENT_CRITICAL)
		{
			animationsId[i] = 0xB;
		}
		else
		{
			animationsId[i] = Item::weaponAnimationByElement(weaponsCharacteristic[i].element);
		}
	}

	over_battle_bin->write(game.offset().file.over_battle_bin.tableOfBattleWeaponsEffectPtr, animationsId);

	static constexpr std::array<File, 17> effectFiles
	{
		File::EFFECT_PL_AIRON_EFD, File::EFFECT_PL_AMETA_EFD,
		File::EFFECT_PL_AORGU_EFD, File::EFFECT_PL_KAGU_EFD,
		File::EFFECT_PL_KDOKU_EFD, File::EFFECT_PL_KFAL_EFD,
		File::EFFECT_PL_KGATO_EFD, File::EFFECT_PL_KKATA_EFD,
		File::EFFECT_PL_KPRE_EFD, File::EFFECT_PL_KSOMA_EFD,
		File::EFFECT_PL_KTETU_EFD, File::EFFECT_PL_KUREI_EFD,
		File::EFFECT_PL_SABU_EFD, File::EFFECT_PL_SCOP_EFD,
		File::EFFECT_PL_SDOHG_EFD, File::EFFECT_PL_SHAKU_EFD,
		File::EFFECT_PL_SHONE_EFD
	};

	auto* const aquazorEffect{ game.filePathByIndex(File::EFFECT_PL_SFUBU_EFD) };

	for (const auto& file : effectFiles)
	{
		static constexpr auto offsetPosition{ 0x00000028u };
		s16 position;

		{
			game.file(file)->read(offsetPosition, &position);
		}
		
		game.builderTree().copyFile(aquazorEffect, game.filePathByIndex(file));
		game.file(file)->write(offsetPosition, position);
	}
}