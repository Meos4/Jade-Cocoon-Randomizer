#include "MiscWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/GuiSettings.hpp"

#include <format>

MiscWidget::MiscWidget(QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.hudRandom) },
		{ SETTINGS(m_ui.hudColorCustom) },
		{ SETTINGS(m_ui.npcsVoiceRandom) }
	};

	GuiSettings::setToolTip(m_ui.hudRandom, "Randomize the colors of the 5 huds.");
	static constexpr auto hudCustom{ "Allows to change the colors of the huds." };
	GuiSettings::setToolTip(m_ui.hudColorCustom, hudCustom);
	GuiSettings::setToolTip(m_ui.hudColorCombo, hudCustom);
	GuiSettings::setToolTip(m_ui.npcsVoiceRandom, "Randomize NPCs voices.");

	m_ui.hudColorCombo->setStyleSheet("font-weight: normal;");
	m_ui.hudColorLabel->setStyleSheet("font-weight: normal;");
	m_ui.hudColorR->setStyleSheet("font-weight: normal;");
	m_ui.hudColorG->setStyleSheet("font-weight: normal;");
	m_ui.hudColorB->setStyleSheet("font-weight: normal;");
	m_ui.hudSelectedLabel->setStyleSheet("font-weight: normal;");
	m_ui.hudSelectedR->setStyleSheet("font-weight: normal;");
	m_ui.hudSelectedG->setStyleSheet("font-weight: normal;");
	m_ui.hudSelectedB->setStyleSheet("font-weight: normal;");
	m_ui.hudHighlightLabel->setStyleSheet("font-weight: normal;");
	m_ui.hudHighlightR->setStyleSheet("font-weight: normal;");
	m_ui.hudHighlightG->setStyleSheet("font-weight: normal;");
	m_ui.hudHighlightB->setStyleSheet("font-weight: normal;");

	m_ui.hudColorCombo->setEnabled(false);
	m_ui.hudColorR->setEnabled(false);
	m_ui.hudColorG->setEnabled(false);
	m_ui.hudColorB->setEnabled(false);
	m_ui.hudSelectedR->setEnabled(false);
	m_ui.hudSelectedG->setEnabled(false);
	m_ui.hudSelectedB->setEnabled(false);
	m_ui.hudHighlightR->setEnabled(false);
	m_ui.hudHighlightG->setEnabled(false);
	m_ui.hudHighlightB->setEnabled(false);

	static constexpr std::array<const char*, 5> hudNames
	{
		"Hud 1", "Hud 2", "Hud 3" , "Hud 4", "Title"
	};

	for (const auto name : hudNames)
	{
		m_ui.hudColorCombo->addItem(name);
	}

	connect(m_ui.hudRandom, &QAbstractButton::toggled, this, &MiscWidget::updateHudColorRandom);
	connect(m_ui.hudColorCustom, &QAbstractButton::toggled, this, &MiscWidget::updateHudColorCustom);
	connect(m_ui.hudColorCombo, &QComboBox::currentIndexChanged, this, &MiscWidget::updateHudThemes);
	connect(m_ui.hudColorR, &QSpinBox::valueChanged, this, &MiscWidget::updateHudColorRGBColor);
	connect(m_ui.hudColorG, &QSpinBox::valueChanged, this, &MiscWidget::updateHudColorRGBColor);
	connect(m_ui.hudColorB, &QSpinBox::valueChanged, this, &MiscWidget::updateHudColorRGBColor);
	connect(m_ui.hudSelectedR, &QSpinBox::valueChanged, this, &MiscWidget::updateHudColorRGBSelected);
	connect(m_ui.hudSelectedG, &QSpinBox::valueChanged, this, &MiscWidget::updateHudColorRGBSelected);
	connect(m_ui.hudSelectedB, &QSpinBox::valueChanged, this, &MiscWidget::updateHudColorRGBSelected);
	connect(m_ui.hudHighlightR, &QSpinBox::valueChanged, this, &MiscWidget::updateHudColorRGBHighlight);
	connect(m_ui.hudHighlightG, &QSpinBox::valueChanged, this, &MiscWidget::updateHudColorRGBHighlight);
	connect(m_ui.hudHighlightB, &QSpinBox::valueChanged, this, &MiscWidget::updateHudColorRGBHighlight);
}

void MiscWidget::enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
{
	m_misc = std::make_unique<Misc>(game, sharedData);

	if (m_isFirstEnableUI)
	{
		m_colors = m_misc->hudColor();
		m_previousThemeIndex = m_ui.hudColorCombo->currentIndex();
		colorToUI(m_previousThemeIndex);
		m_isFirstEnableUI = false;
	}

	setEnabled(true);
}

void MiscWidget::disableUI()
{
	setDisabled(true);
	if (m_misc)
	{
		m_misc.reset();
	}
}

void MiscWidget::write() const
{
	if (!m_misc)
	{
		throw JcrException{ "Game is uninitialized" };
	}

	if (m_ui.hudRandom->isChecked())
	{
		m_misc->setHudColor();
	}
	else if (m_ui.hudColorCustom->isChecked())
	{
		m_misc->setHudColor(hudColor());
	}

	if (m_ui.npcsVoiceRandom->isChecked())
	{
		m_misc->setNPCsVoice();
	}
}

const char* MiscWidget::name() const
{
	return "Misc";
}

void MiscWidget::loadPresets(const Json::Read& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}

	for (std::size_t i{}; i < m_colors.size(); ++i)
	{
		Json::set<u8>(json["colors"][i], "red", [&](auto v) { m_colors[i].red = v; });
		Json::set<u8>(json["colors"][i], "green", [&](auto v) { m_colors[i].green = v; });
		Json::set<u8>(json["colors"][i], "blue", [&](auto v) { m_colors[i].blue = v; });
	}
	colorToUI(m_ui.hudColorCombo->currentIndex());
}

