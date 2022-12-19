#include "Item.hpp"

#include <cstdlib>

s32 EquipmentStats::totalStatsPoint() const
{
	return std::abs(attack) + std::abs(defense) + std::abs(magicAttack) + 
		std::abs(magicDefense) + std::abs(speed);
}

s32 EquipmentStats::totalNegativePoint() const
{
	s32 total{};
	if (attack < 0)
	{
		total -= attack;
	}
	if (defense < 0)
	{
		total -= defense;
	}
	if (magicAttack < 0)
	{
		total -= magicAttack;
	}
	if (magicDefense < 0)
	{
		total -= magicDefense;
	}
	if (speed < 0)
	{
		total -= speed;
	}
	return total;
}

namespace Item
{
	ChapterPoolArray availableItemsChapter(Chapter_t chapter)
	{
		ChapterPoolArray chapterItem;

		if (chapter == CHAPTER_ETERNAL_CORRIDOR)
		{
			static constexpr std::array<Item_t, CATEGORY_COUNT> categoryCount
			{
				ITEM_COUNT, WEAPON_COUNT, ARMOR_COUNT, OTHER_COUNT,
				ECSKIN_COUNT, VALUABLE_COUNT
			};

			for (std::size_t i{}; i < categoryCount.size(); ++i)
			{
				chapterItem[i].reserve(categoryCount[i]);
				for (Item_t j{}; j < categoryCount[i]; ++j)
				{
					chapterItem[i].emplace_back(j);
				}
			}

			return chapterItem;
		}
		
		if (chapter >= CHAPTER_TUTORIAL)
		{
			// Dialogue
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_HUNTERS_EARRING);
		}
		if (chapter >= CHAPTER_BEETLE)
		{
			// Shop
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_KNIFE);
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_DAGGER);
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_BRONZE_SWORD);
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_LEATHER_VEST);
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_CHAIN_MAIL_VEST);
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_LEATHER_HEADBAND);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_MUGWORT);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_SHAB_LIQUOR);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_VALERIAN_POWDER);

			// Treasure
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_SKELETON_KEY);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_KNOWLEDGE1);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_KNOWLEDGE2);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_KNOWLEDGE3);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_BEETLE_WARP);
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_AQUAZOR);

			// Chest
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_META_MUGWORT);
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_IRON_HEADBAND);

			// Dialogue
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_TEARS_OF_REESE);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_FLUTE_OF_CAPTURE);

			// Marrdreg
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_CHESTNUT_OIL);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_SHISHIUDO_OIL); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_KUKUMIRA_OIL); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_BLETILLA_OIL); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_ICHISHI_OIL); //

			// Mukhambu
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_MIRROR_OF_DEVA);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_SPECIAL_LIQUOR);

			// Pataimel
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_AGNIS_SEAL1);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_URVYS_SEAL1); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_VARYS_SEAL1); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_MALTYS_SEAL1); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_BITTER_MUGWORT);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_GREAT_WALNUT);

			// Nushab
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_RING_OF_VENOM);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_RING_OF_SLEEP); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_RING_OF_STONING); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_RING_OF_DEATH); //
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_POISON_CHOKER);

			// Minion level
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_CLOUD_SILK);
		}
		if (chapter >= CHAPTER_DRAGONFLY)
		{
			// Shop
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_SWORD_OF_SPEED);
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_WIRE_VEST);
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_PLATE_MAIL_VEST);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_TENDAI_UYAKU);

			// Treasure
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_SLEEP_SPORES);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_ATTRACT_SPORES); //
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_KICKLEBERRY);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_KNOWLEDGE4);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_MAGIC_USAGE1);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_MAGIC_USAGE2);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_SECRET_OF_KILLING);

			// Dialogue
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_BEETLE_KEY);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_DRAGONFLY_KEY);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_CALABAS_POWDER);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_OCARINA);

			// Chest
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_AGNIS_SEAL2);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_URVYS_SEAL2); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_VARYS_SEAL2); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_MALTYS_SEAL2); //
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_SILVER_CROWN);

			// Maskhira
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_SLEEP_CHOKER);

			// Goat
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_IRON_AXE);

			// Minion level
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_THUNDER_SILK);
		}
		if (chapter >= CHAPTER_SPIDER)
		{
			// Shop
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_IRON_SWORD);
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_LITTLE_BIRD);
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_SILVER_VEST);
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_DIVINE_VEST);
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_SILVER_HEADBAND);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_FINE_LIQUOR);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_IKARI_POWDER);

			// Treasure
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_DRAGONFLY_WARP); // Should be available after beating Kikinak
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_DIVINE_TABLET);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_MINION_FANG);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_SPIDER_WARP);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_KNOWLEDGE5);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_MERGING1);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_MERGING2);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_MERGING3); // 
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_WHITE_HORN);

			// Chest
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_STONE_CHOKER);

			// Dialogue
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_SPIDER_KEY);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_LARVA_KEY);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_CALABAS_HERB);
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_LIFE_STEALER);
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_NAGI_BELT);

			// Rugdogle
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_DRAGONCREST);

			// Doglchu
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_REAPER);

			// Minion level
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_LILY_SILK);
		}
		if (chapter >= CHAPTER_SPIDER_2)
		{
			// Dialogue
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_CAPTURE_AMULET);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_CHAMPIONS_BELT);

			// Maryen
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_URVYS_BLESSING);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_VARYS_BLESSING); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_AGNIS_BLESSING); //
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_MALTYS_BLESSING); //
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_CLOUDCOVER);

			// Chest
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_YELLOW_BEAD);
		}
		if (chapter >= CHAPTER_MOTH)
		{
			// Dialogue
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_JEWEL_OF_WATER);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_MAHBUS_COCOON);
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_RIKETZ_GARB);

			// Radddreg
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_REFLECTING_VEST);
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_WHISTLER);

			// Raddlchu
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_PALEMOON);

			// Asha
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_NIGHTGLOW);
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_DIVINE_GARB1);

			// Chest
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_SPARROW_RUFF);
		}
		if (chapter >= CHAPTER_NETHERWORLD)
		{
			// Shop
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_BLUE_PHANTOM);
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_SPRING_BLOSSOM);
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_NAGI_GARB);
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_NAVY_BLUE_GARB);
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_FOREST_GARB);
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_IRON_BRACELET);
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_HOLLY_RUFF);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_EXCELLENT_LIQUOR);

			// Treasure
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_THUNDERBOLT);

			// Dialogue
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_MOTH_KEY);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_PUPA_KEY);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_JEWEL_OF_FIRE);
			chapterItem[CATEGORY_VALUABLE].emplace_back(VALUABLE_JEWEL_OF_WIND);

			// Patdreg
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_RED_BEAD);

			// Dogle
			chapterItem[CATEGORY_WEAPON].emplace_back(WEAPON_TSUNAMI);

			// Geenwee
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_SUPER_CHOKER);

			// Mugoo
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_GREEN_BEAD);

			// Noobwee
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_DIVINE_GARB2);

			// Srikarta
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_BLUE_BEAD);

			// Fire Boss
			chapterItem[CATEGORY_ARMOR].emplace_back(ARMOR_BLOODY_GARB);

			// Chest
			chapterItem[CATEGORY_OTHER].emplace_back(OTHER_HAWK_RUFF);

			// Minion level
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_PEARL_SILK);
			chapterItem[CATEGORY_ITEM].emplace_back(ITEM_FIRST_SNOW_SILK);
		}
		return chapterItem;
	}
}