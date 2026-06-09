#pragma once

class BossWidget;
class ChallengeWidget;
class DefaultDialog;
class EquipmentWidget;
class ForestWidget;
class LevantWidget;
class MinionWidget;
class MiscWidget;
class ShopWidget;
class TreasureWidget;
class Randomizer;

class RandomizerUiManager final
{
public:
	RandomizerUiManager(LevantWidget* levant, MinionWidget* minion, BossWidget* boss, ForestWidget* forest,
		EquipmentWidget* equipment, TreasureWidget* treasure, ShopWidget* shop, MiscWidget* misc,
		ChallengeWidget* challenge,
		DefaultDialog* defaultDialog);

	void write(Randomizer* randomizer) const;
private:
	LevantWidget* m_levant;
	MinionWidget* m_minion;
	BossWidget* m_boss;
	ForestWidget* m_forest;
	EquipmentWidget* m_equipment;
	TreasureWidget* m_treasure;
	ShopWidget* m_shop;
	MiscWidget* m_misc;
	ChallengeWidget* m_challenge;
	DefaultDialog* m_default;
};
