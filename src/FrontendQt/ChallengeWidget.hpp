#pragma once

#include "ui_ChallengeWidget.h"

#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class HelpConsoleWidget;

class ChallengeWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	ChallengeWidget(HelpConsoleWidget* helpConsole, QWidget* parent = nullptr);

	void enableUI(Randomizer* randomizer) override;
	void disableUI() override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;

	const Ui::ChallengeWidget& Ui() const;
public Q_SLOTS:
	void updateDifficultyHard();
	void updateDifficultyExtreme();
private:
	Ui::ChallengeWidget m_ui;

	std::vector<RandomizerQCheckBox> m_qCheckBox;
};