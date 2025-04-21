#pragma once

#include "ui_ForestWidget.h"

#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class HelpConsoleWidget;

class ForestWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	ForestWidget(HelpConsoleWidget* helpConsole, QWidget* parent = nullptr);

	void enableUI(Randomizer* randomizer) override;
	void disableUI() override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;

	const Ui::ForestWidget& Ui() const;
	Randomizer::ForestOstArray ostArray() const;
public Q_SLOTS:
	void updateOstRandom();
	void updateOstShuffle();
	void updateOstCustom();
	void updateOst();
private:
	void ostToUI(s32 forest);
	void UIToOst(s32 forest);

	Ui::ForestWidget m_ui;

	std::array<s32, Randomizer::nbForest> m_ostIndex{};
	s32 m_previousForestIndex{};
	bool m_isFirstEnableUI{ true };

	std::vector<RandomizerQCheckBox> m_qCheckBox;
};