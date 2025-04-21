#pragma once

#include "ui_AddonsWidget.h"

#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class HelpConsoleWidget;

class AddonsWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	AddonsWidget(HelpConsoleWidget* helpConsole, QWidget* parent = nullptr);

	void enableUI(Randomizer* randomizer) override;
	void disableUI() override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;

	const Ui::AddonsWidget& Ui() const;
public Q_SLOTS:
	void updateDifficultyHard();
	void updateDifficultyExtreme();
	void setItemQuantityText(s32 value);
	void setEternalCorridorLevelCapText(s32 value);
private:
	Ui::AddonsWidget m_ui;

	std::vector<RandomizerQCheckBox> m_qCheckBox;
	std::vector<RandomizerQSlider> m_qSlider;
};