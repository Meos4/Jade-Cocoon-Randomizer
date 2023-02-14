#include "Offset.hpp"

#include "Common/JcrException.hpp"

#include <type_traits>

const static Offset::File
fileNtscJ1
{
	.executable
	{
		.entityModelsBehavior = 0x00069C4C,
		.entityStats = 0x00069240,
		.entityMagics = 0x00069438,
		.entitySpecials = 0x0006951C,
		.entityDamageModifiers = 0x00069990,
		.hudColors = 0x00074FAC,
		.afterGetStatsJump = 0x00002734,
		.afterFramerateLimiter = 0x00000BE0,
		.hpMpBars = 0x0002656C,
		.bodyEnhancementFn = 0x000145E0,
		.setVideoModeArgument = 0x00000924,
		.stretchingY = 0x000485AC,
		.tableOfWeaponAnimationFileId = 0x000686F4,
		.tableOfWeaponStats = 0x0006AD00,
		.tableOfArmorStats = 0x0006B0D4,
		.tableOfOtherStats = 0x0006B364,
		.tableOfWeaponCharacteristic = 0x0006AF28,
		.tableOfArmorsAppearanceId = 0x0006B23C,
		.tableOfYPositionMinionStatsFrame = 0x00070E78,
		.drawMinionStatsFrameFn = 0x0002883C,
		.drawMinionTIMStatsFrameFn = 0x000286E0,
		.tableOfModelAnimations = 0x0006B980,
		.tableOfModelAnimationsPtr = 0x00070180,
		.actorBehaviorFn = 0x000228C8,
		.mainFn = 0x00000800,
		.branchSetVatolkaSpecials = 0x00014004,
		.tableOfLevantFluteStylingsId = 0x0006A774,
		.cc_begin = 0x000AD800 + NtscJ1::customCodeShift
	},
	.over_battle_bin
	{
		.endOfBattle = 0x00000590,
		.tableOfBattleWeaponsEffectPtr = 0x000000F4,
		.setBattleWeaponsEffectFn = 0x00003E78,
		.endOfBattleSetSceneFn = 0x00000740,
		.storyMinionAI = 0,
		.minionAgniaId = 0,
		.delfanelEarthBossId = 0,
		.tuturisFireBossId = 0,
		.seterianAirBossId = 0,
		.cushidraFireBossId = 0,
		.tickerUlvi = 0,
		.kolnaAdMumuls = 0,
		.captureSuccess = 0,
		.getFireflyCounterBattle = 0,
	},
	.over_btend_bin
	{
		.entityTreasures = 0x0000177C,
	},
	.over_btlexec_bin
	{
		.captureSuccess = 0x00000CAC,
	},
	.over_encom_bin
	{
		.storyMinionAI = 0x00000128,
		.minionAgniaId = 0x00001AD0,
		.delfanelEarthBossId = 0x00001D50,
		.tuturisFireBossId = 0x00001F04,
		.seterianAirBossId = 0x000020C4,
		.cushidraFireBossId = 0x000024D4,
		.tickerUlvi = 0x00000354,
		.kolnaAdMumuls = 0x000003CC,
	},
	.over_game_bin
	{
		.tableOfStoryMinionsId = 0x0000DCC0,
		.spawnStory = 0x00004AA4,
		.fireMapBonus = 0x00004C54,
		.battleMaps = 0x0000D90C,
		.levelCapEC = 0x000048E8,
		.levantAnimArgs = 0x00007C5C,
		.chooseMagicLevantAnim = 0x0000B5C4,
		.levantCastingItemId = 0x0001097C,
		.tableOfWeaponAnimationsId = 0x00010AB0,
		.isQuantityLimitReachedFn = 0x0000CF1C,
		.itemShopBuyFn = 0x0000CD84,
		.tableOfMapsBehavior = 0x0000E4F0,
		.readFlagRotateSpawn = 0,
	},
	.over_menu_bin
	{
		.entityTreasures = 0,
	},
	.over_plcom_bin
	{
		.getFireflyCounterBattle = 0x00002150,
	},
	.over_title_bin
	{
		.levantStats = 0x00001C98,
		.setLevantGarb = 0x000016C0,
		.initMapNewGame = 0x000011B0,
	},
	.over_wpnshop_bin
	{
		.items = 0x0000669C,
		.itemShopQuantityLimitFn = 0x000056B0,
		.equipmentShopQuantityLimitFn = 0x00005580,
		.initEternalCorridorShopFn = 0x00002734,
	},
	.scene_field1_forest1_sce00_sbh
	{
		.knowledge1 = 0x00000F2C,
	},
	.scene_field1_forest1_sce01_sbh
	{
		.shabLiquor = 0x00001420,
		.treeBranchBehaviorFn = 0x000001C0,
	},
	.scene_field1_forest1_sce02_sbh
	{
		.chest_metaMugwort = 0x00000EF4,
	},
	.scene_field1_forest1_sce03_sbh
	{
		.knowledge2 = 0x00000F38,
	},
	.scene_field1_forest1_sce04_sbh
	{
		.knowledge3 = 0x00000F40,
	},
	.scene_field1_forest1_sce05_sbh
	{
		.skeletonKey = 0x00000F30,
	},
	.scene_field1_forest1_sce06_sbh
	{
		.aquazor = 0x00000F30,
	},
	.scene_field1_forest1_sce06a_sbh
	{
		.aquazor = 0x00001318,
		.tableOfVoices = 0x000014D4,
	},
	.scene_field1_forest1_sce08_sbh
	{
		.beetleWarp = 0x00002118,
	},
	.scene_field1_forest1_sce09_sbh
	{
		.tableOfVoices = 0x00000B60,
	},
	.scene_field1_forest2_sce00_sbh
	{
		.kickleberry = 0x00000F34,
	},
	.scene_field1_forest2_sce01_sbh
	{
		.knowledge4 = 0x00000F24,
	},
	.scene_field1_forest2_sce03_sbh
	{
		.skeletonKey = 0x00000F34,
	},
	.scene_field1_forest2_sce04_sbh
	{
		.magicUsage1 = 0x00000F3C,
	},
	.scene_field1_forest2_sce05_sbh
	{
		.chest_chestNutOil = 0x00000EF4,
	},
	.scene_field1_forest2_sce06_sbh
	{
		.larvaKeyBehaviorFn = 0x00000468,
		.doorBehaviorFn = 0x00000178,
	},
	.scene_field1_forest2_sce07_sbh
	{
		.gift_calabasPowder = 0x00001D68,
		.gift_ocarina = 0x000026F0,
		.gift_specialLiquor = 0x00001D78,
		.sleepSpore = 0x000018B8,
		.tableOfVoices = 0x00001AC8,
	},
	.scene_field1_forest2_sce07a_sbh
	{
		.sleepSpore = 0x00001484,
	},
	.scene_field1_forest2_sce08_sbh
	{
		.dragonflyWarp = 0x00002118,
	},
	.scene_field1_forest2_sce10_sbh
	{
		.skeletonKey = 0x0000140C,
		.bridgeBehaviorFn = 0x000001B0,
	},
	.scene_field1_forest2_sce12_sbh
	{
		.chest_agnisSeal1 = 0x00000EF4,
	},
	.scene_field1_forest2_sce13_sbh
	{
		.magicUsage2 = 0x00000F28,
	},
	.scene_field1_forest2_sce13a_sbh
	{
		.magicUsage2 = 0x00001290,
		.tableOfVoices = 0x00001650,
	},
	.scene_field1_forest2_sce14a_sbh
	{
		.secretOfKilling = 0x0000174C,
		.tableOfVoices = 0x0000192C,
		.kikinakReturnOstId = 0x00001B8C,
	},
	.scene_field1_forest3_sce01_sbh
	{
		.whiteHorn = 0x00000F3C,
	},
	.scene_field1_forest3_sce03_sbh
	{
		.ikariPowder = 0x00000F24,
	},
	.scene_field1_forest3_sce03a_sbh
	{
		.ikariPowder = 0x00001284,
		.tableOfVoices = 0x00001644,
	},
	.scene_field1_forest3_sce05_sbh
	{
		.chest_stoneChoker = 0x00000EF4,
	},
	.scene_field1_forest3_sce06_sbh
	{
		.skeletonKey = 0x00000F2C,
	},
	.scene_field1_forest3_sce07_sbh
	{
		.divineTablet = 0x00000F34,
	},
	.scene_field1_forest3_sce09_sbh
	{
		.minionFang = 0x00000F38,
	},
	.scene_field1_forest3_sce10_sbh
	{
		.spiderWarp = 0x00002118,
	},
	.scene_field1_forest3_sce11_sbh
	{
		.knowledge5 = 0x00001294,
		.tableOfVoices = 0x00001378,
	},
	.scene_field1_forest3_sce11a_sbh
	{
		.knowledge5 = 0x00001298,
		.tableOfVoices = 0x0000137C,
	},
	.scene_field1_forest3_sce12_sbh
	{
		.tableOfVoices = 0x000010B8,
	},
	.scene_field1_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x000010B8,
	},
	.scene_field1_forest3_sce13_sbh
	{
		.merging2 = 0x00001280,
		.tableOfVoices = 0x00001364,
	},
	.scene_field1_forest3_sce13a_sbh
	{
		.merging2 = 0x00001280,
		.tableOfVoices = 0x00001364,
	},
	.scene_field1_forest3_sce14_sbh
	{
		.merging1 = 0x0000128C,
		.tableOfVoices = 0x00001370,
	},
	.scene_field1_forest3_sce14a_sbh
	{
		.gift_tearsOfReese = 0x00001570,
		.merging1 = 0x00001288,
		.tableOfVoices = 0x0000136C,
	},
	.scene_field1_forest3_sce16_sbh
	{
		.excellentLiquor = 0x00000F2C,
	},
	.scene_field1_forest3_sce17_sbh
	{
		.chest_yellowBead = 0x000013CC,
	},
	.scene_field1_forest3_sce18_sbh
	{
		.skeletonKey = 0x00000F40,
	},
	.scene_field1_forest3_sce20_sbh
	{
		.tableOfVoices = 0x00001414,
	},
	.scene_field1_forest3_sce22_sbh
	{
		.whiteHorn = 0x00000F3C,
	},
	.scene_field1_forest3_sce24_sbh
	{
		.ikariPowder = 0x00000F24,
	},
	.scene_field1_forest3_sce26_sbh
	{
		.chest_stoneChoker = 0x00000EF4,
	},
	.scene_field1_forest3_sce27_sbh
	{
		.skeletonKey = 0x00000F2C,
	},
	.scene_field1_forest3_sce28_sbh
	{
		.divineTablet = 0x00000F34,
	},
	.scene_field1_forest3_sce30_sbh
	{
		.minionFang = 0x00000F38,
	},
	.scene_field1_forest3_sce31_sbh
	{
		.spiderWarp = 0x00001C50,
	},
	.scene_field1_forest3_sce32_sbh
	{
		.knowledge5 = 0x00001298,
		.tableOfVoices = 0x000013AC,
	},
	.scene_field1_forest3_sce33_sbh
	{
		.tableOfVoices = 0x00000C10,
	},
	.scene_field1_forest3_sce34_sbh
	{
		.merging2 = 0x00001280,
		.tableOfVoices = 0x00001394,
	},
	.scene_field1_forest3_sce35_sbh
	{
		.merging1 = 0x00000F34,
	},
	.scene_field1_forest3_sce37_sbh
	{
		.excellentLiquor = 0x00000F2C,
	},
	.scene_field1_forest3_sce38_sbh
	{
		.chest_yellowBead = 0x00000EF4,
	},
	.scene_field1_forest3_sce39_sbh
	{
		.skeletonKey = 0x00000F40,
	},
	.scene_field1_forest4_sce00_sbh
	{
		.excellentLiquor = 0x00000F40,
	},
	.scene_field1_forest4_sce01_sbh
	{
		.chest_bitterMugwort = 0x00000EF4,
	},
	.scene_field1_forest4_sce03_sbh
	{
		.chest_shishiudoOil = 0x00000EF4,
	},
	.scene_field1_forest4_sce04_sbh
	{
		.whistler = 0x0000140C,
	},
	.scene_field1_forest4_sce05_sbh
	{
		.greatWalnut = 0x00000F40,
	},
	.scene_field1_forest4_sce06_sbh
	{
		.skeletonKey = 0x00000F2C,
	},
	.scene_field1_forest4_sce07_sbh
	{
		.pupaKeyBehaviorFn = 0x00000654,
		.doorBehaviorFn = 0x00000178,
	},
	.scene_field1_forest4_sce08_sbh
	{
		.chest_urvysBlessing = 0x00000EF4,
	},
	.scene_field1_forest4_sce09_sbh
	{
		.skeletonKey = 0x00000F24,
	},
	.scene_field1_forest4_sce09a_sbh
	{
		.skeletonKey = 0x0000128C,
		.tableOfVoices = 0x0000164C,
	},
	.scene_field1_forest4_sce10_sbh
	{
		.skeletonKey = 0x00000F24,
	},
	.scene_field1_forest4_sce12_sbh
	{
		.tableOfVoices = 0x00001B60,
		.kemuelTempleGate = 0x00025334
	},
	.scene_field1_forest4_sce14_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_field1_forest4_sce15_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_field1_forest4_sce16_sbh
	{
		.tableOfVoices = 0x00001BAC,
	},
	.scene_field1_forest4_sce16a_sbh
	{
		.tableOfVoices = 0x00001C24,
	},
	.scene_field1_forest4_sce16b_sbh
	{
		.tableOfVoices = 0x00001C24,
	},
	.scene_field1_forest4_sce17_sbh
	{
		.tableOfVoices = 0x000013B0,
	},
	.scene_field1_forest4_sce18_sbh
	{
		.tableOfVoices = 0x00001524,
	},
	.scene_field1_forest4_sce19_sbh
	{
		.gift_jewelOfWater = 0x00002160,
		.tableOfVoices = 0x000018BC,
	},
	.scene_field1_gate_sce00_sbh
	{
		.forestOstId = 0x00001F94,
		.forestOstIdFromBehind = 0x00000048,
	},
	.scene_field1_gate_sce00a_sbh
	{
		.korisMinionId = 0x000024E0,
		.gift_mugwort = 0x00002C10,
		.forestOstId = 0x000039D8,
		.forestOstIdFromBehind = 0x00000068,
		.tableOfVoices = 0x00002234,
	},
	.scene_field1_gate_sce00b_sbh
	{
		.forestOstId = 0x00002100,
		.forestOstIdFromBehind = 0x00000048,
	},
	.scene_field1_gate_sce00c_sbh
	{
		.forestOstId = 0x00002CA0,
		.forestOstIdFromBehind = 0x00000064,
	},
	.scene_field1_gate_sce01_sbh
	{
		.tableOfVoices = 0x00000A1C,
	},
	.scene_field1_gate_sce06_sbh
	{
		.forestOstId = 0x00002CA8,
		.forestOstIdFromBehind = 0x00000064,
		.tableOfVoices = 0x000027D4,
	},
	.scene_field2_forest1_sce02_sbh
	{
		.chest_varysBlessing = 0x00000EF4,
	},
	.scene_field2_forest1_sce06_sbh
	{
		.skeletonKey = 0x00000F2C,
	},
	.scene_field2_forest1_sce07_sbh
	{
		.gift_jewelOfFire = 0x00002570,
		.tableOfVoices = 0x000019F4,
	},
	.scene_field2_forest2_sce06_sbh
	{
		.chest_bletillaOil = 0x00000EF4,
	},
	.scene_field2_forest2_sce07_sbh
	{
		.gift_jewelOfWind = 0x00002E60,
		.tableOfVoices = 0x00002064,
	},
	.scene_field2_forest2_sce13_sbh
	{
		.thunderbolt = 0x00000F2C,
	},
	.scene_field2_forest2_sce14_sbh
	{
		.skeletonKey = 0x000013FC,
	},
	.scene_field2_forest3_sce07_sbh
	{
		.skeletonKey = 0x00000F2C,
	},
	.scene_field2_forest3_sce11a_sbh
	{
		.tableOfVoices = 0x00001574,
	},
	.scene_field2_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x00001578,
	},
	.scene_field2_forest3_sce13a_sbh
	{
		.tableOfVoices = 0x00001574,
	},
	.scene_field2_forest3_sce16_sbh
	{
		.chest_bitterMugwort = 0x00000EF4,
	},
	.scene_field2_forest3_sce18_sbh
	{
		.chest_urvysBlessing = 0x00000EF4,
	},
	.scene_field2_forest3_sce20_sbh
	{
		.tableOfVoices = 0x00001A98,
	},
	.scene_field2_forest3_sce22a_sbh
	{
		.tableOfVoices = 0x00001DB8,
	},
	.scene_other_clear_sce00_sbh
	{
		.tableOfVoices = 0x00000D90,
	},
	.scene_other_clear_sce03_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_other_clear_sce04_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_other_dream_sce00_sbh
	{
		.dreamManIntroVoice = 0x00001514,
		.tableOfVoices = 0x00001450,
	},
	.scene_other_dream_sce00a_sbh
	{
		.dreamMinionId = 0x00001338,
		.dreamManIntroVoice = 0x000012B2,
		.tableOfVoices = 0x00001274,
	},
	.scene_other_hunting_sce00_sbh
	{
		.neutralSpawnEC = 0x00000CD4,
		.magicsEC = 0x00001DA4,
		.statsEC = 0x00002EA4,
		.specialsEC = 0x0000200C,
		.bossSpecialsEC = 0x00002B4C,
		.bossDamageModifiersEC = 0x00002EF4,
	},
	.scene_psyrus1_cemetery_sce00_sbh
	{
		.tableOfVoices = 0x00000E20,
	},
	.scene_psyrus1_cemetery_sce00a_sbh
	{
		.tableOfVoices = 0x00000E18,
	},
	.scene_psyrus1_cemetery_sce00b_sbh
	{
		.tableOfVoices = 0x00000E70,
	},
	.scene_psyrus1_cemetery_sce00c_sbh
	{
		.tableOfVoices = 0x00000E38,
	},
	.scene_psyrus1_garai_sce00_sbh
	{
		.gift_fluteOfCapture = 0x00001588,
		.tableOfVoices = 0x000010F4,
	},
	.scene_psyrus1_garai_sce01_sbh
	{
		.tableOfVoices = 0x00002120,
	},
	.scene_psyrus1_garai_sce01a_sbh
	{
		.tableOfVoices = 0x00002120,
	},
	.scene_psyrus1_garai_sce01b_sbh
	{
		.tableOfVoices = 0x0000214C,
	},
	.scene_psyrus1_garai_sce01c_sbh
	{
		.tableOfVoices = 0x000021C0,
	},
	.scene_psyrus1_garai_sce03_sbh
	{
		.gift_tearsOfReese = 0x00001030,
		.tableOfVoices = 0x00000DB4,
	},
	.scene_psyrus1_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000EF0,
	},
	.scene_psyrus1_kajiya_sce00a_sbh
	{
		.tableOfVoices = 0x00000EB8,
	},
	.scene_psyrus1_kajiya_sce00b_sbh
	{
		.gift_lifeStealer = 0x00001240,
		.tableOfVoices = 0x00000EF0,
	},
	.scene_psyrus1_kajiya_sce00c_sbh
	{
		.tableOfVoices = 0x00000EF4,
	},
	.scene_psyrus1_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00000EC8,
	},
	.scene_psyrus1_lebant_sce00a_sbh
	{
		.tableOfVoices = 0x00001434,
	},
	.scene_psyrus1_lebant_sce00c_sbh
	{
		.tableOfVoices = 0x00001410,
	},
	.scene_psyrus1_lebant_sce00d_sbh
	{
		.tableOfVoices = 0x00001410,
	},
	.scene_psyrus1_lookout_sce00_sbh
	{
		.tableOfVoices = 0x00000A58,
	},
	.scene_psyrus1_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000B04,
	},
	.scene_psyrus1_lookout_sce00b_sbh
	{
		.tableOfVoices = 0x00000B04,
	},
	.scene_psyrus1_lookout_sce00c_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus1_lookout_sce01_sbh
	{
		.gift_nagiBelt = 0x000016EC,
		.tableOfVoices = 0x00000B74,
	},
	.scene_psyrus1_zokucho_sce01_sbh
	{
		.tableOfVoices = 0x0000166C,
	},
	.scene_psyrus1_zokucho_sce01a_sbh
	{
		.gift_championBelt = 0x00001E90,
		.gift_captureAmulet = 0x00001EA0,
		.tableOfVoices = 0x0000160C,
	},
	.scene_psyrus1_zokucho_sce05_sbh
	{
		.gift_huntersEarring = 0x00001FC4,
		.tableOfVoices = 0x00001518,
	},
	.scene_psyrus2_garai_sce00_sbh
	{
		.tableOfVoices = 0x00002188,
	},
	.scene_psyrus2_garai_sce03_sbh
	{
		.gift_mahbusCocoon = 0x00001F38,
		.gift_riketzGarb = 0x00002038,
		.tableOfVoices = 0x00001538,
	},
	.scene_psyrus2_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000EF4,
	},
	.scene_psyrus2_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00001410,
	},
	.scene_psyrus2_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus2_lookout_sce01_sbh
	{
		.tableOfVoices = 0x000018C0,
	},
	.scene_psyrus2_lookout_sce01a_sbh
	{
		.tableOfVoices = 0x00000EFC,
	},
	.scene_psyrus2_zokucho_sce03_sbh
	{
		.tableOfVoices = 0x00001704,
	}
},
fileNtscJ2
{
	.executable
	{
		.entityModelsBehavior = 0x00069228,
		.entityStats = 0x0006881C,
		.entityMagics = 0x00068A14,
		.entitySpecials = 0x00068AF8,
		.entityDamageModifiers = 0x00068F6C,
		.hudColors = 0x00074B98,
		.afterGetStatsJump = 0x00002734,
		.afterFramerateLimiter = 0x00000BC4,
		.hpMpBars = 0x00026A54,
		.bodyEnhancementFn = 0x00014644,
		.setVideoModeArgument = 0x00000924,
		.stretchingY = 0x00048448,
		.tableOfWeaponAnimationFileId = 0x00067CD0,
		.tableOfWeaponStats = 0x0006A2DC,
		.tableOfArmorStats = 0x0006A6B0,
		.tableOfOtherStats = 0x0006A940,
		.tableOfWeaponCharacteristic = 0x0006A504,
		.tableOfArmorsAppearanceId = 0x0006A818,
		.tableOfYPositionMinionStatsFrame = 0x0007018C,
		.drawMinionStatsFrameFn = 0x000287F4,
		.drawMinionTIMStatsFrameFn = 0x00028698,
		.tableOfModelAnimations = 0x0006AF5C,
		.tableOfModelAnimationsPtr = 0x0006F75C,
		.actorBehaviorFn = 0x00022CF0,
		.mainFn = 0x00000800,
		.branchSetVatolkaSpecials = 0x00014064,
		.tableOfLevantFluteStylingsId = 0x00069D50,
		.cc_begin = 0x000B6000,
	},
	.over_battle_bin
	{
		.endOfBattle = 0x00002480,
		.tableOfBattleWeaponsEffectPtr = 0x00000134,
		.setBattleWeaponsEffectFn = 0x0000791C,
		.endOfBattleSetSceneFn = 0x00002630,
		.storyMinionAI = 0x0000C000,
		.minionAgniaId = 0x0000D9B0,
		.delfanelEarthBossId = 0x0000DC38,
		.tuturisFireBossId = 0x0000DDEC,
		.seterianAirBossId = 0x0000DFAC,
		.cushidraFireBossId = 0x0000E3BC,
		.tickerUlvi = 0x0000C22C,
		.kolnaAdMumuls = 0x0000C2A4,	
		.captureSuccess = 0x00006544,
		.getFireflyCounterBattle = 0x0000F050,		
	},
	.over_btend_bin
	{
		.entityTreasures = 0,
	},
	.over_btlexec_bin
	{
		.captureSuccess = 0,
	},
	.over_encom_bin
	{
		.storyMinionAI = 0,
		.minionAgniaId = 0,
		.delfanelEarthBossId = 0,
		.tuturisFireBossId = 0,
		.seterianAirBossId = 0,
		.cushidraFireBossId = 0,
		.tickerUlvi = 0,
		.kolnaAdMumuls = 0,
	},
	.over_game_bin
	{
		.tableOfStoryMinionsId = 0x0000DF84,
		.spawnStory = 0x00004B28,
		.fireMapBonus = 0x00004CD8,
		.battleMaps = 0x0000DBD0,
		.levelCapEC = 0x0000496C,
		.levantAnimArgs = 0x00007E28,
		.chooseMagicLevantAnim = 0x0000B888,
		.levantCastingItemId = 0x00010C18,
		.tableOfWeaponAnimationsId = 0x00010D4C,
		.isQuantityLimitReachedFn = 0x0000D1E0,
		.itemShopBuyFn = 0x0000D048,
		.tableOfMapsBehavior = 0x0000E7B4,
		.readFlagRotateSpawn = 0,
	},
	.over_menu_bin
	{
		.entityTreasures = 0x00008110,
	},
	.over_plcom_bin
	{
		.getFireflyCounterBattle = 0,
	},
	.over_title_bin
	{
		.levantStats = 0x000020B0,
		.setLevantGarb = 0x00001A58,
		.initMapNewGame = 0x00001554,
	},
	.over_wpnshop_bin
	{
		.items = 0x00006AB4,
		.itemShopQuantityLimitFn = 0x000059B8,
		.equipmentShopQuantityLimitFn = 0x00005888,
		.initEternalCorridorShopFn = 0x00002844,
	},
	.scene_field1_forest1_sce00_sbh
	{
		.knowledge1 = 0x00000F2C,
	},
	.scene_field1_forest1_sce01_sbh
	{
		.shabLiquor = 0x00001420,
		.treeBranchBehaviorFn = 0x000001C0,
	},
	.scene_field1_forest1_sce02_sbh
	{
		.chest_metaMugwort = 0x00000EF4,
	},
	.scene_field1_forest1_sce03_sbh
	{
		.knowledge2 = 0x00000F38,
	},
	.scene_field1_forest1_sce04_sbh
	{
		.knowledge3 = 0x00000F40,
	},
	.scene_field1_forest1_sce05_sbh
	{
		.skeletonKey = 0x00000F30,
	},
	.scene_field1_forest1_sce06_sbh
	{
		.aquazor = 0x00000F30,
	},
	.scene_field1_forest1_sce06a_sbh
	{
		.aquazor = 0x00001318,
		.tableOfVoices = 0x000014D4,
	},
	.scene_field1_forest1_sce08_sbh
	{
		.beetleWarp = 0x00002118,
	},
	.scene_field1_forest1_sce09_sbh
	{
		.tableOfVoices = 0x00000B60,
	},
	.scene_field1_forest2_sce00_sbh
	{
		.kickleberry = 0x00000F34,
	},
	.scene_field1_forest2_sce01_sbh
	{
		.knowledge4 = 0x00000F24,
	},
	.scene_field1_forest2_sce03_sbh
	{
		.skeletonKey = 0x00000F34,
	},
	.scene_field1_forest2_sce04_sbh
	{
		.magicUsage1 = 0x00000F3c,
	},
	.scene_field1_forest2_sce05_sbh
	{
		.chest_chestNutOil = 0x00000EF4,
	},
	.scene_field1_forest2_sce06_sbh
	{
		.larvaKeyBehaviorFn = 0x00000468,
		.doorBehaviorFn = 0x00000178,
	},
	.scene_field1_forest2_sce07_sbh
	{
		.gift_calabasPowder = 0x00001D68,
		.gift_ocarina = 0x000026F0,
		.gift_specialLiquor = 0x00001D78,
		.sleepSpore = 0x000018B8,
		.tableOfVoices = 0x00001AC8,
	},
	.scene_field1_forest2_sce07a_sbh
	{
		.sleepSpore = 0x00001484,
	},
	.scene_field1_forest2_sce08_sbh
	{
		.dragonflyWarp = 0x00002118,
	},
	.scene_field1_forest2_sce10_sbh
	{
		.skeletonKey = 0x0000140C,
		.bridgeBehaviorFn = 0x000001B0,
	},
	.scene_field1_forest2_sce12_sbh
	{
		.chest_agnisSeal1 = 0x00000EF4,
	},
	.scene_field1_forest2_sce13_sbh
	{
		.magicUsage2 = 0x00000F28,
	},
	.scene_field1_forest2_sce13a_sbh
	{
		.magicUsage2 = 0x00001290,
		.tableOfVoices = 0x00001650,
	},
	.scene_field1_forest2_sce14a_sbh
	{
		.secretOfKilling = 0x0000174C,
		.tableOfVoices = 0x0000192C,
		.kikinakReturnOstId = 0x00001B8C,
	},
	.scene_field1_forest3_sce01_sbh
	{
		.whiteHorn = 0x00000F3c,
	},
	.scene_field1_forest3_sce03_sbh
	{
		.ikariPowder = 0x00000F24,
	},
	.scene_field1_forest3_sce03a_sbh
	{
		.ikariPowder = 0x00001284,
		.tableOfVoices = 0x00001644,
	},
	.scene_field1_forest3_sce05_sbh
	{
		.chest_stoneChoker = 0x00000EF4,
	},
	.scene_field1_forest3_sce06_sbh
	{
		.skeletonKey = 0x00000F2C,
	},
	.scene_field1_forest3_sce07_sbh
	{
		.divineTablet = 0x00000F34,
	},
	.scene_field1_forest3_sce09_sbh
	{
		.minionFang = 0x00000F38,
	},
	.scene_field1_forest3_sce10_sbh
	{
		.spiderWarp = 0x00002118,
	},
	.scene_field1_forest3_sce11_sbh
	{
		.knowledge5 = 0x00001294,
		.tableOfVoices = 0x00001378,
	},
	.scene_field1_forest3_sce11a_sbh
	{
		.knowledge5 = 0x00001298,
		.tableOfVoices = 0x0000137C,
	},
	.scene_field1_forest3_sce12_sbh
	{
		.tableOfVoices = 0x000010B8,
	},
	.scene_field1_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x000010B8,
	},
	.scene_field1_forest3_sce13_sbh
	{
		.merging2 = 0x00001280,
		.tableOfVoices = 0x00001364,
	},
	.scene_field1_forest3_sce13a_sbh
	{
		.merging2 = 0x00001280,
		.tableOfVoices = 0x00001364,
	},
	.scene_field1_forest3_sce14_sbh
	{
		.merging1 = 0x0000128C,
		.tableOfVoices = 0x00001370,
	},
	.scene_field1_forest3_sce14a_sbh
	{
		.gift_tearsOfReese = 0x00001570,
		.merging1 = 0x00001288,
		.tableOfVoices = 0x0000136C,
	},
	.scene_field1_forest3_sce16_sbh
	{
		.excellentLiquor = 0x00000F2C,
	},
	.scene_field1_forest3_sce17_sbh
	{
		.chest_yellowBead = 0x000013CC,
	},
	.scene_field1_forest3_sce18_sbh
	{
		.skeletonKey = 0x00000F40,
	},
	.scene_field1_forest3_sce20_sbh
	{
		.tableOfVoices = 0x00001414,
	},
	.scene_field1_forest3_sce22_sbh
	{
		.whiteHorn = 0x00000F3C,
	},
	.scene_field1_forest3_sce24_sbh
	{
		.ikariPowder = 0x00000F24,
	},
	.scene_field1_forest3_sce26_sbh
	{
		.chest_stoneChoker = 0x00000EF4,
	},
	.scene_field1_forest3_sce27_sbh
	{
		.skeletonKey = 0x00000F2C,
	},
	.scene_field1_forest3_sce28_sbh
	{
		.divineTablet = 0x00000F34,
	},
	.scene_field1_forest3_sce30_sbh
	{
		.minionFang = 0x00000F38,
	},
	.scene_field1_forest3_sce31_sbh
	{
		.spiderWarp = 0x00001C50,
	},
	.scene_field1_forest3_sce32_sbh
	{
		.knowledge5 = 0x00001298,
		.tableOfVoices = 0x000013AC,
	},
	.scene_field1_forest3_sce33_sbh
	{
		.tableOfVoices = 0x00000C10,
	},
	.scene_field1_forest3_sce34_sbh
	{
		.merging2 = 0x00001280,
		.tableOfVoices = 0x00001394,
	},
	.scene_field1_forest3_sce35_sbh
	{
		.merging1 = 0x00000F34,
	},
	.scene_field1_forest3_sce37_sbh
	{
		.excellentLiquor = 0x00000F2C,
	},
	.scene_field1_forest3_sce38_sbh
	{
		.chest_yellowBead = 0x00000EF4,
	},
	.scene_field1_forest3_sce39_sbh
	{
		.skeletonKey = 0x00000F40,
	},
	.scene_field1_forest4_sce00_sbh
	{
		.excellentLiquor = 0x00000F40,
	},
	.scene_field1_forest4_sce01_sbh
	{
		.chest_bitterMugwort = 0x00000EF4,
	},
	.scene_field1_forest4_sce03_sbh
	{
		.chest_shishiudoOil = 0x00000EF4,
	},
	.scene_field1_forest4_sce04_sbh
	{
		.whistler = 0x0000140C,
	},
	.scene_field1_forest4_sce05_sbh
	{
		.greatWalnut = 0x00000F40,
	},
	.scene_field1_forest4_sce06_sbh
	{
		.skeletonKey = 0x00000F2C,
	},
	.scene_field1_forest4_sce07_sbh
	{
		.pupaKeyBehaviorFn = 0x00000654,
		.doorBehaviorFn = 0x00000178,
	},
	.scene_field1_forest4_sce08_sbh
	{
		.chest_urvysBlessing = 0x00000EF4,
	},
	.scene_field1_forest4_sce09_sbh
	{
		.skeletonKey = 0x00000F24,
	},
	.scene_field1_forest4_sce09a_sbh
	{
		.skeletonKey = 0x0000128C,
		.tableOfVoices = 0x0000164C,
	},
	.scene_field1_forest4_sce10_sbh
	{
		.skeletonKey = 0x00000F24,
	},
	.scene_field1_forest4_sce12_sbh
	{
		.tableOfVoices = 0x00001B7C,
		.kemuelTempleGate = 0x00025350
	},
	.scene_field1_forest4_sce14_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_field1_forest4_sce15_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_field1_forest4_sce16_sbh
	{
		.tableOfVoices = 0x00001BAC,
	},
	.scene_field1_forest4_sce16a_sbh
	{
		.tableOfVoices = 0x00001C24,
	},
	.scene_field1_forest4_sce16b_sbh
	{
		.tableOfVoices = 0x00001C24,
	},
	.scene_field1_forest4_sce17_sbh
	{
		.tableOfVoices = 0x000013B0,
	},
	.scene_field1_forest4_sce18_sbh
	{
		.tableOfVoices = 0x00001524,
	},
	.scene_field1_forest4_sce19_sbh
	{
		.gift_jewelOfWater = 0x00002170,
		.tableOfVoices = 0x000018BC,
	},
	.scene_field1_gate_sce00_sbh
	{
		.forestOstId = 0x00001F94,
		.forestOstIdFromBehind = 0x00000048,
	},
	.scene_field1_gate_sce00a_sbh
	{
		.korisMinionId = 0x000024E0,
		.gift_mugwort = 0x00002C10,
		.forestOstId = 0x000039D8,
		.forestOstIdFromBehind = 0x00000068,
		.tableOfVoices = 0x00002234,
	},
	.scene_field1_gate_sce00b_sbh
	{
		.forestOstId = 0x00002100,
		.forestOstIdFromBehind = 0x00000048,
	},
	.scene_field1_gate_sce00c_sbh
	{
		.forestOstId = 0x00002CA0,
		.forestOstIdFromBehind = 0x00000064,
	},
	.scene_field1_gate_sce01_sbh
	{
		.tableOfVoices = 0x00000A1C,
	},
	.scene_field1_gate_sce06_sbh
	{
		.forestOstId = 0x00002CA8,
		.forestOstIdFromBehind = 0x00000064,
		.tableOfVoices = 0x000027D4,
	},
	.scene_field2_forest1_sce02_sbh
	{
		.chest_varysBlessing = 0x00000EF4,
	},
	.scene_field2_forest1_sce06_sbh
	{
		.skeletonKey = 0x00000F2C,
	},
	.scene_field2_forest1_sce07_sbh
	{
		.gift_jewelOfFire = 0x00002590,
		.tableOfVoices = 0x000019F4,
	},
	.scene_field2_forest2_sce06_sbh
	{
		.chest_bletillaOil = 0x00000EF4,
	},
	.scene_field2_forest2_sce07_sbh
	{
		.gift_jewelOfWind = 0x00002EA0,
		.tableOfVoices = 0x00002064,
	},
	.scene_field2_forest2_sce13_sbh
	{
		.thunderbolt = 0x00000F2C,
	},
	.scene_field2_forest2_sce14_sbh
	{
		.skeletonKey = 0x000013FC,
	},
	.scene_field2_forest3_sce07_sbh
	{
		.skeletonKey = 0x00000F2C,
	},
	.scene_field2_forest3_sce11a_sbh
	{
		.tableOfVoices = 0x00001574,
	},
	.scene_field2_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x00001578,
	},
	.scene_field2_forest3_sce13a_sbh
	{
		.tableOfVoices = 0x00001574,
	},
	.scene_field2_forest3_sce16_sbh
	{
		.chest_bitterMugwort = 0x00000EF4,
	},
	.scene_field2_forest3_sce18_sbh
	{
		.chest_urvysBlessing = 0x00000EF4,
	},
	.scene_field2_forest3_sce20_sbh
	{
		.tableOfVoices = 0x00001A98,
	},
	.scene_field2_forest3_sce22a_sbh
	{
		.tableOfVoices = 0x00001DB8,
	},
	.scene_other_clear_sce00_sbh
	{
		.tableOfVoices = 0x00000D90,
	},
	.scene_other_clear_sce03_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_other_clear_sce04_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_other_dream_sce00_sbh
	{
		.dreamManIntroVoice = 0x00001514,
		.tableOfVoices = 0x00001450,
	},
	.scene_other_dream_sce00a_sbh
	{
		.dreamMinionId = 0x00001338,
		.dreamManIntroVoice = 0x000012B2,
		.tableOfVoices = 0x00001274,
	},
	.scene_other_hunting_sce00_sbh
	{
		.neutralSpawnEC = 0x00000D50,
		.magicsEC = 0x00001EAC,
		.statsEC = 0x00002FAC,
		.specialsEC = 0x00002114,
		.bossSpecialsEC = 0x00002C54,
		.bossDamageModifiersEC = 0x00002FFC,
	},
	.scene_psyrus1_cemetery_sce00_sbh
	{
		.tableOfVoices = 0x00000E20,
	},
	.scene_psyrus1_cemetery_sce00a_sbh
	{
		.tableOfVoices = 0x00000E18,
	},
	.scene_psyrus1_cemetery_sce00b_sbh
	{
		.tableOfVoices = 0x00000E70,
	},
	.scene_psyrus1_cemetery_sce00c_sbh
	{
		.tableOfVoices = 0x00000E38,
	},
	.scene_psyrus1_garai_sce00_sbh
	{
		.gift_fluteOfCapture = 0x00001588,
		.tableOfVoices = 0x000010F4,
	},
	.scene_psyrus1_garai_sce01_sbh
	{
		.tableOfVoices = 0x00002120,
	},
	.scene_psyrus1_garai_sce01a_sbh
	{
		.tableOfVoices = 0x00002120,
	},
	.scene_psyrus1_garai_sce01b_sbh
	{
		.tableOfVoices = 0x0000214C,
	},
	.scene_psyrus1_garai_sce01c_sbh
	{
		.tableOfVoices = 0x000021C0,
	},
	.scene_psyrus1_garai_sce03_sbh
	{
		.gift_tearsOfReese = 0x00001030,
		.tableOfVoices = 0x00000DB4,
	},
	.scene_psyrus1_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000EF8,
	},
	.scene_psyrus1_kajiya_sce00a_sbh
	{
		.tableOfVoices = 0x00000EC0,
	},
	.scene_psyrus1_kajiya_sce00b_sbh
	{
		.gift_lifeStealer = 0x00001274,
		.tableOfVoices = 0x00000EF8,
	},
	.scene_psyrus1_kajiya_sce00c_sbh
	{
		.tableOfVoices = 0x00000EFC,
	},
	.scene_psyrus1_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00000EC8,
	},
	.scene_psyrus1_lebant_sce00a_sbh
	{
		.tableOfVoices = 0x00001434,
	},
	.scene_psyrus1_lebant_sce00c_sbh
	{
		.tableOfVoices = 0x00001410,
	},
	.scene_psyrus1_lebant_sce00d_sbh
	{
		.tableOfVoices = 0x00001410,
	},
	.scene_psyrus1_lookout_sce00_sbh
	{
		.tableOfVoices = 0x00000A58,
	},
	.scene_psyrus1_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000B04,
	},
	.scene_psyrus1_lookout_sce00b_sbh
	{
		.tableOfVoices = 0x00000B04,
	},
	.scene_psyrus1_lookout_sce00c_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus1_lookout_sce01_sbh
	{
		.gift_nagiBelt = 0x000016EC,
		.tableOfVoices = 0x00000B74,
	},
	.scene_psyrus1_zokucho_sce01_sbh
	{
		.tableOfVoices = 0x0000166C,
	},
	.scene_psyrus1_zokucho_sce01a_sbh
	{
		.gift_championBelt = 0x00001E90,
		.gift_captureAmulet = 0x00001EA0,
		.tableOfVoices = 0x0000160C,
	},
	.scene_psyrus1_zokucho_sce05_sbh
	{
		.gift_huntersEarring = 0x00001FC4,
		.tableOfVoices = 0x00001518,
	},
	.scene_psyrus2_garai_sce00_sbh
	{
		.tableOfVoices = 0x00002188,
	},
	.scene_psyrus2_garai_sce03_sbh
	{
		.gift_mahbusCocoon = 0x00001F38,
		.gift_riketzGarb = 0x00002038,
		.tableOfVoices = 0x00001538,
	},
	.scene_psyrus2_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000EFC,
	},
	.scene_psyrus2_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00001410,
	},
	.scene_psyrus2_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus2_lookout_sce01_sbh
	{
		.tableOfVoices = 0x000018C0,
	},
	.scene_psyrus2_lookout_sce01a_sbh
	{
		.tableOfVoices = 0x00000EFC,
	},
	.scene_psyrus2_zokucho_sce03_sbh
	{
		.tableOfVoices = 0x00001704,
	}
},
fileNtscU
{
	.executable
	{
		.entityModelsBehavior = 0x0006C530,
		.entityStats = 0x0006ACB4,
		.entityMagics = 0x0006AEAC,
		.entitySpecials = 0x0006B1F8,
		.entityDamageModifiers = 0x0006C274,
		.hudColors = 0x000777EC,
		.afterGetStatsJump = 0x0000273C,
		.afterFramerateLimiter = 0x00000BC4,
		.hpMpBars = 0x0001C274,
		.bodyEnhancementFn = 0x0000A40C,
		.setVideoModeArgument = 0x00000924,
		.stretchingY = 0x000490A0,
		.tableOfWeaponAnimationFileId = 0x00069F08,
		.tableOfWeaponStats = 0x0006D84C,
		.tableOfArmorStats = 0x0006DC20,
		.tableOfOtherStats = 0x0006DEB0,
		.tableOfWeaponCharacteristic = 0x0006DA74,
		.tableOfArmorsAppearanceId = 0x0006DD88,
		.tableOfYPositionMinionStatsFrame = 0x000736E8,
		.drawMinionStatsFrameFn = 0x0001E348,
		.drawMinionTIMStatsFrameFn = 0x0002DA3C,
		.tableOfModelAnimations = 0x0006E4CC,
		.tableOfModelAnimationsPtr = 0x00072CCC,
		.actorBehaviorFn = 0x000184F8,
		.mainFn = 0x00000800,
		.branchSetVatolkaSpecials = 0x00009E2C,
		.tableOfLevantFluteStylingsId = 0x0006D2C0,
		.cc_begin = 0x000BA800,
	},
	.over_battle_bin
	{
		.endOfBattle = 0x00003FD8,
		.tableOfBattleWeaponsEffectPtr = 0x00000134,
		.setBattleWeaponsEffectFn = 0x00009438,
		.endOfBattleSetSceneFn = 0x00004188,
		.storyMinionAI = 0x0000DB1C,
		.minionAgniaId = 0x0000F4CC,
		.delfanelEarthBossId = 0x0000F754,
		.tuturisFireBossId = 0x0000F908,
		.seterianAirBossId = 0x0000FAC8,
		.cushidraFireBossId = 0x0000FED8,
		.tickerUlvi = 0x0000DD48,
		.kolnaAdMumuls = 0x0000DDC0,	
		.captureSuccess = 0x00008060,
		.getFireflyCounterBattle = 0x00010B6C,		
	},
	.over_btend_bin
	{
		.entityTreasures = 0,
	},
	.over_btlexec_bin
	{
		.captureSuccess = 0,
	},
	.over_encom_bin
	{
		.storyMinionAI = 0,
		.minionAgniaId = 0,
		.delfanelEarthBossId = 0,
		.tuturisFireBossId = 0,
		.seterianAirBossId = 0,
		.cushidraFireBossId = 0,
		.tickerUlvi = 0,
		.kolnaAdMumuls = 0,
	},
	.over_game_bin
	{
		.tableOfStoryMinionsId = 0x0000DE74,
		.spawnStory = 0x00004C34,
		.fireMapBonus = 0x00004E08,
		.battleMaps = 0x0000DAC0,
		.levelCapEC = 0x000049BC,
		.levantAnimArgs = 0x00007E10,
		.chooseMagicLevantAnim = 0x0000B778,
		.levantCastingItemId = 0x00010B08,
		.tableOfWeaponAnimationsId = 0x00010C3C,
		.isQuantityLimitReachedFn = 0x0000D0D0,
		.itemShopBuyFn = 0x0000CF38,
		.tableOfMapsBehavior = 0x0000E6A4,
		.readFlagRotateSpawn = 0x00004BD0,
	},
	.over_menu_bin
	{
		.entityTreasures = 0x0000A9C0,
	},
	.over_plcom_bin
	{
		.getFireflyCounterBattle = 0,
	},
	.over_title_bin
	{
		.levantStats = 0x000021BC,
		.setLevantGarb = 0x00001B64,
		.initMapNewGame = 0x00001660,
	},
	.over_wpnshop_bin
	{
		.items = 0x00008B2C,
		.itemShopQuantityLimitFn = 0x000078A8,
		.equipmentShopQuantityLimitFn = 0x00007778,
		.initEternalCorridorShopFn = 0x0000476C,
	},
	.scene_field1_forest1_sce00_sbh
	{
		.knowledge1 = 0x00000F94,
	},
	.scene_field1_forest1_sce01_sbh
	{
		.shabLiquor = 0x00001494,
		.treeBranchBehaviorFn = 0x00000238,
	},
	.scene_field1_forest1_sce02_sbh
	{
		.chest_metaMugwort = 0x00000F1C,
	},
	.scene_field1_forest1_sce03_sbh
	{
		.knowledge2 = 0x00000F9C,
	},
	.scene_field1_forest1_sce04_sbh
	{
		.knowledge3 = 0x00000FDC,
	},
	.scene_field1_forest1_sce05_sbh
	{
		.skeletonKey = 0x00000F74,
	},
	.scene_field1_forest1_sce06_sbh
	{
		.aquazor = 0x00000F94,
	},
	.scene_field1_forest1_sce06a_sbh
	{
		.aquazor = 0x00001384,
		.tableOfVoices = 0x00001540,
	},
	.scene_field1_forest1_sce08_sbh
	{
		.beetleWarp = 0x00002190,
	},
	.scene_field1_forest1_sce09_sbh
	{
		.tableOfVoices = 0x00000B64,
	},
	.scene_field1_forest2_sce00_sbh
	{
		.kickleberry = 0x00000FB4,
	},
	.scene_field1_forest2_sce01_sbh
	{
		.knowledge4 = 0x00000F8C,
	},
	.scene_field1_forest2_sce03_sbh
	{
		.skeletonKey = 0x00000FB4,
	},
	.scene_field1_forest2_sce04_sbh
	{
		.magicUsage1 = 0x00000F8C,
	},
	.scene_field1_forest2_sce05_sbh
	{
		.chest_chestNutOil = 0x00000F1C,
	},
	.scene_field1_forest2_sce06_sbh
	{
		.larvaKeyBehaviorFn = 0x000004E8,
		.doorBehaviorFn = 0x000001F8,
	},
	.scene_field1_forest2_sce07_sbh
	{
		.gift_calabasPowder = 0x00001DE4,
		.gift_ocarina = 0x0000276C,
		.gift_specialLiquor = 0x00001DF4,
		.sleepSpore = 0x00001934,
		.tableOfVoices = 0x00001B44,
	},
	.scene_field1_forest2_sce07a_sbh
	{
		.sleepSpore = 0x00001508,
	},
	.scene_field1_forest2_sce08_sbh
	{
		.dragonflyWarp = 0x00002190,
	},
	.scene_field1_forest2_sce10_sbh
	{
		.skeletonKey = 0x00001474,
		.bridgeBehaviorFn = 0x00000218,
	},
	.scene_field1_forest2_sce12_sbh
	{
		.chest_agnisSeal1 = 0x00000F1C,
	},
	.scene_field1_forest2_sce13_sbh
	{
		.magicUsage2 = 0x00000F94,
	},
	.scene_field1_forest2_sce13a_sbh
	{
		.magicUsage2 = 0x00001304,
		.tableOfVoices = 0x000016C4,
	},
	.scene_field1_forest2_sce14a_sbh
	{
		.secretOfKilling = 0x000017AC,
		.tableOfVoices = 0x0000198C,
		.kikinakReturnOstId = 0x00001BEC,
	},
	.scene_field1_forest3_sce01_sbh
	{
		.whiteHorn = 0x00000FD4,
	},
	.scene_field1_forest3_sce03_sbh
	{
		.ikariPowder = 0x00000F74,
	},
	.scene_field1_forest3_sce03a_sbh
	{
		.ikariPowder = 0x000012DC,
		.tableOfVoices = 0x0000169C,
	},
	.scene_field1_forest3_sce05_sbh
	{
		.chest_stoneChoker = 0x00000F1C,
	},
	.scene_field1_forest3_sce06_sbh
	{
		.skeletonKey = 0x00000FB4,
	},
	.scene_field1_forest3_sce07_sbh
	{
		.divineTablet = 0x00000F9C,
	},
	.scene_field1_forest3_sce09_sbh
	{
		.minionFang = 0x00000F84,
	},
	.scene_field1_forest3_sce10_sbh
	{
		.spiderWarp = 0x00002180,
	},
	.scene_field1_forest3_sce11_sbh
	{
		.knowledge5 = 0x000012F8,
		.tableOfVoices = 0x000013DC,
	},
	.scene_field1_forest3_sce11a_sbh
	{
		.knowledge5 = 0x000012FC,
		.tableOfVoices = 0x000013E0,
	},
	.scene_field1_forest3_sce12_sbh
	{
		.tableOfVoices = 0x000010C4,
	},
	.scene_field1_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x000010C4,
	},
	.scene_field1_forest3_sce13_sbh
	{
		.merging2 = 0x00001318,
		.tableOfVoices = 0x000013FC,
	},
	.scene_field1_forest3_sce13a_sbh
	{
		.merging2 = 0x00001318,
		.tableOfVoices = 0x000013FC,
	},
	.scene_field1_forest3_sce14_sbh
	{
		.merging1 = 0x000012FC,
		.tableOfVoices = 0x000013E0,
	},
	.scene_field1_forest3_sce14a_sbh
	{
		.gift_tearsOfReese = 0x000015E0,
		.merging1 = 0x000012F8,
		.tableOfVoices = 0x000013DC,
	},
	.scene_field1_forest3_sce16_sbh
	{
		.excellentLiquor = 0x00000F94,
	},
	.scene_field1_forest3_sce17_sbh
	{
		.chest_yellowBead = 0x000013FC,
	},
	.scene_field1_forest3_sce18_sbh
	{
		.skeletonKey = 0x00000FE4,
	},
	.scene_field1_forest3_sce20_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_field1_forest3_sce22_sbh
	{
		.whiteHorn = 0x00000FD4,
	},
	.scene_field1_forest3_sce24_sbh
	{
		.ikariPowder = 0x00000F74,
	},
	.scene_field1_forest3_sce26_sbh
	{
		.chest_stoneChoker = 0x00000F1C,
	},
	.scene_field1_forest3_sce27_sbh
	{
		.skeletonKey = 0x00000FB4,
	},
	.scene_field1_forest3_sce28_sbh
	{
		.divineTablet = 0x00000F9C,
	},
	.scene_field1_forest3_sce30_sbh
	{
		.minionFang = 0x00000F84,
	},
	.scene_field1_forest3_sce31_sbh
	{
		.spiderWarp = 0x00001CB0,
	},
	.scene_field1_forest3_sce32_sbh
	{
		.knowledge5 = 0x000012FC,
		.tableOfVoices = 0x00001410,
	},
	.scene_field1_forest3_sce33_sbh
	{
		.tableOfVoices = 0x00000C18,
	},
	.scene_field1_forest3_sce34_sbh
	{
		.merging2 = 0x00001318,
		.tableOfVoices = 0x0000142C,
	},
	.scene_field1_forest3_sce35_sbh
	{
		.merging1 = 0x00000F9C,
	},
	.scene_field1_forest3_sce37_sbh
	{
		.excellentLiquor = 0x00000F94,
	},
	.scene_field1_forest3_sce38_sbh
	{
		.chest_yellowBead = 0x00000F1C,
	},
	.scene_field1_forest3_sce39_sbh
	{
		.skeletonKey = 0x00000FE4,
	},
	.scene_field1_forest4_sce00_sbh
	{
		.excellentLiquor = 0x00000FBC,
	},
	.scene_field1_forest4_sce01_sbh
	{
		.chest_bitterMugwort = 0x00000F1C,
	},
	.scene_field1_forest4_sce03_sbh
	{
		.chest_shishiudoOil = 0x00000F1C,
	},
	.scene_field1_forest4_sce04_sbh
	{
		.whistler = 0x00001474,
	},
	.scene_field1_forest4_sce05_sbh
	{
		.greatWalnut = 0x00000F94,
	},
	.scene_field1_forest4_sce06_sbh
	{
		.skeletonKey = 0x00000F7C,
	},
	.scene_field1_forest4_sce07_sbh
	{
		.pupaKeyBehaviorFn = 0x000006D4,
		.doorBehaviorFn = 0x000001F8,
	},
	.scene_field1_forest4_sce08_sbh
	{
		.chest_urvysBlessing = 0x00000F1C,
	},
	.scene_field1_forest4_sce09_sbh
	{
		.skeletonKey = 0x00000F74,
	},
	.scene_field1_forest4_sce09a_sbh
	{
		.skeletonKey = 0x000012E4,
		.tableOfVoices = 0x000016A4,
	},
	.scene_field1_forest4_sce10_sbh
	{
		.skeletonKey = 0x00000F8C,
	},
	.scene_field1_forest4_sce12_sbh
	{
		.tableOfVoices = 0x00001B80,
		.kemuelTempleGate = 0x00025498
	},
	.scene_field1_forest4_sce14_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_field1_forest4_sce15_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_field1_forest4_sce16_sbh
	{
		.tableOfVoices = 0x00001BB8,
	},
	.scene_field1_forest4_sce16a_sbh
	{
		.tableOfVoices = 0x00001C30,
	},
	.scene_field1_forest4_sce16b_sbh
	{
		.tableOfVoices = 0x00001C30,
	},
	.scene_field1_forest4_sce17_sbh
	{
		.tableOfVoices = 0x000013B4,
	},
	.scene_field1_forest4_sce18_sbh
	{
		.tableOfVoices = 0x00001520,
	},
	.scene_field1_forest4_sce19_sbh
	{
		.gift_jewelOfWater = 0x0000216C,
		.tableOfVoices = 0x000018B8,
	},
	.scene_field1_gate_sce00_sbh
	{
		.forestOstId = 0x00001FA4,
		.forestOstIdFromBehind = 0x00000054,
	},
	.scene_field1_gate_sce00a_sbh
	{
		.korisMinionId = 0x000024F4,
		.gift_mugwort = 0x00002C24,
		.forestOstId = 0x000039EC,
		.forestOstIdFromBehind = 0x00000078,
		.tableOfVoices = 0x00002248,
	},
	.scene_field1_gate_sce00b_sbh
	{
		.forestOstId = 0x00002110,
		.forestOstIdFromBehind = 0x00000054,
	},
	.scene_field1_gate_sce00c_sbh
	{
		.forestOstId = 0x00002CB0,
		.forestOstIdFromBehind = 0x00000074,
	},
	.scene_field1_gate_sce01_sbh
	{
		.tableOfVoices = 0x00000A1C,
	},
	.scene_field1_gate_sce06_sbh
	{
		.forestOstId = 0x00002CB8,
		.forestOstIdFromBehind = 0x00000074,
		.tableOfVoices = 0x000027E4,
	},
	.scene_field2_forest1_sce02_sbh
	{
		.chest_varysBlessing = 0x00000F1C,
	},
	.scene_field2_forest1_sce06_sbh
	{
		.skeletonKey = 0x00000FA4,
	},
	.scene_field2_forest1_sce07_sbh
	{
		.gift_jewelOfFire = 0x0000259C,
		.tableOfVoices = 0x00001A00,
	},
	.scene_field2_forest2_sce06_sbh
	{
		.chest_bletillaOil = 0x00000F1C,
	},
	.scene_field2_forest2_sce07_sbh
	{
		.gift_jewelOfWind = 0x00002EB8,
		.tableOfVoices = 0x0000207C,
	},
	.scene_field2_forest2_sce13_sbh
	{
		.thunderbolt = 0x00000F74,
	},
	.scene_field2_forest2_sce14_sbh
	{
		.skeletonKey = 0x0000147C,
	},
	.scene_field2_forest3_sce07_sbh
	{
		.skeletonKey = 0x00000FAC,
	},
	.scene_field2_forest3_sce11a_sbh
	{
		.tableOfVoices = 0x00001584,
	},
	.scene_field2_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x0000158C,
	},
	.scene_field2_forest3_sce13a_sbh
	{
		.tableOfVoices = 0x00001584,
	},
	.scene_field2_forest3_sce16_sbh
	{
		.chest_bitterMugwort = 0x00000F1C,
	},
	.scene_field2_forest3_sce18_sbh
	{
		.chest_urvysBlessing = 0x00000F1C,
	},
	.scene_field2_forest3_sce20_sbh
	{
		.tableOfVoices = 0x00001AB0,
	},
	.scene_field2_forest3_sce22a_sbh
	{
		.tableOfVoices = 0x00001DC8,
	},
	.scene_other_clear_sce00_sbh
	{
		.tableOfVoices = 0x00000D90,
	},
	.scene_other_clear_sce03_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_other_clear_sce04_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_other_dream_sce00_sbh
	{
		.dreamManIntroVoice = 0x00001524,
		.tableOfVoices = 0x00001460,
	},
	.scene_other_dream_sce00a_sbh
	{
		.dreamMinionId = 0x00001344,
		.dreamManIntroVoice = 0x000012BE,
		.tableOfVoices = 0x00001280,
	},
	.scene_other_hunting_sce00_sbh
	{
		.neutralSpawnEC = 0x00000D50,
		.magicsEC = 0x00001EAC,
		.statsEC = 0x00002FAC,
		.specialsEC = 0x00002114,
		.bossSpecialsEC = 0x00002C54,
		.bossDamageModifiersEC = 0x00002FFC,
	},
	.scene_psyrus1_cemetery_sce00_sbh
	{
		.tableOfVoices = 0x00000E28,
	},
	.scene_psyrus1_cemetery_sce00a_sbh
	{
		.tableOfVoices = 0x00000E20,
	},
	.scene_psyrus1_cemetery_sce00b_sbh
	{
		.tableOfVoices = 0x00000E70,
	},
	.scene_psyrus1_cemetery_sce00c_sbh
	{
		.tableOfVoices = 0x00000E40,
	},
	.scene_psyrus1_garai_sce00_sbh
	{
		.gift_fluteOfCapture = 0x0000159C,
		.tableOfVoices = 0x00001108,
	},
	.scene_psyrus1_garai_sce01_sbh
	{
		.tableOfVoices = 0x00002134,
	},
	.scene_psyrus1_garai_sce01a_sbh
	{
		.tableOfVoices = 0x00002134,
	},
	.scene_psyrus1_garai_sce01b_sbh
	{
		.tableOfVoices = 0x00002160,
	},
	.scene_psyrus1_garai_sce01c_sbh
	{
		.tableOfVoices = 0x000021D4,
	},
	.scene_psyrus1_garai_sce03_sbh
	{
		.gift_tearsOfReese = 0x00001038,
		.tableOfVoices = 0x00000DBC,
	},
	.scene_psyrus1_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F00,
	},
	.scene_psyrus1_kajiya_sce00a_sbh
	{
		.tableOfVoices = 0x00000EC8,
	},
	.scene_psyrus1_kajiya_sce00b_sbh
	{
		.gift_lifeStealer = 0x0000127C,
		.tableOfVoices = 0x00000F00,
	},
	.scene_psyrus1_kajiya_sce00c_sbh
	{
		.tableOfVoices = 0x00000F08,
	},
	.scene_psyrus1_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00000ED4,
	},
	.scene_psyrus1_lebant_sce00a_sbh
	{
		.tableOfVoices = 0x0000143C,
	},
	.scene_psyrus1_lebant_sce00c_sbh
	{
		.tableOfVoices = 0x00001418,
	},
	.scene_psyrus1_lebant_sce00d_sbh
	{
		.tableOfVoices = 0x00001418,
	},
	.scene_psyrus1_lookout_sce00_sbh
	{
		.tableOfVoices = 0x00000A5C,
	},
	.scene_psyrus1_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000B08,
	},
	.scene_psyrus1_lookout_sce00b_sbh
	{
		.tableOfVoices = 0x00000B08,
	},
	.scene_psyrus1_lookout_sce00c_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus1_lookout_sce01_sbh
	{
		.gift_nagiBelt = 0x000016F0,
		.tableOfVoices = 0x00000B78,
	},
	.scene_psyrus1_zokucho_sce01_sbh
	{
		.tableOfVoices = 0x0000167C,
	},
	.scene_psyrus1_zokucho_sce01a_sbh
	{
		.gift_championBelt = 0x00001E9C,
		.gift_captureAmulet = 0x00001EAC,
		.tableOfVoices = 0x00001618,
	},
	.scene_psyrus1_zokucho_sce05_sbh
	{
		.gift_huntersEarring = 0x00001FD0,
		.tableOfVoices = 0x00001524,
	},
	.scene_psyrus2_garai_sce00_sbh
	{
		.tableOfVoices = 0x00002198,
	},
	.scene_psyrus2_garai_sce03_sbh
	{
		.gift_mahbusCocoon = 0x00001F40,
		.gift_riketzGarb = 0x00002040,
		.tableOfVoices = 0x00001540,
	},
	.scene_psyrus2_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F08,
	},
	.scene_psyrus2_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00001418,
	},
	.scene_psyrus2_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus2_lookout_sce01_sbh
	{
		.tableOfVoices = 0x000018C0,
	},
	.scene_psyrus2_lookout_sce01a_sbh
	{
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus2_zokucho_sce03_sbh
	{
		.tableOfVoices = 0x00001710,
	}
},
filePalEn
{
	.executable
	{
		.entityModelsBehavior = 0x0006C454,
		.entityStats = 0x0006ABD8,
		.entityMagics = 0x0006ADD0,
		.entitySpecials = 0x0006B11C,
		.entityDamageModifiers = 0x0006C198,
		.hudColors = 0x000770DC,
		.afterGetStatsJump = 0x0000273C,
		.afterFramerateLimiter = 0x00000BC4,
		.hpMpBars = 0x0001C2D0,
		.bodyEnhancementFn = 0x0000A40C,
		.setVideoModeArgument = 0x00000924,
		.stretchingY = 0x000490C4,
		.tableOfWeaponAnimationFileId = 0x00069E2C,
		.tableOfWeaponStats = 0x0006D770,
		.tableOfArmorStats = 0x0006DB44,
		.tableOfOtherStats = 0x0006DDD4,
		.tableOfWeaponCharacteristic = 0x0006D998,
		.tableOfArmorsAppearanceId = 0x0006DCAC,
		.tableOfYPositionMinionStatsFrame = 0x0007360C,
		.drawMinionStatsFrameFn = 0x0001E3A4,
		.drawMinionTIMStatsFrameFn = 0x0001E298,
		.tableOfModelAnimations = 0x0006E3F0,
		.tableOfModelAnimationsPtr = 0x00072BF0,
		.actorBehaviorFn = 0x00018560,
		.mainFn = 0x00000800,
		.branchSetVatolkaSpecials = 0x00009E2C,
		.tableOfLevantFluteStylingsId = 0x0006D1E4,
		.cc_begin = 0x000BB000,
	},
	.over_battle_bin
	{
		.endOfBattle = 0x00003FD8,
		.tableOfBattleWeaponsEffectPtr = 0x00000134,
		.setBattleWeaponsEffectFn = 0x00009438,
		.endOfBattleSetSceneFn = 0x00004188,
		.storyMinionAI = 0x0000DB1C,
		.minionAgniaId = 0x0000F4CC,
		.delfanelEarthBossId = 0x0000F754,
		.tuturisFireBossId = 0x0000F908,
		.seterianAirBossId = 0x0000FAC8,
		.cushidraFireBossId = 0x0000FED8,
		.tickerUlvi = 0x0000DD48,
		.kolnaAdMumuls = 0x0000DDC0,	
		.captureSuccess = 0x00008060,
		.getFireflyCounterBattle = 0x00010B6C,
	},
	.over_btend_bin
	{
		.entityTreasures = 0,
	},
	.over_btlexec_bin
	{
		.captureSuccess = 0,
	},
	.over_encom_bin
	{
		.storyMinionAI = 0,
		.minionAgniaId = 0,
		.delfanelEarthBossId = 0,
		.tuturisFireBossId = 0,
		.seterianAirBossId = 0,
		.cushidraFireBossId = 0,
		.tickerUlvi = 0,
		.kolnaAdMumuls = 0,
	},
	.over_game_bin
	{
		.tableOfStoryMinionsId = 0x0000E0B4,
		.spawnStory = 0x00004C34,
		.fireMapBonus = 0x00004E08,
		.battleMaps = 0x0000DD00,
		.levelCapEC = 0x000049BC,
		.levantAnimArgs = 0x00007F58,
		.chooseMagicLevantAnim = 0x0000B9B8,
		.levantCastingItemId = 0x00010D48,
		.tableOfWeaponAnimationsId = 0x00010E7C,
		.isQuantityLimitReachedFn = 0x0000D310,
		.itemShopBuyFn = 0x0000D178,
		.tableOfMapsBehavior = 0x0000E8E4,
		.readFlagRotateSpawn = 0x00004BD0,
	},
	.over_menu_bin
	{
		.entityTreasures = 0x0000A9E8,
	},
	.over_plcom_bin
	{
		.getFireflyCounterBattle = 0,
	},
	.over_title_bin
	{
		.levantStats = 0x00002600,
		.setLevantGarb = 0x00001F94,
		.initMapNewGame = 0x00001AFC,
	},
	.over_wpnshop_bin
	{
		.items = 0x00008B2C,
		.itemShopQuantityLimitFn = 0x000078A8,
		.equipmentShopQuantityLimitFn = 0x00007778,
		.initEternalCorridorShopFn = 0x0000476C,
	},
	.scene_field1_forest1_sce00_sbh
	{
		.knowledge1 = 0x00000F94,
	},
	.scene_field1_forest1_sce01_sbh
	{
		.shabLiquor = 0x00001494,
		.treeBranchBehaviorFn = 0x00000238,
	},
	.scene_field1_forest1_sce02_sbh
	{
		.chest_metaMugwort = 0x00000F1C,
	},
	.scene_field1_forest1_sce03_sbh
	{
		.knowledge2 = 0x00000F9C,
	},
	.scene_field1_forest1_sce04_sbh
	{
		.knowledge3 = 0x00000FDC,
	},
	.scene_field1_forest1_sce05_sbh
	{
		.skeletonKey = 0x00000F74,
	},
	.scene_field1_forest1_sce06_sbh
	{
		.aquazor = 0x00000F94,
	},
	.scene_field1_forest1_sce06a_sbh
	{
		.aquazor = 0x00001384,
		.tableOfVoices = 0x00001540,
	},
	.scene_field1_forest1_sce08_sbh
	{
		.beetleWarp = 0x00002188,
	},
	.scene_field1_forest1_sce09_sbh
	{
		.tableOfVoices = 0x00000B64,
	},
	.scene_field1_forest2_sce00_sbh
	{
		.kickleberry = 0x00000FB4,
	},
	.scene_field1_forest2_sce01_sbh
	{
		.knowledge4 = 0x00000F8C,
	},
	.scene_field1_forest2_sce03_sbh
	{
		.skeletonKey = 0x00000FB4,
	},
	.scene_field1_forest2_sce04_sbh
	{
		.magicUsage1 = 0x00000F8C,
	},
	.scene_field1_forest2_sce05_sbh
	{
		.chest_chestNutOil = 0x00000F1C,
	},
	.scene_field1_forest2_sce06_sbh
	{
		.larvaKeyBehaviorFn = 0x000004EC,
		.doorBehaviorFn = 0x000001FC,
	},
	.scene_field1_forest2_sce07_sbh
	{
		.gift_calabasPowder = 0x00001DE4,
		.gift_ocarina = 0x0000276C,
		.gift_specialLiquor = 0x00001DF4,
		.sleepSpore = 0x00001934,
		.tableOfVoices = 0x00001B44,
	},
	.scene_field1_forest2_sce07a_sbh
	{
		.sleepSpore = 0x00001508,
	},
	.scene_field1_forest2_sce08_sbh
	{
		.dragonflyWarp = 0x00002190,
	},
	.scene_field1_forest2_sce10_sbh
	{
		.skeletonKey = 0x00001474,
		.bridgeBehaviorFn = 0x00000218,
	},
	.scene_field1_forest2_sce12_sbh
	{
		.chest_agnisSeal1 = 0x00000F1C,
	},
	.scene_field1_forest2_sce13_sbh
	{
		.magicUsage2 = 0x00000F94,
	},
	.scene_field1_forest2_sce13a_sbh
	{
		.magicUsage2 = 0x00001304,
		.tableOfVoices = 0x000016C4,
	},
	.scene_field1_forest2_sce14a_sbh
	{
		.secretOfKilling = 0x000017AC,
		.tableOfVoices = 0x0000198C,
		.kikinakReturnOstId = 0x00001BEC,
	},
	.scene_field1_forest3_sce01_sbh
	{
		.whiteHorn = 0x00000FD4,
	},
	.scene_field1_forest3_sce03_sbh
	{
		.ikariPowder = 0x00000F74,
	},
	.scene_field1_forest3_sce03a_sbh
	{
		.ikariPowder = 0x000012DC,
		.tableOfVoices = 0x0000169C,
	},
	.scene_field1_forest3_sce05_sbh
	{
		.chest_stoneChoker = 0x00000F1C,
	},
	.scene_field1_forest3_sce06_sbh
	{
		.skeletonKey = 0x00000FB4,
	},
	.scene_field1_forest3_sce07_sbh
	{
		.divineTablet = 0x00000F9C,
	},
	.scene_field1_forest3_sce09_sbh
	{
		.minionFang = 0x00000F84,
	},
	.scene_field1_forest3_sce10_sbh
	{
		.spiderWarp = 0x00002180,
	},
	.scene_field1_forest3_sce11_sbh
	{
		.knowledge5 = 0x000012F8,
		.tableOfVoices = 0x000013DC,
	},
	.scene_field1_forest3_sce11a_sbh
	{
		.knowledge5 = 0x000012FC,
		.tableOfVoices = 0x000013E0,
	},
	.scene_field1_forest3_sce12_sbh
	{
		.tableOfVoices = 0x000010C4,
	},
	.scene_field1_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x000010C4,
	},
	.scene_field1_forest3_sce13_sbh
	{
		.merging2 = 0x00001318,
		.tableOfVoices = 0x000013FC,
	},
	.scene_field1_forest3_sce13a_sbh
	{
		.merging2 = 0x00001318,
		.tableOfVoices = 0x000013FC,
	},
	.scene_field1_forest3_sce14_sbh
	{
		.merging1 = 0x000012FC,
		.tableOfVoices = 0x000013E0,
	},
	.scene_field1_forest3_sce14a_sbh
	{
		.gift_tearsOfReese = 0x000015E0,
		.merging1 = 0x000012F8,
		.tableOfVoices = 0x000013DC,
	},
	.scene_field1_forest3_sce16_sbh
	{
		.excellentLiquor = 0x00000F94,
	},
	.scene_field1_forest3_sce17_sbh
	{
		.chest_yellowBead = 0x000013FC,
	},
	.scene_field1_forest3_sce18_sbh
	{
		.skeletonKey = 0x00000FE4,
	},
	.scene_field1_forest3_sce20_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_field1_forest3_sce22_sbh
	{
		.whiteHorn = 0x00000FD4,
	},
	.scene_field1_forest3_sce24_sbh
	{
		.ikariPowder = 0x00000F74,
	},
	.scene_field1_forest3_sce26_sbh
	{
		.chest_stoneChoker = 0x00000F1C,
	},
	.scene_field1_forest3_sce27_sbh
	{
		.skeletonKey = 0x00000FB4,
	},
	.scene_field1_forest3_sce28_sbh
	{
		.divineTablet = 0x00000F9C,
	},
	.scene_field1_forest3_sce30_sbh
	{
		.minionFang = 0x00000F84,
	},
	.scene_field1_forest3_sce31_sbh
	{
		.spiderWarp = 0x00001CB0,
	},
	.scene_field1_forest3_sce32_sbh
	{
		.knowledge5 = 0x000012FC,
		.tableOfVoices = 0x00001410,
	},
	.scene_field1_forest3_sce33_sbh
	{
		.tableOfVoices = 0x00000C18,
	},
	.scene_field1_forest3_sce34_sbh
	{
		.merging2 = 0x00001318,
		.tableOfVoices = 0x0000142C,
	},
	.scene_field1_forest3_sce35_sbh
	{
		.merging1 = 0x00000F9C,
	},
	.scene_field1_forest3_sce37_sbh
	{
		.excellentLiquor = 0x00000F94,
	},
	.scene_field1_forest3_sce38_sbh
	{
		.chest_yellowBead = 0x00000F1C,
	},
	.scene_field1_forest3_sce39_sbh
	{
		.skeletonKey = 0x00000FE4,
	},
	.scene_field1_forest4_sce00_sbh
	{
		.excellentLiquor = 0x00000FBC,
	},
	.scene_field1_forest4_sce01_sbh
	{
		.chest_bitterMugwort = 0x00000F1C,
	},
	.scene_field1_forest4_sce03_sbh
	{
		.chest_shishiudoOil = 0x00000F1C,
	},
	.scene_field1_forest4_sce04_sbh
	{
		.whistler = 0x00001474,
	},
	.scene_field1_forest4_sce05_sbh
	{
		.greatWalnut = 0x00000F94,
	},
	.scene_field1_forest4_sce06_sbh
	{
		.skeletonKey = 0x00000F7C,
	},
	.scene_field1_forest4_sce07_sbh
	{
		.pupaKeyBehaviorFn = 0x000006D8,
		.doorBehaviorFn = 0x000001FC,
	},
	.scene_field1_forest4_sce08_sbh
	{
		.chest_urvysBlessing = 0x00000F1C,
	},
	.scene_field1_forest4_sce09_sbh
	{
		.skeletonKey = 0x00000F74,
	},
	.scene_field1_forest4_sce09a_sbh
	{
		.skeletonKey = 0x000012E4,
		.tableOfVoices = 0x000016A4,
	},
	.scene_field1_forest4_sce10_sbh
	{
		.skeletonKey = 0x00000F8C,
	},
	.scene_field1_forest4_sce12_sbh
	{
		.tableOfVoices = 0x00001B80,
		.kemuelTempleGate = 0x00025498
	},
	.scene_field1_forest4_sce14_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_field1_forest4_sce15_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_field1_forest4_sce16_sbh
	{
		.tableOfVoices = 0x00001BB8,
	},
	.scene_field1_forest4_sce16a_sbh
	{
		.tableOfVoices = 0x00001C30,
	},
	.scene_field1_forest4_sce16b_sbh
	{
		.tableOfVoices = 0x00001C30,
	},
	.scene_field1_forest4_sce17_sbh
	{
		.tableOfVoices = 0x000013B4,
	},
	.scene_field1_forest4_sce18_sbh
	{
		.tableOfVoices = 0x00001520,
	},
	.scene_field1_forest4_sce19_sbh
	{
		.gift_jewelOfWater = 0x0000216C,
		.tableOfVoices = 0x000018B8,
	},
	.scene_field1_gate_sce00_sbh
	{
		.forestOstId = 0x00001FA4,
		.forestOstIdFromBehind = 0x00000054,
	},
	.scene_field1_gate_sce00a_sbh
	{
		.korisMinionId = 0x000024F4,
		.gift_mugwort = 0x00002C24,
		.forestOstId = 0x000039EC,
		.forestOstIdFromBehind = 0x00000078,
		.tableOfVoices = 0x00002248,
	},
	.scene_field1_gate_sce00b_sbh
	{
		.forestOstId = 0x00002110,
		.forestOstIdFromBehind = 0x00000054,
	},
	.scene_field1_gate_sce00c_sbh
	{
		.forestOstId = 0x00002CB0,
		.forestOstIdFromBehind = 0x00000074,
	},
	.scene_field1_gate_sce01_sbh
	{
		.tableOfVoices = 0x00000A1C,
	},
	.scene_field1_gate_sce06_sbh
	{
		.forestOstId = 0x00002CB8,
		.forestOstIdFromBehind = 0x00000074,
		.tableOfVoices = 0x000027E4,
	},
	.scene_field2_forest1_sce02_sbh
	{
		.chest_varysBlessing = 0x00000F1C,
	},
	.scene_field2_forest1_sce06_sbh
	{
		.skeletonKey = 0x00000FA4,
	},
	.scene_field2_forest1_sce07_sbh
	{
		.gift_jewelOfFire = 0x0000259C,
		.tableOfVoices = 0x00001A00,
	},
	.scene_field2_forest2_sce06_sbh
	{
		.chest_bletillaOil = 0x00000F1C,
	},
	.scene_field2_forest2_sce07_sbh
	{
		.gift_jewelOfWind = 0x00002EB8,
		.tableOfVoices = 0x0000207C,
	},
	.scene_field2_forest2_sce13_sbh
	{
		.thunderbolt = 0x00000F74,
	},
	.scene_field2_forest2_sce14_sbh
	{
		.skeletonKey = 0x0000147C,
	},
	.scene_field2_forest3_sce07_sbh
	{
		.skeletonKey = 0x00000FAC,
	},
	.scene_field2_forest3_sce11a_sbh
	{
		.tableOfVoices = 0x00001584,
	},
	.scene_field2_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x0000158C,
	},
	.scene_field2_forest3_sce13a_sbh
	{
		.tableOfVoices = 0x00001584,
	},
	.scene_field2_forest3_sce16_sbh
	{
		.chest_bitterMugwort = 0x00000F1C,
	},
	.scene_field2_forest3_sce18_sbh
	{
		.chest_urvysBlessing = 0x00000F1C,
	},
	.scene_field2_forest3_sce20_sbh
	{
		.tableOfVoices = 0x00001AB0,
	},
	.scene_field2_forest3_sce22a_sbh
	{
		.tableOfVoices = 0x00001DC8,
	},
	.scene_other_clear_sce00_sbh
	{
		.tableOfVoices = 0x00000D90,
	},
	.scene_other_clear_sce03_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_other_clear_sce04_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_other_dream_sce00_sbh
	{
		.dreamManIntroVoice = 0x00001524,
		.tableOfVoices = 0x00001460,
	},
	.scene_other_dream_sce00a_sbh
	{
		.dreamMinionId = 0x00001344,
		.dreamManIntroVoice = 0x000012BE,
		.tableOfVoices = 0x00001280,
	},
	.scene_other_hunting_sce00_sbh
	{
		.neutralSpawnEC = 0x00000D50,
		.magicsEC = 0x00001EAC,
		.statsEC = 0x00002FAC,
		.specialsEC = 0x00002114,
		.bossSpecialsEC = 0x00002C54,
		.bossDamageModifiersEC = 0x00002FFC,
	},
	.scene_psyrus1_cemetery_sce00_sbh
	{
		.tableOfVoices = 0x00000E28,
	},
	.scene_psyrus1_cemetery_sce00a_sbh
	{
		.tableOfVoices = 0x00000E20,
	},
	.scene_psyrus1_cemetery_sce00b_sbh
	{
		.tableOfVoices = 0x00000E70,
	},
	.scene_psyrus1_cemetery_sce00c_sbh
	{
		.tableOfVoices = 0x00000E40,
	},
	.scene_psyrus1_garai_sce00_sbh
	{
		.gift_fluteOfCapture = 0x0000159C,
		.tableOfVoices = 0x00001108,
	},
	.scene_psyrus1_garai_sce01_sbh
	{
		.tableOfVoices = 0x00002134,
	},
	.scene_psyrus1_garai_sce01a_sbh
	{
		.tableOfVoices = 0x00002134,
	},
	.scene_psyrus1_garai_sce01b_sbh
	{
		.tableOfVoices = 0x00002160,
	},
	.scene_psyrus1_garai_sce01c_sbh
	{
		.tableOfVoices = 0x000021D4,
	},
	.scene_psyrus1_garai_sce03_sbh
	{
		.gift_tearsOfReese = 0x0000103C,
		.tableOfVoices = 0x00000DC0,
	},
	.scene_psyrus1_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus1_kajiya_sce00a_sbh
	{
		.tableOfVoices = 0x00000ECC,
	},
	.scene_psyrus1_kajiya_sce00b_sbh
	{
		.gift_lifeStealer = 0x00001280,
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus1_kajiya_sce00c_sbh
	{
		.tableOfVoices = 0x00000F0C,
	},
	.scene_psyrus1_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00000ED8,
	},
	.scene_psyrus1_lebant_sce00a_sbh
	{
		.tableOfVoices = 0x00001440,
	},
	.scene_psyrus1_lebant_sce00c_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_psyrus1_lebant_sce00d_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_psyrus1_lookout_sce00_sbh
	{
		.tableOfVoices = 0x00000A5C,
	},
	.scene_psyrus1_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000B08,
	},
	.scene_psyrus1_lookout_sce00b_sbh
	{
		.tableOfVoices = 0x00000B08,
	},
	.scene_psyrus1_lookout_sce00c_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus1_lookout_sce01_sbh
	{
		.gift_nagiBelt = 0x000016F0,
		.tableOfVoices = 0x00000B78,
	},
	.scene_psyrus1_zokucho_sce01_sbh
	{
		.tableOfVoices = 0x0000167C,
	},
	.scene_psyrus1_zokucho_sce01a_sbh
	{
		.gift_championBelt = 0x00001E9C,
		.gift_captureAmulet = 0x00001EAC,
		.tableOfVoices = 0x00001618,
	},
	.scene_psyrus1_zokucho_sce05_sbh
	{
		.gift_huntersEarring = 0x00001FD0,
		.tableOfVoices = 0x00001524,
	},
	.scene_psyrus2_garai_sce00_sbh
	{
		.tableOfVoices = 0x00002198,
	},
	.scene_psyrus2_garai_sce03_sbh
	{
		.gift_mahbusCocoon = 0x00001F40,
		.gift_riketzGarb = 0x00002040,
		.tableOfVoices = 0x00001540,
	},
	.scene_psyrus2_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F0C,
	},
	.scene_psyrus2_lebant_sce00_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_psyrus2_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus2_lookout_sce01_sbh
	{
		.tableOfVoices = 0x000018C0,
	},
	.scene_psyrus2_lookout_sce01a_sbh
	{
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus2_zokucho_sce03_sbh
	{
		.tableOfVoices = 0x00001710,
	}
},
filePalFr
{
	.executable
	{
		.entityModelsBehavior = 0x0006C6C8,
		.entityStats = 0x0006AE4C,
		.entityMagics = 0x0006B044,
		.entitySpecials = 0x0006B390,
		.entityDamageModifiers = 0x0006C40C,
		.hudColors = 0x000779DC,
		.afterGetStatsJump = 0x0000273C,
		.afterFramerateLimiter = 0x00000BC4,
		.hpMpBars = 0x0001C2D0,
		.bodyEnhancementFn = 0x0000A40C,
		.setVideoModeArgument = 0x00000924,
		.stretchingY = 0x00049178,
		.tableOfWeaponAnimationFileId = 0x0006A0A0,
		.tableOfWeaponStats = 0x0006D9E4,
		.tableOfArmorStats = 0x0006DDB8,
		.tableOfOtherStats = 0x0006E048,
		.tableOfWeaponCharacteristic = 0x0006DC0C,
		.tableOfArmorsAppearanceId = 0x0006DF20,
		.tableOfYPositionMinionStatsFrame = 0x00073880,
		.drawMinionStatsFrameFn = 0x0001E3A4,
		.drawMinionTIMStatsFrameFn = 0x0001E298,
		.tableOfModelAnimations = 0x0006E664,
		.tableOfModelAnimationsPtr = 0x00072E64,
		.actorBehaviorFn = 0x00018560,
		.mainFn = 0x00000800,
		.branchSetVatolkaSpecials = 0x00009E2C,
		.tableOfLevantFluteStylingsId = 0x0006D458,
		.cc_begin = 0x000BB000,
	},
	.over_battle_bin
	{
		.endOfBattle = 0x000042D8,
		.tableOfBattleWeaponsEffectPtr = 0x00000134,
		.setBattleWeaponsEffectFn = 0x00009738,
		.endOfBattleSetSceneFn = 0x00004488,
		.storyMinionAI = 0x0000DE1C,
		.minionAgniaId = 0x0000F7CC,
		.delfanelEarthBossId = 0x0000FA54,
		.tuturisFireBossId = 0x0000FC08,
		.seterianAirBossId = 0x0000FDC8,
		.cushidraFireBossId = 0x000101D8,
		.tickerUlvi = 0x0000E048,
		.kolnaAdMumuls = 0x0000E0C0,
		.captureSuccess = 0x00008360,
		.getFireflyCounterBattle = 0x00010E6C,
	},
	.over_btend_bin
	{
		.entityTreasures = 0,
	},
	.over_btlexec_bin
	{
		.captureSuccess = 0,
	},
	.over_encom_bin
	{
		.storyMinionAI = 0,
		.minionAgniaId = 0,
		.delfanelEarthBossId = 0,
		.tuturisFireBossId = 0,
		.seterianAirBossId = 0,
		.cushidraFireBossId = 0,
		.tickerUlvi = 0,
		.kolnaAdMumuls = 0,
	},
	.over_game_bin
	{
		.tableOfStoryMinionsId = 0x0000E0B4,
		.spawnStory = 0x00004C34,
		.fireMapBonus = 0x00004E08,
		.battleMaps = 0x0000DD00,
		.levelCapEC = 0x000049BC,
		.levantAnimArgs = 0x00007F58,
		.chooseMagicLevantAnim = 0x0000B9B8,
		.levantCastingItemId = 0x00010D48,
		.tableOfWeaponAnimationsId = 0x00010E7C,
		.isQuantityLimitReachedFn = 0x0000D310,
		.itemShopBuyFn = 0x0000D178,
		.tableOfMapsBehavior = 0x0000E8E4,
		.readFlagRotateSpawn = 0x00004BD0,
	},
	.over_menu_bin
	{
		.entityTreasures = 0x0000ACE4,
	},
	.over_plcom_bin
	{
		.getFireflyCounterBattle = 0,
	},
	.over_title_bin
	{
		.levantStats = 0x00002658,
		.setLevantGarb = 0x00001FEC,
		.initMapNewGame = 0x00001B54,
	},
	.over_wpnshop_bin
	{
		.items = 0x00008624,
		.itemShopQuantityLimitFn = 0x00007398,
		.equipmentShopQuantityLimitFn = 0x00007268,
		.initEternalCorridorShopFn = 0x0000425C,
	},
	.scene_field1_forest1_sce00_sbh
	{
		.knowledge1 = 0x00000F94,
	},
	.scene_field1_forest1_sce01_sbh
	{
		.shabLiquor = 0x00001474,
		.treeBranchBehaviorFn = 0x00000218,
	},
	.scene_field1_forest1_sce02_sbh
	{
		.chest_metaMugwort = 0x00000F1C,
	},
	.scene_field1_forest1_sce03_sbh
	{
		.knowledge2 = 0x00000F94,
	},
	.scene_field1_forest1_sce04_sbh
	{
		.knowledge3 = 0x00000FBC,
	},
	.scene_field1_forest1_sce05_sbh
	{
		.skeletonKey = 0x00000F7C,
	},
	.scene_field1_forest1_sce06_sbh
	{
		.aquazor = 0x00000F94,
	},
	.scene_field1_forest1_sce06a_sbh
	{
		.aquazor = 0x0000137C,
		.tableOfVoices = 0x00001538,
	},
	.scene_field1_forest1_sce08_sbh
	{
		.beetleWarp = 0x00002180,
	},
	.scene_field1_forest1_sce09_sbh
	{
		.tableOfVoices = 0x00000B64,
	},
	.scene_field1_forest2_sce00_sbh
	{
		.kickleberry = 0x00000F6C,
	},
	.scene_field1_forest2_sce01_sbh
	{
		.knowledge4 = 0x00000F7C,
	},
	.scene_field1_forest2_sce03_sbh
	{
		.skeletonKey = 0x00000F7C,
	},
	.scene_field1_forest2_sce04_sbh
	{
		.magicUsage1 = 0x00000FA4,
	},
	.scene_field1_forest2_sce05_sbh
	{
		.chest_chestNutOil = 0x00000F1C,
	},
	.scene_field1_forest2_sce06_sbh
	{
		.larvaKeyBehaviorFn = 0x000004EC,
		.doorBehaviorFn = 0x000001FC,
	},
	.scene_field1_forest2_sce07_sbh
	{
		.gift_calabasPowder = 0x00001DC4,
		.gift_ocarina = 0x0000274C,
		.gift_specialLiquor = 0x00001DD4,
		.sleepSpore = 0x00001914,
		.tableOfVoices = 0x00001B24,
	},
	.scene_field1_forest2_sce07a_sbh
	{
		.sleepSpore = 0x000014E8,
	},
	.scene_field1_forest2_sce08_sbh
	{
		.dragonflyWarp = 0x00002188,
	},
	.scene_field1_forest2_sce10_sbh
	{
		.skeletonKey = 0x00001444,
		.bridgeBehaviorFn = 0x000001E8,
	},
	.scene_field1_forest2_sce12_sbh
	{
		.chest_agnisSeal1 = 0x00000F1C,
	},
	.scene_field1_forest2_sce13_sbh
	{
		.magicUsage2 = 0x00000FAC,
	},
	.scene_field1_forest2_sce13a_sbh
	{
		.magicUsage2 = 0x0000131C,
		.tableOfVoices = 0x000016DC,
	},
	.scene_field1_forest2_sce14a_sbh
	{
		.secretOfKilling = 0x000017B4,
		.tableOfVoices = 0x00001994,
		.kikinakReturnOstId = 0x00001BF4,
	},
	.scene_field1_forest3_sce01_sbh
	{
		.whiteHorn = 0x00000F8C,
	},
	.scene_field1_forest3_sce03_sbh
	{
		.ikariPowder = 0x00000F74,
	},
	.scene_field1_forest3_sce03a_sbh
	{
		.ikariPowder = 0x000012E4,
		.tableOfVoices = 0x000016A4,
	},
	.scene_field1_forest3_sce05_sbh
	{
		.chest_stoneChoker = 0x00000F1C,
	},
	.scene_field1_forest3_sce06_sbh
	{
		.skeletonKey = 0x00000F8C,
	},
	.scene_field1_forest3_sce07_sbh
	{
		.divineTablet = 0x00000F9C,
	},
	.scene_field1_forest3_sce09_sbh
	{
		.minionFang = 0x00000F7C,
	},
	.scene_field1_forest3_sce10_sbh
	{
		.spiderWarp = 0x00002170,
	},
	.scene_field1_forest3_sce11_sbh
	{
		.knowledge5 = 0x00001320,
		.tableOfVoices = 0x00001404,
	},
	.scene_field1_forest3_sce11a_sbh
	{
		.knowledge5 = 0x00001324,
		.tableOfVoices = 0x00001408,
	},
	.scene_field1_forest3_sce12_sbh
	{
		.tableOfVoices = 0x000010C8,
	},
	.scene_field1_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x000010C8,
	},
	.scene_field1_forest3_sce13_sbh
	{
		.merging2 = 0x00001310,
		.tableOfVoices = 0x000013F4,
	},
	.scene_field1_forest3_sce13a_sbh
	{
		.merging2 = 0x00001310,
		.tableOfVoices = 0x000013F4,
	},
	.scene_field1_forest3_sce14_sbh
	{
		.merging1 = 0x00001304,
		.tableOfVoices = 0x000013E8,
	},
	.scene_field1_forest3_sce14a_sbh
	{
		.gift_tearsOfReese = 0x000015E8,
		.merging1 = 0x00001300,
		.tableOfVoices = 0x000013E4,
	},
	.scene_field1_forest3_sce16_sbh
	{
		.excellentLiquor = 0x00000FB4,
	},
	.scene_field1_forest3_sce17_sbh
	{
		.chest_yellowBead = 0x000013FC,
	},
	.scene_field1_forest3_sce18_sbh
	{
		.skeletonKey = 0x00000FA4,
	},
	.scene_field1_forest3_sce20_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_field1_forest3_sce22_sbh
	{
		.whiteHorn = 0x00000F8C,
	},
	.scene_field1_forest3_sce24_sbh
	{
		.ikariPowder = 0x00000F74,
	},
	.scene_field1_forest3_sce26_sbh
	{
		.chest_stoneChoker = 0x00000F1C,
	},
	.scene_field1_forest3_sce27_sbh
	{
		.skeletonKey = 0x00000F8C,
	},
	.scene_field1_forest3_sce28_sbh
	{
		.divineTablet = 0x00000F9C,
	},
	.scene_field1_forest3_sce30_sbh
	{
		.minionFang = 0x00000F7C,
	},
	.scene_field1_forest3_sce31_sbh
	{
		.spiderWarp = 0x00001CA0,
	},
	.scene_field1_forest3_sce32_sbh
	{
		.knowledge5 = 0x00001324,
		.tableOfVoices = 0x00001438,
	},
	.scene_field1_forest3_sce33_sbh
	{
		.tableOfVoices = 0x00000C18,
	},
	.scene_field1_forest3_sce34_sbh
	{
		.merging2 = 0x00001310,
		.tableOfVoices = 0x00001424,
	},
	.scene_field1_forest3_sce35_sbh
	{
		.merging1 = 0x00000FA4,
	},
	.scene_field1_forest3_sce37_sbh
	{
		.excellentLiquor = 0x00000FB4,
	},
	.scene_field1_forest3_sce38_sbh
	{
		.chest_yellowBead = 0x00000F1C,
	},
	.scene_field1_forest3_sce39_sbh
	{
		.skeletonKey = 0x00000FA4,
	},
	.scene_field1_forest4_sce00_sbh
	{
		.excellentLiquor = 0x00000FD0,
	},
	.scene_field1_forest4_sce01_sbh
	{
		.chest_bitterMugwort = 0x00000F1C,
	},
	.scene_field1_forest4_sce03_sbh
	{
		.chest_shishiudoOil = 0x00000F1C,
	},
	.scene_field1_forest4_sce04_sbh
	{
		.whistler = 0x00001494,
	},
	.scene_field1_forest4_sce05_sbh
	{
		.greatWalnut = 0x00000F84,
	},
	.scene_field1_forest4_sce06_sbh
	{
		.skeletonKey = 0x00000F8C,
	},
	.scene_field1_forest4_sce07_sbh
	{
		.pupaKeyBehaviorFn = 0x000006D8,
		.doorBehaviorFn = 0x000001FC,
	},
	.scene_field1_forest4_sce08_sbh
	{
		.chest_urvysBlessing = 0x00000F1C,
	},
	.scene_field1_forest4_sce09_sbh
	{
		.skeletonKey = 0x00000F74,
	},
	.scene_field1_forest4_sce09a_sbh
	{
		.skeletonKey = 0x000012EC,
		.tableOfVoices = 0x000016AC,
	},
	.scene_field1_forest4_sce10_sbh
	{
		.skeletonKey = 0x00000F84,
	},
	.scene_field1_forest4_sce12_sbh
	{
		.tableOfVoices = 0x00001B80,
		.kemuelTempleGate = 0x000254F4
	},
	.scene_field1_forest4_sce14_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_field1_forest4_sce15_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_field1_forest4_sce16_sbh
	{
		.tableOfVoices = 0x00001BB8,
	},
	.scene_field1_forest4_sce16a_sbh
	{
		.tableOfVoices = 0x00001C30,
	},
	.scene_field1_forest4_sce16b_sbh
	{
		.tableOfVoices = 0x00001C30,
	},
	.scene_field1_forest4_sce17_sbh
	{
		.tableOfVoices = 0x000013B4,
	},
	.scene_field1_forest4_sce18_sbh
	{
		.tableOfVoices = 0x00001520,
	},
	.scene_field1_forest4_sce19_sbh
	{
		.gift_jewelOfWater = 0x0000216C,
		.tableOfVoices = 0x000018B8,
	},
	.scene_field1_gate_sce00_sbh
	{
		.forestOstId = 0x00001FA4,
		.forestOstIdFromBehind = 0x00000054,
	},
	.scene_field1_gate_sce00a_sbh
	{
		.korisMinionId = 0x000024F4,
		.gift_mugwort = 0x00002C24,
		.forestOstId = 0x000039EC,
		.forestOstIdFromBehind = 0x00000078,
		.tableOfVoices = 0x00002248,
	},
	.scene_field1_gate_sce00b_sbh
	{
		.forestOstId = 0x00002110,
		.forestOstIdFromBehind = 0x00000054,
	},
	.scene_field1_gate_sce00c_sbh
	{
		.forestOstId = 0x00002CB0,
		.forestOstIdFromBehind = 0x00000074,
	},
	.scene_field1_gate_sce01_sbh
	{
		.tableOfVoices = 0x00000A20,
	},
	.scene_field1_gate_sce06_sbh
	{
		.forestOstId = 0x00002CB8,
		.forestOstIdFromBehind = 0x00000074,
		.tableOfVoices = 0x000027E4,
	},
	.scene_field2_forest1_sce02_sbh
	{
		.chest_varysBlessing = 0x00000F1C,
	},
	.scene_field2_forest1_sce06_sbh
	{
		.skeletonKey = 0x00000F74,
	},
	.scene_field2_forest1_sce07_sbh
	{
		.gift_jewelOfFire = 0x000025A4,
		.tableOfVoices = 0x00001A08,
	},
	.scene_field2_forest2_sce06_sbh
	{
		.chest_bletillaOil = 0x00000F1C,
	},
	.scene_field2_forest2_sce07_sbh
	{
		.gift_jewelOfWind = 0x00002EC8,
		.tableOfVoices = 0x0000208C,
	},
	.scene_field2_forest2_sce13_sbh
	{
		.thunderbolt = 0x00000F94,
	},
	.scene_field2_forest2_sce14_sbh
	{
		.skeletonKey = 0x0000144C,
	},
	.scene_field2_forest3_sce07_sbh
	{
		.skeletonKey = 0x00000F94,
	},
	.scene_field2_forest3_sce11a_sbh
	{
		.tableOfVoices = 0x00001594,
	},
	.scene_field2_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x0000159C,
	},
	.scene_field2_forest3_sce13a_sbh
	{
		.tableOfVoices = 0x00001594,
	},
	.scene_field2_forest3_sce16_sbh
	{
		.chest_bitterMugwort = 0x00000F1C,
	},
	.scene_field2_forest3_sce18_sbh
	{
		.chest_urvysBlessing = 0x00000F1C,
	},
	.scene_field2_forest3_sce20_sbh
	{
		.tableOfVoices = 0x00001AC0,
	},
	.scene_field2_forest3_sce22a_sbh
	{
		.tableOfVoices = 0x00001DBC,
	},
	.scene_other_clear_sce00_sbh
	{
		.tableOfVoices = 0x00000D90,
	},
	.scene_other_clear_sce03_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_other_clear_sce04_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_other_dream_sce00_sbh
	{
		.dreamManIntroVoice = 0x00001534,
		.tableOfVoices = 0x00001470,
	},
	.scene_other_dream_sce00a_sbh
	{
		.dreamMinionId = 0x00001350,
		.dreamManIntroVoice = 0x000012CA,
		.tableOfVoices = 0x0000128C,
	},
	.scene_other_hunting_sce00_sbh
	{
		.neutralSpawnEC = 0x00000D50,
		.magicsEC = 0x00001EAC,
		.statsEC = 0x00002FAC,
		.specialsEC = 0x00002114,
		.bossSpecialsEC = 0x00002C54,
		.bossDamageModifiersEC = 0x00002FFC,
	},
	.scene_psyrus1_cemetery_sce00_sbh
	{
		.tableOfVoices = 0x00000E28,
	},
	.scene_psyrus1_cemetery_sce00a_sbh
	{
		.tableOfVoices = 0x00000E20,
	},
	.scene_psyrus1_cemetery_sce00b_sbh
	{
		.tableOfVoices = 0x00000E70,
	},
	.scene_psyrus1_cemetery_sce00c_sbh
	{
		.tableOfVoices = 0x00000E40,
	},
	.scene_psyrus1_garai_sce00_sbh
	{
		.gift_fluteOfCapture = 0x0000159C,
		.tableOfVoices = 0x00001108,
	},
	.scene_psyrus1_garai_sce01_sbh
	{
		.tableOfVoices = 0x00002134,
	},
	.scene_psyrus1_garai_sce01a_sbh
	{
		.tableOfVoices = 0x00002134,
	},
	.scene_psyrus1_garai_sce01b_sbh
	{
		.tableOfVoices = 0x00002160,
	},
	.scene_psyrus1_garai_sce01c_sbh
	{
		.tableOfVoices = 0x000021D4,
	},
	.scene_psyrus1_garai_sce03_sbh
	{
		.gift_tearsOfReese = 0x00001048,
		.tableOfVoices = 0x00000DCC,
	},
	.scene_psyrus1_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F10,
	},
	.scene_psyrus1_kajiya_sce00a_sbh
	{
		.tableOfVoices = 0x00000ED8,
	},
	.scene_psyrus1_kajiya_sce00b_sbh
	{
		.gift_lifeStealer = 0x0000128C,
		.tableOfVoices = 0x00000F10,
	},
	.scene_psyrus1_kajiya_sce00c_sbh
	{
		.tableOfVoices = 0x00000F18,
	},
	.scene_psyrus1_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00000EE4,
	},
	.scene_psyrus1_lebant_sce00a_sbh
	{
		.tableOfVoices = 0x00001450,
	},
	.scene_psyrus1_lebant_sce00c_sbh
	{
		.tableOfVoices = 0x0000142C,
	},
	.scene_psyrus1_lebant_sce00d_sbh
	{
		.tableOfVoices = 0x0000142C,
	},
	.scene_psyrus1_lookout_sce00_sbh
	{
		.tableOfVoices = 0x00000A60,
	},
	.scene_psyrus1_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000B0C,
	},
	.scene_psyrus1_lookout_sce00b_sbh
	{
		.tableOfVoices = 0x00000B0C,
	},
	.scene_psyrus1_lookout_sce00c_sbh
	{
		.tableOfVoices = 0x00000AC8,
	},
	.scene_psyrus1_lookout_sce01_sbh
	{
		.gift_nagiBelt = 0x000016F0,
		.tableOfVoices = 0x00000B78,
	},
	.scene_psyrus1_zokucho_sce01_sbh
	{
		.tableOfVoices = 0x0000167C,
	},
	.scene_psyrus1_zokucho_sce01a_sbh
	{
		.gift_championBelt = 0x00001E9C,
		.gift_captureAmulet = 0x00001EAC,
		.tableOfVoices = 0x00001618,
	},
	.scene_psyrus1_zokucho_sce05_sbh
	{
		.gift_huntersEarring = 0x00001FD0,
		.tableOfVoices = 0x00001524,
	},
	.scene_psyrus2_garai_sce00_sbh
	{
		.tableOfVoices = 0x00002198,
	},
	.scene_psyrus2_garai_sce03_sbh
	{
		.gift_mahbusCocoon = 0x00001F40,
		.gift_riketzGarb = 0x00002040,
		.tableOfVoices = 0x00001540,
	},
	.scene_psyrus2_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F18,
	},
	.scene_psyrus2_lebant_sce00_sbh
	{
		.tableOfVoices = 0x0000142C,
	},
	.scene_psyrus2_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000AC8,
	},
	.scene_psyrus2_lookout_sce01_sbh
	{
		.tableOfVoices = 0x000018C8,
	},
	.scene_psyrus2_lookout_sce01a_sbh
	{
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus2_zokucho_sce03_sbh
	{
		.tableOfVoices = 0x00001710,
	}
},
filePalDe
{
	.executable
	{
		.entityModelsBehavior = 0x0006C584,
		.entityStats = 0x0006AD08,
		.entityMagics = 0x0006AF00,
		.entitySpecials = 0x0006B24C,
		.entityDamageModifiers = 0x0006C2C8,
		.hudColors = 0x00077890,
		.afterGetStatsJump = 0x0000273C,
		.afterFramerateLimiter = 0x00000BC4,
		.hpMpBars = 0x0001C2D0,
		.bodyEnhancementFn = 0x0000A40C,
		.setVideoModeArgument = 0x00000924,
		.stretchingY = 0x0004911C,
		.tableOfWeaponAnimationFileId = 0x00069F5C,
		.tableOfWeaponStats = 0x0006D8A0,
		.tableOfArmorStats = 0x0006DC74,
		.tableOfOtherStats = 0x0006DF04,
		.tableOfWeaponCharacteristic = 0x0006DAC8,
		.tableOfArmorsAppearanceId = 0x0006DDDC,
		.tableOfYPositionMinionStatsFrame = 0x0007373C,
		.drawMinionStatsFrameFn = 0x0001E3A4,
		.drawMinionTIMStatsFrameFn = 0x0001E298,
		.tableOfModelAnimations = 0x0006E520,
		.tableOfModelAnimationsPtr = 0x00072D20,
		.actorBehaviorFn = 0x00018560,
		.mainFn = 0x00000800,
		.branchSetVatolkaSpecials = 0x00009E2C,
		.tableOfLevantFluteStylingsId = 0x0006D314,
		.cc_begin = 0x000BB000,
	},
	.over_battle_bin
	{
		.endOfBattle = 0x000045F8,
		.tableOfBattleWeaponsEffectPtr = 0x00000134,
		.setBattleWeaponsEffectFn = 0x00009A58,
		.endOfBattleSetSceneFn = 0x000047A8,
		.storyMinionAI = 0x0000E13C,
		.minionAgniaId = 0x0000FAEC,
		.delfanelEarthBossId = 0x0000FD74,
		.tuturisFireBossId = 0x0000FF28,
		.seterianAirBossId = 0x000100E8,
		.cushidraFireBossId = 0x000104F8,
		.tickerUlvi = 0x0000E368,
		.kolnaAdMumuls = 0x0000E3E0,		
		.captureSuccess = 0x00008680,
		.getFireflyCounterBattle = 0x0001118C,	
	},
	.over_btend_bin
	{
		.entityTreasures = 0,
	},
	.over_btlexec_bin
	{
		.captureSuccess = 0,
	},
	.over_encom_bin
	{
		.storyMinionAI = 0,
		.minionAgniaId = 0,
		.delfanelEarthBossId = 0,
		.tuturisFireBossId = 0,
		.seterianAirBossId = 0,
		.cushidraFireBossId = 0,
		.tickerUlvi = 0,
		.kolnaAdMumuls = 0,
	},
	.over_game_bin
	{
		.tableOfStoryMinionsId = 0x0000E0B4,
		.spawnStory = 0x00004C34,
		.fireMapBonus = 0x00004E08,
		.battleMaps = 0x0000DD00,
		.levelCapEC = 0x000049BC,
		.levantAnimArgs = 0x00007F58,		
		.chooseMagicLevantAnim = 0x0000B9B8,
		.levantCastingItemId = 0x00010D48,
		.tableOfWeaponAnimationsId = 0x00010E7C,
		.isQuantityLimitReachedFn = 0x0000D310,
		.itemShopBuyFn = 0x0000D178,
		.tableOfMapsBehavior = 0x0000E8E4,
		.readFlagRotateSpawn = 0x00004BD0,
	},
	.over_menu_bin
	{
		.entityTreasures = 0x0000B240,
	},
	.over_plcom_bin
	{
		.getFireflyCounterBattle = 0,
	},
	.over_title_bin
	{
		.levantStats = 0x00002604,
		.setLevantGarb = 0x00001F98,
		.initMapNewGame = 0x00001B00,
	},
	.over_wpnshop_bin
	{
		.items = 0x00009040,
		.itemShopQuantityLimitFn = 0x00007DB8,
		.equipmentShopQuantityLimitFn = 0x00007C88,
		.initEternalCorridorShopFn = 0x00004C7C,
	},
	.scene_field1_forest1_sce00_sbh
	{
		.knowledge1 = 0x00000F64,
	},
	.scene_field1_forest1_sce01_sbh
	{
		.shabLiquor = 0x000014a4,
		.treeBranchBehaviorFn = 0x00000248,
	},
	.scene_field1_forest1_sce02_sbh
	{
		.chest_metaMugwort = 0x00000EF4,
	},
	.scene_field1_forest1_sce03_sbh
	{
		.knowledge2 = 0x00000FA4,
	},
	.scene_field1_forest1_sce04_sbh
	{
		.knowledge3 = 0x00000FCC,
	},
	.scene_field1_forest1_sce05_sbh
	{
		.skeletonKey = 0x00000F64,
	},
	.scene_field1_forest1_sce06_sbh
	{
		.aquazor = 0x00000F74,
	},
	.scene_field1_forest1_sce06a_sbh
	{
		.aquazor = 0x0000135C,
		.tableOfVoices = 0x00001518,
	},
	.scene_field1_forest1_sce08_sbh
	{
		.beetleWarp = 0x00002158,
	},
	.scene_field1_forest1_sce09_sbh
	{
		.tableOfVoices = 0x00000B64,
	},
	.scene_field1_forest2_sce00_sbh
	{
		.kickleberry = 0x00000F6C,
	},
	.scene_field1_forest2_sce01_sbh
	{
		.knowledge4 = 0x00000F74,
	},
	.scene_field1_forest2_sce03_sbh
	{
		.skeletonKey = 0x00000FAC,
	},
	.scene_field1_forest2_sce04_sbh
	{
		.magicUsage1 = 0x00000F5C,
	},
	.scene_field1_forest2_sce05_sbh
	{
		.chest_chestNutOil = 0x00000EF4,
	},
	.scene_field1_forest2_sce06_sbh
	{
		.larvaKeyBehaviorFn = 0x000004D8,
		.doorBehaviorFn = 0x000001E8,
	},
	.scene_field1_forest2_sce07_sbh
	{
		.gift_calabasPowder = 0x00001DD4,
		.gift_ocarina = 0x0000275C,
		.gift_specialLiquor = 0x00001DE4,
		.sleepSpore = 0x00001924,
		.tableOfVoices = 0x00001B34,
	},
	.scene_field1_forest2_sce07a_sbh
	{
		.sleepSpore = 0x000014F0,
	},
	.scene_field1_forest2_sce08_sbh
	{
		.dragonflyWarp = 0x00002160,
	},
	.scene_field1_forest2_sce10_sbh
	{
		.skeletonKey = 0x00001454,
		.bridgeBehaviorFn = 0x000001F8,
	},
	.scene_field1_forest2_sce12_sbh
	{
		.chest_agnisSeal1 = 0x00000EF4,
	},
	.scene_field1_forest2_sce13_sbh
	{
		.magicUsage2 = 0x00000F74,
	},
	.scene_field1_forest2_sce13a_sbh
	{
		.magicUsage2 = 0x000012E4,
		.tableOfVoices = 0x000016A4,
	},
	.scene_field1_forest2_sce14a_sbh
	{
		.secretOfKilling = 0x0000178C,
		.tableOfVoices = 0x0000196C,
		.kikinakReturnOstId = 0x00001BCC,
	},
	.scene_field1_forest3_sce01_sbh
	{
		.whiteHorn = 0x00000FA4,
	},
	.scene_field1_forest3_sce03_sbh
	{
		.ikariPowder = 0x00000F4C,
	},
	.scene_field1_forest3_sce03a_sbh
	{
		.ikariPowder = 0x000012BC,
		.tableOfVoices = 0x0000167C,
	},
	.scene_field1_forest3_sce05_sbh
	{
		.chest_stoneChoker = 0x00000EF4,
	},
	.scene_field1_forest3_sce06_sbh
	{
		.skeletonKey = 0x00000F84,
	},
	.scene_field1_forest3_sce07_sbh
	{
		.divineTablet = 0x00000F74,
	},
	.scene_field1_forest3_sce09_sbh
	{
		.minionFang = 0x00000F84,
	},
	.scene_field1_forest3_sce10_sbh
	{
		.spiderWarp = 0x00002150,
	},
	.scene_field1_forest3_sce11_sbh
	{
		.knowledge5 = 0x000012D8,
		.tableOfVoices = 0x000013BC,
	},
	.scene_field1_forest3_sce11a_sbh
	{
		.knowledge5 = 0x000012DC,
		.tableOfVoices = 0x000013C0,
	},
	.scene_field1_forest3_sce12_sbh
	{
		.tableOfVoices = 0x000010C4,
	},
	.scene_field1_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x000010C4,
	},
	.scene_field1_forest3_sce13_sbh
	{
		.merging2 = 0x00001318,
		.tableOfVoices = 0x000013FC,
	},
	.scene_field1_forest3_sce13a_sbh
	{
		.merging2 = 0x00001318,
		.tableOfVoices = 0x000013FC,
	},
	.scene_field1_forest3_sce14_sbh
	{
		.merging1 = 0x000012D4,
		.tableOfVoices = 0x000013B8,
	},
	.scene_field1_forest3_sce14a_sbh
	{
		.gift_tearsOfReese = 0x000015B8,
		.merging1 = 0x000012D0,
		.tableOfVoices = 0x000013B4,
	},
	.scene_field1_forest3_sce16_sbh
	{
		.excellentLiquor = 0x00000F7C,
	},
	.scene_field1_forest3_sce17_sbh
	{
		.chest_yellowBead = 0x000013D4,
	},
	.scene_field1_forest3_sce18_sbh
	{
		.skeletonKey = 0x00000FD4,
	},
	.scene_field1_forest3_sce20_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_field1_forest3_sce22_sbh
	{
		.whiteHorn = 0x00000FA4,
	},
	.scene_field1_forest3_sce24_sbh
	{
		.ikariPowder = 0x00000F4C,
	},
	.scene_field1_forest3_sce26_sbh
	{
		.chest_stoneChoker = 0x00000EF4,
	},
	.scene_field1_forest3_sce27_sbh
	{
		.skeletonKey = 0x00000F84,
	},
	.scene_field1_forest3_sce28_sbh
	{
		.divineTablet = 0x00000F74,
	},
	.scene_field1_forest3_sce30_sbh
	{
		.minionFang = 0x00000F84,
	},
	.scene_field1_forest3_sce31_sbh
	{
		.spiderWarp = 0x00001C80,
	},
	.scene_field1_forest3_sce32_sbh
	{
		.knowledge5 = 0x000012DC,
		.tableOfVoices = 0x000013F0,
	},
	.scene_field1_forest3_sce33_sbh
	{
		.tableOfVoices = 0x00000C18,
	},
	.scene_field1_forest3_sce34_sbh
	{
		.merging2 = 0x00001318,
		.tableOfVoices = 0x0000142C,
	},
	.scene_field1_forest3_sce35_sbh
	{
		.merging1 = 0x00000F74,
	},
	.scene_field1_forest3_sce37_sbh
	{
		.excellentLiquor = 0x00000F7C,
	},
	.scene_field1_forest3_sce38_sbh
	{
		.chest_yellowBead = 0x00000EF4,
	},
	.scene_field1_forest3_sce39_sbh
	{
		.skeletonKey = 0x00000FD4,
	},
	.scene_field1_forest4_sce00_sbh
	{
		.excellentLiquor = 0x00000FC4,
	},
	.scene_field1_forest4_sce01_sbh
	{
		.chest_bitterMugwort = 0x00000EF4,
	},
	.scene_field1_forest4_sce03_sbh
	{
		.chest_shishiudoOil = 0x00000EF4,
	},
	.scene_field1_forest4_sce04_sbh
	{
		.whistler = 0x0000146C,
	},
	.scene_field1_forest4_sce05_sbh
	{
		.greatWalnut = 0x00000F6C,
	},
	.scene_field1_forest4_sce06_sbh
	{
		.skeletonKey = 0x00000F54,
	},
	.scene_field1_forest4_sce07_sbh
	{
		.pupaKeyBehaviorFn = 0x000006C4,
		.doorBehaviorFn = 0x000001E8,
	},
	.scene_field1_forest4_sce08_sbh
	{
		.chest_urvysBlessing = 0x00000EF4,
	},
	.scene_field1_forest4_sce09_sbh
	{
		.skeletonKey = 0x00000F44,
	},
	.scene_field1_forest4_sce09a_sbh
	{
		.skeletonKey = 0x000012B4,
		.tableOfVoices = 0x00001674,
	},
	.scene_field1_forest4_sce10_sbh
	{
		.skeletonKey = 0x00000F64,
	},
	.scene_field1_forest4_sce12_sbh
	{
		.tableOfVoices = 0x00001B80,
		.kemuelTempleGate = 0x00025504
	},
	.scene_field1_forest4_sce14_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_field1_forest4_sce15_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_field1_forest4_sce16_sbh
	{
		.tableOfVoices = 0x00001BB0,
	},
	.scene_field1_forest4_sce16a_sbh
	{
		.tableOfVoices = 0x00001C28,
	},
	.scene_field1_forest4_sce16b_sbh
	{
		.tableOfVoices = 0x00001C28,
	},
	.scene_field1_forest4_sce17_sbh
	{
		.tableOfVoices = 0x000013B4,
	},
	.scene_field1_forest4_sce18_sbh
	{
		.tableOfVoices = 0x00001520,
	},
	.scene_field1_forest4_sce19_sbh
	{
		.gift_jewelOfWater = 0x0000216C,
		.tableOfVoices = 0x000018B8,
	},
	.scene_field1_gate_sce00_sbh
	{
		.forestOstId = 0x00001FA4,
		.forestOstIdFromBehind = 0x00000054,
	},
	.scene_field1_gate_sce00a_sbh
	{
		.korisMinionId = 0x000024F4,
		.gift_mugwort = 0x00002C24,
		.forestOstId = 0x000039EC,
		.forestOstIdFromBehind = 0x00000078,
		.tableOfVoices = 0x00002248,
	},
	.scene_field1_gate_sce00b_sbh
	{
		.forestOstId = 0x00002110,
		.forestOstIdFromBehind = 0x00000054,
	},
	.scene_field1_gate_sce00c_sbh
	{
		.forestOstId = 0x00002CB0,
		.forestOstIdFromBehind = 0x00000074,
	},
	.scene_field1_gate_sce01_sbh
	{
		.tableOfVoices = 0x00000A1C,
	},
	.scene_field1_gate_sce06_sbh
	{
		.forestOstId = 0x00002CB8,
		.forestOstIdFromBehind = 0x00000074,
		.tableOfVoices = 0x000027E4,
	},
	.scene_field2_forest1_sce02_sbh
	{
		.chest_varysBlessing = 0x00000EF4,
	},
	.scene_field2_forest1_sce06_sbh
	{
		.skeletonKey = 0x00000F74,
	},
	.scene_field2_forest1_sce07_sbh
	{
		.gift_jewelOfFire = 0x000025A0,
		.tableOfVoices = 0x00001A04,
	},
	.scene_field2_forest2_sce06_sbh
	{
		.chest_bletillaOil = 0x00000EF4,
	},
	.scene_field2_forest2_sce07_sbh
	{
		.gift_jewelOfWind = 0x00002EBC,
		.tableOfVoices = 0x00002080,
	},
	.scene_field2_forest2_sce13_sbh
	{
		.thunderbolt = 0x00000F5C,
	},
	.scene_field2_forest2_sce14_sbh
	{
		.skeletonKey = 0x00001464,
	},
	.scene_field2_forest3_sce07_sbh
	{
		.skeletonKey = 0x00000FB4,
	},
	.scene_field2_forest3_sce11a_sbh
	{
		.tableOfVoices = 0x00001580,
	},
	.scene_field2_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x00001588,
	},
	.scene_field2_forest3_sce13a_sbh
	{
		.tableOfVoices = 0x00001580,
	},
	.scene_field2_forest3_sce16_sbh
	{
		.chest_bitterMugwort = 0x00000EF4,
	},
	.scene_field2_forest3_sce18_sbh
	{
		.chest_urvysBlessing = 0x00000EF4,
	},
	.scene_field2_forest3_sce20_sbh
	{
		.tableOfVoices = 0x00001AAC,
	},
	.scene_field2_forest3_sce22a_sbh
	{
		.tableOfVoices = 0x00001DCC,
	},
	.scene_other_clear_sce00_sbh
	{
		.tableOfVoices = 0x00000D90,
	},
	.scene_other_clear_sce03_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_other_clear_sce04_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_other_dream_sce00_sbh
	{
		.dreamManIntroVoice = 0x0000152C,
		.tableOfVoices = 0x00001468,
	},
	.scene_other_dream_sce00a_sbh
	{
		.dreamMinionId = 0x0000134C,
		.dreamManIntroVoice = 0x000012C6,
		.tableOfVoices = 0x00001288,
	},
	.scene_other_hunting_sce00_sbh
	{
		.neutralSpawnEC = 0x00000D50,
		.magicsEC = 0x00001EAC,
		.statsEC = 0x00002FAC,
		.specialsEC = 0x00002114,
		.bossSpecialsEC = 0x00002C54,
		.bossDamageModifiersEC = 0x00002FFC,
	},
	.scene_psyrus1_cemetery_sce00_sbh
	{
		.tableOfVoices = 0x00000E28,
	},
	.scene_psyrus1_cemetery_sce00a_sbh
	{
		.tableOfVoices = 0x00000E20,
	},
	.scene_psyrus1_cemetery_sce00b_sbh
	{
		.tableOfVoices = 0x00000E70,
	},
	.scene_psyrus1_cemetery_sce00c_sbh
	{
		.tableOfVoices = 0x00000E40,
	},
	.scene_psyrus1_garai_sce00_sbh
	{
		.gift_fluteOfCapture = 0x00001598,
		.tableOfVoices = 0x00001104,
	},
	.scene_psyrus1_garai_sce01_sbh
	{
		.tableOfVoices = 0x00002130,
	},
	.scene_psyrus1_garai_sce01a_sbh
	{
		.tableOfVoices = 0x00002130,
	},
	.scene_psyrus1_garai_sce01b_sbh
	{
		.tableOfVoices = 0x0000215C,
	},
	.scene_psyrus1_garai_sce01c_sbh
	{
		.tableOfVoices = 0x000021D0,
	},
	.scene_psyrus1_garai_sce03_sbh
	{
		.gift_tearsOfReese = 0x0000103C,
		.tableOfVoices = 0x00000DC0,
	},
	.scene_psyrus1_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus1_kajiya_sce00a_sbh
	{
		.tableOfVoices = 0x00000ECC,
	},
	.scene_psyrus1_kajiya_sce00b_sbh
	{
		.gift_lifeStealer = 0x00001280,
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus1_kajiya_sce00c_sbh
	{
		.tableOfVoices = 0x00000F0C,
	},
	.scene_psyrus1_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00000ED8,
	},
	.scene_psyrus1_lebant_sce00a_sbh
	{
		.tableOfVoices = 0x00001440,
	},
	.scene_psyrus1_lebant_sce00c_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_psyrus1_lebant_sce00d_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_psyrus1_lookout_sce00_sbh
	{
		.tableOfVoices = 0x00000A5C,
	},
	.scene_psyrus1_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000B08,
	},
	.scene_psyrus1_lookout_sce00b_sbh
	{
		.tableOfVoices = 0x00000B08,
	},
	.scene_psyrus1_lookout_sce00c_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus1_lookout_sce01_sbh
	{
		.gift_nagiBelt = 0x000016F0,
		.tableOfVoices = 0x00000B78,
	},
	.scene_psyrus1_zokucho_sce01_sbh
	{
		.tableOfVoices = 0x0000167C,
	},
	.scene_psyrus1_zokucho_sce01a_sbh
	{
		.gift_championBelt = 0x00001E9C,
		.gift_captureAmulet = 0x00001EAC,
		.tableOfVoices = 0x00001618,
	},
	.scene_psyrus1_zokucho_sce05_sbh
	{
		.gift_huntersEarring = 0x00001FD0,
		.tableOfVoices = 0x00001524,
	},
	.scene_psyrus2_garai_sce00_sbh
	{
		.tableOfVoices = 0x00002194,
	},
	.scene_psyrus2_garai_sce03_sbh
	{
		.gift_mahbusCocoon = 0x00001F40,
		.gift_riketzGarb = 0x00002040,
		.tableOfVoices = 0x00001540,
	},
	.scene_psyrus2_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F0C,
	},
	.scene_psyrus2_lebant_sce00_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_psyrus2_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus2_lookout_sce01_sbh
	{
		.tableOfVoices = 0x000018C0,
	},
	.scene_psyrus2_lookout_sce01a_sbh
	{
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus2_zokucho_sce03_sbh
	{
		.tableOfVoices = 0x00001710,
	}
},
filePalEs
{
	.executable
	{
		.entityModelsBehavior = 0x0006C7E8,
		.entityStats = 0x0006AF6C,
		.entityMagics = 0x0006B164,
		.entitySpecials = 0x0006B4B0,
		.entityDamageModifiers = 0x0006C52C,
		.hudColors = 0x00077AFC,
		.afterGetStatsJump = 0x0000273C,
		.afterFramerateLimiter = 0x00000BC4,
		.hpMpBars = 0x0001C2D0,
		.bodyEnhancementFn = 0x0000A40C,
		.setVideoModeArgument = 0x00000924,
		.stretchingY = 0x00049134,
		.tableOfWeaponAnimationFileId = 0x0006A1C0,
		.tableOfWeaponStats = 0x0006DB04,
		.tableOfArmorStats = 0x0006DED8,
		.tableOfOtherStats = 0x0006E168,
		.tableOfWeaponCharacteristic = 0x0006DD2C,
		.tableOfArmorsAppearanceId = 0x0006E040,
		.tableOfYPositionMinionStatsFrame = 0x000739A0,
		.drawMinionStatsFrameFn = 0x0001E3A4,
		.drawMinionTIMStatsFrameFn = 0x0001E298,
		.tableOfModelAnimations = 0x0006E784,
		.tableOfModelAnimationsPtr = 0x00072F84,
		.actorBehaviorFn = 0x00018560,
		.mainFn = 0x00000800,
		.branchSetVatolkaSpecials = 0x00009E2C,
		.tableOfLevantFluteStylingsId = 0x0006D578,
		.cc_begin = 0x000BB800,
	},
	.over_battle_bin
	{
		.endOfBattle = 0x000047B0,
		.tableOfBattleWeaponsEffectPtr = 0x00000134,
		.setBattleWeaponsEffectFn = 0x00009C10,
		.endOfBattleSetSceneFn = 0x00004960,
		.storyMinionAI = 0x0000E2F4,
		.minionAgniaId = 0x0000FCA4,
		.delfanelEarthBossId = 0x0000FF2C,
		.tuturisFireBossId = 0x000100E0,
		.seterianAirBossId = 0x000102A0,
		.cushidraFireBossId = 0x000106B0,
		.tickerUlvi = 0x0000E520,
		.kolnaAdMumuls = 0x0000E598,		
		.captureSuccess = 0x00008838,
		.getFireflyCounterBattle = 0x00011344,		
	},
	.over_btend_bin
	{
		.entityTreasures = 0,
	},
	.over_btlexec_bin
	{
		.captureSuccess = 0,
	},
	.over_encom_bin
	{
		.storyMinionAI = 0,
		.minionAgniaId = 0,
		.delfanelEarthBossId = 0,
		.tuturisFireBossId = 0,
		.seterianAirBossId = 0,
		.cushidraFireBossId = 0,
		.tickerUlvi = 0,
		.kolnaAdMumuls = 0,
	},
	.over_game_bin
	{
		.tableOfStoryMinionsId = 0x0000E0B4,
		.spawnStory = 0x00004C34,
		.fireMapBonus = 0x00004E08,
		.battleMaps = 0x0000DD00,
		.levelCapEC = 0x000049BC,
		.levantAnimArgs = 0x00007F58,
		.chooseMagicLevantAnim = 0x0000B9B8,
		.levantCastingItemId = 0x00010D48,
		.tableOfWeaponAnimationsId = 0x00010E7C,
		.isQuantityLimitReachedFn = 0x0000D310,
		.itemShopBuyFn = 0x0000D178,
		.tableOfMapsBehavior = 0x0000E8E4,
		.readFlagRotateSpawn = 0x00004BD0,
	},
	.over_menu_bin
	{
		.entityTreasures = 0x0000B4B8,
	},
	.over_plcom_bin
	{
		.getFireflyCounterBattle = 0,
	},
	.over_title_bin
	{
		.levantStats = 0x00002524,
		.setLevantGarb = 0x00001EB8,
		.initMapNewGame = 0x00001A20,
	},
	.over_wpnshop_bin
	{
		.items = 0x00009190,
		.itemShopQuantityLimitFn = 0x00007F08,
		.equipmentShopQuantityLimitFn = 0x00007DD8,
		.initEternalCorridorShopFn = 0x00004DCC,
	},
	.scene_field1_forest1_sce00_sbh
	{
		.knowledge1 = 0x00000F8C,
	},
	.scene_field1_forest1_sce01_sbh
	{
		.shabLiquor = 0x0000149C,
		.treeBranchBehaviorFn = 0x00000240,
	},
	.scene_field1_forest1_sce02_sbh
	{
		.chest_metaMugwort = 0x00000F14,
	},
	.scene_field1_forest1_sce03_sbh
	{
		.knowledge2 = 0x00000FB4,
	},
	.scene_field1_forest1_sce04_sbh
	{
		.knowledge3 = 0x00000FD4,
	},
	.scene_field1_forest1_sce05_sbh
	{
		.skeletonKey = 0x00000F64,
	},
	.scene_field1_forest1_sce06_sbh
	{
		.aquazor = 0x00000F7C,
	},
	.scene_field1_forest1_sce06a_sbh
	{
		.aquazor = 0x00001374,
		.tableOfVoices = 0x00001530,
	},
	.scene_field1_forest1_sce08_sbh
	{
		.beetleWarp = 0x00002178,
	},
	.scene_field1_forest1_sce09_sbh
	{
		.tableOfVoices = 0x00000B64,
	},
	.scene_field1_forest2_sce00_sbh
	{
		.kickleberry = 0x00000F84,
	},
	.scene_field1_forest2_sce01_sbh
	{
		.knowledge4 = 0x00000F7C,
	},
	.scene_field1_forest2_sce03_sbh
	{
		.skeletonKey = 0x00000FBC,
	},
	.scene_field1_forest2_sce04_sbh
	{
		.magicUsage1 = 0x00000F94,
	},
	.scene_field1_forest2_sce05_sbh
	{
		.chest_chestNutOil = 0x00000F14,
	},
	.scene_field1_forest2_sce06_sbh
	{
		.larvaKeyBehaviorFn = 0x000004D0,
		.doorBehaviorFn = 0x000001E0,
	},
	.scene_field1_forest2_sce07_sbh
	{
		.gift_calabasPowder = 0x00001DCC,
		.gift_ocarina = 0x00002754,
		.gift_specialLiquor = 0x00001DDC,
		.sleepSpore = 0x0000191C,
		.tableOfVoices = 0x00001B2C,
	},
	.scene_field1_forest2_sce07a_sbh
	{
		.sleepSpore = 0x000014F8,
	},
	.scene_field1_forest2_sce08_sbh
	{
		.dragonflyWarp = 0x00002178,
	},
	.scene_field1_forest2_sce10_sbh
	{
		.skeletonKey = 0x0000146C,
		.bridgeBehaviorFn = 0x00000210,
	},
	.scene_field1_forest2_sce12_sbh
	{
		.chest_agnisSeal1 = 0x00000F14,
	},
	.scene_field1_forest2_sce13_sbh
	{
		.magicUsage2 = 0x00000F8C,
	},
	.scene_field1_forest2_sce13a_sbh
	{
		.magicUsage2 = 0x000012F4,
		.tableOfVoices = 0x000016B4,
	},
	.scene_field1_forest2_sce14a_sbh
	{
		.secretOfKilling = 0x000017A4,
		.tableOfVoices = 0x00001984,
		.kikinakReturnOstId = 0x00001BE4,
	},
	.scene_field1_forest3_sce01_sbh
	{
		.whiteHorn = 0x00000F94,
	},
	.scene_field1_forest3_sce03_sbh
	{
		.ikariPowder = 0x00000F6C,
	},
	.scene_field1_forest3_sce03a_sbh
	{
		.ikariPowder = 0x000012D4,
		.tableOfVoices = 0x00001694,
	},
	.scene_field1_forest3_sce05_sbh
	{
		.chest_stoneChoker = 0x00000F14,
	},
	.scene_field1_forest3_sce06_sbh
	{
		.skeletonKey = 0x00000FA4,
	},
	.scene_field1_forest3_sce07_sbh
	{
		.divineTablet = 0x00000F7C,
	},
	.scene_field1_forest3_sce09_sbh
	{
		.minionFang = 0x00000F8C,
	},
	.scene_field1_forest3_sce10_sbh
	{
		.spiderWarp = 0x00002170,
	},
	.scene_field1_forest3_sce11_sbh
	{
		.knowledge5 = 0x00001300,
		.tableOfVoices = 0x000013E4,
	},
	.scene_field1_forest3_sce11a_sbh
	{
		.knowledge5 = 0x00001304,
		.tableOfVoices = 0x000013E8,
	},
	.scene_field1_forest3_sce12_sbh
	{
		.tableOfVoices = 0x000010C0,
	},
	.scene_field1_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x000010C0,
	},
	.scene_field1_forest3_sce13_sbh
	{
		.merging2 = 0x00001310,
		.tableOfVoices = 0x000013F4,
	},
	.scene_field1_forest3_sce13a_sbh
	{
		.merging2 = 0x00001310,
		.tableOfVoices = 0x000013F4,
	},
	.scene_field1_forest3_sce14_sbh
	{
		.merging1 = 0x000012EC,
		.tableOfVoices = 0x000013D0,
	},
	.scene_field1_forest3_sce14a_sbh
	{
		.gift_tearsOfReese = 0x000015D0,
		.merging1 = 0x000012E8,
		.tableOfVoices = 0x000013CC,
	},
	.scene_field1_forest3_sce16_sbh
	{
		.excellentLiquor = 0x00000F7C,
	},
	.scene_field1_forest3_sce17_sbh
	{
		.chest_yellowBead = 0x000013EC,
	},
	.scene_field1_forest3_sce18_sbh
	{
		.skeletonKey = 0x00000FCC,
	},
	.scene_field1_forest3_sce20_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_field1_forest3_sce22_sbh
	{
		.whiteHorn = 0x00000F94,
	},
	.scene_field1_forest3_sce24_sbh
	{
		.ikariPowder = 0x00000F6C,
	},
	.scene_field1_forest3_sce26_sbh
	{
		.chest_stoneChoker = 0x00000F14,
	},
	.scene_field1_forest3_sce27_sbh
	{
		.skeletonKey = 0x00000FA4,
	},
	.scene_field1_forest3_sce28_sbh
	{
		.divineTablet = 0x00000F7C,
	},
	.scene_field1_forest3_sce30_sbh
	{
		.minionFang = 0x00000F8C,
	},
	.scene_field1_forest3_sce31_sbh
	{
		.spiderWarp = 0x00001CA8,
	},
	.scene_field1_forest3_sce32_sbh
	{
		.knowledge5 = 0x00001304,
		.tableOfVoices = 0x00001418,
	},
	.scene_field1_forest3_sce33_sbh
	{
		.tableOfVoices = 0x00000C18,
	},
	.scene_field1_forest3_sce34_sbh
	{
		.merging2 = 0x00001310,
		.tableOfVoices = 0x00001424,
	},
	.scene_field1_forest3_sce35_sbh
	{
		.merging1 = 0x00000F8C,
	},
	.scene_field1_forest3_sce37_sbh
	{
		.excellentLiquor = 0x00000F7C,
	},
	.scene_field1_forest3_sce38_sbh
	{
		.chest_yellowBead = 0x00000F14,
	},
	.scene_field1_forest3_sce39_sbh
	{
		.skeletonKey = 0x00000FCC,
	},
	.scene_field1_forest4_sce00_sbh
	{
		.excellentLiquor = 0x00000FD4,
	},
	.scene_field1_forest4_sce01_sbh
	{
		.chest_bitterMugwort = 0x00000F14,
	},
	.scene_field1_forest4_sce03_sbh
	{
		.chest_shishiudoOil = 0x00000F14,
	},
	.scene_field1_forest4_sce04_sbh
	{
		.whistler = 0x0000146C,
	},
	.scene_field1_forest4_sce05_sbh
	{
		.greatWalnut = 0x00000F74,
	},
	.scene_field1_forest4_sce06_sbh
	{
		.skeletonKey = 0x00000F5C,
	},
	.scene_field1_forest4_sce07_sbh
	{
		.pupaKeyBehaviorFn = 0x000006BC,
		.doorBehaviorFn = 0x000001E0,
	},
	.scene_field1_forest4_sce08_sbh
	{
		.chest_urvysBlessing = 0x00000F14,
	},
	.scene_field1_forest4_sce09_sbh
	{
		.skeletonKey = 0x00000F5C,
	},
	.scene_field1_forest4_sce09a_sbh
	{
		.skeletonKey = 0x000012CC,
		.tableOfVoices = 0x0000168C,
	},
	.scene_field1_forest4_sce10_sbh
	{
		.skeletonKey = 0x00000F6C,
	},
	.scene_field1_forest4_sce12_sbh
	{
		.tableOfVoices = 0x00001B80,
		.kemuelTempleGate = 0x00025498
	},
	.scene_field1_forest4_sce14_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_field1_forest4_sce15_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_field1_forest4_sce16_sbh
	{
		.tableOfVoices = 0x00001BB4,
	},
	.scene_field1_forest4_sce16a_sbh
	{
		.tableOfVoices = 0x00001C2C,
	},
	.scene_field1_forest4_sce16b_sbh
	{
		.tableOfVoices = 0x00001C2C,
	},
	.scene_field1_forest4_sce17_sbh
	{
		.tableOfVoices = 0x000013B4,
	},
	.scene_field1_forest4_sce18_sbh
	{
		.tableOfVoices = 0x00001520,
	},
	.scene_field1_forest4_sce19_sbh
	{
		.gift_jewelOfWater = 0x0000216C,
		.tableOfVoices = 0x000018B8,
	},
	.scene_field1_gate_sce00_sbh
	{
		.forestOstId = 0x00001FA4,
		.forestOstIdFromBehind = 0x00000054,
	},
	.scene_field1_gate_sce00a_sbh
	{
		.korisMinionId = 0x000024F4,
		.gift_mugwort = 0x00002C24,
		.forestOstId = 0x000039EC,
		.forestOstIdFromBehind = 0x00000078,
		.tableOfVoices = 0x00002248,
	},
	.scene_field1_gate_sce00b_sbh
	{
		.forestOstId = 0x00002110,
		.forestOstIdFromBehind = 0x00000054,
	},
	.scene_field1_gate_sce00c_sbh
	{
		.forestOstId = 0x00002CB0,
		.forestOstIdFromBehind = 0x00000074,
	},
	.scene_field1_gate_sce01_sbh
	{
		.tableOfVoices = 0x00000A1C,
	},
	.scene_field1_gate_sce06_sbh
	{
		.forestOstId = 0x00002CB8,
		.forestOstIdFromBehind = 0x00000074,
		.tableOfVoices = 0x000027E4,
	},
	.scene_field2_forest1_sce02_sbh
	{
		.chest_varysBlessing = 0x00000F14,
	},
	.scene_field2_forest1_sce06_sbh
	{
		.skeletonKey = 0x00000F8C,
	},
	.scene_field2_forest1_sce07_sbh
	{
		.gift_jewelOfFire = 0x000025A8,
		.tableOfVoices = 0x00001A0C,
	},
	.scene_field2_forest2_sce06_sbh
	{
		.chest_bletillaOil = 0x00000F14,
	},
	.scene_field2_forest2_sce07_sbh
	{
		.gift_jewelOfWind = 0x00002EC8,
		.tableOfVoices = 0x0000208C,
	},
	.scene_field2_forest2_sce13_sbh
	{
		.thunderbolt = 0x00000F74,
	},
	.scene_field2_forest2_sce14_sbh
	{
		.skeletonKey = 0x00001444,
	},
	.scene_field2_forest3_sce07_sbh
	{
		.skeletonKey = 0x00000F94,
	},
	.scene_field2_forest3_sce11a_sbh
	{
		.tableOfVoices = 0x00001590,
	},
	.scene_field2_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x00001598,
	},
	.scene_field2_forest3_sce13a_sbh
	{
		.tableOfVoices = 0x00001590,
	},
	.scene_field2_forest3_sce16_sbh
	{
		.chest_bitterMugwort = 0x00000F14,
	},
	.scene_field2_forest3_sce18_sbh
	{
		.chest_urvysBlessing = 0x00000F14,
	},
	.scene_field2_forest3_sce20_sbh
	{
		.tableOfVoices = 0x00001ABC,
	},
	.scene_field2_forest3_sce22a_sbh
	{
		.tableOfVoices = 0x00001DC8,
	},
	.scene_other_clear_sce00_sbh
	{
		.tableOfVoices = 0x00000D90,
	},
	.scene_other_clear_sce03_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_other_clear_sce04_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_other_dream_sce00_sbh
	{
		.dreamManIntroVoice = 0x0000153C,
		.tableOfVoices = 0x00001478,
	},
	.scene_other_dream_sce00a_sbh
	{
		.dreamMinionId = 0x0000135C,
		.dreamManIntroVoice = 0x000012D6,
		.tableOfVoices = 0x00001298,
	},
	.scene_other_hunting_sce00_sbh
	{
		.neutralSpawnEC = 0x00000D50,
		.magicsEC = 0x00001EAC,
		.statsEC = 0x00002FAC,
		.specialsEC = 0x00002114,
		.bossSpecialsEC = 0x00002C54,
		.bossDamageModifiersEC = 0x00002FFC,
	},
	.scene_psyrus1_cemetery_sce00_sbh
	{
		.tableOfVoices = 0x00000E28,
	},
	.scene_psyrus1_cemetery_sce00a_sbh
	{
		.tableOfVoices = 0x00000E20,
	},
	.scene_psyrus1_cemetery_sce00b_sbh
	{
		.tableOfVoices = 0x00000E70,
	},
	.scene_psyrus1_cemetery_sce00c_sbh
	{
		.tableOfVoices = 0x00000E40,
	},
	.scene_psyrus1_garai_sce00_sbh
	{
		.gift_fluteOfCapture = 0x00001598,
		.tableOfVoices = 0x00001104,
	},
	.scene_psyrus1_garai_sce01_sbh
	{
		.tableOfVoices = 0x00002130,
	},
	.scene_psyrus1_garai_sce01a_sbh
	{
		.tableOfVoices = 0x00002130,
	},
	.scene_psyrus1_garai_sce01b_sbh
	{
		.tableOfVoices = 0x0000215C,
	},
	.scene_psyrus1_garai_sce01c_sbh
	{
		.tableOfVoices = 0x000021D0,
	},
	.scene_psyrus1_garai_sce03_sbh
	{
		.gift_tearsOfReese = 0x0000103C,
		.tableOfVoices = 0x00000DC0,
	},
	.scene_psyrus1_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus1_kajiya_sce00a_sbh
	{
		.tableOfVoices = 0x00000ECC,
	},
	.scene_psyrus1_kajiya_sce00b_sbh
	{
		.gift_lifeStealer = 0x00001280,
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus1_kajiya_sce00c_sbh
	{
		.tableOfVoices = 0x00000F0C,
	},
	.scene_psyrus1_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00000ED8,
	},
	.scene_psyrus1_lebant_sce00a_sbh
	{
		.tableOfVoices = 0x00001440,
	},
	.scene_psyrus1_lebant_sce00c_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_psyrus1_lebant_sce00d_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_psyrus1_lookout_sce00_sbh
	{
		.tableOfVoices = 0x00000A5C,
	},
	.scene_psyrus1_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000B08,
	},
	.scene_psyrus1_lookout_sce00b_sbh
	{
		.tableOfVoices = 0x00000B08,
	},
	.scene_psyrus1_lookout_sce00c_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus1_lookout_sce01_sbh
	{
		.gift_nagiBelt = 0x000016F0,
		.tableOfVoices = 0x00000B78,
	},
	.scene_psyrus1_zokucho_sce01_sbh
	{
		.tableOfVoices = 0x0000167C,
	},
	.scene_psyrus1_zokucho_sce01a_sbh
	{
		.gift_championBelt = 0x00001E9C,
		.gift_captureAmulet = 0x00001EAC,
		.tableOfVoices = 0x00001618,
	},
	.scene_psyrus1_zokucho_sce05_sbh
	{
		.gift_huntersEarring = 0x00001FD0,
		.tableOfVoices = 0x00001524,
	},
	.scene_psyrus2_garai_sce00_sbh
	{
		.tableOfVoices = 0x00002194,
	},
	.scene_psyrus2_garai_sce03_sbh
	{
		.gift_mahbusCocoon = 0x00001F40,
		.gift_riketzGarb = 0x00002040,
		.tableOfVoices = 0x00001540,
	},
	.scene_psyrus2_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F0C,
	},
	.scene_psyrus2_lebant_sce00_sbh
	{
		.tableOfVoices = 0x0000141C,

	},
	.scene_psyrus2_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus2_lookout_sce01_sbh
	{
		.tableOfVoices = 0x000018C0,
	},
	.scene_psyrus2_lookout_sce01a_sbh
	{
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus2_zokucho_sce03_sbh
	{
		.tableOfVoices = 0x00001710,
	}
},
filePalIt
{
	.executable
	{
		.entityModelsBehavior = 0x0006C7CC,
		.entityStats = 0x0006AF50,
		.entityMagics = 0x0006B148,
		.entitySpecials = 0x0006B494,
		.entityDamageModifiers = 0x0006C510,
		.hudColors = 0x00077B14,
		.afterGetStatsJump = 0x0000273C,
		.afterFramerateLimiter = 0x00000BC4,
		.hpMpBars = 0x0001C2D0,
		.bodyEnhancementFn = 0x0000A40C,
		.setVideoModeArgument = 0x00000924,
		.stretchingY = 0x0004912C,
		.tableOfWeaponAnimationFileId = 0x0006A1A4,
		.tableOfWeaponStats = 0x0006DAE8,
		.tableOfArmorStats = 0x0006DEBC,
		.tableOfOtherStats = 0x0006E14C,
		.tableOfWeaponCharacteristic = 0x0006DD10,
		.tableOfArmorsAppearanceId = 0x0006E024,
		.tableOfYPositionMinionStatsFrame = 0x00073984,
		.drawMinionStatsFrameFn = 0x0001E3A4,
		.drawMinionTIMStatsFrameFn = 0x0001E298,
		.tableOfModelAnimations = 0x0006E768,
		.tableOfModelAnimationsPtr = 0x00072F68,
		.actorBehaviorFn = 0x00018560,
		.mainFn = 0x00000800,
		.branchSetVatolkaSpecials = 0x00009E2C,
		.tableOfLevantFluteStylingsId = 0x0006D55C,
		.cc_begin = 0x000BB800,
	},
	.over_battle_bin
	{
		.endOfBattle = 0x000046D8,
		.tableOfBattleWeaponsEffectPtr = 0x00000134,
		.setBattleWeaponsEffectFn = 0x00009B38,
		.endOfBattleSetSceneFn = 0x00004888,
		.storyMinionAI = 0x0000E21C,
		.minionAgniaId = 0x0000FBCC,
		.delfanelEarthBossId = 0x0000FE54,
		.tuturisFireBossId = 0x00010008,
		.seterianAirBossId = 0x000101C8,
		.cushidraFireBossId = 0x000105D8,
		.tickerUlvi = 0x0000E448,
		.kolnaAdMumuls = 0x0000E4C0,	
		.captureSuccess = 0x00008760,
		.getFireflyCounterBattle = 0x0001126C,
	},
	.over_btend_bin
	{
		.entityTreasures = 0,
	},
	.over_btlexec_bin
	{
		.captureSuccess = 0,
	},
	.over_encom_bin
	{
		.storyMinionAI = 0,
		.minionAgniaId = 0,
		.delfanelEarthBossId = 0,
		.tuturisFireBossId = 0,
		.seterianAirBossId = 0,
		.cushidraFireBossId = 0,
		.tickerUlvi = 0,
		.kolnaAdMumuls = 0,
	},
	.over_game_bin
	{
		.tableOfStoryMinionsId = 0x0000E0B4,
		.spawnStory = 0x00004C34,
		.fireMapBonus = 0x00004E08,
		.battleMaps = 0x0000DD00,
		.levelCapEC = 0x000049BC,
		.levantAnimArgs = 0x00007F58,		
		.chooseMagicLevantAnim = 0x0000B9B8,
		.levantCastingItemId = 0x00010D48,
		.tableOfWeaponAnimationsId = 0x00010E7C,
		.isQuantityLimitReachedFn = 0x0000D310,
		.itemShopBuyFn = 0x0000D178,
		.tableOfMapsBehavior = 0x0000E8E4,
		.readFlagRotateSpawn = 0x00004BD0,
	},
	.over_menu_bin
	{
		.entityTreasures = 0x0000B390,
	},
	.over_plcom_bin
	{
		.getFireflyCounterBattle = 0,
	},
	.over_title_bin
	{
		.levantStats = 0x00002524,
		.setLevantGarb = 0x00001EB8,
		.initMapNewGame = 0x00001A20,
	},
	.over_wpnshop_bin
	{
		.items = 0x000090BC,
		.itemShopQuantityLimitFn = 0x00007E38,
		.equipmentShopQuantityLimitFn = 0x00007D08,
		.initEternalCorridorShopFn = 0x00004CFC,
	},
	.scene_field1_forest1_sce00_sbh
	{
		.knowledge1 = 0x00000FC4,
	},
	.scene_field1_forest1_sce01_sbh
	{
		.shabLiquor = 0x000014C4,
		.treeBranchBehaviorFn = 0x00000268,
	},
	.scene_field1_forest1_sce02_sbh
	{
		.chest_metaMugwort = 0x00000F3C,
	},
	.scene_field1_forest1_sce03_sbh
	{
		.knowledge2 = 0x00000FE4,
	},
	.scene_field1_forest1_sce04_sbh
	{
		.knowledge3 = 0x0000100C,
	},
	.scene_field1_forest1_sce05_sbh
	{
		.skeletonKey = 0x00000F94,
	},
	.scene_field1_forest1_sce06_sbh
	{
		.aquazor = 0x00000FA4,
	},
	.scene_field1_forest1_sce06a_sbh
	{
		.aquazor = 0x000013A4,
		.tableOfVoices = 0x00001560,
	},
	.scene_field1_forest1_sce08_sbh
	{
		.beetleWarp = 0x000021A8,
	},
	.scene_field1_forest1_sce09_sbh
	{
		.tableOfVoices = 0x00000B64,
	},
	.scene_field1_forest2_sce00_sbh
	{
		.kickleberry = 0x00000F9C,
	},
	.scene_field1_forest2_sce01_sbh
	{
		.knowledge4 = 0x00000FB4,
	},
	.scene_field1_forest2_sce03_sbh
	{
		.skeletonKey = 0x00000FE4,
	},
	.scene_field1_forest2_sce04_sbh
	{
		.magicUsage1 = 0x00000FBC,
	},
	.scene_field1_forest2_sce05_sbh
	{
		.chest_chestNutOil = 0x00000F3C,
	},
	.scene_field1_forest2_sce06_sbh
	{
		.larvaKeyBehaviorFn = 0x00000524,
		.doorBehaviorFn = 0x00000234,
	},
	.scene_field1_forest2_sce07_sbh
	{
		.gift_calabasPowder = 0x00001E0C,
		.gift_ocarina = 0x00002794,
		.gift_specialLiquor = 0x00001E1C,
		.sleepSpore = 0x0000195C,
		.tableOfVoices = 0x00001B6C,
	},
	.scene_field1_forest2_sce07a_sbh
	{
		.sleepSpore = 0x00001530,
	},
	.scene_field1_forest2_sce08_sbh
	{
		.dragonflyWarp = 0x000021A8,
	},
	.scene_field1_forest2_sce10_sbh
	{
		.skeletonKey = 0x00001484,
		.bridgeBehaviorFn = 0x00000228,
	},
	.scene_field1_forest2_sce12_sbh
	{
		.chest_agnisSeal1 = 0x00000F3C,
	},
	.scene_field1_forest2_sce13_sbh
	{
		.magicUsage2 = 0x00000FBC,
	},
	.scene_field1_forest2_sce13a_sbh
	{
		.magicUsage2 = 0x00001334,
		.tableOfVoices = 0x000016F4,
	},
	.scene_field1_forest2_sce14a_sbh
	{
		.secretOfKilling = 0x000017DC,
		.tableOfVoices = 0x000019BC,
		.kikinakReturnOstId = 0x00001C1C,
	},
	.scene_field1_forest3_sce01_sbh
	{
		.whiteHorn = 0x00000FBC,
	},
	.scene_field1_forest3_sce03_sbh
	{
		.ikariPowder = 0x00000F94,
	},
	.scene_field1_forest3_sce03a_sbh
	{
		.ikariPowder = 0x0000130C,
		.tableOfVoices = 0x000016CC,
	},
	.scene_field1_forest3_sce05_sbh
	{
		.chest_stoneChoker = 0x00000F3C,
	},
	.scene_field1_forest3_sce06_sbh
	{
		.skeletonKey = 0x00000FC4,
	},
	.scene_field1_forest3_sce07_sbh
	{
		.divineTablet = 0x00000FB4,
	},
	.scene_field1_forest3_sce09_sbh
	{
		.minionFang = 0x00000F9C,
	},
	.scene_field1_forest3_sce10_sbh
	{
		.spiderWarp = 0x000021A0,
	},
	.scene_field1_forest3_sce11_sbh
	{
		.knowledge5 = 0x00001338,
		.tableOfVoices = 0x0000141C,
	},
	.scene_field1_forest3_sce11a_sbh
	{
		.knowledge5 = 0x0000133C,
		.tableOfVoices = 0x00001420,
	},
	.scene_field1_forest3_sce12_sbh
	{
		.tableOfVoices = 0x000010C0,
	},
	.scene_field1_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x000010C0,
	},
	.scene_field1_forest3_sce13_sbh
	{
		.merging2 = 0x00001320,
		.tableOfVoices = 0x00001404,
	},
	.scene_field1_forest3_sce13a_sbh
	{
		.merging2 = 0x00001320,
		.tableOfVoices = 0x00001404,
	},
	.scene_field1_forest3_sce14_sbh
	{
		.merging1 = 0x0000131C,
		.tableOfVoices = 0x00001400,
	},
	.scene_field1_forest3_sce14a_sbh
	{
		.gift_tearsOfReese = 0x00001600,
		.merging1 = 0x00001318,
		.tableOfVoices = 0x000013FC,
	},
	.scene_field1_forest3_sce16_sbh
	{
		.excellentLiquor = 0x00000FBC,
	},
	.scene_field1_forest3_sce17_sbh
	{
		.chest_yellowBead = 0x00001414,
	},
	.scene_field1_forest3_sce18_sbh
	{
		.skeletonKey = 0x00000FFC,
	},
	.scene_field1_forest3_sce20_sbh
	{
		.tableOfVoices = 0x0000141C,
	},
	.scene_field1_forest3_sce22_sbh
	{
		.whiteHorn = 0x00000FBC,
	},
	.scene_field1_forest3_sce24_sbh
	{
		.ikariPowder = 0x00000F94,
	},
	.scene_field1_forest3_sce26_sbh
	{
		.chest_stoneChoker = 0x00000F3C,
	},
	.scene_field1_forest3_sce27_sbh
	{
		.skeletonKey = 0x00000FC4,
	},
	.scene_field1_forest3_sce28_sbh
	{
		.divineTablet = 0x00000FB4,
	},
	.scene_field1_forest3_sce30_sbh
	{
		.minionFang = 0x00000F9C,
	},
	.scene_field1_forest3_sce31_sbh
	{
		.spiderWarp = 0x00001CD8,
	},
	.scene_field1_forest3_sce32_sbh
	{
		.knowledge5 = 0x0000133C,
		.tableOfVoices = 0x00001450,
	},
	.scene_field1_forest3_sce33_sbh
	{
		.tableOfVoices = 0x00000C18,
	},
	.scene_field1_forest3_sce34_sbh
	{
		.merging2 = 0x00001320,
		.tableOfVoices = 0x00001434,
	},
	.scene_field1_forest3_sce35_sbh
	{
		.merging1 = 0x00000FBC,
	},
	.scene_field1_forest3_sce37_sbh
	{
		.excellentLiquor = 0x00000FBC,
	},
	.scene_field1_forest3_sce38_sbh
	{
		.chest_yellowBead = 0x00000F3C,
	},
	.scene_field1_forest3_sce39_sbh
	{
		.skeletonKey = 0x00000FFC,
	},
	.scene_field1_forest4_sce00_sbh
	{
		.excellentLiquor = 0x0000100C,
	},
	.scene_field1_forest4_sce01_sbh
	{
		.chest_bitterMugwort = 0x00000F3C,
	},
	.scene_field1_forest4_sce03_sbh
	{
		.chest_shishiudoOil = 0x00000F3C,
	},
	.scene_field1_forest4_sce04_sbh
	{
		.whistler = 0x000014AC,
	},
	.scene_field1_forest4_sce05_sbh
	{
		.greatWalnut = 0x00000FC4,
	},
	.scene_field1_forest4_sce06_sbh
	{
		.skeletonKey = 0x00000F8C,
	},
	.scene_field1_forest4_sce07_sbh
	{
		.pupaKeyBehaviorFn = 0x00000710,
		.doorBehaviorFn = 0x00000234,
	},
	.scene_field1_forest4_sce08_sbh
	{
		.chest_urvysBlessing = 0x00000F3C,
	},
	.scene_field1_forest4_sce09_sbh
	{
		.skeletonKey = 0x00000F84,
	},
	.scene_field1_forest4_sce09a_sbh
	{
		.skeletonKey = 0x000012FC,
		.tableOfVoices = 0x000016BC,
	},
	.scene_field1_forest4_sce10_sbh
	{
		.skeletonKey = 0x00000FAC,
	},
	.scene_field1_forest4_sce12_sbh
	{
		.tableOfVoices = 0x00001B80,
		.kemuelTempleGate = 0x000254B8
	},
	.scene_field1_forest4_sce14_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_field1_forest4_sce15_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_field1_forest4_sce16_sbh
	{
		.tableOfVoices = 0x00001BB8,
	},
	.scene_field1_forest4_sce16a_sbh
	{
		.tableOfVoices = 0x00001C30,
	},
	.scene_field1_forest4_sce16b_sbh
	{
		.tableOfVoices = 0x00001C30,
	},
	.scene_field1_forest4_sce17_sbh
	{
		.tableOfVoices = 0x000013B4,
	},
	.scene_field1_forest4_sce18_sbh
	{
		.tableOfVoices = 0x00001520,
	},
	.scene_field1_forest4_sce19_sbh
	{
		.gift_jewelOfWater = 0x0000216C,
		.tableOfVoices = 0x000018B8,
	},
	.scene_field1_gate_sce00_sbh
	{
		.forestOstId = 0x00001F9C,
		.forestOstIdFromBehind = 0x0000004C,
	},
	.scene_field1_gate_sce00a_sbh
	{
		.korisMinionId = 0x000024EC,
		.gift_mugwort = 0x00002C1C,
		.forestOstId = 0x000039E4,
		.forestOstIdFromBehind = 0x00000070,
		.tableOfVoices = 0x00002240,
	},
	.scene_field1_gate_sce00b_sbh
	{
		.forestOstId = 0x00002108,
		.forestOstIdFromBehind = 0x0000004C,
	},
	.scene_field1_gate_sce00c_sbh
	{
		.forestOstId = 0x00002CA8,
		.forestOstIdFromBehind = 0x0000006C,
	},
	.scene_field1_gate_sce01_sbh
	{
		.tableOfVoices = 0x00000A1C,
	},
	.scene_field1_gate_sce06_sbh
	{
		.forestOstId = 0x00002CB0,
		.forestOstIdFromBehind = 0x0000006C,
		.tableOfVoices = 0x000027DC,
	},
	.scene_field2_forest1_sce02_sbh
	{
		.chest_varysBlessing = 0x00000F3C,
	},
	.scene_field2_forest1_sce06_sbh
	{
		.skeletonKey = 0x00000FA4,
	},
	.scene_field2_forest1_sce07_sbh
	{
		.gift_jewelOfFire = 0x000025AC,
		.tableOfVoices = 0x00001A10,
	},
	.scene_field2_forest2_sce06_sbh
	{
		.chest_bletillaOil = 0x00000F3C,
	},
	.scene_field2_forest2_sce07_sbh
	{
		.gift_jewelOfWind = 0x00002ECC,
		.tableOfVoices = 0x00002090,
	},
	.scene_field2_forest2_sce13_sbh
	{
		.thunderbolt = 0x00000FA4,
	},
	.scene_field2_forest2_sce14_sbh
	{
		.skeletonKey = 0x0000148C,
	},
	.scene_field2_forest3_sce07_sbh
	{
		.skeletonKey = 0x00000FCC,
	},
	.scene_field2_forest3_sce11a_sbh
	{
		.tableOfVoices = 0x00001594,
	},
	.scene_field2_forest3_sce12a_sbh
	{
		.tableOfVoices = 0x0000159C,
	},
	.scene_field2_forest3_sce13a_sbh
	{
		.tableOfVoices = 0x00001594,
	},
	.scene_field2_forest3_sce16_sbh
	{
		.chest_bitterMugwort = 0x00000F3C,
	},
	.scene_field2_forest3_sce18_sbh
	{
		.chest_urvysBlessing = 0x00000F3C,
	},
	.scene_field2_forest3_sce20_sbh
	{
		.tableOfVoices = 0x00001AC0,
	},
	.scene_field2_forest3_sce22a_sbh
	{
		.tableOfVoices = 0x00001DC0,
	},
	.scene_other_clear_sce00_sbh
	{
		.tableOfVoices = 0x00000D90,
	},
	.scene_other_clear_sce03_sbh
	{
		.tableOfVoices = 0x00000C04,
	},
	.scene_other_clear_sce04_sbh
	{
		.tableOfVoices = 0x00001D9C,
	},
	.scene_other_dream_sce00_sbh
	{
		.dreamManIntroVoice = 0x00001530,
		.tableOfVoices = 0x0000146C,
	},
	.scene_other_dream_sce00a_sbh
	{
		.dreamMinionId = 0x00001350,
		.dreamManIntroVoice = 0x000012CA,
		.tableOfVoices = 0x0000128C,
	},
	.scene_other_hunting_sce00_sbh
	{
		.neutralSpawnEC = 0x00000D50,
		.magicsEC = 0x00001EAC,
		.statsEC = 0x00002FAC,
		.specialsEC = 0x00002114,
		.bossSpecialsEC = 0x00002C54,
		.bossDamageModifiersEC = 0x00002FFC,

	},
	.scene_psyrus1_cemetery_sce00_sbh
	{
		.tableOfVoices = 0x00000E28,
	},
	.scene_psyrus1_cemetery_sce00a_sbh
	{
		.tableOfVoices = 0x00000E20,
	},
	.scene_psyrus1_cemetery_sce00b_sbh
	{
		.tableOfVoices = 0x00000E70,
	},
	.scene_psyrus1_cemetery_sce00c_sbh
	{
		.tableOfVoices = 0x00000E40,
	},
	.scene_psyrus1_garai_sce00_sbh
	{
		.gift_fluteOfCapture = 0x0000159C,
		.tableOfVoices = 0x00001108,
	},
	.scene_psyrus1_garai_sce01_sbh
	{
		.tableOfVoices = 0x00002134,
	},
	.scene_psyrus1_garai_sce01a_sbh
	{
		.tableOfVoices = 0x00002134,
	},
	.scene_psyrus1_garai_sce01b_sbh
	{
		.tableOfVoices = 0x00002160,
	},
	.scene_psyrus1_garai_sce01c_sbh
	{
		.tableOfVoices = 0x000021D4,
	},
	.scene_psyrus1_garai_sce03_sbh
	{
		.gift_tearsOfReese = 0x0000105C,
		.tableOfVoices = 0x00000DE0,
	},
	.scene_psyrus1_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F24,
	},
	.scene_psyrus1_kajiya_sce00a_sbh
	{
		.tableOfVoices = 0x00000EEC,
	},
	.scene_psyrus1_kajiya_sce00b_sbh
	{
		.gift_lifeStealer = 0x000012A0,
		.tableOfVoices = 0x00000F24,
	},
	.scene_psyrus1_kajiya_sce00c_sbh
	{
		.tableOfVoices = 0x00000F2C,
	},
	.scene_psyrus1_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00000EF8,
	},
	.scene_psyrus1_lebant_sce00a_sbh
	{
		.tableOfVoices = 0x0000145C,
	},
	.scene_psyrus1_lebant_sce00c_sbh
	{
		.tableOfVoices = 0x00001438,
	},
	.scene_psyrus1_lebant_sce00d_sbh
	{
		.tableOfVoices = 0x00001438,
	},
	.scene_psyrus1_lookout_sce00_sbh
	{
		.tableOfVoices = 0x00000A5C,
	},
	.scene_psyrus1_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000B08,
	},
	.scene_psyrus1_lookout_sce00b_sbh
	{
		.tableOfVoices = 0x00000B08,
	},
	.scene_psyrus1_lookout_sce00c_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus1_lookout_sce01_sbh
	{
		.gift_nagiBelt = 0x000016F0,
		.tableOfVoices = 0x00000B78,
	},
	.scene_psyrus1_zokucho_sce01_sbh
	{
		.tableOfVoices = 0x0000167C,
	},
	.scene_psyrus1_zokucho_sce01a_sbh
	{
		.gift_championBelt = 0x00001E9C,
		.gift_captureAmulet = 0x00001EAC,
		.tableOfVoices = 0x00001618,
	},
	.scene_psyrus1_zokucho_sce05_sbh
	{
		.gift_huntersEarring = 0x00001FD0,
		.tableOfVoices = 0x00001524,
	},
	.scene_psyrus2_garai_sce00_sbh
	{
		.tableOfVoices = 0x00002198,
	},
	.scene_psyrus2_garai_sce03_sbh
	{
		.gift_mahbusCocoon = 0x00001F40,
		.gift_riketzGarb = 0x00002040,
		.tableOfVoices = 0x00001540,
	},
	.scene_psyrus2_kajiya_sce00_sbh
	{
		.tableOfVoices = 0x00000F2C,
	},
	.scene_psyrus2_lebant_sce00_sbh
	{
		.tableOfVoices = 0x00001438,
	},
	.scene_psyrus2_lookout_sce00a_sbh
	{
		.tableOfVoices = 0x00000AC4,
	},
	.scene_psyrus2_lookout_sce01_sbh
	{
		.tableOfVoices = 0x000018C0,
	},
	.scene_psyrus2_lookout_sce01a_sbh
	{
		.tableOfVoices = 0x00000F04,
	},
	.scene_psyrus2_zokucho_sce03_sbh
	{
		.tableOfVoices = 0x00001710,
	}
};

