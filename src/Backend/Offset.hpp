#pragma once

#include "Backend/Version.hpp"
#include "Common/Types.hpp"

class Offset final
{
public:
	struct File
	{
		struct
		{
			u32 entityModelsBehavior,
				entityStats,
				entityMagics,
				entitySpecials,
				entityDamageModifiers,
				hudColors,
				afterGetStatsJump,
				afterFramerateLimiter,
				hpMpBars,
				bodyEnhancementFn,
				setVideoModeArgument,
				stretchingY,
				tableOfWeaponAnimationFileId,
				tableOfWeaponStats,
				tableOfArmorStats,
				tableOfOtherStats,
				tableOfWeaponCharacteristic,
				tableOfArmorsAppearanceId,
				tableOfYPositionMinionStatsFrame,
				drawMinionStatsFrameFn,
				drawMinionTIMStatsFrameFn,
				tableOfModelAnimations,
				tableOfModelAnimationsPtr,
				actorBehaviorFn,
				mainFn,
				branchSetVatolkaSpecials,
				tableOfLevantFluteStylingsId,
				mainSceneBehaviorFn,
				cc_begin;
		} executable;

		struct
		{
			u32 endOfBattle,
				tableOfBattleWeaponsEffectPtr,
				setBattleWeaponsEffectFn,
				endOfBattleSetSceneFn;

			// not in NTSCJ1
			u32 storyMinionAI,
				minionAgniaId,
				delfanelEarthBossId,
				tuturisFireBossId,
				seterianAirBossId,
				cushidraFireBossId,
				tickerUlvi,
				kolnaAdMumuls,
				captureSuccess,
				getFireflyCounterBattle;
		} over_battle_bin;

		// NTSCJ1 Only
		struct
		{
			u32 entityTreasures;
		} over_btend_bin;

		// NTSCJ1 Only
		struct
		{
			u32 captureSuccess;
		} over_btlexec_bin;

		// NTSCJ1 Only
		struct
		{
			u32 storyMinionAI,
				minionAgniaId,
				delfanelEarthBossId,
				tuturisFireBossId,
				seterianAirBossId,
				cushidraFireBossId,
				tickerUlvi,
				kolnaAdMumuls;
		} over_encom_bin;

		struct
		{
			u32 tableOfStoryMinionsId,
				spawnStory,
				fireMapBonus,
				battleMaps,
				levelCapEC,
				levantAnimArgs,
				chooseMagicLevantAnim,
				levantCastingItemId,
				tableOfWeaponAnimationsId,
				isQuantityLimitReachedFn,
				itemShopBuyFn,
				tableOfMapsBehavior,
				setItemQuantityFromChestFn;

			// not in NTSCJ
			u32 readFlagRotateSpawn;
		} over_game_bin;

		// not in NTSCJ1
		struct
		{
			u32 entityTreasures;
		} over_menu_bin;

		// NTSCJ1 Only
		struct
		{
			u32 getFireflyCounterBattle;
		} over_plcom_bin;

		struct
		{
			u32 levantStats,
				setLevantGarb,
				initMapNewGame;
		} over_title_bin;

		struct
		{
			u32 items,
				itemShopQuantityLimitFn,
				equipmentShopQuantityLimitFn,
				initEternalCorridorShopFn;
		} over_wpnshop_bin;

		struct
		{
			u32 knowledge1;
		} scene_field1_forest1_sce00_sbh;

		struct
		{
			u32 shabLiquor,
				treeBranchBehaviorFn;
		} scene_field1_forest1_sce01_sbh;

		struct
		{
			u32 chest_metaMugwort;
		} scene_field1_forest1_sce02_sbh;

		struct
		{
			u32 knowledge2;
		} scene_field1_forest1_sce03_sbh;

		struct
		{
			u32 knowledge3;
		} scene_field1_forest1_sce04_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field1_forest1_sce05_sbh;

		struct
		{
			u32 aquazor;
		} scene_field1_forest1_sce06_sbh;

