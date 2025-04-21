#include "RandomizerTabWidget.hpp"

#include "Backend/Game.hpp"
#include "Common/JcrException.hpp"
#include "Common/Json.hpp"
#include "FrontendQt/AddonsWidget.hpp"
#include "FrontendQt/BossWidget.hpp"
#include "FrontendQt/EquipmentWidget.hpp"
#include "FrontendQt/FixesWidget.hpp"
#include "FrontendQt/ForestWidget.hpp"
#include "FrontendQt/HelpConsoleWidget.hpp"
#include "FrontendQt/LevantWidget.hpp"
#include "FrontendQt/MinionWidget.hpp"
#include "FrontendQt/MiscWidget.hpp"
#include "FrontendQt/QtUtil.hpp"
#include "FrontendQt/RandomizerWidget.hpp"
#include "FrontendQt/ShopWidget.hpp"
#include "FrontendQt/TreasureWidget.hpp"

#include <QMessageBox>

#include <format>

RandomizerTabWidget::RandomizerTabWidget(HelpConsoleWidget* helpConsole, QWidget* parent)
	: QTabWidget(parent)
{
	m_ui.setupUi(this);

	auto* const levantWidget{ new LevantWidget(helpConsole, this) };
	auto* const minionWidget{ new MinionWidget(helpConsole, this) };
	auto* const bossWidget{ new BossWidget(helpConsole, this) };
	auto* const forestWidget{ new ForestWidget(helpConsole, this) };
	auto* const equipmentWidget{ new EquipmentWidget(helpConsole, this) };
	auto* const treasureWidget{ new TreasureWidget(helpConsole, this) };
	auto* const shopWidget{ new ShopWidget(helpConsole, this) };
	auto* const miscWidget{ new MiscWidget(helpConsole, this) };
	auto* const addonsWidget{ new AddonsWidget(helpConsole, this) };
	auto* const fixesWidget{ new FixesWidget(helpConsole, this) };

	m_randomizerUiManager = std::make_unique<RandomizerUiManager>
	(
		levantWidget, minionWidget, bossWidget, forestWidget,
		equipmentWidget, treasureWidget, shopWidget, miscWidget,
		addonsWidget, fixesWidget
	);

	m_randomizerWidgets[TAB_LEVANT] = levantWidget;
	m_randomizerWidgets[TAB_MINION] = minionWidget;
	m_randomizerWidgets[TAB_BOSS] = bossWidget;
	m_randomizerWidgets[TAB_FOREST] = forestWidget;
	m_randomizerWidgets[TAB_EQUIPMENT] = equipmentWidget;
	m_randomizerWidgets[TAB_TREASURE] = treasureWidget;
	m_randomizerWidgets[TAB_SHOP] = shopWidget;
	m_randomizerWidgets[TAB_MISC] = miscWidget;
	m_randomizerWidgets[TAB_ADDONS] = addonsWidget;
	m_randomizerWidgets[TAB_FIXES] = fixesWidget;

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
	m_randomizer = std::make_unique<Randomizer>(game);

	for (const auto widget : m_randomizerWidgets)
	{
		widget->enableUI(m_randomizer.get());
	}
}

void RandomizerTabWidget::disableUI()
{
	for (const auto widget : m_randomizerWidgets)
	{
		widget->disableUI();
	}
	
	m_randomizer.reset();
}

void RandomizerTabWidget::write() const
{
	if (!m_randomizer)
	{
		throw JcrException{ "Randomizer is uninitialized" };
	}

	m_randomizerUiManager->write(m_randomizer.get());
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
				const auto it{ jsonValue.find(widget->name()) };
				if (it != jsonValue.end())
				{
					widget->loadPresets(it.value());
				}
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