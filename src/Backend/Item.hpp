#pragma once

#include "Backend/Behavior.hpp"
#include "Backend/Entity.hpp"
#include "Common/JcrException.hpp"
#include "Common/Random.hpp"
#include "Common/TemplateTypes.hpp"
#include "Common/Types.hpp"

#include <array>
#include <type_traits>
#include <vector>

using Item_t = u8;

enum : Item_t
{
	ITEM_MUGWORT = 0,
	ITEM_META_MUGWORT,
	ITEM_BITTER_MUGWORT,
	ITEM_GREAT_WALNUT,
	ITEM_CHESTNUT_OIL,
	ITEM_SHISHIUDO_OIL,
	ITEM_KUKUMIRA_OIL,
	ITEM_BLETILLA_OIL,
	ITEM_ICHISHI_OIL,
	ITEM_SHAB_LIQUOR,
	ITEM_FINE_LIQUOR,
	ITEM_EXCELLENT_LIQUOR,
	ITEM_SPECIAL_LIQUOR,
	ITEM_TENDAI_UYAKU,
	ITEM_VALERIAN_POWDER,
	ITEM_IKARI_POWDER,
	ITEM_RING_OF_VENOM,
	ITEM_RING_OF_SLEEP,
	ITEM_RING_OF_STONING,
	ITEM_RING_OF_DEATH,
	ITEM_MIRROR_OF_DEVA,
	ITEM_URVYS_BLESSING,
	ITEM_VARYS_BLESSING,
	ITEM_AGNIS_BLESSING,
	ITEM_MALTYS_BLESSING,
	ITEM_URVYS_SEAL1,
	ITEM_URVYS_SEAL2,
	ITEM_VARYS_SEAL1,
	ITEM_VARYS_SEAL2,
	ITEM_AGNIS_SEAL1,
	ITEM_AGNIS_SEAL2,
	ITEM_MALTYS_SEAL1,
	ITEM_MALTYS_SEAL2,
	ITEM_CLOUD_SILK,
	ITEM_THUNDER_SILK,
	ITEM_LILY_SILK,
	ITEM_PEARL_SILK,
	ITEM_FIRST_SNOW_SILK,
	ITEM_ANGELWING_SILK,
	ITEM_MOONLIGHT_SILK,
	ITEM_SKELETON_KEY,
	ITEM_SLEEP_SPORES,
	ITEM_ATTRACT_SPORES = 42,
	ITEM_COUNT,

	WEAPON_KNIFE = 0,
	WEAPON_DAGGER,
	WEAPON_AQUAZOR,
	WEAPON_BRONZE_SWORD,
	WEAPON_SWORD_OF_SPEED,
	WEAPON_IRON_SWORD,
	WEAPON_IRON_AXE,
	WEAPON_LIFE_STEALER,
	WEAPON_WHITE_HORN,
	WEAPON_LITTLE_BIRD,
	WEAPON_WHISTLER,
	WEAPON_NIGHTGLOW,
	WEAPON_BLUE_PHANTOM,
	WEAPON_THUNDERBOLT,
	WEAPON_TSUNAMI,
	WEAPON_SPRING_BLOSSOM,
	WEAPON_DRAGONCREST,
	WEAPON_CLOUDCOVER,
	WEAPON_PALEMOON,
	WEAPON_REAPER,
	WEAPON_DRAINER,
	WEAPON_AUTUMN_MOON,
	WEAPON_ICICLE = 22,
	WEAPON_COUNT,

	ARMOR_HUNTERS_VEST = 0,
	ARMOR_LEATHER_VEST,
	ARMOR_CHAIN_MAIL_VEST,
	ARMOR_WIRE_VEST,
	ARMOR_PLATE_MAIL_VEST,
	ARMOR_REFLECTING_VEST,
	ARMOR_SILVER_VEST,
	ARMOR_DIVINE_VEST,
	ARMOR_RIKETZ_GARB,
	ARMOR_NAGI_GARB,
	ARMOR_BLOODY_GARB,
	ARMOR_NAVY_BLUE_GARB,
	ARMOR_FOREST_GARB,
	ARMOR_DIVINE_GARB1,
	ARMOR_DIVINE_GARB2 = 14,
	ARMOR_COUNT,

