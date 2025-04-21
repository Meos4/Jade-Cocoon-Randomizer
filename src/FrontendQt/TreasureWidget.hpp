#pragma once

#include "ui_TreasureWidget.h"

#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class HelpConsoleWidget;

class TreasureWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	TreasureWidget(HelpConsoleWidget* helpConsole, QWidget* parent = nullptr);

	void enableUI(Randomizer* randomizer) override;
	void disableUI() override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;

	const Ui::TreasureWidget& Ui() const;
public Q_SLOTS:
	void updateItems();
	void updateValuables();
	void updateEquipmentsRandom();
	void updateEquipmentsRandomByCategory();
private:
	Ui::TreasureWidget m_ui;

	std::vector<RandomizerQCheckBox> m_qCheckBox;
};