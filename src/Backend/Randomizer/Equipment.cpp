#include "Backend/Randomizer.hpp"

#include "Backend/File.hpp"
#include "Backend/Item.hpp"
#include "Backend/MipsFn.hpp"
#include "Backend/Path.hpp"
#include "Backend/SkinZones.hpp"
#include "Backend/SkinZonesMask.hpp"
#include "Backend/TimPalette.hpp"
#include "Common/JcrException.hpp"

#include <array>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <span>
#include <type_traits>
#include <utility>
#include <vector>

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

	if (totalPattern == 0)
	{
		return;
	}

	for (s32 i{}; i < totalStats; ++i)
	{
		const auto rng{ random->generate(totalPattern - 1) };

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

void Randomizer::equipmentWeapons(Randomizer::EquipmentWeapons_t state) const
{
	auto executable{ m_game->executable() };

	const auto statsOffset{ m_game->offset().file.executable.tableOfWeaponStats };
	auto stats{ executable.read<std::array<EquipmentStats, WEAPON_COUNT>>(statsOffset) };

	if (state & Randomizer::EQUIPMENT_WEAPONS_RANDOM_STATS_AND_ELEMENT)
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

	if (state & Randomizer::EQUIPMENT_WEAPONS_RANDOM_APPEARANCE)
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

void Randomizer::equipmentArmors(Randomizer::EquipmentArmors_t state) const
{
	auto executable{ m_game->executable() };

	if (state & Randomizer::EQUIPMENT_ARMORS_RANDOM_STATS)
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

	if (state & Randomizer::EQUIPMENT_ARMORS_RANDOM_APPEARANCE)
	{
		std::array<u8, ARMOR_COUNT> armorsAppearanceId;

		for (auto& id : armorsAppearanceId)
		{
			id = m_game->random()->generate<u8>(1, 8);
		}

		executable.write(m_game->offset().file.executable.tableOfArmorsAppearanceId, armorsAppearanceId);
	}

	if (state & Randomizer::EQUIPMENT_ARMORS_RANDOM_COLORS)
	{
		struct LevantGroup
		{
			u32 clut;
			std::vector<SkinZones::MaskedTexture> textures;
		};

		auto scanGroups = [](const Buffer& bytes)
		{
			std::vector<LevantGroup> groups;

			for (std::size_t p{}; p + 12 <= bytes.size(); )
			{
				u32 size;
				s16 vx, vy;
				u16 w, h;
				std::memcpy(&size, &bytes[p], 4);
				std::memcpy(&vx, &bytes[p + 4], 2);
				std::memcpy(&vy, &bytes[p + 6], 2);
				std::memcpy(&w, &bytes[p + 8], 2);
				std::memcpy(&h, &bytes[p + 10], 2);

				if (w > 0 && h > 0 && vx >= 0 && vx < 1024 && vy >= 0 && vy < 512
					&& size == 12u + u32(w) * h * 2 && size < 0x20000u)
				{
					if (h == 1 && w == 256)
					{
						groups.push_back({ static_cast<u32>(p) + 12, {} });
					}
					else if (h > 2 && !groups.empty() && w * 2 >= 64) // skips the 12-wide shadow
					{
						groups.back().textures.push_back({ static_cast<u32>(p) + 12, static_cast<u16>(w * 2), h, nullptr });
					}

					p += size;
				}
				else
				{
					p += 2;
				}
			}

			return groups;
		};

		struct Recolor
		{
			bool blackAndWhite;
			TimPalette::BlackAndWhiteMethod method;
			s32 hue;
		};

		auto recolorGroup = [](RawFile* file, LevantGroup& group, Model_t model, u32 shape, const Recolor& recolor)
		{
			u32 bodySlot{};
			bool isPortrait{};

			for (auto& texture : group.textures)
			{
				u32 slot;

				if (texture.width == 64)
				{
					slot = 3;
					isPortrait = true;
				}
				else if (texture.width >= 128)
				{
					slot = bodySlot++;
				}
				else
				{
					continue;
				}

				const u8* mask{ SkinZonesMask::levantMask(model, shape, slot) };

				if (!mask && shape != 0)
				{
					mask = SkinZonesMask::levantMask(model, 0, slot);
				}

				texture.mask = mask;
			}

			auto result{ SkinZones::rearrangeTextures(file, group.clut, group.textures) };

			if (recolor.blackAndWhite)
			{
				const auto originalPalette{ result.palette };
				TimPalette::blackAndWhiteCLUT(result.palette, recolor.method, result.protectedSlots);

				if (isPortrait)
				{
					TimPalette::setBlackOpaque(result.palette, originalPalette);
				}
			}
			else
			{
				TimPalette::rotateCLUT(result.palette, recolor.hue, result.protectedSlots);
			}

			file->write(group.clut, result.palette);
		};

		const auto random{ m_game->random() };

		auto rollRecolor = [&](const auto& allowedMethods)
		{
			Recolor recolor{};
			static constexpr auto blackAndWhiteChance{ 12.5f };
			
			if (random->generateProba(blackAndWhiteChance))
			{
				recolor.blackAndWhite = true;
				recolor.method = allowedMethods[random->generate(allowedMethods.size() - 1)];
			}
			else
			{
				recolor.hue = random->generate(TimPalette::clutRotationLimit);
			}

			return recolor;
		};

		using Method = TimPalette::BlackAndWhiteMethod;

		{
			static constexpr std::array<Method, 4> lebantMethods
			{
				Method::Red, Method::Green, Method::Maximum, Method::Lightness
			};

			const auto file{ m_game->file(File::MODEL_LEBANT_MDL) };
			auto groups{ scanGroups(file->readFile()) };
			const auto recolor{ rollRecolor(lebantMethods) };

			for (std::size_t gi{}; gi < groups.size(); ++gi)
			{
				// Variant #13 (group 14) has a different skin layout.
				recolorGroup(file.get(), groups[gi], MODEL_LEBANT, gi == 14 ? 1u : 0u, recolor);
			}
		}

		{
			static constexpr Method redMaxLight[]{ Method::Red, Method::Maximum, Method::Lightness };
			static constexpr Method maxLight[]{ Method::Maximum, Method::Lightness };
			static constexpr Method redMaxAvgLight[]{ Method::Red, Method::Maximum, Method::Average, Method::Lightness };
			static constexpr Method redGreenMaxAvgLight[]{ Method::Red, Method::Green, Method::Maximum, Method::Average, Method::Lightness };

			static constexpr std::array<std::span<const Method>, 8> slotMethods
			{
				redMaxLight,          // variant 1 & 2
				redMaxLight,          // variant 3
				maxLight,             // variant 4
				redMaxLight,          // variant 5
				redGreenMaxAvgLight,  // variant 6
				redMaxAvgLight,       // variant 7
				redMaxAvgLight,       // variant 8
				redMaxLight           // variant 9
			};

			std::array<Recolor, slotMethods.size()> slotRecolor;

			for (std::size_t s{}; s < slotRecolor.size(); ++s)
			{
				slotRecolor[s] = rollRecolor(slotMethods[s]);
			}

			const auto file{ m_game->file(File::MODEL_LEB2_MDL) };
			auto groups{ scanGroups(file->readFile()) };

			for (std::size_t gi{}; gi < groups.size(); ++gi)
			{
				std::size_t slot{};
				u32 shape{};

				if (gi >= 2)
				{
					const u32 position{ (static_cast<u32>(gi) - 2) % 9 };
					slot = position == 0 ? 0 : position - 1;
					shape = (position == 6 || position == 8) ? 1u : 0u;
				}

				recolorGroup(file.get(), groups[gi], MODEL_LEB2, shape, slotRecolor[slot]);
			}
		}
	}
}

void Randomizer::equipmentOthers() const
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

void Randomizer::setDamageEffectFromWeaponIdFn(const Game& game, bool setAutumnMoonVisualAttackEffect)
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
		if (setAutumnMoonVisualAttackEffect && weaponsCharacteristic[i].critical == 90)
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