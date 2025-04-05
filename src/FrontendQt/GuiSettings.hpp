#pragma once

#include "Common/Json.hpp"

class QString;
class QWidget;

enum class Theme
{
	Dark,
	Light,
	Count
};

class GuiSettings final
{
public:
	static void setToolTip(QWidget* widget, const QString& str);

	void setOsTheme();
	void updateTheme(Theme theme);
	void loadSettings(const Json::Read& json);
	void saveSettings(Json::Write* json);

	Theme theme() const;
private:
	Theme m_theme{ Theme::Dark };
};