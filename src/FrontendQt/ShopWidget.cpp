#include "ShopWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/GuiSettings.hpp"

ShopWidget::ShopWidget(QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.weaponRandom) },
		{ SETTINGS(m_ui.armorRandom) },
		{ SETTINGS(m_ui.otherRandom) },
		{ SETTINGS(m_ui.itemRandom) },
		{ SETTINGS(m_ui.eternalCorridorUnlockAllEnable) },
	};

	GuiSettings::setToolTip(m_ui.weaponRandom, "Randomize shops weapons.\n\n*Shops are randomized by chapter pool.");
	GuiSettings::setToolTip(m_ui.armorRandom, "Randomize shops armors.\n\n*Shops are randomized by chapter pool.");
	GuiSettings::setToolTip(m_ui.otherRandom, "Randomize shops others.\n\n*Shops are randomized by chapter pool.");
	GuiSettings::setToolTip(m_ui.itemRandom,
		"Randomize shops items."
		"\n\n*Ban items: Great Walnut, Skeleton Key, Oils, Silks."
		"\n\n*Shops are randomized by chapter pool."
	);

	GuiSettings::setToolTip(m_ui.eternalCorridorUnlockAllEnable,
		"Unlock all in eternal corridor shops."
		"\n\n*Ban items: Great Walnut, Skeleton Key, Oils, Silks."
	);
}

void ShopWidget::enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
{
	m_shop = std::make_unique<Shop>(game, sharedData);
	setEnabled(true);
}

void ShopWidget::disableUI()
{
	setDisabled(true);
	if (m_shop)
	{
		m_shop.reset();
	}
}

void ShopWidget::write() const
{
	if (!m_shop)
	{
		throw JcrException{ "Game is uninitialized" };
	}

	if (m_ui.weaponRandom->isChecked())
	{
		m_shop->setWeapon();
	}

	if (m_ui.armorRandom->isChecked())
	{
		m_shop->setArmor();
	}

	if (m_ui.otherRandom->isChecked())
	{
		m_shop->setOther();
	}

	if (m_ui.itemRandom->isChecked())
	{
		m_shop->setItem();
	}

	if (m_ui.eternalCorridorUnlockAllEnable->isChecked())
	{
		m_shop->setEternalCorridorUnlockAll();
	}
}

const char* ShopWidget::name() const
{
	return "Shop";
}

void ShopWidget::loadPresets(const Json::Read& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}
}

void ShopWidget::savePresets(Json::Write* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}
}