#pragma once

#include <QCheckBox>

class AddonsWidget;
class BossWidget;
class ChallengeWidget;
class EquipmentWidget;
class FixesWidget;
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
		AddonsWidget* addons, ChallengeWidget* challenge, FixesWidget* fixes);

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
	AddonsWidget* m_addons;
	ChallengeWidget* m_challenge;
	FixesWidget* m_fixes;
};