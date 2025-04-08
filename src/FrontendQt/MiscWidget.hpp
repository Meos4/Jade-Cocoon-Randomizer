#pragma once

#include "ui_MiscWidget.h"

#include "Backend/Misc.hpp"
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

	void enableUI(Game* game, std::shared_ptr<SharedData> sharedData) override;
	void disableUI() override;
	void write() const override;
	const char* name() const override;
	void loadPresets(const Json::Read& json) override;
	void savePresets(Json::Write* json) override;
	Misc::HudColorArray hudColor() const;
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

	Misc::HudColorArray m_colors{};
	s32 m_previousThemeIndex{};
	bool m_isFirstEnableUI{ true };

	std::unique_ptr<Misc> m_misc;
	std::vector<RandomizerQCheckBox> m_qCheckBox;
};