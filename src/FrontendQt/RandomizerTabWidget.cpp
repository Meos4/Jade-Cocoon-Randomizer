#include "RandomizerTabWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/LevantWidget.hpp"
#include "FrontendQt/MinionWidget.hpp"
#include "FrontendQt/BossWidget.hpp"
#include "FrontendQt/EquipmentWidget.hpp"
#include "FrontendQt/ForestWidget.hpp"
#include "FrontendQt/TreasureWidget.hpp"
#include "FrontendQt/ShopWidget.hpp"
#include "FrontendQt/MiscWidget.hpp"
#include "FrontendQt/AddonsWidget.hpp"
#include "FrontendQt/FixesWidget.hpp"
#include "FrontendQt/QtUtil.hpp"

#include "Common/Json.hpp"

#include <QMessageBox>

#include <format>

RandomizerTabWidget::RandomizerTabWidget(QWidget* parent)
	: QTabWidget(parent)
{
	m_ui.setupUi(this);

	m_randomizerWidgets[TAB_LEVANT] = new LevantWidget(this);
	m_randomizerWidgets[TAB_MINION] = new MinionWidget(this);
	m_randomizerWidgets[TAB_BOSS] = new BossWidget(this);
	m_randomizerWidgets[TAB_FOREST] = new ForestWidget(this);
	m_randomizerWidgets[TAB_EQUIPMENT] = new EquipmentWidget(this);
	m_randomizerWidgets[TAB_TREASURE] = new TreasureWidget(this);
	m_randomizerWidgets[TAB_SHOP] = new ShopWidget(this);
	m_randomizerWidgets[TAB_MISC] = new MiscWidget(this);
	m_randomizerWidgets[TAB_ADDONS] = new AddonsWidget(this);
	m_randomizerWidgets[TAB_FIXES] = new FixesWidget(this);

	m_ui.levantScroll->setWidget(m_randomizerWidgets[TAB_LEVANT]);
	m_ui.minionScroll->setWidget(m_randomizerWidgets[TAB_MINION]);
	m_ui.bossScroll->setWidget(m_randomizerWidgets[TAB_BOSS]);
	m_ui.forestScroll->setWidget(m_randomizerWidgets[TAB_FOREST]);
	m_ui.equipmentScroll->setWidget(m_randomizerWidgets[TAB_EQUIPMENT]);
	m_ui.treasureScroll->setWidget(m_randomizerWidgets[TAB_TREASURE]);
	m_ui.shopScroll->setWidget(m_randomizerWidgets[TAB_SHOP]);
	m_ui.miscScroll->setWidget(m_randomizerWidgets[TAB_MISC]);
	m_ui.addonsScroll->setWidget(m_randomizerWidgets[TAB_ADDONS]);
	m_ui.fixesScroll->setWidget(m_randomizerWidgets[TAB_FIXES]);
}

void RandomizerTabWidget::enableUI(Game* game)
{
	m_game = game;
	m_sharedData = std::make_shared<SharedData>(game);
	for (const auto widget : m_randomizerWidgets)
	{
		widget->enableUI(game, m_sharedData);
	}
}

void RandomizerTabWidget::disableUI()
{
	for (const auto widget : m_randomizerWidgets)
	{
		widget->disableUI();
	}
	m_game = nullptr;
	m_sharedData.reset();
}

void RandomizerTabWidget::write() const
{
	if (!m_game || !m_sharedData)
	{
		throw JcrException{ "Game is uninitialized" };
	}

	m_sharedData->read();
	m_game->expandExecutable();
	for (const auto widget : m_randomizerWidgets)
	{
		widget->write();
	}
}

void RandomizerTabWidget::loadPresets(const std::filesystem::path& path)
{
	try
	{
		const auto json{ Json::read(path) };

		if (json.has_value())
		{
			const auto jsonValue{ json.value() };
			for (const auto widget : m_randomizerWidgets)
			{
				widget->loadPresets(jsonValue[widget->name()]);
			}
		}
	}
	catch (const Json::Exception& e)
	{
		QMessageBox::critical(this, "Error", QtUtil::jsonErrorMessage(path, e));
	}
}

void RandomizerTabWidget::savePresets(const std::filesystem::path& path) const
{
	Json::Write json;

	for (const auto widget : m_randomizerWidgets)
	{
		widget->savePresets(&json[widget->name()]);
	}

	Json::overwrite(json, path);
}