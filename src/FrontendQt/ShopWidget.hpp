#pragma once

#include "ui_ShopWidget.h"

#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class HelpConsoleWidget;

class ShopWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	ShopWidget(HelpConsoleWidget* helpConsole, QWidget* parent = nullptr);

	void enableUI(Randomizer* randomizer) override;
	void disableUI() override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;

	const Ui::ShopWidget& Ui() const;
private:
	Ui::ShopWidget m_ui;

	std::vector<RandomizerQCheckBox> m_qCheckBox;
};