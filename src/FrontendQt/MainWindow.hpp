#pragma once

#include "ui_MainWindow.h"

#include "Backend/Game.hpp"
#include "FrontendQt/GuiSettings.hpp"

#include <QMainWindow>

#include <array>
#include <filesystem>
#include <memory>

class ExtractGameDialog;
class RandomizerTabWidget;
class SaveGameDialog;
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

	bool createGameFromDirectory(std::filesystem::path&& gameDirectory);
	std::shared_ptr<Game> extractGame(std::filesystem::path* isoPath, ExtractGameDialog* extractGameDialog);
	bool saveGame(const QString& filePath, SaveGameDialog* saveGameDialog);
	void enableUI(std::filesystem::path* isoPath);
	void enableUI(std::shared_ptr<Game> game);
	void disableUI();
	void saveSettings();
public Q_SLOTS:
	void loadPresets();
	void savePresets();
	void onFileOpen();
	void onFileSaveAs();
	void onThemeChanged();
	void onHelpAbout();
private:
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

	Ui::MainWindow m_ui;

	TopInfoWidget* m_topInfoWidget;
	RandomizerTabWidget* m_randomizerTabWidget;
	QActionGroup* m_themeActionsGroup;
	std::array<QAction*, static_cast<std::size_t>(Theme::Count)> m_themeActions;

	std::shared_ptr<Game> m_game;
	GuiSettings m_guiSettings;
};