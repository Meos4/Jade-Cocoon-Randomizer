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

#include "nlohmann/json.hpp"

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

void RandomizerTabWidget::enableUI(std::shared_ptr<Game> game)
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
	m_game.reset();
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

bool RandomizerTabWidget::isVanilla() const
{
	if (!m_game)
	{
		throw JcrException{ "Game is uninitialized" };
	}
	return m_game->isVanilla();
}

void RandomizerTabWidget::loadPresets(const std::filesystem::path& path)
{
	try
	{
		std::ifstream jsonFile(path);
		nlohmann::json json;
		jsonFile >> json;

		for (const auto widget : m_randomizerWidgets)
		{
			widget->loadPresets(json[widget->name()]);
		}
	}
	catch (nlohmann::json::exception& e)
	{
		QString errorMessage
		{
			#ifdef _WIN32
				QString::fromStdWString(std::format(L"\"{}\" is not a valid json file, ", path.wstring()))
			#else
				QString::fromStdString(std::format("\"{}\" is not a valid json file, ", path.string()))
			#endif
		};
		errorMessage += QString::fromStdString(std::format("Reason:\n{}", e.what()));
		QMessageBox::critical(this, "Error", errorMessage);
	}
}

void RandomizerTabWidget::savePresets(const std::filesystem::path& path) const
{
	nlohmann::ordered_json json;

	for (const auto widget : m_randomizerWidgets)
	{
		widget->savePresets(&json[widget->name()]);
	}

	std::ofstream jsonFile(path);
	jsonFile << std::setw(4) << json;
}