	OTHER_LEATHER_HEADBAND = 0,
	OTHER_IRON_HEADBAND,
	OTHER_SILVER_HEADBAND,
	OTHER_BLUE_BEAD,
	OTHER_GREEN_BEAD,
	OTHER_YELLOW_BEAD,
	OTHER_RED_BEAD,
	OTHER_SPARROW_RUFF,
	OTHER_IRON_BRACELET,
	OTHER_SILVER_CROWN,
	OTHER_HOLLY_RUFF,
	OTHER_HAWK_RUFF,
	OTHER_POISON_CHOKER,
	OTHER_SLEEP_CHOKER,
	OTHER_STONE_CHOKER,
	OTHER_SUPER_CHOKER,
	OTHER_NAGI_BELT = 16,
	OTHER_COUNT,

	ECSKIN_PALOOJA = 0,
	ECSKIN_TICKER,
	ECSKIN_JEECHWO,
	ECSKIN_KOLNA,
	ECSKIN_SICANJAL,
	ECSKIN_TIMAIOS,
	ECSKIN_YOLGA,
	ECSKIN_KLARRGAS = 7,
	ECSKIN_COUNT,
	// Unused
	ECSKIN_TX_8 = 8,
	ECSKIN_TX_9 = 9,
	//

	VALUABLE_HUNTERS_EARRING = 0,
	VALUABLE_FLUTE_OF_CAPTURE,
	VALUABLE_TEARS_OF_REESE,
	VALUABLE_BEETLE_KEY,
	VALUABLE_DRAGONFLY_KEY,
	VALUABLE_CALABAS_POWDER,
	VALUABLE_SPIDER_KEY,
	VALUABLE_OCARINA,
	VALUABLE_CALABAS_HERB,
	VALUABLE_CHAMPIONS_BELT,
	VALUABLE_CAPTURE_AMULET,
	VALUABLE_MOTH_KEY,
	VALUABLE_MAHBUS_COCOON,
	VALUABLE_JEWEL_OF_WATER,
	VALUABLE_JEWEL_OF_FIRE,
	VALUABLE_JEWEL_OF_WIND,
	VALUABLE_DIVINE_TABLET,
	VALUABLE_KICKLEBERRY,
	VALUABLE_MINION_FANG,
	VALUABLE_LARVA_KEY,
	VALUABLE_PUPA_KEY,
	VALUABLE_BEETLE_WARP,
	VALUABLE_DRAGONFLY_WARP,
	VALUABLE_SPIDER_WARP,
	VALUABLE_SECRET_OF_KILLING,
	VALUABLE_KNOWLEDGE1,
	VALUABLE_KNOWLEDGE2,
	VALUABLE_KNOWLEDGE3,
	VALUABLE_KNOWLEDGE4,
	VALUABLE_KNOWLEDGE5,
	VALUABLE_MAGIC_USAGE1,
	VALUABLE_MAGIC_USAGE2,
	VALUABLE_MERGING1,
	VALUABLE_MERGING2,
	VALUABLE_MERGING3,
	VALUABLE_SECRET1,
	VALUABLE_SECRET2,
	VALUABLE_SECRET3 = 37,
	VALUABLE_COUNT
};

using EquipmentIcon_t = s16;

enum : EquipmentIcon_t
{
	EQUIPMENT_ICON_DAGGER,
	EQUIPMENT_ICON_ARMOR,
	EQUIPMENT_ICON_OTHER,
	EQUIPMENT_ICON_SWORD,
	EQUIPMENT_ICON_SWORD_2_HAND,
	EQUIPMENT_ICON_AXE,
	EQUIPMENT_ICON_SPEAR,
	EQUIPMENT_ICON_KATANA
};