void MiscWidget::savePresets(Json::Write* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}

	UIToColor(m_previousThemeIndex);
	for (std::size_t i{}; i < m_colors.size(); ++i)
	{
		(*json)["colors"][i]["red"] = m_colors[i].red;
		(*json)["colors"][i]["green"] = m_colors[i].green;
		(*json)["colors"][i]["blue"] = m_colors[i].blue;
	}
}

Misc::HudColorArray MiscWidget::hudColor() const
{
	auto hud{ m_colors };
	const auto theme{ m_ui.hudColorCombo->currentIndex() };

	hud[theme].red = static_cast<u8>(m_ui.hudColorR->value());
	hud[theme].green = static_cast<u8>(m_ui.hudColorG->value());
	hud[theme].blue = static_cast<u8>(m_ui.hudColorB->value());
	hud[theme + 5].red = static_cast<u8>(m_ui.hudSelectedR->value());
	hud[theme + 5].green = static_cast<u8>(m_ui.hudSelectedG->value());
	hud[theme + 5].blue = static_cast<u8>(m_ui.hudSelectedB->value());
	hud[theme + 10].red = static_cast<u8>(m_ui.hudHighlightR->value());
	hud[theme + 10].green = static_cast<u8>(m_ui.hudHighlightG->value());
	hud[theme + 10].blue = static_cast<u8>(m_ui.hudHighlightB->value());

	return hud;
}

void MiscWidget::colorToUI(s32 theme)
{
	m_ui.hudColorR->setValue(m_colors[theme].red);
	m_ui.hudColorG->setValue(m_colors[theme].green);
	m_ui.hudColorB->setValue(m_colors[theme].blue);
	m_ui.hudSelectedR->setValue(m_colors[theme + 5].red);
	m_ui.hudSelectedG->setValue(m_colors[theme + 5].green);
	m_ui.hudSelectedB->setValue(m_colors[theme + 5].blue);
	m_ui.hudHighlightR->setValue(m_colors[theme + 10].red);
	m_ui.hudHighlightG->setValue(m_colors[theme + 10].green);
	m_ui.hudHighlightB->setValue(m_colors[theme + 10].blue);
}

void MiscWidget::UIToColor(s32 theme)
{
	m_colors[theme].red = static_cast<u8>(m_ui.hudColorR->value());
	m_colors[theme].green = static_cast<u8>(m_ui.hudColorG->value());
	m_colors[theme].blue = static_cast<u8>(m_ui.hudColorB->value());
	m_colors[theme + 5].red = static_cast<u8>(m_ui.hudSelectedR->value());
	m_colors[theme + 5].green = static_cast<u8>(m_ui.hudSelectedG->value());
	m_colors[theme + 5].blue = static_cast<u8>(m_ui.hudSelectedB->value());
	m_colors[theme + 10].red = static_cast<u8>(m_ui.hudHighlightR->value());
	m_colors[theme + 10].green = static_cast<u8>(m_ui.hudHighlightG->value());
	m_colors[theme + 10].blue = static_cast<u8>(m_ui.hudHighlightB->value());
}

void MiscWidget::updateHudColorRandom()
{
	const bool isChecked{ m_ui.hudRandom->isChecked() };
	if (isChecked)
	{
		m_ui.hudColorCustom->setChecked(false);
	}
	m_ui.hudColorCustom->setEnabled(!isChecked);
}

void MiscWidget::updateHudColorCustom()
{
	const bool isChecked{ m_ui.hudColorCustom->isChecked() };
	if (isChecked)
	{
		m_ui.hudRandom->setChecked(false);
	}
	m_ui.hudRandom->setEnabled(!isChecked);
	m_ui.hudColorCombo->setEnabled(isChecked);
	m_ui.hudColorR->setEnabled(isChecked);
	m_ui.hudColorG->setEnabled(isChecked);
	m_ui.hudColorB->setEnabled(isChecked);
	m_ui.hudSelectedR->setEnabled(isChecked);
	m_ui.hudSelectedG->setEnabled(isChecked);
	m_ui.hudSelectedB->setEnabled(isChecked);
	m_ui.hudHighlightR->setEnabled(isChecked);
	m_ui.hudHighlightG->setEnabled(isChecked);
	m_ui.hudHighlightB->setEnabled(isChecked);
}

void MiscWidget::updateHudThemes()
{
	UIToColor(m_previousThemeIndex);
	m_previousThemeIndex = m_ui.hudColorCombo->currentIndex();
	colorToUI(m_previousThemeIndex);
}

void MiscWidget::updateHudColorRGBColor()
{
	const auto colorRGB
	{
		std::format("background-color:rgb({},{},{});", 
			m_ui.hudColorR->value(), m_ui.hudColorG->value(), m_ui.hudColorB->value())
	};
	m_ui.hudColorLabelResult->setStyleSheet(QString::fromStdString(colorRGB));
}

void MiscWidget::updateHudColorRGBSelected()
{
	const auto selectedRGB
	{
		std::format("background-color:rgb({},{},{});", 
			m_ui.hudSelectedR->value(), m_ui.hudSelectedG->value(), m_ui.hudSelectedB->value())
	};
	m_ui.hudSelectedLabelResult->setStyleSheet(QString::fromStdString(selectedRGB));
}

void MiscWidget::updateHudColorRGBHighlight()
{
	const auto highlightRGB
	{
		std::format("background-color:rgb({},{},{});", 
			m_ui.hudHighlightR->value(), m_ui.hudHighlightG->value(), m_ui.hudHighlightB->value())
	};
	m_ui.hudHighlightLabelResult->setStyleSheet(QString::fromStdString(highlightRGB));
}