const static Offset::Game
gameNtscJ1
{
	.levantStats = 0x80089A14,
	.levantFireflyCounter = 0x800894F8,
	.randSeed = 0x80098620,
	.minionFlagRotateSpawn = 0,
	.minionStats = 0x80078A40,
	.idOpponent1 = 0x800AEA2C,
	.minion1State = 0x80089AC8,
	.bossStatsEC = 0x800B27A0,
	.gameStateStruct = 0x80089274,
	.fnInitMapNewGame = 0x8009F7D0,
	.afterGetStats = 0x80011F3C,
	.controllerTemp = 0x80093DDC,
	.frameLimiter = 0x800103D4,
	.levantAnimArgs = 0x800A5A4C,
	.captureText = 0x800C04F0,
	.getFireflyCounter = 0x800AAF80,
	.writeNewFirefly = 0x800A0074,
	.getEntityEnhancement = 0x80023DE0,
	.mapId = 0x80089534,
	.setBattleWeaponsEffectFn = 0x800B7B80,
	.drawLargeTextOnScreenFn = 0x8003A014,
	.criticalRateTransformFn = 0x800140F8,
	.cc_begin = 0x800BD000 + NtscJ1::customCodeShift
},
gameNtscJ2
{
	.levantStats = 0x800910C4,
	.levantFireflyCounter = 0x80090BA8,
	.randSeed = 0x8009FC80,
	.minionFlagRotateSpawn = 0,
	.minionStats = 0x8008001C,
	.idOpponent1 = 0x800B65F4,
	.minion1State = 0x80091178,
	.bossStatsEC = 0x800BA478,
	.gameStateStruct = 0x80090924,
	.fnInitMapNewGame = 0x800A74B0,
	.afterGetStats = 0x80019F3C,
	.controllerTemp = 0x8009B48C,
	.frameLimiter = 0x800183B8,
	.levantAnimArgs = 0x800AD548,
	.captureText = 0x800CCAB4,
	.getFireflyCounter = 0x800B2B74,
	.writeNewFirefly = 0x800A79F8,
	.getEntityEnhancement = 0x8002BE44,
	.mapId = 0x80090BE4,
	.setBattleWeaponsEffectFn = 0x800C31D4,
	.drawLargeTextOnScreenFn = 0x80041FC0,
	.criticalRateTransformFn = 0x8001C15C,
	.cc_begin = 0x800CD800
},
gameNtscU
{
	.levantStats = 0x8008BD38,
	.levantFireflyCounter = 0x8008B818,
	.randSeed = 0x8009AB00,
	.minionFlagRotateSpawn = 0x8008B554,
	.minionStats = 0x8007A4B4,
	.idOpponent1 = 0x800B1368,
	.minion1State = 0x8008BF7C,
	.bossStatsEC = 0x800B5268,
	.gameStateStruct = 0x8008B594,
	.fnInitMapNewGame = 0x800A242C,
	.afterGetStats = 0x80011F44,
	.controllerTemp = 0x8009194C,
	.frameLimiter = 0x800103B8,
	.levantAnimArgs = 0x800A83B0,
	.captureText = 0x800C93C0,
	.getFireflyCounter = 0x800AD8E4,
	.writeNewFirefly = 0x800A2898,
	.getEntityEnhancement = 0x80019C0C,
	.mapId = 0x8008B854,
	.setBattleWeaponsEffectFn = 0x800BFAF8,
	.drawLargeTextOnScreenFn = 0x8002FBD8,
	.criticalRateTransformFn = 0x80014164,
	.cc_begin = 0x800CA000
},
gamePalEn
{
	.levantStats = 0x8008BCC8,
	.levantFireflyCounter = 0x8008B7A8,
	.randSeed = 0x8009AA90,
	.minionFlagRotateSpawn = 0x8008B4E4,
	.minionStats = 0x8007A3D8,
	.idOpponent1 = 0x800B1538,
	.minion1State = 0x8008BF0C,
	.bossStatsEC = 0x800B5438,
	.gameStateStruct = 0x8008B524,
	.fnInitMapNewGame = 0x800A2800,
	.afterGetStats = 0x80011F44,
	.controllerTemp = 0x800918DC,
	.frameLimiter = 0x800103B8,
	.levantAnimArgs = 0x800A8488,
	.captureText = 0x800C9590,
	.getFireflyCounter = 0x800ADAB4,
	.writeNewFirefly = 0x800A2828,
	.getEntityEnhancement = 0x80019C0C,
	.mapId = 0x8008B7E4,
	.setBattleWeaponsEffectFn = 0x800BFCC8,
	.drawLargeTextOnScreenFn = 0x8002FC34,
	.criticalRateTransformFn = 0x80014164,
	.cc_begin = 0x800CA800
},
gamePalFr
{
	.levantStats = 0x8008BF58,
	.levantFireflyCounter = 0x8008BA38,
	.randSeed = 0x8009AD20,
	.minionFlagRotateSpawn = 0x8008B774,
	.minionStats = 0x8007A64C,
	.idOpponent1 = 0x800B17C8,
	.minion1State = 0x8008C19C,
	.bossStatsEC = 0x800B56C8,
	.gameStateStruct = 0x8008B7B4,
	.fnInitMapNewGame = 0x800A2AE8,
	.afterGetStats = 0x80011F44,
	.controllerTemp = 0x80091B6C,
	.frameLimiter = 0x800103B8,
	.levantAnimArgs = 0x800A8718,
	.captureText = 0x800C9B20,
	.getFireflyCounter = 0x800ADD44,
	.writeNewFirefly = 0x800A2AB8,
	.getEntityEnhancement = 0x80019C0C,
	.mapId = 0x8008BA74,
	.setBattleWeaponsEffectFn = 0x800C0430,
	.drawLargeTextOnScreenFn = 0x8002FC34,
	.criticalRateTransformFn = 0x80014164,
	.cc_begin = 0x800CA800
},
gamePalDe
{
	.levantStats = 0x8008BE10,
	.levantFireflyCounter = 0x8008B8F0,
	.randSeed = 0x8009ABD8,
	.minionFlagRotateSpawn = 0x8008B62C,
	.minionStats = 0x8007A508,
	.idOpponent1 = 0x800B1680,
	.minion1State = 0x8008C054,
	.bossStatsEC = 0x800B5580,
	.gameStateStruct = 0x8008B66C,
	.fnInitMapNewGame = 0x800A294C,
	.afterGetStats = 0x80011F44,
	.controllerTemp = 0x80091A24,
	.frameLimiter = 0x800103B8,
	.levantAnimArgs = 0x800A85D0,
	.captureText = 0x800C9CF8,
	.getFireflyCounter = 0x800ADBFC,
	.writeNewFirefly = 0x800A2970,
	.getEntityEnhancement = 0x80019C0C,
	.mapId = 0x8008B92C,
	.setBattleWeaponsEffectFn = 0x800C0258,
	.drawLargeTextOnScreenFn = 0x8002FC34,
	.criticalRateTransformFn = 0x80014164,
	.cc_begin = 0x800CA800
},
gamePalEs
{
	.levantStats = 0x8008C078,
	.levantFireflyCounter = 0x8008BB58,
	.randSeed = 0x8009AE40,
	.minionFlagRotateSpawn = 0x8008B894,
	.minionStats = 0x8007A76C,
	.idOpponent1 = 0x800B18E8,
	.minion1State = 0x8008C2BC,
	.bossStatsEC = 0x800B57E8,
	.gameStateStruct = 0x8008B8D4,
	.fnInitMapNewGame = 0x800A2AD4,
	.afterGetStats = 0x80011F44,
	.controllerTemp = 0x80091C8C,
	.frameLimiter = 0x800103B8,
	.levantAnimArgs = 0x800A8838,
	.captureText = 0x800CA118,
	.getFireflyCounter = 0x800ADE64,
	.writeNewFirefly = 0x800A2BD8,
	.getEntityEnhancement = 0x80019C0C,
	.mapId = 0x8008BB94,
	.setBattleWeaponsEffectFn = 0x800C0850,
	.drawLargeTextOnScreenFn = 0x8002FC34,
	.criticalRateTransformFn = 0x80014164,
	.cc_begin = 0x800CB000
},
gamePalIt
{
	.levantStats = 0x8008C060,
	.levantFireflyCounter = 0x8008BB40,
	.randSeed = 0x8009AE28,
	.minionFlagRotateSpawn = 0x8008B87C,
	.minionStats = 0x8007A750,
	.idOpponent1 = 0x800B18D0,
	.minion1State = 0x8008C2A4,
	.bossStatsEC = 0x800B57D0,
	.gameStateStruct = 0x8008B8BC,
	.fnInitMapNewGame = 0x800A2ABC,
	.afterGetStats = 0x80011F44,
	.controllerTemp = 0x80091C74,
	.frameLimiter = 0x800103B8,
	.levantAnimArgs = 0x800A8820,
	.captureText = 0x800CA020,
	.getFireflyCounter = 0x800ADE4C,
	.writeNewFirefly = 0x800A2BC0,
	.getEntityEnhancement = 0x80019C0C,
	.mapId = 0x8008BB7C,
	.setBattleWeaponsEffectFn = 0x800C0758,
	.drawLargeTextOnScreenFn = 0x8002FC34,
	.criticalRateTransformFn = 0x80014164,
	.cc_begin = 0x800CB000
};

