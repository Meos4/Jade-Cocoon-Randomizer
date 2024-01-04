#pragma once

#include "ui_BossWidget.h"

#include "Backend/Boss.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class BossWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	BossWidget(QWidget* parent = nullptr);

	void enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData) override;
	void disableUI() override;
	void write() const override;
	const char* name() const override;
	void loadPresets(const nlohmann::json& json) override;
	void savePresets(nlohmann::ordered_json* json) override;
public Q_SLOTS:
	void updateStoryElement();
	void updateStoryAppearanceTextureBasedOnElement();
	void updateStoryAppearanceTextureRandomColor();
	void updateEternalCorridorElementRandom();
private:
	Ui::BossWidget m_ui;

	std::unique_ptr<Boss> m_boss;
	std::vector<RandomizerQCheckBox> m_qCheckBox;
};