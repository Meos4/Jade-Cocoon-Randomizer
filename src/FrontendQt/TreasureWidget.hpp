#pragma once

#include "ui_TreasureWidget.h"

#include "Backend/Treasure.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class TreasureWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	TreasureWidget(QWidget* parent = nullptr);

	void enableUI(Game* game, std::shared_ptr<SharedData> sharedData) override;
	void disableUI() override;
	void write() const override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;
public Q_SLOTS:
	void updateItems();
	void updateValuables();
	void updateEquipmentsRandom();
	void updateEquipmentsRandomByCategory();
private:
	Ui::TreasureWidget m_ui;

	std::unique_ptr<Treasure> m_treasure;
	std::vector<RandomizerQCheckBox> m_qCheckBox;
};