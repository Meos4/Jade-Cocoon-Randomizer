#pragma once

#include "Backend/Entity.hpp"
#include "Backend/Game.hpp"
#include "Backend/MipsFn.hpp"
#include "Backend/SharedData.hpp"
#include "Common/Types.hpp"

#include <array>
#include <unordered_set>

class Randomizer final
{
public:
	using LevantAnimation_t = u32;
	enum : LevantAnimation_t
	{
		LEVANT_ANIMATION_BETA_USING_ITEM = 1 << 0,
		LEVANT_ANIMATION_RANDOM_WEAPONS_POSTURE = 1 << 1
	};

	enum class MinionSpawnStory
	{
		RandomRealtime,
		RandomPremade
	};

	using MinionStats_t = u32;
	enum : MinionStats_t
	{
		MINION_STATS_SHUFFLE_BETWEEN_MINIONS = 1 << 0,
		MINION_STATS_SHUFFLE_STATS = 1 << 1
	};

	using MinionAppearance_t = u32;
	enum : MinionAppearance_t
	{
		MINION_APPEARANCE_RANDOM_NEW_MINION = 1 << 0,
		MINION_APPEARANCE_MODEL_RANDOM = 1 << 1,
		MINION_APPEARANCE_TEXTURE_RANDOM = 1 << 2,
		MINION_APPEARANCE_TEXTURE_INCLUDE_COMPATIBLE = 1 << 3,
		MINION_APPEARANCE_TEXTURE_RANDOM_COLOR = 1 << 4,
		MINION_APPEARANCE_GROWTH_SIZE_SHUFFLE = 1 << 5,
		MINION_APPEARANCE_GROWTH_SIZE_INVERT = 1 << 6
	};

	enum class BossElement
	{
		RandomAll,
		RandomElemental
	};

	enum class BossAppearance
	{
		ColorBasedOnElement,
		RandomColor
	};

	using BossAppearanceEC_t = u32;
	enum : BossAppearanceEC_t
	{
		BOSS_APPEARANCE_EC_RANDOM_NEW_APPEARANCE = 1 << 0,
		BOSS_APPEARANCE_EC_TEXTURE_RANDOM = 1 << 1,
		BOSS_APPEARANCE_EC_TEXTURE_INCLUDE_COMPATIBLE = 1 << 2,
		BOSS_APPEARANCE_EC_TEXTURE_RANDOM_COLOR = 1 << 3
	};

	static constexpr auto nbForest{ 8u };
	using ForestOstArray = std::array<s16, Randomizer::nbForest>;

	static constexpr s16 randomOstVal{ -1 };

	enum class ForestOst
	{
		Random,
		Shuffle
	};

	using EquipmentWeapons_t = u32;
	enum : EquipmentWeapons_t
	{
		EQUIPMENT_WEAPONS_RANDOM_STATS_AND_ELEMENT = 1 << 0,
		EQUIPMENT_WEAPONS_RANDOM_APPEARANCE = 1 << 1
	};

	using EquipmentArmors_t = u32;
	enum : EquipmentArmors_t
	{
		EQUIPMENT_ARMORS_RANDOM_STATS = 1 << 0,
		EQUIPMENT_ARMORS_RANDOM_APPEARANCE = 1 << 1
	};

	enum class TreasureItems
	{
		Random,
		RandomByCategory
	};

	enum class TreasureValuables
	{
		Random,
		RandomByCategory
	};

	enum class TreasureEquipments
	{
		Random,
		RandomByCategory
	};

	struct Color
	{
		u8 red;
		u8 green;
		u8 blue;
		u8 alpha;

		constexpr auto& operator=(u32 rgba)
		{
			red = static_cast<u8>(rgba);
			green = static_cast<u8>(rgba >> 8);
			blue = static_cast<u8>(rgba >> 16);
			alpha = static_cast<u8>(rgba >> 24);
			return *this;
		}
	};