		struct
		{
			u32 aquazor,
				tableOfVoices;
		} scene_field1_forest1_sce06a_sbh;

		struct
		{
			u32 beetleWarp;
		} scene_field1_forest1_sce08_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest1_sce09_sbh;

		struct
		{
			u32 kickleberry;
		} scene_field1_forest2_sce00_sbh;

		struct
		{
			u32 knowledge4;
		} scene_field1_forest2_sce01_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field1_forest2_sce03_sbh;

		struct
		{
			u32 magicUsage1;
		} scene_field1_forest2_sce04_sbh;

		struct
		{
			u32 chest_chestNutOil;
		} scene_field1_forest2_sce05_sbh;

		struct
		{
			u32 larvaKeyBehaviorFn,
				doorBehaviorFn;
		} scene_field1_forest2_sce06_sbh;

		struct
		{
			u32 gift_calabasPowder,
				gift_ocarina,
				gift_specialLiquor,
				sleepSpore,
				tableOfVoices;
		} scene_field1_forest2_sce07_sbh;

		struct
		{
			u32 sleepSpore;
		} scene_field1_forest2_sce07a_sbh;

		struct
		{
			u32 dragonflyWarp;
		} scene_field1_forest2_sce08_sbh;

		struct
		{
			u32 skeletonKey,
				bridgeBehaviorFn;
		} scene_field1_forest2_sce10_sbh;

		struct
		{
			u32 chest_agnisSeal1;
		} scene_field1_forest2_sce12_sbh;

		struct
		{
			u32 magicUsage2;
		} scene_field1_forest2_sce13_sbh;

		struct
		{
			u32 magicUsage2,
				tableOfVoices;
		} scene_field1_forest2_sce13a_sbh;

		struct
		{
			u32 secretOfKilling,
				tableOfVoices,
				kikinakReturnOstId;
		} scene_field1_forest2_sce14a_sbh;

		struct
		{
			u32 whiteHorn;
		} scene_field1_forest3_sce01_sbh;

		struct
		{
			u32 ikariPowder;
		} scene_field1_forest3_sce03_sbh;

		struct
		{
			u32 ikariPowder,
				tableOfVoices;
		} scene_field1_forest3_sce03a_sbh;

		struct
		{
			u32 chest_stoneChoker;
		} scene_field1_forest3_sce05_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field1_forest3_sce06_sbh;

		struct
		{
			u32 divineTablet;
		} scene_field1_forest3_sce07_sbh;

		struct
		{
			u32 minionFang;
		} scene_field1_forest3_sce09_sbh;

		struct
		{
			u32 spiderWarp;
		} scene_field1_forest3_sce10_sbh;

		struct
		{
			u32 knowledge5,
				tableOfVoices;
		} scene_field1_forest3_sce11_sbh;

		struct
		{
			u32 knowledge5,
				tableOfVoices;
		} scene_field1_forest3_sce11a_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest3_sce12_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest3_sce12a_sbh;

		struct
		{
			u32 merging2,
				tableOfVoices;
		} scene_field1_forest3_sce13_sbh;

		struct
		{
			u32 merging2,
				tableOfVoices;
		} scene_field1_forest3_sce13a_sbh;

		struct
		{
			u32 merging1,
				tableOfVoices;
		} scene_field1_forest3_sce14_sbh;

		struct
		{
			u32 gift_tearsOfReese,
				merging1,
				tableOfVoices;
		} scene_field1_forest3_sce14a_sbh;

		struct
		{
			u32 excellentLiquor;
		} scene_field1_forest3_sce16_sbh;

		struct
		{
			u32 chest_yellowBead;
		} scene_field1_forest3_sce17_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field1_forest3_sce18_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest3_sce20_sbh;

		struct
		{
			u32 whiteHorn;
		} scene_field1_forest3_sce22_sbh;

		struct
		{
			u32 ikariPowder;
		} scene_field1_forest3_sce24_sbh;

