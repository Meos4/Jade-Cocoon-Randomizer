#include "EquipmentWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/HelpConsoleWidget.hpp"

EquipmentWidget::EquipmentWidget(HelpConsoleWidget* helpConsole, QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.weaponsRandomStatsAndElement) },
		{ SETTINGS(m_ui.weaponsRandomAppearance) },
		{ SETTINGS(m_ui.armorsRandomStats) },
		{ SETTINGS(m_ui.armorsRandomAppearance) },
		{ SETTINGS(m_ui.othersRandomStats) }
	};

	const QString _Weapons{ m_ui.weaponsBox->title() };

	helpConsole->addFeature(m_ui.weaponsRandomStatsAndElement, _Weapons,
		"Randomize weapons stats and element."
		"\n\nStats and element are generated based on the pattern of other weapons and are based on the weapon's total stats."
	);

	helpConsole->addFeature(m_ui.weaponsRandomAppearance, _Weapons,
		"Randomize the appearance of weapons."
		"\n\nEx: a dagger can look like an axe, a sword a spear..."
	);

	const QString _Armors{ m_ui.armorsBox->title() };

	helpConsole->addFeature(m_ui.armorsRandomStats, _Armors,
		"Randomize armors stats."
		"\n\nStats are generated based on the pattern of other armors and are based on the armor's total stats."
	);

	helpConsole->addFeature(m_ui.armorsRandomAppearance, _Armors,
		"Randomize the appearance of armors."
	);

	helpConsole->addFeature(m_ui.othersRandomStats, m_ui.othersBox->title(),
		"Randomize \"others\" stats."
		"\n\nStats are generated based on the pattern of other \"others\" and are based on the \"other\"'s total stats."
	);
}

void EquipmentWidget::enableUI(Randomizer* randomizer)
{
	setEnabled(true);
}

void EquipmentWidget::disableUI()
{
	setDisabled(true);
}

const char* EquipmentWidget::name() const
{
	return "Equipment";
}

void EquipmentWidget::loadPresets(const Json::Read& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}
}

void EquipmentWidget::savePresets(Json::Write* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}
}

const Ui::EquipmentWidget& EquipmentWidget::Ui() const
{
	return m_ui;
}