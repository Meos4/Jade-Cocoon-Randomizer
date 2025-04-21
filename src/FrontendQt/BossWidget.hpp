#pragma once

#include "ui_BossWidget.h"

#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class HelpConsoleWidget;

class BossWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	BossWidget(HelpConsoleWidget* helpConsole, QWidget* parent = nullptr);

	void enableUI(Randomizer* randomizer) override;
	void disableUI() override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;

	const Ui::BossWidget& Ui() const;
public Q_SLOTS:
	void updateStoryElement();
	void updateStoryAppearanceTextureBasedOnElement();
	void updateStoryAppearanceTextureRandomColor();
	void updateEternalCorridorElementRandom();
private:
	Ui::BossWidget m_ui;

	std::vector<RandomizerQCheckBox> m_qCheckBox;
};