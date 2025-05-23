#include "MainWindow.hpp"

#include "Backend/Iso.hpp"
#include "Backend/Path.hpp"
#include "Backend/Util.hpp"
#include "Common/JcrException.hpp"
#include "Common/Json.hpp"
#include "FrontendQt/AboutDialog.hpp"
#include "FrontendQt/ExtractGameDialog.hpp"
#include "FrontendQt/GuiPath.hpp"
#include "FrontendQt/HelpConsoleWidget.hpp"
#include "FrontendQt/RandomizerTabWidget.hpp"
#include "FrontendQt/SaveGameDialog.hpp"
#include "FrontendQt/TopInfoWidget.hpp"
#include "FrontendQt/QtUtil.hpp"

#include <QActionGroup>
#include <QDesktopServices>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>

#include <format>
#include <future>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	m_topInfoWidget = new TopInfoWidget(this);
	m_ui.mainLayout->addWidget(m_topInfoWidget);

	m_helpConsoleWidget = new HelpConsoleWidget(this);
	m_randomizerTabWidget = new RandomizerTabWidget(m_helpConsoleWidget, this);
	m_ui.mainLayout->addWidget(m_randomizerTabWidget);
	m_ui.mainLayout->addWidget(m_helpConsoleWidget);

	m_themeActionsGroup = new QActionGroup(this);
	m_themeActionsGroup->addAction(m_ui.actionSettingsThemeDark);
	m_themeActionsGroup->addAction(m_ui.actionSettingsThemeLight);

	m_themeActions = 
	{
		m_ui.actionSettingsThemeDark,
		m_ui.actionSettingsThemeLight
	};

	connect(m_topInfoWidget, &TopInfoWidget::buttonLoadPresetsClicked, this, &MainWindow::loadPresets);
	connect(m_topInfoWidget, &TopInfoWidget::buttonSavePresetsClicked, this, &MainWindow::savePresets);
	connect(m_ui.actionFileOpen, &QAction::triggered, this, &MainWindow::onFileOpen);
	connect(m_ui.actionFileClose, &QAction::triggered, this, &MainWindow::disableUI);
	connect(m_ui.actionFileSaveAs, &QAction::triggered, this, &MainWindow::onFileSaveAs);
	connect(m_ui.actionFileExit, &QAction::triggered, this, &QWidget::close);

	connect(m_ui.actionSettingsThemeDark, &QAction::toggled, this, &MainWindow::onThemeChanged);
	connect(m_ui.actionSettingsThemeLight, &QAction::toggled, this, &MainWindow::onThemeChanged);

	connect(m_ui.actionHelpGitHub, &QAction::triggered, this,
		[]() { QDesktopServices::openUrl(QUrl{ "https://github.com/Meos4/Jade-Cocoon-Randomizer" }); });

	connect(m_ui.actionHelpDiscord, &QAction::triggered, this,
		[]() { QDesktopServices::openUrl(QUrl{ "https://discord.com/invite/ErwKG2v" }); });

	connect(m_ui.actionHelpJadeCocoonWiki, &QAction::triggered, this,
		[]() { QDesktopServices::openUrl(QUrl{ "https://thejadecocoonproject.fandom.com/wiki/Jade_Cocoon_Wiki" }); });

	connect(m_ui.actionHelpChangelog, &QAction::triggered, this,
		[]() { QDesktopServices::openUrl(QUrl{ "https://github.com/Meos4/Jade-Cocoon-Randomizer/blob/main/CHANGELOG.md" }); });

	connect(m_ui.actionHelpAbout, &QAction::triggered, this, &MainWindow::onHelpAbout);

	disableUI();

	m_guiSettings.setOsTheme();

	const std::filesystem::path guiSettingsPath{ GuiPath::jcrGuiSettingsFilename };
	if (std::filesystem::is_regular_file(guiSettingsPath))
	{
		try
		{
			const auto json{ Json::read(guiSettingsPath) };
			if (json.has_value())
			{
				m_guiSettings.loadSettings(json.value());
			}		
		}
		catch (const Json::Exception& e)
		{
			QMessageBox::critical(this, "Error", QtUtil::jsonErrorMessage(guiSettingsPath, e));
		}
	}

	m_themeActions[static_cast<std::size_t>(m_guiSettings.theme())]->setChecked(true);

	createGameFromDirectory(Path::defaultGameDirectory);
}

