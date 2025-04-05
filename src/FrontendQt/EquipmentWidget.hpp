#pragma once

#include "ui_EquipmentWidget.h"

#include "Backend/Equipment.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class EquipmentWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	EquipmentWidget(QWidget* parent = nullptr);

	void enableUI(Game* game, std::shared_ptr<SharedData> sharedData) override;
	void disableUI() override;
	void write() const override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;
private:
	Ui::EquipmentWidget m_ui;

	std::unique_ptr<Equipment> m_equipment;
	std::vector<RandomizerQCheckBox> m_qCheckBox;
};