		struct
		{
			u32 chest_stoneChoker;
		} scene_field1_forest3_sce26_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field1_forest3_sce27_sbh;

		struct
		{
			u32 divineTablet;
		} scene_field1_forest3_sce28_sbh;

		struct
		{
			u32 minionFang;
		} scene_field1_forest3_sce30_sbh;

		struct
		{
			u32 spiderWarp;
		} scene_field1_forest3_sce31_sbh;

		struct
		{
			u32 knowledge5,
				tableOfVoices;
		} scene_field1_forest3_sce32_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest3_sce33_sbh;

		struct
		{
			u32 merging2,
				tableOfVoices;
		} scene_field1_forest3_sce34_sbh;

		struct
		{
			u32 merging1;
		} scene_field1_forest3_sce35_sbh;

		struct
		{
			u32 excellentLiquor;
		} scene_field1_forest3_sce37_sbh;

		struct
		{
			u32 chest_yellowBead;
		} scene_field1_forest3_sce38_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field1_forest3_sce39_sbh;

		struct
		{
			u32 excellentLiquor;
		} scene_field1_forest4_sce00_sbh;

		struct
		{
			u32 chest_bitterMugwort;
		} scene_field1_forest4_sce01_sbh;

		struct
		{
			u32 chest_shishiudoOil;
		} scene_field1_forest4_sce03_sbh;

		struct
		{
			u32 whistler;
		} scene_field1_forest4_sce04_sbh;

		struct
		{
			u32 greatWalnut;
		} scene_field1_forest4_sce05_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field1_forest4_sce06_sbh;

		struct
		{
			u32 pupaKeyBehaviorFn, 
				doorBehaviorFn;
		} scene_field1_forest4_sce07_sbh;

		struct
		{
			u32 chest_urvysBlessing;
		} scene_field1_forest4_sce08_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field1_forest4_sce09_sbh;

		struct
		{
			u32 skeletonKey,
				tableOfVoices;
		} scene_field1_forest4_sce09a_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field1_forest4_sce10_sbh;

		struct
		{
			u32 tableOfVoices,
				kemuelTempleGate;
		} scene_field1_forest4_sce12_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest4_sce14_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest4_sce15_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest4_sce16_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest4_sce16a_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest4_sce16b_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest4_sce17_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_forest4_sce18_sbh;

		struct
		{
			u32 gift_jewelOfWater,
				tableOfVoices;
		} scene_field1_forest4_sce19_sbh;

		struct
		{
			u32 forestOstId,
				forestOstIdFromBehind;
		} scene_field1_gate_sce00_sbh;

		struct
		{
			u32 korisMinionId,
				gift_mugwort,
				forestOstId,
				forestOstIdFromBehind,
				tableOfVoices;
		} scene_field1_gate_sce00a_sbh;

		struct
		{
			u32 forestOstId,
				forestOstIdFromBehind;
		} scene_field1_gate_sce00b_sbh;

		struct
		{
			u32 forestOstId,
				forestOstIdFromBehind;
		} scene_field1_gate_sce00c_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field1_gate_sce01_sbh;

		struct
		{
			u32 forestOstId,
				forestOstIdFromBehind,
				tableOfVoices;
		} scene_field1_gate_sce06_sbh;

		struct
		{
			u32 chest_varysBlessing;
		} scene_field2_forest1_sce02_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field2_forest1_sce06_sbh;

		struct
		{
			u32 gift_jewelOfFire,
				tableOfVoices;
		} scene_field2_forest1_sce07_sbh;

		struct
		{
			u32 chest_bletillaOil;
		} scene_field2_forest2_sce06_sbh;

		struct
		{
			u32 gift_jewelOfWind,
				tableOfVoices;
		} scene_field2_forest2_sce07_sbh;

		struct
		{
			u32 thunderbolt;
		} scene_field2_forest2_sce13_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field2_forest2_sce14_sbh;