bool MainWindow::createGameFromDirectory(std::filesystem::path&& gameDirectory)
{
	auto game{ Game::createGame(std::move(gameDirectory)) };

	if (game)
	{
		enableUI(std::move(game));
		return true;
	}
	return false;
}

std::unique_ptr<Game> MainWindow::extractGame(std::filesystem::path* isoPath, ExtractGameDialog* extractGameDialog)
{
	try
	{
		emit extractGameDialog->onStateChanged("Extracting game, please wait");

		if (isoPath->extension() == ".cue")
		{
			isoPath->replace_extension(".bin");
		}

		auto game{ Game::createGame(*isoPath, Path::defaultGameDirectory) };

		emit extractGameDialog->shouldClose();

		return game;
	}
	catch (const std::exception& e)
	{
		emit extractGameDialog->onStateError(QString::fromStdString(std::format("An error occured, Reason: {}", e.what())));
		emit extractGameDialog->taskCompleted();
		emit extractGameDialog->onOkButtonVisibilityChanged(true);
		return nullptr;
	}
}

bool MainWindow::saveGame(const QString& filePath, SaveGameDialog* saveGameDialog)
{
	try
	{
		emit saveGameDialog->onStateChanged("Copying Files...");
		m_game->createBuilderDirectory();

		emit saveGameDialog->progressBarChanged(25);
		emit saveGameDialog->onStateChanged("Randomizing game...");	
		m_randomizerTabWidget->write();

		emit saveGameDialog->progressBarChanged(50);
		emit saveGameDialog->onStateChanged("Repack game files...");
		m_game->builderTree().repackDATA001();

		emit saveGameDialog->progressBarChanged(75);
		emit saveGameDialog->onStateChanged("Repack iso...");

		const auto destPath{ std::filesystem::path{ QtUtil::qStrToPlatformStr(filePath) } };
		m_game->builderTree().createIso(&destPath);
		m_game->builderTree().remove();

		emit saveGameDialog->progressBarChanged(100);
		emit saveGameDialog->onStateChanged("Done");
		emit saveGameDialog->taskCompleted();
		return true;
	}
	catch (const std::exception& e)
	{
		m_game->builderTree().remove();
		emit saveGameDialog->onStateError(QString::fromStdString(std::format("An error occured, Reason: {}", e.what())));
		emit saveGameDialog->taskCompleted();
		return false;
	}
}

void MainWindow::enableUI(std::filesystem::path* isoPath)
{
	ExtractGameDialog extractGameDialog(this);
	auto future{ std::async(std::launch::async, &MainWindow::extractGame, this, isoPath, &extractGameDialog) };
	extractGameDialog.exec();

	future.wait();
	auto game{ future.get() };

	if (!game)
	{
		disableUI();
		return;
	}

	if (!game->isVanilla())
	{
		game->removeStaticDirectory();
		QMessageBox::critical(this, "Error", "This iso is already randomized, it is not allowed to re-randomize it,\nuse a vanilla iso instead.");
		disableUI();
		return;
	}

	enableUI(std::move(game));
}

void MainWindow::enableUI(std::unique_ptr<Game> game)
{
	try
	{
		m_game = std::move(game);
		m_randomizerTabWidget->enableUI(m_game.get());
		const QString verSerial{ QString::fromStdString(std::format("{} [{}]", m_game->versionText(), m_game->serialText())) };
		m_topInfoWidget->enableUI(verSerial);
		m_ui.actionFileClose->setEnabled(true);
		m_ui.actionFileSaveAs->setEnabled(true);
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(this, "Error", QString::fromStdString(std::format("An error occured, Reason: {}", e.what())));
		disableUI();
	}
}

