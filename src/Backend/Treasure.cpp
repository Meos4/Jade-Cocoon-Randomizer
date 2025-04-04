#include "Treasure.hpp"

#include "Backend/Behavior.hpp"
#include "Backend/File.hpp"
#include "Backend/Item.hpp"
#include "Common/Random.hpp"
#include "Common/TemplateTypes.hpp"

#include <array>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

class TreasureRandomizer final
{
public:
	TreasureRandomizer(std::shared_ptr<Game> game)
		: m_game(std::move(game))
	{
		static constexpr std::array<Chapter_t, 7> chapters
		{
			CHAPTER_BEETLE, CHAPTER_DRAGONFLY, CHAPTER_SPIDER, CHAPTER_SPIDER_2,
			CHAPTER_MOTH, CHAPTER_NETHERWORLD
		};

		for (const auto& chapter : chapters)
		{
			m_chaptersPool[chapter] = Item::availableItemsChapter(chapter);
		}
	}

	void append(std::initializer_list<FileOffset> fo, Chapter_t chapter, bool fourBytes = false)
	{
		std::vector<TreasureInfo> treasuresInfo;
		for (auto& [file, offset] : fo)
		{
			treasuresInfo.emplace_back(m_game->file(file), offset, fourBytes);
		}

		m_treasuresInfo[chapter].emplace_back(std::move(treasuresInfo));
	}

	void random()
	{
		for (const auto& [chapter, treasures] : m_treasuresInfo)
		{
			for (const auto& treasure : treasures)
			{
				const auto cItem{ Item::generateItemAndCategoryPool<u16>(m_chaptersPool.at(chapter), m_game->random()) };

				for (const auto& [file, offset, fourBytes] : treasure)
				{
					if (fourBytes)
					{
						file->write(offset, static_cast<u32>(cItem.id));
						file->write(offset + 8, static_cast<u32>(cItem.category));
					}
					else
					{
						file->write(offset, cItem);
					}
				}
			}
		}
	}

	void randomByCategory()
	{
		for (const auto& [chapter, treasures] : m_treasuresInfo)
		{
			for (const auto& treasure : treasures)
			{
				u16 category;

				if (treasure[0].fourBytes)
				{
					treasure[0].file->read(treasure[0].offset + 8, &category);
				}
				else
				{
					treasure[0].file->read(treasure[0].offset, &category);
				}

				const auto item{ Item::generateItemFromCategoryPool(category, m_chaptersPool.at(chapter), m_game->random()) };

				for (const auto& [file, offset, fourBytes] : treasure)
				{
					if (fourBytes)
					{
						file->write(offset, static_cast<u32>(item));
						file->write(offset + 8, static_cast<u32>(category));
					}
					else
					{
						file->write(offset, CategoryItem<u16>{ category, item });
					}
				}
			}
		}
	}
private:
	struct TreasureInfo
	{
		std::unique_ptr<RawFile> file;
		u32 offset;
		bool fourBytes;
	};

	std::shared_ptr<Game> m_game;

	std::unordered_map<Chapter_t, ChapterPoolArray> m_chaptersPool;
	std::unordered_map<Chapter_t, std::vector<std::vector<TreasureInfo>>> m_treasuresInfo;
};

