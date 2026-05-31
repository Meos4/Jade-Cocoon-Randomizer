#pragma once

#include "Common/Json.hpp"

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
	void loadSettings(const Json::Read& json);
	void saveSettings(Json::Write* json) const;

	Theme theme() const;
private:
	Theme m_theme{ Theme::Dark };
};