#pragma once

#include "ui_FixesWidget.h"

#include "Backend/Fixes.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class FixesWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	FixesWidget(QWidget* parent = nullptr);

	void enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData) override;
	void disableUI() override;
	void write() const override;
	const char* name() const override;
	void loadSettings(const nlohmann::json& json) override;
	void saveSettings(nlohmann::ordered_json* json) override;
public Q_SLOTS:
	void setHpMpBarsSizeText(s32 value);
private:
	Ui::FixesWidget m_ui;

	std::unique_ptr<Fixes> m_fixes;
	std::vector<RandomizerQCheckBox> m_qCheckBox;
	std::vector<RandomizerQSlider> m_qSlider;
};