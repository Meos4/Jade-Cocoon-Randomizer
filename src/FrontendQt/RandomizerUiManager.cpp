#include "RandomizerUiManager.hpp"

#include "Backend/Randomizer.hpp"
#include "FrontendQt/AddonsWidget.hpp"
#include "FrontendQt/BossWidget.hpp"
#include "FrontendQt/ChallengeWidget.hpp"
#include "FrontendQt/EquipmentWidget.hpp"
#include "FrontendQt/FixesWidget.hpp"
#include "FrontendQt/ForestWidget.hpp"
#include "FrontendQt/LevantWidget.hpp"
#include "FrontendQt/MinionWidget.hpp"
#include "FrontendQt/MiscWidget.hpp"
#include "FrontendQt/ShopWidget.hpp"
#include "FrontendQt/SpecialMagicDialog.hpp"
#include "FrontendQt/TreasureWidget.hpp"

RandomizerUiManager::RandomizerUiManager(LevantWidget* levant, MinionWidget* minion, BossWidget* boss, ForestWidget* forest,
    EquipmentWidget* equipment, TreasureWidget* treasure, ShopWidget* shop, MiscWidget* misc,
    AddonsWidget* addons, ChallengeWidget* challenge, FixesWidget* fixes)
    : m_levant(levant),
      m_minion(minion),
      m_boss(boss),
      m_equipment(equipment),
      m_forest(forest),
      m_treasure(treasure),
      m_shop(shop),
      m_misc(misc),
      m_addons(addons),
	  m_challenge(challenge),
      m_fixes(fixes)
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
		randomizer->minionAppearance(appearance);
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
	if (bossUi.eternalCorridorAppearanceTextureRandom->isChecked())
	{
		appearanceEC |= Randomizer::BOSS_APPEARANCE_EC_TEXTURE_RANDOM;

		if (bossUi.eternalCorridorAppearanceTextureIncludeCompatible->isChecked())
		{
			appearanceEC |= Randomizer::BOSS_APPEARANCE_EC_TEXTURE_INCLUDE_COMPATIBLE;
		}
	}
	if (bossUi.eternalCorridorAppearanceTextureRandomColor->isChecked())
	{
		appearanceEC |= Randomizer::BOSS_APPEARANCE_EC_TEXTURE_RANDOM_COLOR;
	}
	if (appearanceEC)
	{
		randomizer->bossAppearanceEC(appearanceEC);
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

	if (treasureUi.equipmentsRandom->isChecked())
	{
		randomizer->treasureEquipments(Randomizer::TreasureEquipments::Random);
	}
	else if (treasureUi.equipmentsRandomByCategory->isChecked())
	{
		randomizer->treasureEquipments(Randomizer::TreasureEquipments::RandomByCategory);
	}

	if (treasureUi.battleShuffle->isChecked())
	{
		randomizer->treasureBattle(treasureUi.battleShuffleSkeletonKeys->isChecked());
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

    // Addons
	const auto& addonsUi{ m_addons->Ui() };

	if (addonsUi.skipTutorialEnable->isChecked())
	{
		randomizer->addonsSkipTutorial(addonsUi.skipTutorialSkipKoris->isChecked());
	}

	if (addonsUi.x2FramerateEnable->isChecked())
	{
		randomizer->addonsX2Framerate();
	}

	if (addonsUi.showHiddenStatsEnable->isChecked())
	{
		randomizer->addonsShowHiddenStats();
	}

	randomizer->addonsItemQuantityLimit(static_cast<u8>(addonsUi.itemQuantityLimitSlider->value()));
	randomizer->addonsLevelCapEC(static_cast<u8>(addonsUi.eternalCorridorLevelCapSlider->value()));

	if (addonsUi.palToNtscBox->isEnabled() && addonsUi.palToNtscEnable->isChecked())
	{
		randomizer->addonsPalToNtsc();
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

    // Fixes
	const auto& fixesUi{ m_fixes->Ui() };

    if (fixesUi.bodyEnhancementFix->isChecked())
	{
		randomizer->fixesBodyEnhancement();
	}

	if (fixesUi.autumnMoonEffectFix->isChecked())
	{
		randomizer->fixesAutumnMoonEffect();
	}

	randomizer->fixesHpMpBarsSize(static_cast<u16>(fixesUi.hpMpBarsSizeSlider->value()));

	if (fixesUi.specialAttackModifiersDisplayBox->isEnabled() && 
		fixesUi.specialAttackModifiersDisplayFix->isChecked())
	{
		randomizer->fixesSpecialsModifiers();
	}
}