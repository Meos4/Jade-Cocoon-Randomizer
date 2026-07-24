#pragma once

#include "Backend/Entity.hpp"
#include "Backend/Randomizer.hpp"
#include "Common/Types.hpp"

#include <unordered_set>

struct RandomizerConfig
{
	// Levant
	bool levantBaseStats;
	bool levantAnimationBetaUsingItem;
	bool levantAnimationBetaSummon;
	bool levantAnimationBetaCapture;
	bool levantAnimationRandomWeaponsPosture;
	bool levantFluteStylingShuffle;

	// Minion
	bool minionSpawnStoryRandomRealtime;
	bool minionSpawnStoryRandomPremade;
	bool minionSpawnEternalCorridorRandom;
	bool minionSpawnDreamRandom;
	bool minionSpawnDreamCustom;
	s32 minionSpawnDreamComboIndex;
	bool minionSpawnKorisRandom;
	bool minionSpawnKorisCustom;
	s32 minionSpawnKorisComboIndex;
	bool minionStatsShuffleBetweenMinions;
	bool minionStatsShuffleStats;
	bool minionSpecialMagicRandom;
	std::unordered_set<Special_t> minionSpecials;
	Magic_t minionMagics;
	bool minionAppearanceRandomNewMinion;
	bool minionAppearanceModelRandom;
	bool minionAppearanceTextureRandom;
	bool minionAppearanceTextureIncludeECSkins;
	bool minionAppearanceTextureRandomColor;
	bool minionAppearanceGrowthSizeShuffle;
	bool minionAppearanceGrowthSizeInvert;
	s32 minionAppearanceCraziness;

	// Boss
	bool bossStoryElementRandomAll;
	bool bossStoryElementRandomElemental;
	bool bossStorySpecialMagicRandom;
	bool bossStoryAppearanceTextureColorBasedOnElement;
	bool bossStoryAppearanceTextureRandomColor;
	bool bossEternalCorridorElementRandom;
	bool bossEternalCorridorSpecialMagicRandom;
	bool bossEternalCorridorRandomNewAppearance;
	bool bossEternalCorridorAppearanceTextureRandomColor;
	s32 bossEternalCorridorCraziness;

	// Forest
	bool forestPaletteColorRandom;
	bool forestBattleMapsRandom;
	bool forestOstRandom;
	bool forestOstShuffle;
	bool forestOstCustom;
	Randomizer::ForestOstArray forestOstArray;
	bool forestOstRandomEternalCorridorOstPerCorridor;

	// Equipment
	bool equipmentWeaponsRandomStatsAndElement;
	bool equipmentWeaponsRandomAppearance;
	bool equipmentArmorsRandomStats;
	bool equipmentArmorsRandomAppearance;
	bool equipmentArmorsRandomColors;
	bool equipmentOthersRandomStats;

	// Treasure
	bool treasureEquipmentsRandom;
	bool treasureEquipmentsRandomByCategory;
	bool treasureItemsRandom;
	bool treasureItemsRandomByCategory;
	bool treasureItemsRandomSkeletonKeys;
	bool treasureValuablesRandom;
	bool treasureValuablesRandomByCategory;
	bool treasureValuablesRandomUsableValuables;
	bool treasureBattleDropsShuffle;
	bool treasureBattleDropsShuffleSkeletonKeys;

	// Shop
	bool shopWeaponRandom;
	bool shopArmorRandom;
	bool shopOtherRandom;
	bool shopItemRandom;
	bool shopEternalCorridorUnlockAllEnable;

	// Misc
	bool miscSkipPrologueEnable;
	bool miscSkipPrologueSkipKoris;
	bool miscHudRandom;
	bool miscHudColorCustom;
	Randomizer::HudColorArray miscHudColor;
	bool miscNpcsVoiceRandom;
	bool miscBetaBattleThemeEnable;
	s32 miscItemQuantityLimit;
	s32 miscEternalCorridorLevelCap;
	bool miscPalToNtscBoxEnabled;
	bool miscPalToNtscEnable;

	// Challenge
	bool challengeNuzlockeOneCapturePerArea;
	bool challengeNuzlockeDefinitiveMinionDeath;
	bool challengeNuzlockeDefinitiveLevantDeath;
	bool challengeDifficultyHard;
	bool challengeDifficultyExtreme;

	// Default
	bool defaultX2Framerate;
	bool defaultCanPauseTheGame;
	bool defaultAutoHealInSafeArea;
	bool defaultSkipOpeningLogos;
	bool defaultShowHiddenStats;
	bool defaultTurboModeInDialogues;
	bool defaultAnalogMode;
	bool defaultBugFixesHpMpBars;
	bool defaultBugFixesBodyEnhancement;
	bool defaultBugFixesAutumnMoonVisualAttackEffect;
	bool defaultBugFixesSpecialAttackModifiersDisplay;
};