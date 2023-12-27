#include "GuiSettings.hpp"

#include <limits>

#include <QApplication>
#include <QIcon>
#include <QPalette>
#include <QString>
#include <QWidget>

void GuiSettings::setToolTip(QWidget* widget, const QString& str)
{
	widget->setToolTip(str);
	widget->setToolTipDuration(std::numeric_limits<int>::max());
}

void GuiSettings::setOsTheme()
{
	qApp->setStyle("Fusion");
	const auto palette{ qApp->palette() };
	const bool isPaletteDark{ palette.windowText().color().value() > palette.window().color().value() };
	updateTheme(isPaletteDark ? Theme::Dark : Theme::Light);
}

void GuiSettings::updateTheme(Theme theme)
{
	QPalette palette;

	if (theme == Theme::Dark)
	{
		QIcon::setThemeName("White");

		palette.setColor(QPalette::WindowText, { 255, 255, 255 });
		palette.setColor(QPalette::Button, { 42, 42, 42 });
		palette.setColor(QPalette::Light, { 120, 120, 120 });
		palette.setColor(QPalette::Midlight, { 90, 90, 90 });
		palette.setColor(QPalette::Dark, { 30, 30, 30 });
		palette.setColor(QPalette::Mid, { 40, 40, 40 });
		palette.setColor(QPalette::Text, { 255, 255, 255 });
		palette.setColor(QPalette::BrightText, { 255, 232, 69 });
		palette.setColor(QPalette::ButtonText, { 255, 255, 255 });
		palette.setColor(QPalette::Base, { 56, 56, 56 });
		palette.setColor(QPalette::Window, { 25, 25, 25 });
		palette.setColor(QPalette::Shadow, { 0, 0, 0 });
		palette.setColor(QPalette::HighlightedText, { 255, 255, 255 });
		palette.setColor(QPalette::AlternateBase, { 92, 33, 0 });
		palette.setColor(QPalette::NoRole, { 0, 0, 0 });
		palette.setColor(QPalette::ToolTipBase, { 43, 43, 43 });
		palette.setColor(QPalette::ToolTipText, { 232, 232, 232 });
		palette.setColor(QPalette::PlaceholderText, { 255, 255, 255 });

		palette.setColor(QPalette::Disabled, QPalette::WindowText, { 157, 157, 157 });
		palette.setColor(QPalette::Disabled, QPalette::Text, { 157, 157, 157 });
		palette.setColor(QPalette::Disabled, QPalette::BrightText, { 255, 151, 209 });
		palette.setColor(QPalette::Disabled, QPalette::ButtonText, { 157, 157, 157 });
		palette.setColor(QPalette::Disabled, QPalette::Base, { 43, 43, 43 });
		palette.setColor(QPalette::Disabled, QPalette::AlternateBase, { 52, 52, 52 });
	}
	else if (theme == Theme::Light)
	{
		QIcon::setThemeName("Black");

		palette.setColor(QPalette::WindowText, { 0, 0, 0 });
		palette.setColor(QPalette::Button, { 240, 240, 240 });
		palette.setColor(QPalette::Light, { 255, 255, 255 });
		palette.setColor(QPalette::Midlight, { 227, 227, 227 });
		palette.setColor(QPalette::Dark, { 160, 160, 160 });
		palette.setColor(QPalette::Mid, { 160, 160, 160 });
		palette.setColor(QPalette::Text, { 0, 0, 0 });
		palette.setColor(QPalette::BrightText, { 255, 255, 255 });
		palette.setColor(QPalette::ButtonText, { 0, 0, 0 });
		palette.setColor(QPalette::Base, { 255, 255, 255 });
		palette.setColor(QPalette::Window, { 240, 240, 240 });
		palette.setColor(QPalette::Shadow, { 105, 105, 105 });
		palette.setColor(QPalette::HighlightedText, { 255, 255, 255 });
		palette.setColor(QPalette::AlternateBase, { 233, 231, 227 });
		palette.setColor(QPalette::NoRole, { 0, 0, 0 });
		palette.setColor(QPalette::ToolTipBase, { 255, 255, 220 });
		palette.setColor(QPalette::ToolTipText, { 0, 0, 0 });
		palette.setColor(QPalette::PlaceholderText, { 0, 0, 0 });

		palette.setColor(QPalette::Disabled, QPalette::WindowText, { 120, 120, 120 });
		palette.setColor(QPalette::Disabled, QPalette::Midlight, { 247, 247, 247 });
		palette.setColor(QPalette::Disabled, QPalette::Text, { 120, 120, 120 });
		palette.setColor(QPalette::Disabled, QPalette::ButtonText, { 120, 120, 120 });
		palette.setColor(QPalette::Disabled, QPalette::Shadow, { 0, 0, 0 });
		palette.setColor(QPalette::Disabled, QPalette::AlternateBase, { 247, 247, 247 });
	}

	qApp->setPalette(palette);
	qApp->setStyle("Fusion");

	m_theme = theme;
}

void GuiSettings::loadSettings(const nlohmann::json& json)
{
	if (json.contains("theme"))
	{
		m_theme = json.at("theme");
		using ThemeType = std::underlying_type_t<Theme>;
		m_theme = static_cast<Theme>(std::clamp(static_cast<ThemeType>(m_theme), 0, static_cast<ThemeType>(Theme::Count) - 1));
	}
}

void GuiSettings::saveSettings(nlohmann::ordered_json* json)
{
	(*json)["theme"] = m_theme;
}

Theme GuiSettings::theme() const
{
	return m_theme;
}