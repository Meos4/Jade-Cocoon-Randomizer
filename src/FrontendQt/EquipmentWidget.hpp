#pragma once

#include "ui_EquipmentWidget.h"

#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class HelpConsoleWidget;

class EquipmentWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	EquipmentWidget(HelpConsoleWidget* helpConsole, QWidget* parent = nullptr);

	void enableUI(Randomizer* randomizer) override;
	void disableUI() override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;

	const Ui::EquipmentWidget& Ui() const;
private:
	Ui::EquipmentWidget m_ui;

	std::vector<RandomizerQCheckBox> m_qCheckBox;
};