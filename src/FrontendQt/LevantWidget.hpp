#pragma once

#include "ui_LevantWidget.h"

#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class HelpConsoleWidget;

class LevantWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	LevantWidget(HelpConsoleWidget* helpConsole, QWidget* parent = nullptr);

	void enableUI(Randomizer* randomizer) override;
	void disableUI() override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;

	const Ui::LevantWidget& Ui() const;
private:
	Ui::LevantWidget m_ui;

	std::vector<RandomizerQCheckBox> m_qCheckBox;
};