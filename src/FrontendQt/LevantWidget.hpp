#pragma once

#include "ui_LevantWidget.h"

#include "Backend/Levant.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class LevantWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	LevantWidget(QWidget* parent = nullptr);

	void enableUI(Game* game, std::shared_ptr<SharedData> sharedData) override;
	void disableUI() override;
	void write() const override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;
private:
	Ui::LevantWidget m_ui;

	std::unique_ptr<Levant> m_levant;
	std::vector<RandomizerQCheckBox> m_qCheckBox;
};