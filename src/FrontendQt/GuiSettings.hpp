#pragma once

#include "nlohmann/json.hpp"

enum class Theme
{
	Dark,
	Light,
	Count
};

class GuiSettings final
{
public:
	void setOsTheme();
	void updateTheme(Theme theme);
	void loadSettings(const nlohmann::json& json);
	void saveSettings(nlohmann::ordered_json* json);

	Theme theme() const;
private:
	Theme m_theme{ Theme::Dark };
};