#pragma once

#include "ui_MiscWidget.h"

#include "Backend/Misc.hpp"
#include "Common/Types.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <vector>

class MiscWidget final : public RandomizerWidget
{
	Q_OBJECT
public:
	MiscWidget(QWidget* parent = nullptr);

	void enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData) override;
	void disableUI() override;
	void write() const override;
	const char* name() const override;
	void loadPresets(const nlohmann::json& json) override;
	void savePresets(nlohmann::ordered_json* json) override;
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