	using HudColorArray = std::array<Randomizer::Color, 15>;

	using AddonsNuzlocke_t = u32;
	enum : AddonsNuzlocke_t
	{
		ADDONS_NUZLOCKE_ONE_CAPTURE = 1 << 0,
		ADDONS_NUZLOCKE_DEFINITIVE_DEATH = 1 << 1,
		ADDONS_NUZLOCKE_DEFINITIVE_LEVANT_DEATH = 1 << 2
	};

	enum class AddonsDifficulty
	{
		Hard,
		Extreme
	};

	Randomizer(Game* game);

	void init();
	const Game& game() const;

	Id_Entity_t dreamMinion() const;
	Id_Entity_t korisMinion() const;
	Randomizer::ForestOstArray forestOst() const;
	Id_Entity_t generateMinion(bool eternalCorridor) const;
	MipsFn::GenerateValidMinion generateValidMinionFn() const;
	bool isValidMinionForStory(Id_Entity_t id) const;
	HudColorArray hudColor() const;

	void levantBaseStats() const;
	void levantAnimation(Randomizer::LevantAnimation_t state) const;
	void levantFluteStyling() const;
	void minionSpawnStory(Randomizer::MinionSpawnStory state) const;
	void minionSpawnEC() const;
	void minionDreamMinion() const;
	void minionDreamMinion(Id_Entity_t id) const;
	void minionKorisMinion() const;
	void minionKorisMinion(Id_Entity_t id) const;
	void minionStats(Randomizer::MinionStats_t state) const;
	void minionSpecialMagic(const std::unordered_set<Special_t>& special, Magic_t magic) const;
	void minionAppearance(Randomizer::MinionAppearance_t state);
	void bossElement(Randomizer::BossElement state) const;
	void bossSpecialMagic() const;
	void bossAppearance(Randomizer::BossAppearance state) const;
	void bossElementEC() const;
	void bossSpecialMagicEC() const;
	void bossAppearanceEC(Randomizer::BossAppearanceEC_t state) const;
	void forestPaletteColor() const;
	void forestBattleMaps() const;
	void forestOst(Randomizer::ForestOst state) const;
	void forestOst(const Randomizer::ForestOstArray& ostsId) const;
	void forestRandomEternalCorridorOstPerCorridor() const;
	void equipmentWeapons(Randomizer::EquipmentWeapons_t state) const;
	void equipmentArmors(Randomizer::EquipmentArmors_t state) const;
	void equipmentOthers() const;
	static void setDamageEffectFromWeaponIdFn(const Game& game, bool setAutumnMoonEffect);
	void treasureItems(Randomizer::TreasureItems state, bool randomSkeletonKeys) const;
	void treasureValuables(Randomizer::TreasureValuables state, bool randomUsableValuables) const;
	void treasureEquipments(Randomizer::TreasureEquipments state) const;
	void treasureBattle(bool shuffleSkeletonKeys) const;
	void shopWeapon() const;
	void shopArmor() const;
	void shopOther() const;
	void shopItem() const;
	void shopEternalCorridorUnlockAll() const;
	void miscHudColor() const;
	void miscHudColor(const Randomizer::HudColorArray& hud) const;
	void miscNPCsVoice() const;
	void addonsNuzlocke(Randomizer::AddonsNuzlocke_t state) const;
	void addonsDifficulty(Randomizer::AddonsDifficulty state) const;
	void addonsSkipTutorial(bool skipKoris) const;
	void addonsX2Framerate() const;
	void addonsItemQuantityLimit(u8 limit) const;
	void addonsLevelCapEC(u8 levelCap) const;
	void addonsShowHiddenStats() const;
	void addonsPalToNtsc() const;
	void fixesBodyEnhancement() const;
	void fixesAutumnMoonEffect() const;
	void fixesHpMpBarsSize(u16 value) const;
	void fixesSpecialsModifiers() const;
private:
	Game* m_game;
	SharedData m_sharedData;
};