#include "EquipmentWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/GuiSettings.hpp"

EquipmentWidget::EquipmentWidget(QWidget* parent)
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

	GuiSettings::setToolTip(m_ui.weaponsRandomStatsAndElement,
		"Randomize weapons stats and element."
		"\n\n*Stats and element are generated based on the pattern of other weapons and are based on the weapon's total stats."
	);

	GuiSettings::setToolTip(m_ui.weaponsRandomAppearance, "Randomize the appearance of weapons.\n\nEx: a dagger can look like an axe, a sword a spear...");
	GuiSettings::setToolTip(m_ui.armorsRandomStats,
		"Randomize armors stats."
		"\n\n*Stats are generated based on the pattern of other armors and are based on the armor's total stats."
	);

	GuiSettings::setToolTip(m_ui.armorsRandomAppearance, "Randomize the appearance of armors.");
	GuiSettings::setToolTip(m_ui.othersRandomStats,
		"Randomize \"others\" stats."
		"\n\n*Stats are generated based on the pattern of other \"others\" and are based on the \"other\"'s total stats."
	);
}

void EquipmentWidget::enableUI(Game* game, std::shared_ptr<SharedData> sharedData)
{
	m_equipment = std::make_unique<Equipment>(game, sharedData);
	setEnabled(true);
}

void EquipmentWidget::disableUI()
{
	setDisabled(true);
	if (m_equipment)
	{
		m_equipment.reset();
	}
}

void EquipmentWidget::write() const
{
	if (!m_equipment)
	{
		throw JcrException{ "Game is uninitialized" };
	}

	Equipment::Weapons_t weapons{};
	if (m_ui.weaponsRandomStatsAndElement->isChecked())
	{
		weapons |= Equipment::WEAPONS_RANDOM_STATS_AND_ELEMENT;
	}
	if (m_ui.weaponsRandomAppearance->isChecked())
	{
		weapons |= Equipment::WEAPONS_RANDOM_APPEARANCE;
	}
	if (weapons)
	{
		m_equipment->setWeapons(weapons);
	}

	Equipment::Armors_t armors{};
	if (m_ui.armorsRandomStats->isChecked())
	{
		armors |= Equipment::ARMORS_RANDOM_STATS;
	}
	if (m_ui.armorsRandomAppearance->isChecked())
	{
		armors |= Equipment::ARMORS_RANDOM_APPEARANCE;
	}
	if (armors)
	{
		m_equipment->setArmors(armors);
	}

	if (m_ui.othersRandomStats->isChecked())
	{
		m_equipment->setOthers();
	}
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