		struct
		{
			u32 skeletonKey;
		} scene_field2_forest3_sce07_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field2_forest3_sce11a_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field2_forest3_sce12a_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field2_forest3_sce13a_sbh;

		struct
		{
			u32 chest_bitterMugwort;
		} scene_field2_forest3_sce16_sbh;

		struct
		{
			u32 chest_urvysBlessing;
		} scene_field2_forest3_sce18_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field2_forest3_sce20_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_field2_forest3_sce22a_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_other_clear_sce00_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_other_clear_sce03_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_other_clear_sce04_sbh;

		struct
		{
			u32 dreamManIntroVoice,
				tableOfVoices;
		} scene_other_dream_sce00_sbh;

		struct
		{
			u32 dreamMinionId,
				dreamManIntroVoice,
				tableOfVoices;
		} scene_other_dream_sce00a_sbh;

		struct
		{
			u32 neutralSpawnEC,
				magicsEC,
				statsEC,
				specialsEC,
				bossSpecialsEC,
				bossDamageModifiersEC;
		} scene_other_hunting_sce00_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_cemetery_sce00_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_cemetery_sce00a_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_cemetery_sce00b_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_cemetery_sce00c_sbh;

		struct
		{
			u32 gift_fluteOfCapture,
				tableOfVoices;
		} scene_psyrus1_garai_sce00_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_garai_sce01_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_garai_sce01a_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_garai_sce01b_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_garai_sce01c_sbh;

		struct
		{
			u32 gift_tearsOfReese,
				tableOfVoices;
		} scene_psyrus1_garai_sce03_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_kajiya_sce00_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_kajiya_sce00a_sbh;

		struct
		{
			u32 gift_lifeStealer,
				tableOfVoices;
		} scene_psyrus1_kajiya_sce00b_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_kajiya_sce00c_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_lebant_sce00_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_lebant_sce00a_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_lebant_sce00c_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_lebant_sce00d_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_lookout_sce00_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_lookout_sce00a_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_lookout_sce00b_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_lookout_sce00c_sbh;

		struct
		{
			u32 gift_nagiBelt,
				tableOfVoices;
		} scene_psyrus1_lookout_sce01_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus1_zokucho_sce01_sbh;

		struct
		{
			u32 gift_championBelt,
				gift_captureAmulet,
				tableOfVoices;
		} scene_psyrus1_zokucho_sce01a_sbh;

		struct
		{
			u32 gift_huntersEarring,
				tableOfVoices;
		} scene_psyrus1_zokucho_sce05_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus2_garai_sce00_sbh;

		struct
		{
			u32 gift_mahbusCocoon,
				gift_riketzGarb,
				tableOfVoices;
		} scene_psyrus2_garai_sce03_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus2_kajiya_sce00_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus2_lebant_sce00_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus2_lookout_sce00a_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus2_lookout_sce01_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus2_lookout_sce01a_sbh;

		struct
		{
			u32 tableOfVoices;
		} scene_psyrus2_zokucho_sce03_sbh;
	};

	struct Game
	{
		u32 levantStats,
			levantFireflyCounter,
			randSeed,
			minionFlagRotateSpawn, // not in NTSCJ
			minionStats,
			idOpponent1,
			minion1State,
			bossStatsEC,
			gameStateStruct,
			fnInitMapNewGame,
			afterGetStats,
			controllerTemp,
			frameLimiter,
			afterJumpOnToggleX2Framerate,
			levantAnimArgs,
			captureText,
			getFireflyCounter,
			writeNewFirefly,
			getEntityEnhancement,
			mapId,
			setBattleWeaponsEffectFn,
			drawLargeTextOnScreenFn,
			criticalRateTransformFn,
			randFn,
			eternalCorridorCurrentCorridor,
			cc_begin;
	};

	Offset(Version version);

	const Offset::File& file;
	const Offset::Game& game;
private:
	const Offset::File& setFile(Version version);
	const Offset::Game& setGame(Version version);
};