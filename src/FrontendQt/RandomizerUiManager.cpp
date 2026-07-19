#include "RandomizerUiManager.hpp"

#include "Backend/Randomizer.hpp"
#include "Backend/Version.hpp"
#include "FrontendQt/BossWidget.hpp"
#include "FrontendQt/ChallengeWidget.hpp"
#include "FrontendQt/DefaultDialog.hpp"
#include "FrontendQt/EquipmentWidget.hpp"
#include "FrontendQt/ForestWidget.hpp"
#include "FrontendQt/LevantWidget.hpp"
#include "FrontendQt/MinionWidget.hpp"
#include "FrontendQt/MiscWidget.hpp"
#include "FrontendQt/ShopWidget.hpp"
#include "FrontendQt/SpecialMagicDialog.hpp"
#include "FrontendQt/TreasureWidget.hpp"

RandomizerUiManager::RandomizerUiManager(LevantWidget* levant, MinionWidget* minion, BossWidget* boss, ForestWidget* forest,
	EquipmentWidget* equipment, TreasureWidget* treasure, ShopWidget* shop, MiscWidget* misc,
	ChallengeWidget* challenge, DefaultDialog* defaultDialog)
	: m_levant(levant),
	m_minion(minion),
	m_boss(boss),
	m_equipment(equipment),
	m_forest(forest),
	m_treasure(treasure),
	m_shop(shop),
	m_misc(misc),
	m_challenge(challenge),
	m_default(defaultDialog)
{
}

