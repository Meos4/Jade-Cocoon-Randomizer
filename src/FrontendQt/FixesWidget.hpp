#pragma once

#include "ui_FixesWidget.h"

#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class HelpConsoleWidget;

class FixesWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	FixesWidget(HelpConsoleWidget* helpConsole, QWidget* parent = nullptr);

	void enableUI(Randomizer* randomizer) override;
	void disableUI() override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;

	const Ui::FixesWidget& Ui() const;
public Q_SLOTS:
	void setHpMpBarsSizeText(s32 value);
private:
	Ui::FixesWidget m_ui;

	std::vector<RandomizerQCheckBox> m_qCheckBox;
	std::vector<RandomizerQSlider> m_qSlider;
};