#include "ShopWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/HelpConsoleWidget.hpp"

ShopWidget::ShopWidget(HelpConsoleWidget* helpConsole, QWidget* parent)
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

	helpConsole->addFeature(m_ui.weaponRandom, m_ui.weaponBox->title(), 
		"Randomize shops weapons."
		"\n\nShops are randomized by chapter pool."
	);

	helpConsole->addFeature(m_ui.armorRandom, m_ui.armorBox->title(), 
		"Randomize shops armors."
		"\n\nShops are randomized by chapter pool."
	);

	helpConsole->addFeature(m_ui.otherRandom, m_ui.otherBox->title(), 
		"Randomize shops others."
		"\n\nShops are randomized by chapter pool."
	);

	helpConsole->addFeature(m_ui.itemRandom, m_ui.itemBox->title(), 
		"Randomize shops items."
		"\n\nBan items: Great Walnut, Skeleton Key, Oils, Silks."
		"\nShops are randomized by chapter pool."
	);

	helpConsole->addFeature(m_ui.eternalCorridorUnlockAllEnable, m_ui.eternalCorridorUnlockAllBox->title(), 
		"Unlock all in eternal corridor shops."
		"\n\nBan items: Great Walnut, Skeleton Key, Oils, Silks."
	);
}

void ShopWidget::enableUI(Game* game, std::shared_ptr<SharedData> sharedData)
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