RandomizerConfig RandomizerUiManager::snapshot() const
{
	RandomizerConfig config{};

	// Levant
	const auto& levantUi{ m_levant->Ui() };
	config.levantBaseStats = levantUi.baseStatsRandom->isChecked();
	config.levantAnimationBetaUsingItem = levantUi.animationBetaUsingItem->isChecked();
	config.levantAnimationBetaSummon = levantUi.animationBetaSummon->isChecked();
	config.levantAnimationBetaCapture = levantUi.animationBetaCapture->isChecked();
	config.levantAnimationRandomWeaponsPosture = levantUi.animationRandomWeaponsPosture->isChecked();
	config.levantFluteStylingShuffle = levantUi.fluteStylingShuffle->isChecked();

	// Minion
	const auto& minionUi{ m_minion->Ui() };
	config.minionSpawnStoryRandomRealtime = minionUi.spawnStoryRandomRealtime->isChecked();
	config.minionSpawnStoryRandomPremade = minionUi.spawnStoryRandomPremade->isChecked();
	config.minionSpawnEternalCorridorRandom = minionUi.spawnEternalCorridorRandom->isChecked();
	config.minionSpawnDreamRandom = minionUi.spawnDreamRandom->isChecked();
	config.minionSpawnDreamCustom = minionUi.spawnDreamCustom->isChecked();
	config.minionSpawnDreamComboIndex = minionUi.spawnDreamCombo->currentIndex();
	config.minionSpawnKorisRandom = minionUi.spawnKorisRandom->isChecked();
	config.minionSpawnKorisCustom = minionUi.spawnKorisCustom->isChecked();
	config.minionSpawnKorisComboIndex = minionUi.spawnKorisCombo->currentIndex();
	config.minionStatsShuffleBetweenMinions = minionUi.statsShuffleBetweenMinions->isChecked();
	config.minionStatsShuffleStats = minionUi.statsShuffleStats->isChecked();
	config.minionSpecialMagicRandom = minionUi.specialMagicRandom->isChecked();
	{
		const auto& specialMagicDialog{ m_minion->specialMagicDialog() };
		config.minionSpecials = specialMagicDialog.specials();
		config.minionMagics = specialMagicDialog.magics();
	}
	config.minionAppearanceRandomNewMinion = minionUi.appearanceRandomNewMinion->isChecked();
	config.minionAppearanceModelRandom = minionUi.appearanceModelRandom->isChecked();
	config.minionAppearanceTextureRandom = minionUi.appearanceTextureRandom->isChecked();
	config.minionAppearanceTextureIncludeCompatible = minionUi.appearanceTextureIncludeCompatible->isChecked();
	config.minionAppearanceTextureRandomColor = minionUi.appearanceTextureRandomColor->isChecked();
	config.minionAppearanceGrowthSizeShuffle = minionUi.appearanceGrowthSizeShuffle->isChecked();
	config.minionAppearanceGrowthSizeInvert = minionUi.appearanceGrowthSizeInvert->isChecked();
	config.minionAppearanceCraziness = minionUi.appearanceCrazinessSlider->value();

	// Boss
	const auto& bossUi{ m_boss->Ui() };
	config.bossStoryElementRandomAll = bossUi.storyElementRandomAll->isChecked();
	config.bossStoryElementRandomElemental = bossUi.storyElementRandomElemental->isChecked();
	config.bossStorySpecialMagicRandom = bossUi.storySpecialMagicRandom->isChecked();
	config.bossStoryAppearanceTextureColorBasedOnElement = bossUi.storyAppearanceTextureColorBasedOnElement->isChecked();
	config.bossStoryAppearanceTextureRandomColor = bossUi.storyAppearanceTextureRandomColor->isChecked();
	config.bossEternalCorridorElementRandom = bossUi.eternalCorridorElementRandom->isChecked();
	config.bossEternalCorridorSpecialMagicRandom = bossUi.eternalCorridorSpecialMagicRandom->isChecked();
	config.bossEternalCorridorRandomNewAppearance = bossUi.eternalCorridorRandomNewAppearance->isChecked();
	config.bossEternalCorridorAppearanceTextureRandomColor = bossUi.eternalCorridorAppearanceTextureRandomColor->isChecked();
	config.bossEternalCorridorCraziness = bossUi.eternalCorridorCrazinessSlider->value();

	// Forest
	const auto& forestUi{ m_forest->Ui() };
	config.forestPaletteColorRandom = forestUi.paletteColorRandom->isChecked();
	config.forestBattleMapsRandom = forestUi.battleMapsRandom->isChecked();
	config.forestOstRandom = forestUi.ostRandom->isChecked();
	config.forestOstShuffle = forestUi.ostShuffle->isChecked();
	config.forestOstCustom = forestUi.ostCustom->isChecked();
	config.forestOstArray = m_forest->ostArray();
	config.forestOstRandomEternalCorridorOstPerCorridor = forestUi.ostRandomEternalCorridorOstPerCorridor->isChecked();

	// Equipment
	const auto& equipmentUi{ m_equipment->Ui() };
	config.equipmentWeaponsRandomStatsAndElement = equipmentUi.weaponsRandomStatsAndElement->isChecked();
	config.equipmentWeaponsRandomAppearance = equipmentUi.weaponsRandomAppearance->isChecked();
	config.equipmentArmorsRandomStats = equipmentUi.armorsRandomStats->isChecked();
	config.equipmentArmorsRandomAppearance = equipmentUi.armorsRandomAppearance->isChecked();
	config.equipmentArmorsRandomColors = equipmentUi.armorsRandomColors->isChecked();
	config.equipmentOthersRandomStats = equipmentUi.othersRandomStats->isChecked();

	// Treasure
	const auto& treasureUi{ m_treasure->Ui() };
	config.treasureEquipmentsRandom = treasureUi.equipmentsRandom->isChecked();
	config.treasureEquipmentsRandomByCategory = treasureUi.equipmentsRandomByCategory->isChecked();
	config.treasureItemsRandom = treasureUi.itemsRandom->isChecked();
	config.treasureItemsRandomByCategory = treasureUi.itemsRandomByCategory->isChecked();
	config.treasureItemsRandomSkeletonKeys = treasureUi.itemsRandomSkeletonKeys->isChecked();
	config.treasureValuablesRandom = treasureUi.valuablesRandom->isChecked();
	config.treasureValuablesRandomByCategory = treasureUi.valuablesRandomByCategory->isChecked();
	config.treasureValuablesRandomUsableValuables = treasureUi.valuablesRandomUsableValuables->isChecked();
	config.treasureBattleDropsShuffle = treasureUi.battleDropsShuffle->isChecked();
	config.treasureBattleDropsShuffleSkeletonKeys = treasureUi.battleDropsShuffleSkeletonKeys->isChecked();

	// Shop
	const auto& shopUi{ m_shop->Ui() };
	config.shopWeaponRandom = shopUi.weaponRandom->isChecked();
	config.shopArmorRandom = shopUi.armorRandom->isChecked();
	config.shopOtherRandom = shopUi.otherRandom->isChecked();
	config.shopItemRandom = shopUi.itemRandom->isChecked();
	config.shopEternalCorridorUnlockAllEnable = shopUi.eternalCorridorUnlockAllEnable->isChecked();

	// Misc
	const auto& miscUi{ m_misc->Ui() };
	config.miscSkipPrologueEnable = miscUi.skipPrologueEnable->isChecked();
	config.miscSkipPrologueSkipKoris = miscUi.skipPrologueSkipKoris->isChecked();
	config.miscHudRandom = miscUi.hudRandom->isChecked();
	config.miscHudColorCustom = miscUi.hudColorCustom->isChecked();
	config.miscHudColor = m_misc->hudColor();
	config.miscNpcsVoiceRandom = miscUi.npcsVoiceRandom->isChecked();
	config.miscBetaBattleThemeEnable = miscUi.betaBattleThemeEnable->isChecked();
	config.miscItemQuantityLimit = miscUi.itemQuantityLimitSlider->value();
	config.miscEternalCorridorLevelCap = miscUi.eternalCorridorLevelCapSlider->value();
	config.miscPalToNtscBoxEnabled = miscUi.palToNtscBox->isEnabled();
	config.miscPalToNtscEnable = miscUi.palToNtscEnable->isChecked();

	// Challenge
	const auto& challengeUi{ m_challenge->Ui() };
	config.challengeNuzlockeOneCapturePerArea = challengeUi.nuzlockeOneCapturePerArea->isChecked();
	config.challengeNuzlockeDefinitiveMinionDeath = challengeUi.nuzlockeDefinitiveMinionDeath->isChecked();
	config.challengeNuzlockeDefinitiveLevantDeath = challengeUi.nuzlockeDefinitiveLevantDeath->isChecked();
	config.challengeDifficultyHard = challengeUi.difficultyHard->isChecked();
	config.challengeDifficultyExtreme = challengeUi.difficultyExtreme->isChecked();

	// Default
	const auto& defaultUi{ m_default->Ui() };
	config.defaultX2Framerate = defaultUi.x2Framerate->isChecked();
	config.defaultCanPauseTheGame = defaultUi.canPauseTheGame->isChecked();
	config.defaultAutoHealInSafeArea = defaultUi.autoHealInSafeArea->isChecked();
	config.defaultSkipOpeningLogos = defaultUi.skipOpeningLogos->isChecked();
	config.defaultShowHiddenStats = defaultUi.showHiddenStats->isChecked();
	config.defaultTurboModeInDialogues = defaultUi.turboModeInDialogues->isChecked();
	config.defaultAnalogMode = defaultUi.analogMode->isChecked();
	config.defaultBugFixesHpMpBars = defaultUi.bugFixesHpMpBars->isChecked();
	config.defaultBugFixesBodyEnhancement = defaultUi.bugFixesBodyEnhancement->isChecked();
	config.defaultBugFixesAutumnMoonVisualAttackEffect = defaultUi.bugFixesAutumnMoonVisualAttackEffect->isChecked();
	config.defaultBugFixesSpecialAttackModifiersDisplay = defaultUi.bugFixesSpecialAttackModifiersDisplay->isChecked();

	return config;
}