Offset::Offset(Version version)
	: file(setFile(version)), game(setGame(version))
{
}

const Offset::File& Offset::setFile(Version version)
{
	switch (version)
	{
	case Version::NtscJ1: return fileNtscJ1;
	case Version::NtscJ2: return fileNtscJ2;
	case Version::NtscU: return fileNtscU;
	case Version::PalEn: return filePalEn;
	case Version::PalFr: return filePalFr;
	case Version::PalDe: return filePalDe;
	case Version::PalEs: return filePalEs;
	case Version::PalIt: return filePalIt;
	default: throw JcrException{ "Invalid game version : {}", static_cast<std::underlying_type_t<Version>>(version) };
	}
}

const Offset::Game& Offset::setGame(Version version)
{
	switch (version)
	{
	case Version::NtscJ1: return gameNtscJ1;
	case Version::NtscJ2: return gameNtscJ2;
	case Version::NtscU: return gameNtscU;
	case Version::PalEn: return gamePalEn;
	case Version::PalFr: return gamePalFr;
	case Version::PalDe: return gamePalDe;
	case Version::PalEs: return gamePalEs;
	case Version::PalIt: return gamePalIt;
	default: throw JcrException{ "Invalid game version : {}", static_cast<std::underlying_type_t<Version>>(version) };
	}
}