void MainWindow::disableUI()
{
	m_ui.actionFileClose->setEnabled(false);
	m_ui.actionFileSaveAs->setEnabled(false);

	m_topInfoWidget->disableUI();
	m_randomizerTabWidget->disableUI();

	if (m_game)
	{
		m_game->removeStaticDirectory();
		m_game.reset();
	}
}

void MainWindow::saveSettings()
{
	Json::Write json;
	m_guiSettings.saveSettings(&json);
	Json::overwrite(json, GuiPath::jcrGuiSettingsFilename);
}

void MainWindow::loadPresets()
{
	const auto presetsPathQStr{ QFileDialog::getOpenFileName(this, "Open Presets File", QString{}, "*.json", nullptr) };
	if (!presetsPathQStr.isEmpty())
	{
		const std::filesystem::path presetsPath{ QtUtil::qStrToPlatformStr(presetsPathQStr) };
		m_randomizerTabWidget->loadPresets(presetsPath);
	}	
}

void MainWindow::savePresets()
{
	const auto presetsPathQStr{ QFileDialog::getSaveFileName(this, "Save Presets File", QString{}, "*.json", nullptr)};
	if (!presetsPathQStr.isEmpty())
	{
		const std::filesystem::path presetsPath{ QtUtil::qStrToPlatformStr(presetsPathQStr) };
		m_randomizerTabWidget->savePresets(presetsPath);
	}
}

void MainWindow::onFileOpen()
{
	const auto filePathQStr{ QFileDialog::getOpenFileName(this, "Open Jade Cocoon Binary File", QString{}, "*.bin *.cue", nullptr)};
	if (!filePathQStr.isEmpty())
	{
		std::filesystem::path filePath{ QtUtil::qStrToPlatformStr(filePathQStr) };
		enableUI(&filePath);
	}
}

void MainWindow::onFileSaveAs()
{
	const QString filePathQStr{ QFileDialog::getSaveFileName(this, "Save Jade Cocoon Binary File", QString{}, "*.bin", nullptr)};
	if (filePathQStr.isEmpty())
	{
		return;
	}
	
	const bool isSeedEnabled{ m_topInfoWidget->isSeedEnabled() };
	const auto firstSeed
	{
		isSeedEnabled ? m_topInfoWidget->seed() : m_game->random()->generate(Random::limit)
	};

	m_game->random()->setSeed(firstSeed);

	SaveGameDialog saveGameDialog(&m_uiRandom, this);
	auto future{ std::async(std::launch::async, &MainWindow::saveGame, this, filePathQStr, &saveGameDialog) };
	saveGameDialog.exec();

	future.wait();
	if (future.get())
	{
		const std::filesystem::path filePath{ QtUtil::qStrToPlatformStr(filePathQStr) };

		if (!isSeedEnabled)
		{
			m_topInfoWidget->setSeed(firstSeed);
		}

		auto filePathCue{ filePath };
		filePathCue.replace_extension(".cue");

		if (!std::filesystem::is_regular_file(filePathCue) &&
			QMessageBox::question(this, "Save", "Generate the associated .cue file?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			Iso::generateCue(filePath);
		}
	}
}

void MainWindow::onThemeChanged()
{
	for (std::size_t i{}; i < m_themeActions.size(); ++i)
	{
		if (m_themeActions[i]->isChecked())
		{
			m_guiSettings.updateTheme(static_cast<Theme>(i));
			break;
		}
	}
}

void MainWindow::onHelpAbout()
{
	AboutDialog about(this);
	about.exec();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* event)
{
	const auto* mimeData{ event->mimeData() };

	if (mimeData->hasUrls())
	{
		std::filesystem::path path
		{ 
			mimeData->urls().front().toLocalFile().
			#ifdef _WIN32
				toStdWString() 
			#else
				toStdString()
			#endif
		};
		
		if (m_game && path.extension() == ".json")
		{
			m_randomizerTabWidget->loadPresets(path);
		}
		else
		{
			enableUI(&path);
		}
	}
}