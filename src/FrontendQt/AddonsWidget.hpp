#pragma once

#include "ui_AddonsWidget.h"

#include "Backend/Addons.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class AddonsWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	AddonsWidget(QWidget* parent = nullptr);

	void enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData) override;
	void disableUI() override;
	void write() const override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;
public Q_SLOTS:
	void updateDifficultyHard();
	void updateDifficultyExtreme();
	void setItemQuantityText(s32 value);
	void setEternalCorridorLevelCapText(s32 value);
private:
	Ui::AddonsWidget m_ui;

	std::unique_ptr<Addons> m_addons;
	std::vector<RandomizerQCheckBox> m_qCheckBox;
	std::vector<RandomizerQSlider> m_qSlider;
};