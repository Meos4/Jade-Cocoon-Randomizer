#pragma once

#include "ui_MiscWidget.h"

#include "Common/Types.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class HelpConsoleWidget;

class MiscWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	MiscWidget(HelpConsoleWidget* helpConsole, QWidget* parent = nullptr);

	void enableUI(Randomizer* randomizer) override;
	void disableUI() override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;

	const Ui::MiscWidget& Ui() const;
	Randomizer::HudColorArray hudColor() const;	
public Q_SLOTS:
	void updateHudColorRandom();
	void updateHudColorCustom();
	void updateHudThemes();
	void updateHudColorRGBColor();
	void updateHudColorRGBSelected();
	void updateHudColorRGBHighlight();
private:
	void colorToUI(s32 theme);
	void UIToColor(s32 theme);

	Ui::MiscWidget m_ui;

	Randomizer::HudColorArray m_colors{};
	s32 m_previousThemeIndex{};
	bool m_isFirstEnableUI{ true };

	std::vector<RandomizerQCheckBox> m_qCheckBox;
};