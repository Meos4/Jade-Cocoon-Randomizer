#pragma once

#include "ui_ForestWidget.h"

#include "Backend/Forest.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class HelpConsoleWidget;

class ForestWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	ForestWidget(HelpConsoleWidget* helpConsole, QWidget* parent = nullptr);

	void enableUI(Game* game, std::shared_ptr<SharedData> sharedData) override;
	void disableUI() override;
	void write() const override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;
	Forest::OstArray ostArray() const;
public Q_SLOTS:
	void updateOstRandom();
	void updateOstShuffle();
	void updateOstCustom();
	void updateOst();
private:
	void ostToUI(s32 forest);
	void UIToOst(s32 forest);

	Ui::ForestWidget m_ui;

	std::array<s32, Forest::nbForest> m_ostIndex{};
	s32 m_previousForestIndex{};
	bool m_isFirstEnableUI{ true };

	std::unique_ptr<Forest> m_forest;
	std::vector<RandomizerQCheckBox> m_qCheckBox;
};