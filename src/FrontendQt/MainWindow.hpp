#pragma once

#include "ui_MainWindow.h"

#include "Backend/Game.hpp"

#include <QMainWindow>

#include <filesystem>
#include <memory>

class RandomizerTabWidget;
class TopInfoWidget;
class QDragEnterEvent;
class QDropEvent;

class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

    void enableUI(std::filesystem::path* isoPath);
    void disableUI();
public Q_SLOTS:
    void loadRandomizerSettings();
    void saveRandomizerSettings();
    void onFileOpen();
    void onFileSaveAs();
    void onHelpAbout();
protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
private:
    Ui::MainWindow m_ui;

    TopInfoWidget* m_topInfoWidget;
    RandomizerTabWidget* m_randomizerTabWidget;

    std::shared_ptr<Game> m_game;
};
