#pragma once

#include "ui_RandomizerTabWidget.h"

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"

#include <QTabWidget>

#include <array>
#include <filesystem>
#include <memory>

class RandomizerWidget;

class RandomizerTabWidget final : public QTabWidget
{
	Q_OBJECT
public:
	enum
	{
		TAB_LEVANT,
		TAB_MINION,
		TAB_BOSS,
		TAB_FOREST,
		TAB_EQUIPMENT,
		TAB_TREASURE,
		TAB_SHOP,
		TAB_MISC,
		TAB_ADDONS,
		TAB_FIXES,
		TAB_COUNT
	};

	RandomizerTabWidget(QWidget* parent = nullptr);

	void enableUI(Game* game);
	void disableUI();
	void write() const;
	void loadPresets(const std::filesystem::path& path);
	void savePresets(const std::filesystem::path& path) const;
private:
	Ui::RandomizerTabWidget m_ui;

	std::array<RandomizerWidget*, TAB_COUNT> m_randomizerWidgets;
	Game* m_game;
	std::shared_ptr<SharedData> m_sharedData;
};