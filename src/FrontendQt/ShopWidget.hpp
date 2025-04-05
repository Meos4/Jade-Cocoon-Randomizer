#pragma once

#include "ui_ShopWidget.h"

#include "Backend/Shop.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class ShopWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	ShopWidget(QWidget* parent = nullptr);

	void enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData) override;
	void disableUI() override;
	void write() const override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;
private:
	Ui::ShopWidget m_ui;

	std::unique_ptr<Shop> m_shop;
	std::vector<RandomizerQCheckBox> m_qCheckBox;
};