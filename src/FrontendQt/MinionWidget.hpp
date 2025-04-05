#pragma once

#include "ui_MinionWidget.h"

#include "Backend/Minion.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class SpecialMagicDialog;

class MinionWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	MinionWidget(QWidget* parent = nullptr);

	void enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData) override;
	void disableUI() override;
	void write() const override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;
public Q_SLOTS:
	void updateSpawnStoryRandomRealtime();
	void updateSpawnStoryRandomPremade();
	void updateSpawnDreamRandom();
	void updateSpawnDreamCustom();
	void updateSpawnKorisRandom();
	void updateSpawnKorisCustom();
	void updateAppearanceGrowthSizeShuffle();
	void updateAppearanceGrowthSizeInvert();
private:
	Ui::MinionWidget m_ui;

	std::unique_ptr<Minion> m_minion;
	std::vector<RandomizerQCheckBox> m_qCheckBox;
	std::vector<RandomizerQComboBox> m_qComboBox;
	bool m_isSpawnStoryRandomRealtimeBanned{ false };
	SpecialMagicDialog* m_specialMagicDialog;
};