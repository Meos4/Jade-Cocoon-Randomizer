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

void RandomizerUiManager::write(Randomizer* randomizer) const
{
	randomizer->init();

	// Levant
	const auto& levantUi{ m_levant->Ui() };

	if (levantUi.baseStatsRandom->isChecked())
	{
		randomizer->levantBaseStats();
	}

	Randomizer::LevantAnimation_t levantAnimation{};
	if (levantUi.animationBetaUsingItem->isChecked())
	{
		levantAnimation |= Randomizer::LEVANT_ANIMATION_BETA_USING_ITEM;
	}
	if (levantUi.animationBetaSummon->isChecked())
	{
		levantAnimation |= Randomizer::LEVANT_ANIMATION_BETA_SUMMON;
	}
	if (levantUi.animationBetaCapture->isChecked())
	{
		levantAnimation |= Randomizer::LEVANT_ANIMATION_BETA_CAPTURE;
	}
	if (levantUi.animationRandomWeaponsPosture->isChecked())
	{
		levantAnimation |= Randomizer::LEVANT_ANIMATION_RANDOM_WEAPONS_POSTURE;
	}
	if (levantAnimation)
	{
		randomizer->levantAnimation(levantAnimation);
	}

	if (levantUi.fluteStylingShuffle->isChecked())
	{
		randomizer->levantFluteStyling();
	}

	// Minion
	const auto& minionUi{ m_minion->Ui() };

	auto getMinionFromCombobox = [](s32 id) -> Id_Entity_t
	{
		return id >= ID_DREAM_MAN ? id + Entity::totalStoryBosses : id;
	};

	if (minionUi.spawnStoryRandomRealtime->isChecked())
	{
		randomizer->minionSpawnStory(Randomizer::MinionSpawnStory::RandomRealtime);
	}
	else if (minionUi.spawnStoryRandomPremade->isChecked())
	{
		randomizer->minionSpawnStory(Randomizer::MinionSpawnStory::RandomPremade);
	}

	if (minionUi.spawnEternalCorridorRandom->isChecked())
	{
		randomizer->minionSpawnEC();
	}

	if (minionUi.spawnDreamRandom->isChecked())
	{
		randomizer->minionDreamMinion();
	}
	else if (minionUi.spawnDreamCustom->isChecked())
	{
		randomizer->minionDreamMinion(getMinionFromCombobox(minionUi.spawnDreamCombo->currentIndex()));
	}

	if (minionUi.spawnKorisRandom->isChecked())
	{
		randomizer->minionKorisMinion();
	}
	else if (minionUi.spawnKorisCustom->isChecked())
	{
		randomizer->minionKorisMinion(getMinionFromCombobox(minionUi.spawnKorisCombo->currentIndex()));
	}

	Randomizer::MinionStats_t minionStats{};
	if (minionUi.statsShuffleBetweenMinions->isChecked())
	{
		minionStats |= Randomizer::MINION_STATS_SHUFFLE_BETWEEN_MINIONS;
	}
	if (minionUi.statsShuffleStats->isChecked())
	{
		minionStats |= Randomizer::MINION_STATS_SHUFFLE_STATS;
	}
	if (minionStats)
	{
		randomizer->minionStats(minionStats);
	}

	if (minionUi.specialMagicRandom->isChecked())
	{
		const auto& specialMagicDialog{ m_minion->specialMagicDialog() };
		randomizer->minionSpecialMagic(specialMagicDialog.specials(), specialMagicDialog.magics());
	}

	Randomizer::MinionAppearance_t appearance{};
	if (minionUi.appearanceRandomNewMinion->isChecked())
	{
		appearance |= Randomizer::MINION_APPEARANCE_RANDOM_NEW_MINION;
	}
	if (minionUi.appearanceModelRandom->isChecked())
	{
		appearance |= Randomizer::MINION_APPEARANCE_MODEL_RANDOM;
	}
	if (minionUi.appearanceTextureRandom->isChecked())
	{
		appearance |= Randomizer::MINION_APPEARANCE_TEXTURE_RANDOM;

		if (minionUi.appearanceTextureIncludeCompatible->isChecked())
		{
			appearance |= Randomizer::MINION_APPEARANCE_TEXTURE_INCLUDE_COMPATIBLE;
		}
	}
	if (minionUi.appearanceTextureRandomColor->isChecked())
	{
		appearance |= Randomizer::MINION_APPEARANCE_TEXTURE_RANDOM_COLOR;
	}
	if (minionUi.appearanceGrowthSizeShuffle->isChecked())
	{
		appearance |= Randomizer::MINION_APPEARANCE_GROWTH_SIZE_SHUFFLE;
	}
	else if (minionUi.appearanceGrowthSizeInvert->isChecked())
	{
		appearance |= Randomizer::MINION_APPEARANCE_GROWTH_SIZE_INVERT;
	}
	if (appearance)
	{
		randomizer->minionAppearance(appearance, minionUi.appearanceCrazinessSlider->value());
	}

	// Boss
	const auto& bossUi{ m_boss->Ui() };

	if (bossUi.storyElementRandomAll->isChecked())
	{
		randomizer->bossElement(Randomizer::BossElement::RandomAll);
	}
	else if (bossUi.storyElementRandomElemental->isChecked())
	{
		randomizer->bossElement(Randomizer::BossElement::RandomElemental);
	}

	if (bossUi.storySpecialMagicRandom->isChecked())
	{
		randomizer->bossSpecialMagic();
	}

	if (bossUi.storyAppearanceTextureColorBasedOnElement->isChecked())
	{
		randomizer->bossAppearance(Randomizer::BossAppearance::ColorBasedOnElement);
	}
	else if (bossUi.storyAppearanceTextureRandomColor->isChecked())
	{
		randomizer->bossAppearance(Randomizer::BossAppearance::RandomColor);
	}

	if (bossUi.eternalCorridorElementRandom->isChecked())
	{
		randomizer->bossElementEC();
	}

	if (bossUi.eternalCorridorSpecialMagicRandom->isChecked())
	{
		randomizer->bossSpecialMagicEC();
	}

	Randomizer::BossAppearanceEC_t appearanceEC{};
	if (bossUi.eternalCorridorRandomNewAppearance->isChecked())
	{
		appearanceEC |= Randomizer::BOSS_APPEARANCE_EC_RANDOM_NEW_APPEARANCE;
	}
	if (bossUi.eternalCorridorAppearanceTextureRandomColor->isChecked())
	{
		appearanceEC |= Randomizer::BOSS_APPEARANCE_EC_TEXTURE_RANDOM_COLOR;
	}
	if (appearanceEC)
	{
		randomizer->bossAppearanceEC(appearanceEC, bossUi.eternalCorridorCrazinessSlider->value());
	}

	// Forest
	const auto& forestUi{ m_forest->Ui() };

	if (forestUi.paletteColorRandom->isChecked())
	{
		randomizer->forestPaletteColor();
	}

	if (forestUi.battleMapsRandom->isChecked())
	{
		randomizer->forestBattleMaps();
	}

	if (forestUi.ostRandom->isChecked())
	{
		randomizer->forestOst(Randomizer::ForestOst::Random);
	}
	else if (forestUi.ostShuffle->isChecked())
	{
		randomizer->forestOst(Randomizer::ForestOst::Shuffle);
	}
	else if (forestUi.ostCustom->isChecked())
	{
		randomizer->forestOst(m_forest->ostArray());
	}

	if (forestUi.ostRandomEternalCorridorOstPerCorridor->isChecked())
	{
		randomizer->forestRandomEternalCorridorOstPerCorridor();
	}

	// Equipment
	const auto& equipmentUi{ m_equipment->Ui() };

	Randomizer::EquipmentWeapons_t equipmentWeapons{};
	if (equipmentUi.weaponsRandomStatsAndElement->isChecked())
	{
		equipmentWeapons |= Randomizer::EQUIPMENT_WEAPONS_RANDOM_STATS_AND_ELEMENT;
	}
	if (equipmentUi.weaponsRandomAppearance->isChecked())
	{
		equipmentWeapons |= Randomizer::EQUIPMENT_WEAPONS_RANDOM_APPEARANCE;
	}
	if (equipmentWeapons)
	{
		randomizer->equipmentWeapons(equipmentWeapons);
	}

	Randomizer::EquipmentArmors_t equipmentArmors{};
	if (equipmentUi.armorsRandomStats->isChecked())
	{
		equipmentArmors |= Randomizer::EQUIPMENT_ARMORS_RANDOM_STATS;
	}
	if (equipmentUi.armorsRandomAppearance->isChecked())
	{
		equipmentArmors |= Randomizer::EQUIPMENT_ARMORS_RANDOM_APPEARANCE;
	}
	if (equipmentUi.armorsRandomColors->isChecked())
	{
		equipmentArmors |= Randomizer::EQUIPMENT_ARMORS_RANDOM_COLORS;
	}
	if (equipmentArmors)
	{
		randomizer->equipmentArmors(equipmentArmors);
	}

	if (equipmentUi.othersRandomStats->isChecked())
	{
		randomizer->equipmentOthers();
	}

	// Treasure
	const auto& treasureUi{ m_treasure->Ui() };

	if (treasureUi.equipmentsRandom->isChecked())
	{
		randomizer->treasureEquipments(Randomizer::TreasureEquipments::Random);
	}
	else if (treasureUi.equipmentsRandomByCategory->isChecked())
	{
		randomizer->treasureEquipments(Randomizer::TreasureEquipments::RandomByCategory);
	}

	if (treasureUi.itemsRandom->isChecked())
	{
		randomizer->treasureItems(Randomizer::TreasureItems::Random, treasureUi.itemsRandomSkeletonKeys->isChecked());
	}
	else if (treasureUi.itemsRandomByCategory->isChecked())
	{
		randomizer->treasureItems(Randomizer::TreasureItems::RandomByCategory, treasureUi.itemsRandomSkeletonKeys->isChecked());
	}

	if (treasureUi.valuablesRandom->isChecked())
	{
		randomizer->treasureValuables(Randomizer::TreasureValuables::Random, treasureUi.valuablesRandomUsableValuables->isChecked());
	}
	else if (treasureUi.valuablesRandomByCategory->isChecked())
	{
		randomizer->treasureValuables(Randomizer::TreasureValuables::RandomByCategory, treasureUi.valuablesRandomUsableValuables->isChecked());
	}

	if (treasureUi.battleDropsShuffle->isChecked())
	{
		randomizer->treasureBattleDrops(treasureUi.battleDropsShuffleSkeletonKeys->isChecked());
	}

	// Shop
	const auto& shopUi{ m_shop->Ui() };

	if (shopUi.weaponRandom->isChecked())
	{
		randomizer->shopWeapon();
	}

	if (shopUi.armorRandom->isChecked())
	{
		randomizer->shopArmor();
	}

	if (shopUi.otherRandom->isChecked())
	{
		randomizer->shopOther();
	}

	if (shopUi.itemRandom->isChecked())
	{
		randomizer->shopItem();
	}

	if (shopUi.eternalCorridorUnlockAllEnable->isChecked())
	{
		randomizer->shopEternalCorridorUnlockAll();
	}
	
	// Misc
	const auto& miscUi{ m_misc->Ui() };

	if (miscUi.skipPrologueEnable->isChecked())
	{
		randomizer->miscSkipPrologue(miscUi.skipPrologueSkipKoris->isChecked());
	}

	if (miscUi.hudRandom->isChecked())
	{
		randomizer->miscHudColor();
	}
	else if (miscUi.hudColorCustom->isChecked())
	{
		randomizer->miscHudColor(m_misc->hudColor());
	}

	if (miscUi.npcsVoiceRandom->isChecked())
	{
		randomizer->miscNPCsVoice();
	}

	if (miscUi.betaBattleThemeEnable->isChecked())
	{
		randomizer->miscBetaBattleTheme();
	}

	randomizer->miscItemQuantityLimit(static_cast<u8>(miscUi.itemQuantityLimitSlider->value()));
	randomizer->miscLevelCapEC(static_cast<u8>(miscUi.eternalCorridorLevelCapSlider->value()));

	if (miscUi.palToNtscBox->isEnabled() && miscUi.palToNtscEnable->isChecked())
	{
		randomizer->miscPalToNtsc();
	}

	// Challenge
	const auto& challengeUi{ m_challenge->Ui() };

	Randomizer::ChallengeNuzlocke_t challengeNuzlocke{};
	if (challengeUi.nuzlockeOneCapturePerArea->isChecked())
	{
		challengeNuzlocke |= Randomizer::CHALLENGE_NUZLOCKE_ONE_CAPTURE;
	}
	if (challengeUi.nuzlockeDefinitiveMinionDeath->isChecked())
	{
		challengeNuzlocke |= Randomizer::CHALLENGE_NUZLOCKE_DEFINITIVE_DEATH;
	}
	if (challengeUi.nuzlockeDefinitiveLevantDeath->isChecked())
	{
		challengeNuzlocke |= Randomizer::CHALLENGE_NUZLOCKE_DEFINITIVE_LEVANT_DEATH;
	}
	if (challengeNuzlocke)
	{
		randomizer->challengeNuzlocke(challengeNuzlocke);
	}

	if (challengeUi.difficultyHard->isChecked())
	{
		randomizer->challengeDifficulty(Randomizer::ChallengeDifficulty::Hard);
	}
	else if (challengeUi.difficultyExtreme->isChecked())
	{
		randomizer->challengeDifficulty(Randomizer::ChallengeDifficulty::Extreme);
	}

	// Default
	const auto& defaultUi{ m_default->Ui() };

	if (defaultUi.x2Framerate->isChecked())
	{
		randomizer->defaultX2Framerate();
	}

	if (defaultUi.canPauseTheGame->isChecked())
	{
		randomizer->defaultCanPauseTheGame();
	}

	if (defaultUi.autoHealInSafeArea->isChecked())
	{
		randomizer->defaultAutoHealInSafeArea();
	}

	if (defaultUi.skipOpeningLogos->isChecked())
	{
		randomizer->defaultSkipOpeningLogos();
	}

	if (defaultUi.showHiddenStats->isChecked())
	{
		randomizer->defaultShowHiddenStats();
	}

	if (defaultUi.turboModeInDialogues->isChecked())
	{
		randomizer->defaultTurboModeInDialogues();
	}

	if (randomizer->game().isVersion(Version::NtscJ1, Version::NtscU) && defaultUi.analogMode->isChecked())
	{
		randomizer->defaultAnalogMode();
	}

	if (defaultUi.bugFixesHpMpBars->isChecked())
	{
		randomizer->defaultBugFixesHpMpBars();
	}

	if (defaultUi.bugFixesBodyEnhancement->isChecked())
	{
		randomizer->defaultBugFixesBodyEnhancement();
	}

	if (defaultUi.bugFixesAutumnMoonVisualAttackEffect->isChecked())
	{
		randomizer->defaultBugFixesAutumnMoonVisualAttackEffect();
	}

	if (randomizer->game().isVersion(Version::PalFr) && defaultUi.bugFixesSpecialAttackModifiersDisplay->isChecked())
	{
		randomizer->defaultBugFixesSpecialAttackModifiersDisplay();
	}

	// Without this, the game crashes during the slowdown in boss fights that occurs when
	// unsummoning in the Netherworld forests while using the Hp Mp Bars fix / Beta Summon
	randomizer->expandPacketArena();
	randomizer->writeFrameFns();
}