void RandomizerUiManager::apply(const RandomizerConfig& config, Randomizer* randomizer) const
{
	randomizer->init();

	// Levant
	if (config.levantBaseStats)
	{
		randomizer->levantBaseStats();
	}

	Randomizer::LevantAnimation_t levantAnimation{};
	if (config.levantAnimationBetaUsingItem)
	{
		levantAnimation |= Randomizer::LEVANT_ANIMATION_BETA_USING_ITEM;
	}
	if (config.levantAnimationBetaSummon)
	{
		levantAnimation |= Randomizer::LEVANT_ANIMATION_BETA_SUMMON;
	}
	if (config.levantAnimationBetaCapture)
	{
		levantAnimation |= Randomizer::LEVANT_ANIMATION_BETA_CAPTURE;
	}
	if (config.levantAnimationRandomWeaponsPosture)
	{
		levantAnimation |= Randomizer::LEVANT_ANIMATION_RANDOM_WEAPONS_POSTURE;
	}
	if (levantAnimation)
	{
		randomizer->levantAnimation(levantAnimation);
	}

	if (config.levantFluteStylingShuffle)
	{
		randomizer->levantFluteStyling();
	}

	// Minion
	auto getMinionFromCombobox = [](s32 id) -> Id_Entity_t
	{
		return id >= ID_DREAM_MAN ? id + Entity::totalStoryBosses : id;
	};

	if (config.minionSpawnStoryRandomRealtime)
	{
		randomizer->minionSpawnStory(Randomizer::MinionSpawnStory::RandomRealtime);
	}
	else if (config.minionSpawnStoryRandomPremade)
	{
		randomizer->minionSpawnStory(Randomizer::MinionSpawnStory::RandomPremade);
	}

	if (config.minionSpawnEternalCorridorRandom)
	{
		randomizer->minionSpawnEC();
	}

	if (config.minionSpawnDreamRandom)
	{
		randomizer->minionDreamMinion();
	}
	else if (config.minionSpawnDreamCustom)
	{
		randomizer->minionDreamMinion(getMinionFromCombobox(config.minionSpawnDreamComboIndex));
	}

	if (config.minionSpawnKorisRandom)
	{
		randomizer->minionKorisMinion();
	}
	else if (config.minionSpawnKorisCustom)
	{
		randomizer->minionKorisMinion(getMinionFromCombobox(config.minionSpawnKorisComboIndex));
	}

	Randomizer::MinionStats_t minionStats{};
	if (config.minionStatsShuffleBetweenMinions)
	{
		minionStats |= Randomizer::MINION_STATS_SHUFFLE_BETWEEN_MINIONS;
	}
	if (config.minionStatsShuffleStats)
	{
		minionStats |= Randomizer::MINION_STATS_SHUFFLE_STATS;
	}
	if (minionStats)
	{
		randomizer->minionStats(minionStats);
	}

	if (config.minionSpecialMagicRandom)
	{
		randomizer->minionSpecialMagic(config.minionSpecials, config.minionMagics);
	}

	Randomizer::MinionAppearance_t appearance{};
	if (config.minionAppearanceRandomNewMinion)
	{
		appearance |= Randomizer::MINION_APPEARANCE_RANDOM_NEW_MINION;
	}
	if (config.minionAppearanceModelRandom)
	{
		appearance |= Randomizer::MINION_APPEARANCE_MODEL_RANDOM;
	}
	if (config.minionAppearanceTextureRandom)
	{
		appearance |= Randomizer::MINION_APPEARANCE_TEXTURE_RANDOM;

		if (config.minionAppearanceTextureIncludeCompatible)
		{
			appearance |= Randomizer::MINION_APPEARANCE_TEXTURE_INCLUDE_COMPATIBLE;
		}
	}
	if (config.minionAppearanceTextureRandomColor)
	{
		appearance |= Randomizer::MINION_APPEARANCE_TEXTURE_RANDOM_COLOR;
	}
	if (config.minionAppearanceGrowthSizeShuffle)
	{
		appearance |= Randomizer::MINION_APPEARANCE_GROWTH_SIZE_SHUFFLE;
	}
	else if (config.minionAppearanceGrowthSizeInvert)
	{
		appearance |= Randomizer::MINION_APPEARANCE_GROWTH_SIZE_INVERT;
	}
	if (appearance)
	{
		randomizer->minionAppearance(appearance, config.minionAppearanceCraziness);
	}

	// Boss
	if (config.bossStoryElementRandomAll)
	{
		randomizer->bossElement(Randomizer::BossElement::RandomAll);
	}
	else if (config.bossStoryElementRandomElemental)
	{
		randomizer->bossElement(Randomizer::BossElement::RandomElemental);
	}

	if (config.bossStorySpecialMagicRandom)
	{
		randomizer->bossSpecialMagic();
	}

	if (config.bossStoryAppearanceTextureColorBasedOnElement)
	{
		randomizer->bossAppearance(Randomizer::BossAppearance::ColorBasedOnElement);
	}
	else if (config.bossStoryAppearanceTextureRandomColor)
	{
		randomizer->bossAppearance(Randomizer::BossAppearance::RandomColor);
	}

	if (config.bossEternalCorridorElementRandom)
	{
		randomizer->bossElementEC();
	}

	if (config.bossEternalCorridorSpecialMagicRandom)
	{
		randomizer->bossSpecialMagicEC();
	}

	Randomizer::BossAppearanceEC_t appearanceEC{};
	if (config.bossEternalCorridorRandomNewAppearance)
	{
		appearanceEC |= Randomizer::BOSS_APPEARANCE_EC_RANDOM_NEW_APPEARANCE;
	}
	if (config.bossEternalCorridorAppearanceTextureRandomColor)
	{
		appearanceEC |= Randomizer::BOSS_APPEARANCE_EC_TEXTURE_RANDOM_COLOR;
	}
	if (appearanceEC)
	{
		randomizer->bossAppearanceEC(appearanceEC, config.bossEternalCorridorCraziness);
	}

	// Forest
	if (config.forestPaletteColorRandom)
	{
		randomizer->forestPaletteColor();
	}

	if (config.forestBattleMapsRandom)
	{
		randomizer->forestBattleMaps();
	}

	if (config.forestOstRandom)
	{
		randomizer->forestOst(Randomizer::ForestOst::Random);
	}
	else if (config.forestOstShuffle)
	{
		randomizer->forestOst(Randomizer::ForestOst::Shuffle);
	}
	else if (config.forestOstCustom)
	{
		randomizer->forestOst(config.forestOstArray);
	}

	if (config.forestOstRandomEternalCorridorOstPerCorridor)
	{
		randomizer->forestRandomEternalCorridorOstPerCorridor();
	}

	// Equipment
	Randomizer::EquipmentWeapons_t equipmentWeapons{};
	if (config.equipmentWeaponsRandomStatsAndElement)
	{
		equipmentWeapons |= Randomizer::EQUIPMENT_WEAPONS_RANDOM_STATS_AND_ELEMENT;
	}
	if (config.equipmentWeaponsRandomAppearance)
	{
		equipmentWeapons |= Randomizer::EQUIPMENT_WEAPONS_RANDOM_APPEARANCE;
	}
	if (equipmentWeapons)
	{
		randomizer->equipmentWeapons(equipmentWeapons);
	}

	Randomizer::EquipmentArmors_t equipmentArmors{};
	if (config.equipmentArmorsRandomStats)
	{
		equipmentArmors |= Randomizer::EQUIPMENT_ARMORS_RANDOM_STATS;
	}
	if (config.equipmentArmorsRandomAppearance)
	{
		equipmentArmors |= Randomizer::EQUIPMENT_ARMORS_RANDOM_APPEARANCE;
	}
	if (config.equipmentArmorsRandomColors)
	{
		equipmentArmors |= Randomizer::EQUIPMENT_ARMORS_RANDOM_COLORS;
	}
	if (equipmentArmors)
	{
		randomizer->equipmentArmors(equipmentArmors);
	}

	if (config.equipmentOthersRandomStats)
	{
		randomizer->equipmentOthers();
	}

	// Treasure
	if (config.treasureEquipmentsRandom)
	{
		randomizer->treasureEquipments(Randomizer::TreasureEquipments::Random);
	}
	else if (config.treasureEquipmentsRandomByCategory)
	{
		randomizer->treasureEquipments(Randomizer::TreasureEquipments::RandomByCategory);
	}

	if (config.treasureItemsRandom)
	{
		randomizer->treasureItems(Randomizer::TreasureItems::Random, config.treasureItemsRandomSkeletonKeys);
	}
	else if (config.treasureItemsRandomByCategory)
	{
		randomizer->treasureItems(Randomizer::TreasureItems::RandomByCategory, config.treasureItemsRandomSkeletonKeys);
	}

	if (config.treasureValuablesRandom)
	{
		randomizer->treasureValuables(Randomizer::TreasureValuables::Random, config.treasureValuablesRandomUsableValuables);
	}
	else if (config.treasureValuablesRandomByCategory)
	{
		randomizer->treasureValuables(Randomizer::TreasureValuables::RandomByCategory, config.treasureValuablesRandomUsableValuables);
	}

	if (config.treasureBattleDropsShuffle)
	{
		randomizer->treasureBattleDrops(config.treasureBattleDropsShuffleSkeletonKeys);
	}

	// Shop
	if (config.shopWeaponRandom)
	{
		randomizer->shopWeapon();
	}

	if (config.shopArmorRandom)
	{
		randomizer->shopArmor();
	}

	if (config.shopOtherRandom)
	{
		randomizer->shopOther();
	}

	if (config.shopItemRandom)
	{
		randomizer->shopItem();
	}

	if (config.shopEternalCorridorUnlockAllEnable)
	{
		randomizer->shopEternalCorridorUnlockAll();
	}

	// Misc
	if (config.miscSkipPrologueEnable)
	{
		randomizer->miscSkipPrologue(config.miscSkipPrologueSkipKoris);
	}

	if (config.miscHudRandom)
	{
		randomizer->miscHudColor();
	}
	else if (config.miscHudColorCustom)
	{
		randomizer->miscHudColor(config.miscHudColor);
	}

	if (config.miscNpcsVoiceRandom)
	{
		randomizer->miscNPCsVoice();
	}

	if (config.miscBetaBattleThemeEnable)
	{
		randomizer->miscBetaBattleTheme();
	}

	randomizer->miscItemQuantityLimit(static_cast<u8>(config.miscItemQuantityLimit));
	randomizer->miscLevelCapEC(static_cast<u8>(config.miscEternalCorridorLevelCap));

	if (config.miscPalToNtscBoxEnabled && config.miscPalToNtscEnable)
	{
		randomizer->miscPalToNtsc();
	}

	// Challenge
	Randomizer::ChallengeNuzlocke_t challengeNuzlocke{};
	if (config.challengeNuzlockeOneCapturePerArea)
	{
		challengeNuzlocke |= Randomizer::CHALLENGE_NUZLOCKE_ONE_CAPTURE;
	}
	if (config.challengeNuzlockeDefinitiveMinionDeath)
	{
		challengeNuzlocke |= Randomizer::CHALLENGE_NUZLOCKE_DEFINITIVE_DEATH;
	}
	if (config.challengeNuzlockeDefinitiveLevantDeath)
	{
		challengeNuzlocke |= Randomizer::CHALLENGE_NUZLOCKE_DEFINITIVE_LEVANT_DEATH;
	}
	if (challengeNuzlocke)
	{
		randomizer->challengeNuzlocke(challengeNuzlocke);
	}

	if (config.challengeDifficultyHard)
	{
		randomizer->challengeDifficulty(Randomizer::ChallengeDifficulty::Hard);
	}
	else if (config.challengeDifficultyExtreme)
	{
		randomizer->challengeDifficulty(Randomizer::ChallengeDifficulty::Extreme);
	}

	// Default
	if (config.defaultX2Framerate)
	{
		randomizer->defaultX2Framerate();
	}

	if (config.defaultCanPauseTheGame)
	{
		randomizer->defaultCanPauseTheGame();
	}

	if (config.defaultAutoHealInSafeArea)
	{
		randomizer->defaultAutoHealInSafeArea();
	}

	if (config.defaultSkipOpeningLogos)
	{
		randomizer->defaultSkipOpeningLogos();
	}

	if (config.defaultShowHiddenStats)
	{
		randomizer->defaultShowHiddenStats();
	}

	if (config.defaultTurboModeInDialogues)
	{
		randomizer->defaultTurboModeInDialogues();
	}

	if (randomizer->game().isVersion(Version::NtscJ1, Version::NtscU) && config.defaultAnalogMode)
	{
		randomizer->defaultAnalogMode();
	}

	if (config.defaultBugFixesHpMpBars)
	{
		randomizer->defaultBugFixesHpMpBars();
	}

	if (config.defaultBugFixesBodyEnhancement)
	{
		randomizer->defaultBugFixesBodyEnhancement();
	}

	if (config.defaultBugFixesAutumnMoonVisualAttackEffect)
	{
		randomizer->defaultBugFixesAutumnMoonVisualAttackEffect();
	}

	if (randomizer->game().isVersion(Version::PalFr) && config.defaultBugFixesSpecialAttackModifiersDisplay)
	{
		randomizer->defaultBugFixesSpecialAttackModifiersDisplay();
	}

	// Without this, the game crashes during the slowdown in boss fights that occurs when
	// unsummoning in the Netherworld forests while using the Hp Mp Bars fix / Beta Summon
	randomizer->expandPacketArena();
	randomizer->writeFrameFns();
}