Treasure::Treasure(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
	: m_game(std::move(game)), m_sharedData(std::move(sharedData))
{
}

void Treasure::setItems(Treasure::Items state, bool randomSkeletonKeys) const
{
	TreasureRandomizer treasureR{ m_game };

	const auto& offsetF{ m_game->offset().file };

	const FileOffset
		shabLiquor{ File::SCENE_FIELD1_FOREST1_SCE01_SBH, offsetF.scene_field1_forest1_sce01_sbh.shabLiquor },
		sleepSpore{ File::SCENE_FIELD1_FOREST2_SCE07_SBH, offsetF.scene_field1_forest2_sce07_sbh.sleepSpore },
		sleepSporeP{ File::SCENE_FIELD1_FOREST2_SCE07A_SBH, offsetF.scene_field1_forest2_sce07a_sbh.sleepSpore },
		ikariPowder{ File::SCENE_FIELD1_FOREST3_SCE03_SBH, offsetF.scene_field1_forest3_sce03_sbh.ikariPowder },
		ikariPowderP{ File::SCENE_FIELD1_FOREST3_SCE03A_SBH, offsetF.scene_field1_forest3_sce03a_sbh.ikariPowder },
		ikariPowderP2{ File::SCENE_FIELD1_FOREST3_SCE24_SBH, offsetF.scene_field1_forest3_sce24_sbh.ikariPowder },
		excellentLiquor{ File::SCENE_FIELD1_FOREST3_SCE16_SBH, offsetF.scene_field1_forest3_sce16_sbh.excellentLiquor },
		excellentLiquorP{ File::SCENE_FIELD1_FOREST3_SCE37_SBH, offsetF.scene_field1_forest3_sce37_sbh.excellentLiquor },
		excellentLiquor2{ File::SCENE_FIELD1_FOREST4_SCE00_SBH, offsetF.scene_field1_forest4_sce00_sbh.excellentLiquor },
		greatWalnut{ File::SCENE_FIELD1_FOREST4_SCE05_SBH, offsetF.scene_field1_forest4_sce05_sbh.greatWalnut },
		metaMugwort{ File::SCENE_FIELD1_FOREST1_SCE02_SBH, offsetF.scene_field1_forest1_sce02_sbh.chest_metaMugwort },
		shabLiquor2{ File::SCENE_FIELD1_FOREST1_SCE02_SBH, offsetF.scene_field1_forest1_sce02_sbh.chest_metaMugwort + 8 },
		chestNutOil{ File::SCENE_FIELD1_FOREST2_SCE05_SBH, offsetF.scene_field1_forest2_sce05_sbh.chest_chestNutOil },
		bletillaOil{ File::SCENE_FIELD1_FOREST2_SCE05_SBH, offsetF.scene_field1_forest2_sce05_sbh.chest_chestNutOil + 8 },
		tendaiUyaku{ File::SCENE_FIELD1_FOREST2_SCE05_SBH, offsetF.scene_field1_forest2_sce05_sbh.chest_chestNutOil + 0x10 },
		agnisSeal1{ File::SCENE_FIELD1_FOREST2_SCE12_SBH, offsetF.scene_field1_forest2_sce12_sbh.chest_agnisSeal1 },
		agnisSeal2{ File::SCENE_FIELD1_FOREST2_SCE12_SBH, offsetF.scene_field1_forest2_sce12_sbh.chest_agnisSeal1 + 8 },
		maltysSeal2{ File::SCENE_FIELD1_FOREST3_SCE05_SBH, offsetF.scene_field1_forest3_sce05_sbh.chest_stoneChoker + 8 },
		maltysSeal1{ File::SCENE_FIELD1_FOREST3_SCE05_SBH, offsetF.scene_field1_forest3_sce05_sbh.chest_stoneChoker + 0x10 },
		maltysSeal2P{ File::SCENE_FIELD1_FOREST3_SCE26_SBH, offsetF.scene_field1_forest3_sce26_sbh.chest_stoneChoker + 8 },
		maltysSeal1P{ File::SCENE_FIELD1_FOREST3_SCE26_SBH, offsetF.scene_field1_forest3_sce26_sbh.chest_stoneChoker + 0x10 },
		kukumiraOil{ File::SCENE_FIELD1_FOREST3_SCE17_SBH, offsetF.scene_field1_forest3_sce17_sbh.chest_yellowBead + 8 },
		ichishiOil{ File::SCENE_FIELD1_FOREST3_SCE17_SBH, offsetF.scene_field1_forest3_sce17_sbh.chest_yellowBead + 0x10 },
		kukumiraOilP{ File::SCENE_FIELD1_FOREST3_SCE38_SBH, offsetF.scene_field1_forest3_sce38_sbh.chest_yellowBead + 8 },
		ichishiOilP{ File::SCENE_FIELD1_FOREST3_SCE38_SBH, offsetF.scene_field1_forest3_sce38_sbh.chest_yellowBead + 0x10 },
		bitterMugwort{ File::SCENE_FIELD1_FOREST4_SCE01_SBH, offsetF.scene_field1_forest4_sce01_sbh.chest_bitterMugwort },
		specialLiquor{ File::SCENE_FIELD1_FOREST4_SCE01_SBH, offsetF.scene_field1_forest4_sce01_sbh.chest_bitterMugwort + 8 },
		tendaiUyaku2{ File::SCENE_FIELD1_FOREST4_SCE01_SBH, offsetF.scene_field1_forest4_sce01_sbh.chest_bitterMugwort + 0x10 },
		shishiudoOil{ File::SCENE_FIELD1_FOREST4_SCE03_SBH, offsetF.scene_field1_forest4_sce03_sbh.chest_shishiudoOil },
		bletillaOil2{ File::SCENE_FIELD1_FOREST4_SCE03_SBH, offsetF.scene_field1_forest4_sce03_sbh.chest_shishiudoOil + 8 },
		ichishiOil2{ File::SCENE_FIELD1_FOREST4_SCE03_SBH, offsetF.scene_field1_forest4_sce03_sbh.chest_shishiudoOil + 0x10 },
		urvysBlessing{ File::SCENE_FIELD1_FOREST4_SCE08_SBH, offsetF.scene_field1_forest4_sce08_sbh.chest_urvysBlessing },
		greatWalnut2{ File::SCENE_FIELD1_FOREST4_SCE08_SBH, offsetF.scene_field1_forest4_sce08_sbh.chest_urvysBlessing + 0x10 },
		varysBlessing{ File::SCENE_FIELD2_FOREST1_SCE02_SBH, offsetF.scene_field2_forest1_sce02_sbh.chest_varysBlessing },
		maltysBlessing{ File::SCENE_FIELD2_FOREST1_SCE02_SBH, offsetF.scene_field2_forest1_sce02_sbh.chest_varysBlessing + 8 },
		bletillaOil3{ File::SCENE_FIELD2_FOREST2_SCE06_SBH, offsetF.scene_field2_forest2_sce06_sbh.chest_bletillaOil},
		chestnutOil{ File::SCENE_FIELD2_FOREST2_SCE06_SBH, offsetF.scene_field2_forest2_sce06_sbh.chest_bletillaOil + 8 },
		sleepSpores{ File::SCENE_FIELD2_FOREST2_SCE06_SBH, offsetF.scene_field2_forest2_sce06_sbh.chest_bletillaOil + 0x10 },
		bitterMugwort2{ File::SCENE_FIELD2_FOREST3_SCE16_SBH, offsetF.scene_field2_forest3_sce16_sbh.chest_bitterMugwort },
		specialLiquor2{ File::SCENE_FIELD2_FOREST3_SCE16_SBH, offsetF.scene_field2_forest3_sce16_sbh.chest_bitterMugwort + 8 },
		greatWalnut3{ File::SCENE_FIELD2_FOREST3_SCE16_SBH, offsetF.scene_field2_forest3_sce16_sbh.chest_bitterMugwort + 0x10 },
		urvysBlessing2{ File::SCENE_FIELD2_FOREST3_SCE18_SBH, offsetF.scene_field2_forest3_sce18_sbh.chest_urvysBlessing },
		agnisBlessing{ File::SCENE_FIELD2_FOREST3_SCE18_SBH, offsetF.scene_field2_forest3_sce18_sbh.chest_urvysBlessing + 8 },
		tendaiUyaku3{ File::SCENE_FIELD2_FOREST3_SCE18_SBH, offsetF.scene_field2_forest3_sce18_sbh.chest_urvysBlessing + 0x10 },
		specialLiquor3{ File::SCENE_FIELD1_FOREST2_SCE07_SBH, offsetF.scene_field1_forest2_sce07_sbh.gift_specialLiquor };

	// Collectable
	treasureR.append({ shabLiquor }, CHAPTER_BEETLE);
	treasureR.append({ sleepSpore, sleepSporeP }, CHAPTER_DRAGONFLY);
	treasureR.append({ ikariPowder, ikariPowderP, ikariPowderP2 }, CHAPTER_SPIDER);
	treasureR.append({ excellentLiquor, excellentLiquorP }, CHAPTER_SPIDER_2);
	treasureR.append({ excellentLiquor2 }, CHAPTER_MOTH);
	treasureR.append({ greatWalnut }, CHAPTER_MOTH);
	// Chest
	treasureR.append({ metaMugwort }, CHAPTER_BEETLE);
	treasureR.append({ shabLiquor2 }, CHAPTER_BEETLE);
	treasureR.append({ chestNutOil }, CHAPTER_DRAGONFLY);
	treasureR.append({ bletillaOil }, CHAPTER_DRAGONFLY);
	treasureR.append({ tendaiUyaku }, CHAPTER_DRAGONFLY);
	treasureR.append({ agnisSeal1 }, CHAPTER_DRAGONFLY);
	treasureR.append({ agnisSeal2 }, CHAPTER_DRAGONFLY);
	treasureR.append({ maltysSeal2, maltysSeal2P }, CHAPTER_SPIDER);
	treasureR.append({ maltysSeal1, maltysSeal1P }, CHAPTER_SPIDER);
	treasureR.append({ kukumiraOil, kukumiraOilP }, CHAPTER_SPIDER_2);
	treasureR.append({ ichishiOil, ichishiOilP }, CHAPTER_SPIDER_2);
	treasureR.append({ bitterMugwort }, CHAPTER_MOTH);
	treasureR.append({ specialLiquor }, CHAPTER_MOTH);
	treasureR.append({ tendaiUyaku2 }, CHAPTER_MOTH);
	treasureR.append({ shishiudoOil }, CHAPTER_MOTH);
	treasureR.append({ bletillaOil2 }, CHAPTER_MOTH);
	treasureR.append({ ichishiOil2 }, CHAPTER_MOTH);
	treasureR.append({ urvysBlessing }, CHAPTER_MOTH);
	treasureR.append({ greatWalnut2 }, CHAPTER_MOTH);
	treasureR.append({ varysBlessing }, CHAPTER_NETHERWORLD);
	treasureR.append({ maltysBlessing }, CHAPTER_NETHERWORLD);
	treasureR.append({ bletillaOil3 }, CHAPTER_NETHERWORLD);
	treasureR.append({ chestnutOil }, CHAPTER_NETHERWORLD);
	treasureR.append({ sleepSpores }, CHAPTER_NETHERWORLD);
	treasureR.append({ bitterMugwort2 }, CHAPTER_NETHERWORLD);
	treasureR.append({ specialLiquor2 }, CHAPTER_NETHERWORLD);
	treasureR.append({ greatWalnut3 }, CHAPTER_NETHERWORLD);
	treasureR.append({ urvysBlessing2 }, CHAPTER_NETHERWORLD);
	treasureR.append({ agnisBlessing }, CHAPTER_NETHERWORLD);
	treasureR.append({ tendaiUyaku3 }, CHAPTER_NETHERWORLD);
	// Dialogue
	treasureR.append({ specialLiquor3 }, CHAPTER_DRAGONFLY);

	if (randomSkeletonKeys)
	{
		const FileOffset
			skeletonKey{ File::SCENE_FIELD1_FOREST1_SCE05_SBH, offsetF.scene_field1_forest1_sce05_sbh.skeletonKey },
			skeletonKey2{ File::SCENE_FIELD1_FOREST2_SCE03_SBH, offsetF.scene_field1_forest2_sce03_sbh.skeletonKey },
			skeletonKey3{ File::SCENE_FIELD1_FOREST2_SCE10_SBH, offsetF.scene_field1_forest2_sce10_sbh.skeletonKey },
			skeletonKey4{ File::SCENE_FIELD1_FOREST3_SCE06_SBH, offsetF.scene_field1_forest3_sce06_sbh.skeletonKey },
			skeletonKey4P{ File::SCENE_FIELD1_FOREST3_SCE27_SBH, offsetF.scene_field1_forest3_sce27_sbh.skeletonKey },
			skeletonKey5{ File::SCENE_FIELD1_FOREST3_SCE18_SBH, offsetF.scene_field1_forest3_sce18_sbh.skeletonKey },
			skeletonKey5P{ File::SCENE_FIELD1_FOREST3_SCE39_SBH, offsetF.scene_field1_forest3_sce39_sbh.skeletonKey },
			skeletonKey6{ File::SCENE_FIELD1_FOREST4_SCE06_SBH, offsetF.scene_field1_forest4_sce06_sbh.skeletonKey },
			skeletonKey7{ File::SCENE_FIELD1_FOREST4_SCE09_SBH, offsetF.scene_field1_forest4_sce09_sbh.skeletonKey },
			skeletonKey7P{ File::SCENE_FIELD1_FOREST4_SCE09A_SBH, offsetF.scene_field1_forest4_sce09a_sbh.skeletonKey },
			skeletonKey8{ File::SCENE_FIELD1_FOREST4_SCE10_SBH, offsetF.scene_field1_forest4_sce10_sbh.skeletonKey },
			skeletonKey9{ File::SCENE_FIELD2_FOREST1_SCE06_SBH, offsetF.scene_field2_forest1_sce06_sbh.skeletonKey },
			skeletonKey10{ File::SCENE_FIELD2_FOREST2_SCE14_SBH, offsetF.scene_field2_forest2_sce14_sbh.skeletonKey },
			skeletonKey11{ File::SCENE_FIELD2_FOREST3_SCE07_SBH, offsetF.scene_field2_forest3_sce07_sbh.skeletonKey };

		treasureR.append({ skeletonKey }, CHAPTER_BEETLE);
		treasureR.append({ skeletonKey2 }, CHAPTER_DRAGONFLY);
		treasureR.append({ skeletonKey3 }, CHAPTER_DRAGONFLY);
		treasureR.append({ skeletonKey4, skeletonKey4P }, CHAPTER_SPIDER);
		treasureR.append({ skeletonKey5, skeletonKey5P }, CHAPTER_SPIDER_2);
		treasureR.append({ skeletonKey6 }, CHAPTER_MOTH);
		treasureR.append({ skeletonKey7, skeletonKey7P }, CHAPTER_MOTH);
		treasureR.append({ skeletonKey8 }, CHAPTER_MOTH);
		treasureR.append({ skeletonKey9 }, CHAPTER_NETHERWORLD);
		treasureR.append({ skeletonKey10 }, CHAPTER_NETHERWORLD);
		treasureR.append({ skeletonKey11 }, CHAPTER_NETHERWORLD);
	}

	if (state == Treasure::Items::Random)
	{
		treasureR.random();
	}
	else // Random By Category
	{
		treasureR.randomByCategory();
	}
}

void Treasure::setValuables(Treasure::Valuables state, bool randomUsableValuables) const
{
	TreasureRandomizer treasureR{ m_game };

	const auto& offsetF{ m_game->offset().file };
	
	const FileOffset
		knowledge1{ File::SCENE_FIELD1_FOREST1_SCE00_SBH, offsetF.scene_field1_forest1_sce00_sbh.knowledge1 },
		knowledge2{ File::SCENE_FIELD1_FOREST1_SCE03_SBH, offsetF.scene_field1_forest1_sce03_sbh.knowledge2 },
		knowledge3{ File::SCENE_FIELD1_FOREST1_SCE04_SBH, offsetF.scene_field1_forest1_sce04_sbh.knowledge3 },
		kickleberry{ File::SCENE_FIELD1_FOREST2_SCE00_SBH, offsetF.scene_field1_forest2_sce00_sbh.kickleberry },
		knowledge4{ File::SCENE_FIELD1_FOREST2_SCE01_SBH, offsetF.scene_field1_forest2_sce01_sbh.knowledge4 },
		magicUsage1{ File::SCENE_FIELD1_FOREST2_SCE04_SBH, offsetF.scene_field1_forest2_sce04_sbh.magicUsage1 },
		magicUsage2{ File::SCENE_FIELD1_FOREST2_SCE13_SBH, offsetF.scene_field1_forest2_sce13_sbh.magicUsage2 },
		magicUsage2P{ File::SCENE_FIELD1_FOREST2_SCE13A_SBH, offsetF.scene_field1_forest2_sce13a_sbh.magicUsage2 },
		secretOfKilling{ File::SCENE_FIELD1_FOREST2_SCE14A_SBH, offsetF.scene_field1_forest2_sce14a_sbh.secretOfKilling },
		divineTablet{ File::SCENE_FIELD1_FOREST3_SCE07_SBH, offsetF.scene_field1_forest3_sce07_sbh.divineTablet },
		divineTabletP{ File::SCENE_FIELD1_FOREST3_SCE28_SBH, offsetF.scene_field1_forest3_sce28_sbh.divineTablet },
		minionFang{ File::SCENE_FIELD1_FOREST3_SCE09_SBH, offsetF.scene_field1_forest3_sce09_sbh.minionFang },
		minionFangP{ File::SCENE_FIELD1_FOREST3_SCE30_SBH, offsetF.scene_field1_forest3_sce30_sbh.minionFang },
		knowledge5{ File::SCENE_FIELD1_FOREST3_SCE11_SBH, offsetF.scene_field1_forest3_sce11_sbh.knowledge5 },
		knowledge5P{ File::SCENE_FIELD1_FOREST3_SCE11A_SBH, offsetF.scene_field1_forest3_sce11a_sbh.knowledge5 },
		knowledge5P2{ File::SCENE_FIELD1_FOREST3_SCE32_SBH, offsetF.scene_field1_forest3_sce32_sbh.knowledge5 },
		merging2{ File::SCENE_FIELD1_FOREST3_SCE13_SBH, offsetF.scene_field1_forest3_sce13_sbh.merging2 },
		merging2P{ File::SCENE_FIELD1_FOREST3_SCE13A_SBH, offsetF.scene_field1_forest3_sce13a_sbh.merging2 },
		merging2P2{ File::SCENE_FIELD1_FOREST3_SCE34_SBH, offsetF.scene_field1_forest3_sce34_sbh.merging2 },
		merging1{ File::SCENE_FIELD1_FOREST3_SCE14_SBH, offsetF.scene_field1_forest3_sce14_sbh.merging1 },
		merging1P{ File::SCENE_FIELD1_FOREST3_SCE14A_SBH, offsetF.scene_field1_forest3_sce14a_sbh.merging1 },
		merging1P2{ File::SCENE_FIELD1_FOREST3_SCE35_SBH, offsetF.scene_field1_forest3_sce35_sbh.merging1 },
		fluteOfCapture{ File::SCENE_PSYRUS1_GARAI_SCE00_SBH, offsetF.scene_psyrus1_garai_sce00_sbh.gift_fluteOfCapture },
		tearsOfReese{ File::SCENE_PSYRUS1_GARAI_SCE03_SBH, offsetF.scene_psyrus1_garai_sce03_sbh.gift_tearsOfReese },
		calabasPowder{ File::SCENE_FIELD1_FOREST2_SCE07_SBH , offsetF.scene_field1_forest2_sce07_sbh.gift_calabasPowder },
		captureAmulet{ File::SCENE_PSYRUS1_ZOKUCHO_SCE01A_SBH, offsetF.scene_psyrus1_zokucho_sce01a_sbh.gift_captureAmulet },
		championBelt{ File::SCENE_PSYRUS1_ZOKUCHO_SCE01A_SBH, offsetF.scene_psyrus1_zokucho_sce01a_sbh.gift_championBelt },
		tearsOfReese2{ File::SCENE_FIELD1_FOREST3_SCE14A_SBH, offsetF.scene_field1_forest3_sce14a_sbh.gift_tearsOfReese },
		mahbusCocoon{ File::SCENE_PSYRUS2_GARAI_SCE03_SBH, offsetF.scene_psyrus2_garai_sce03_sbh.gift_mahbusCocoon },
		jewelOfWater{ File::SCENE_FIELD1_FOREST4_SCE19_SBH, offsetF.scene_field1_forest4_sce19_sbh.gift_jewelOfWater },
		jewelOfFire{ File::SCENE_FIELD2_FOREST1_SCE07_SBH, offsetF.scene_field2_forest1_sce07_sbh.gift_jewelOfFire },
		jewelOfWind{ File::SCENE_FIELD2_FOREST2_SCE07_SBH, offsetF.scene_field2_forest2_sce07_sbh.gift_jewelOfWind };

	// Collectable
	treasureR.append({ knowledge1 }, CHAPTER_BEETLE);
	treasureR.append({ knowledge2 }, CHAPTER_BEETLE);
	treasureR.append({ knowledge3 }, CHAPTER_BEETLE);
	treasureR.append({ kickleberry }, CHAPTER_DRAGONFLY);
	treasureR.append({ knowledge4 }, CHAPTER_DRAGONFLY);
	treasureR.append({ magicUsage1 }, CHAPTER_DRAGONFLY);
	treasureR.append({ magicUsage2, magicUsage2P }, CHAPTER_DRAGONFLY);
	treasureR.append({ secretOfKilling }, CHAPTER_DRAGONFLY);
	treasureR.append({ divineTablet, divineTabletP }, CHAPTER_SPIDER);
	treasureR.append({ minionFang, minionFangP }, CHAPTER_SPIDER);
	treasureR.append({ knowledge5, knowledge5P, knowledge5P2 }, CHAPTER_SPIDER);
	treasureR.append({ merging2, merging2P, merging2P2 }, CHAPTER_SPIDER);
	treasureR.append({ merging1, merging1P, merging1P2 }, CHAPTER_SPIDER);
	// Dialogue
	treasureR.append({ fluteOfCapture }, CHAPTER_BEETLE, true);
	treasureR.append({ tearsOfReese }, CHAPTER_BEETLE, true);
	treasureR.append({ calabasPowder }, CHAPTER_DRAGONFLY);
	treasureR.append({ captureAmulet }, CHAPTER_SPIDER_2 , true);
	treasureR.append({ championBelt }, CHAPTER_SPIDER_2, true);
	treasureR.append({ tearsOfReese2 }, CHAPTER_SPIDER_2, true);
	treasureR.append({ mahbusCocoon }, CHAPTER_MOTH, true);
	treasureR.append({ jewelOfWater }, CHAPTER_MOTH, true);
	treasureR.append({ jewelOfFire }, CHAPTER_NETHERWORLD, true);
	treasureR.append({ jewelOfWind }, CHAPTER_NETHERWORLD, true);

	if (randomUsableValuables)
	{
		const FileOffset
			beetleWarp{ File::SCENE_FIELD1_FOREST1_SCE08_SBH, offsetF.scene_field1_forest1_sce08_sbh.beetleWarp },
			dragonflyWarp{ File::SCENE_FIELD1_FOREST2_SCE08_SBH, offsetF.scene_field1_forest2_sce08_sbh.dragonflyWarp },
			spiderWarp{ File::SCENE_FIELD1_FOREST3_SCE10_SBH, offsetF.scene_field1_forest3_sce10_sbh.spiderWarp },
			spiderWarpP{ File::SCENE_FIELD1_FOREST3_SCE31_SBH, offsetF.scene_field1_forest3_sce31_sbh.spiderWarp },
			ocarina{ File::SCENE_FIELD1_FOREST2_SCE07_SBH , offsetF.scene_field1_forest2_sce07_sbh.gift_ocarina };

		// Collectable
		treasureR.append({ beetleWarp }, CHAPTER_BEETLE);
		treasureR.append({ dragonflyWarp }, CHAPTER_DRAGONFLY);
		treasureR.append({ spiderWarp, spiderWarpP }, CHAPTER_SPIDER);
		// Dialogue
		treasureR.append({ ocarina }, CHAPTER_DRAGONFLY, true);
	}

	if (state == Treasure::Valuables::Random)
	{
		treasureR.random();
	}
	else // Random By Category
	{
		treasureR.randomByCategory();
	}
}

void Treasure::setEquipments(Treasure::Equipments state) const
{
	TreasureRandomizer treasureR{ m_game };

	const auto& offsetF{ m_game->offset().file };

	const FileOffset
		aquazor{ File::SCENE_FIELD1_FOREST1_SCE06_SBH, offsetF.scene_field1_forest1_sce06_sbh.aquazor },
		aquazorP{ File::SCENE_FIELD1_FOREST1_SCE06A_SBH, offsetF.scene_field1_forest1_sce06a_sbh.aquazor },
		whiteHorn{ File::SCENE_FIELD1_FOREST3_SCE01_SBH, offsetF.scene_field1_forest3_sce01_sbh.whiteHorn },
		whiteHornP{ File::SCENE_FIELD1_FOREST3_SCE22_SBH, offsetF.scene_field1_forest3_sce22_sbh.whiteHorn },
		whistler{ File::SCENE_FIELD1_FOREST4_SCE04_SBH, offsetF.scene_field1_forest4_sce04_sbh.whistler },
		thunderBolt{ File::SCENE_FIELD2_FOREST2_SCE13_SBH, offsetF.scene_field2_forest2_sce13_sbh.thunderbolt },
		ironHeadband{ File::SCENE_FIELD1_FOREST1_SCE02_SBH, offsetF.scene_field1_forest1_sce02_sbh.chest_metaMugwort + 0x10 },
		silverCrown{ File::SCENE_FIELD1_FOREST2_SCE12_SBH, offsetF.scene_field1_forest2_sce12_sbh.chest_agnisSeal1 + 0x10 },
		stoneChoker{ File::SCENE_FIELD1_FOREST3_SCE05_SBH, offsetF.scene_field1_forest3_sce05_sbh.chest_stoneChoker },
		stoneChokerP{ File::SCENE_FIELD1_FOREST3_SCE26_SBH, offsetF.scene_field1_forest3_sce26_sbh.chest_stoneChoker },
		yellowBead{ File::SCENE_FIELD1_FOREST3_SCE17_SBH, offsetF.scene_field1_forest3_sce17_sbh.chest_yellowBead },
		yellowBeadP{ File::SCENE_FIELD1_FOREST3_SCE38_SBH, offsetF.scene_field1_forest3_sce38_sbh.chest_yellowBead },
		sparrowRuff{ File::SCENE_FIELD1_FOREST4_SCE08_SBH, offsetF.scene_field1_forest4_sce08_sbh.chest_urvysBlessing + 8 },
		hawkRuff{ File::SCENE_FIELD2_FOREST1_SCE02_SBH, offsetF.scene_field2_forest1_sce02_sbh.chest_varysBlessing + 0x10 },
		nagiBelt{ File::SCENE_PSYRUS1_LOOKOUT_SCE01_SBH, offsetF.scene_psyrus1_lookout_sce01_sbh.gift_nagiBelt },
		lifeStealer{ File::SCENE_PSYRUS1_KAJIYA_SCE00B_SBH, offsetF.scene_psyrus1_kajiya_sce00b_sbh.gift_lifeStealer },
		riketzGarb{ File::SCENE_PSYRUS2_GARAI_SCE03_SBH ,offsetF.scene_psyrus2_garai_sce03_sbh.gift_riketzGarb };

	// Collectable
	treasureR.append({ aquazor, aquazorP }, CHAPTER_BEETLE);
	treasureR.append({ whiteHorn, whiteHornP }, CHAPTER_SPIDER);
	treasureR.append({ whistler }, CHAPTER_MOTH);
	treasureR.append({ thunderBolt }, CHAPTER_NETHERWORLD);
	// Chest
	treasureR.append({ ironHeadband }, CHAPTER_BEETLE);
	treasureR.append({ silverCrown }, CHAPTER_DRAGONFLY);
	treasureR.append({ stoneChoker, stoneChokerP }, CHAPTER_SPIDER);
	treasureR.append({ yellowBead, yellowBeadP }, CHAPTER_SPIDER_2);
	treasureR.append({ sparrowRuff }, CHAPTER_MOTH);
	treasureR.append({ hawkRuff }, CHAPTER_NETHERWORLD);
	// Dialogue
	treasureR.append({ nagiBelt }, CHAPTER_SPIDER, true);
	treasureR.append({ lifeStealer }, CHAPTER_SPIDER, true);
	treasureR.append({ riketzGarb }, CHAPTER_MOTH, true);

	if (state == Treasure::Equipments::Random)
	{
		treasureR.random();
	}
	else // Random By Category
	{
		treasureR.randomByCategory();
	}
}

void Treasure::setBattle(bool shuffleSkeletonKeys) const
{
	struct EntityTreasure
	{
		struct
		{
			s16 rate;
			CategoryItem<u8> content;
		} one, two;
	};

	static constexpr auto totalEntityTreasures{ Entity::totalEntity - 2 }; // -2 no TX_08 and TX_09

	std::vector<EntityTreasure> treasuresList(totalEntityTreasures);
	std::unique_ptr<RawFile> file;
	u32 offset;

	if (m_game->isVersion(Version::NtscJ1))
	{
		file = m_game->file(File::OVER_BTEND_BIN);
		offset = m_game->offset().file.over_btend_bin.entityTreasures;
	}
	else
	{
		file = m_game->file(File::OVER_MENU_BIN);
		offset = m_game->offset().file.over_menu_bin.entityTreasures;
	}

	file->read(offset, treasuresList.data(), treasuresList.size() * sizeof(EntityTreasure));

	std::vector<EntityTreasure> treasures(totalEntityTreasures);

	auto isASkeletonKey = [](CategoryItem<u8> item)
	{
		return item.category == Item::CATEGORY_ITEM && item.id == ITEM_SKELETON_KEY;
	};

	if (!shuffleSkeletonKeys)
	{
		for (std::size_t i{}; i < treasuresList.size(); ++i)
		{
			if (isASkeletonKey(treasuresList[i].one.content))
			{
				treasures[i] = treasuresList[i];
				treasuresList.erase(treasuresList.begin() + i);
			}
		}
	}

	for (auto& treasure : treasures)
	{
		if (!shuffleSkeletonKeys && isASkeletonKey(treasure.one.content))
		{
			continue;
		}

		const auto rngTreasures{ m_game->random()->generate(treasuresList.size() - 1) };
		treasure = treasuresList[rngTreasures];
		treasuresList.erase(treasuresList.begin() + rngTreasures);
	}

	file->write(offset, *treasures.data(), treasures.size() * sizeof(EntityTreasure));
}