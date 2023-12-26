#pragma once

#include "ui_MainWindow.h"

#include "Backend/Game.hpp"
#include "FrontendQt/GuiSettings.hpp"

#include <QMainWindow>

#include <array>
#include <filesystem>
#include <memory>

class RandomizerTabWidget;
class TopInfoWidget;
class QAction;
class QActionGroup;
class QDragEnterEvent;
class QDropEvent;

class MainWindow final : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);

	void enableUI(std::filesystem::path* isoPath);
	void disableUI();
	void saveSettings();
public Q_SLOTS:
	void loadRandomizerSettings();
	void saveRandomizerSettings();
	void onFileOpen();
	void onFileSaveAs();
	void onThemeChanged();
	void onHelpAbout();
protected:
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;
private:
	Ui::MainWindow m_ui;

	TopInfoWidget* m_topInfoWidget;
	RandomizerTabWidget* m_randomizerTabWidget;
	QActionGroup* m_themeActionsGroup;
	std::array<QAction*, static_cast<std::size_t>(Theme::Count)> m_themeActions;

	std::shared_ptr<Game> m_game;
	GuiSettings m_guiSettings;
};