using WeaponSkin_t = s16;

enum : WeaponSkin_t
{
	WEAPON_SKIN_DAGGER,
	WEAPON_SKIN_SWORD_OF_SPEED,
	WEAPON_SKIN_LITTLE_BIRD,
	WEAPON_SKIN_BRONZE_SWORD,
	WEAPON_SKIN_LIFE_STEALER,
	WEAPON_SKIN_NIGHTGLOW,
	WEAPON_SKIN_TSUNAMI,
	WEAPON_SKIN_IRON_SWORD,
	WEAPON_SKIN_DRAGONCREST,
	WEAPON_SKIN_CLOUDCOVER,
	WEAPON_SKIN_BLUE_PHANTOM,
	WEAPON_SKIN_IRON_AXE,
	WEAPON_SKIN_PALEMOON,
	WEAPON_SKIN_REAPER,
	WEAPON_SKIN_WHITE_HORN,
	WEAPON_SKIN_WHISTLER,
	WEAPON_SKIN_THUNDERBOLT,
	WEAPON_SKIN_AQUAZOR,
	WEAPON_SKIN_AUTUMN_MOON,
	WEAPON_SKIN_COUNT
};

using WeaponAnimation_t = u8;

enum : WeaponAnimation_t
{
	WEAPON_ANIMATION_FIRE,
	WEAPON_ANIMATION_AIR,
	WEAPON_ANIMATION_EARTH,
	WEAPON_ANIMATION_WATER,
	WEAPON_ANIMATION_POISON,
	WEAPON_ANIMATION_SLEEP,
	WEAPON_ANIMATION_STONE,
	WEAPON_ANIMATION_CRITICAL,
	WEAPON_ANIMATION_DRAIN,
	WEAPON_ANIMATION_POSITIVE,
	WEAPON_ANIMATION_NONE,
	WEAPON_ANIMATION_NEUTRAL = 255,
};

using StatusEffect_t = u16;

enum : StatusEffect_t
{
	STATUS_EFFECT_POISON = 1 << 0,
	STATUS_EFFECT_SLEEP = 1 << 1,
	STATUS_EFFECT_UNKNOWN = 1 << 2,
	STATUS_EFFECT_STONE = 1 << 3,
	STATUS_EFFECT_STONE_2 = 1 << 4,
	STATUS_EFFECT_STONE_3 = 1 << 5,
	STATUS_EFFECT_STONE_4 = 1 << 6,
	STATUS_EFFECT_STONE_5 = 1 << 7,
	STATUS_EFFECT_STONE_6 = 1 << 8,
	STATUS_EFFECT_STONE_DEATH = 1 << 9,
	STATUS_EFFECT_DEATH = 1 << 10
};

using WeaponElement_t = u8;

enum : WeaponElement_t
{
	WEAPON_ELEMENT_FIRE,
	WEAPON_ELEMENT_AIR,
	WEAPON_ELEMENT_EARTH,
	WEAPON_ELEMENT_WATER,
	WEAPON_ELEMENT_POISON,
	WEAPON_ELEMENT_SLEEP,
	WEAPON_ELEMENT_STONE,
	WEAPON_ELEMENT_CRITICAL,
	WEAPON_ELEMENT_DRAIN_HP,
	WEAPON_ELEMENT_POSITIVE,
	WEAPON_ELEMENT_NONE,
	WEAPON_ELEMENT_ALL
};

using DamageMode_t = u8;

enum : DamageMode_t
{
	DAMAGE_MODE_ATTACK,
	DAMAGE_MODE_MAGIC_ATTACK,
	DAMAGE_MODE_DAMAGE,
	DAMAGE_MODE_NONE
};

using StatusMode_t = u8;

enum : StatusMode_t
{
	STATUS_MODE_100,
	STATUS_MODE_MODIFIERS = 3
};

using WeaponBonus_t = u8;

enum : WeaponBonus_t
{
	WEAPON_BONUS_NONE,
	WEAPON_BONUS_DRAIN_MP,
	WEAPON_BONUS_DAMAGE_DESTROY_MP,
	WEAPON_BONUS_CRITICAL_1HP
};

struct EquipmentStats
{
	EquipmentIcon_t icon;
	WeaponSkin_t skin;
	s8 attack;
	s8 defense;
	s8 magicAttack;
	s8 magicDefense;
	s8 speed;
	std::array<u8, 11> unk;
	s32 damage;

	s32 totalStatsPoint() const;
	s32 totalNegativePoint() const;
};

struct WeaponCharacteristic
{
	StatusEffect_t status;
	s16 damage;
	Element_t deva;
	u8 accuracy;
	u8 statusRate;
	s8 critical;
	WeaponElement_t element;
	DamageMode_t damageMode;
	StatusMode_t statusMode;
	WeaponBonus_t bonus;
};

using ChapterPoolArray = std::array<std::vector<Item_t>, 6>;

template <Integral T>
struct CategoryItem
{
	T category;
	T id;
};

namespace Item
{
	using Category_t = u8;

	enum : Category_t
	{
		CATEGORY_ITEM,
		CATEGORY_WEAPON,
		CATEGORY_ARMOR,
		CATEGORY_OTHER,
		CATEGORY_ECSKIN,
		CATEGORY_VALUABLE,
		CATEGORY_COUNT
	};

	constexpr WeaponAnimation_t weaponAnimationByElement(WeaponElement_t element)
	{
		if (element == WEAPON_ELEMENT_NONE)
		{
			return 0xB;
		}
		else if (element == WEAPON_ELEMENT_POSITIVE)
		{
			return WEAPON_ELEMENT_DRAIN_HP;
		}
		else
		{
			return static_cast<WeaponAnimation_t>(element);
		}
	}

	template <Integral T>
	T generateItemFromCategoryPool(T category, const ChapterPoolArray& pool, Random* random)
	{
		if (category >= Item::CATEGORY_COUNT)
		{
			throw JcrException{ "Invalid item category : {}", category };
		}
		else if (pool[category].empty())
		{
			throw JcrException{ "Attempt to generate an item on an empty pool category : {}", category };
		}

		return pool[category][random->generate(pool[category].size() - 1)];
	}

	template <Integral T>
	CategoryItem<T> generateItemAndCategoryPool(const ChapterPoolArray& pool, Random* random)
	{
		const auto
			step1{ pool[CATEGORY_ITEM].size() },
			step2{ step1 + pool[CATEGORY_WEAPON].size() }, 
			step3{ step2 + pool[CATEGORY_ARMOR].size() },
			step4{ step3 + pool[CATEGORY_OTHER].size() },
			step5{ step4 + pool[CATEGORY_ECSKIN].size() },
			step6{ step5 + pool[CATEGORY_VALUABLE].size() };

		if (step6 == 0)
		{
			throw JcrException{ "Attempt to generate an item on empty pools" };
		}

		const auto rng{ random->generate(step6 - 1) };

		if (rng < step1)
		{
			return { CATEGORY_ITEM, pool[CATEGORY_ITEM][rng] };
		}
		else if (rng < step2)
		{
			return { CATEGORY_WEAPON, pool[CATEGORY_WEAPON][rng - step1] };
		}
		else if (rng < step3)
		{
			return { CATEGORY_ARMOR, pool[CATEGORY_ARMOR][rng - step2] };
		}
		else if (rng < step4)
		{
			return { CATEGORY_OTHER, pool[CATEGORY_OTHER][rng - step3] };
		}
		else if (rng < step5)
		{
			return { CATEGORY_ECSKIN, pool[CATEGORY_ECSKIN][rng - step4] };
		}
		else
		{
			return { CATEGORY_VALUABLE, pool[CATEGORY_VALUABLE][rng - step5] };
		}
	}

	ChapterPoolArray availableItemsChapter(Chapter_t chapter);
};