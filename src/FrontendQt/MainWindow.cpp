#include "MainWindow.hpp"

#include "Backend/Path.hpp"
#include "Backend/Random.hpp"
#include "Backend/Utility.hpp"
#include "Common/JcrException.hpp"
#include "FrontendQt/AboutDialog.hpp"
#include "FrontendQt/ExtractGameDialog.hpp"
#include "FrontendQt/GuiPath.hpp"
#include "FrontendQt/RandomizerTabWidget.hpp"
#include "FrontendQt/SaveGameDialog.hpp"
#include "FrontendQt/TopInfoWidget.hpp"
#include "FrontendQt/QtUtility.hpp"

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

	m_randomizerTabWidget = new RandomizerTabWidget(this);
	m_ui.mainLayout->addWidget(m_randomizerTabWidget);

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

	std::filesystem::path guiSettingsPath{ GuiPath::jcrGuiSettingsFilename };
	if (std::filesystem::is_regular_file(guiSettingsPath))
	{
		try
		{
			std::ifstream jsonFile(guiSettingsPath);
			nlohmann::json json;
			jsonFile >> json;
			m_guiSettings.loadSettings(json);
		}
		catch (nlohmann::json::exception& e)
		{
			QString errorMessage
			{
				#ifdef _WIN32
					QString::fromStdWString(std::format(L"\"{}\" is not a valid json file, ", guiSettingsPath.wstring()))
				#else
					QString::fromStdString(std::format("\"{}\" is not a valid json file, ", guiSettingsPath.string()))
				#endif
			};
			errorMessage += QString::fromStdString(std::format("Reason:\n{}", e.what()));
			QMessageBox::critical(this, "Error", errorMessage);
		}
	}

	m_themeActions[static_cast<std::size_t>(m_guiSettings.theme())]->setChecked(true);
}

std::shared_ptr<Game> MainWindow::extractGame(std::filesystem::path* isoPath, ExtractGameDialog* extractGameDialog)
{
	try
	{
		emit extractGameDialog->onStateChanged("Extracting game, please wait");

		if (isoPath->extension() == ".cue")
		{
			isoPath->replace_extension(".bin");
		}

		auto game{ std::make_shared<Game>(Game::createGame(*isoPath, Path::jcrTempDirectory)) };

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
		emit saveGameDialog->onStateChanged("Randomizing game...");

		m_randomizerTabWidget->write();

		emit saveGameDialog->progressBarChanged(33);
		emit saveGameDialog->onStateChanged("Repack game files...");

		m_game->repackFilesToDATA001();

		emit saveGameDialog->progressBarChanged(66);
		emit saveGameDialog->onStateChanged("Repack iso...");

		const auto destPath{ std::filesystem::path{ QtUtility::qStrToPlatformStr(filePath) } };
		m_game->createIsoFromFiles(&destPath);

		emit saveGameDialog->progressBarChanged(100);
		emit saveGameDialog->onStateChanged("Done");
		emit saveGameDialog->taskCompleted();
		return true;
	}
	catch (const std::exception& e)
	{
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
	const auto game{ future.get() };

	if (game)
	{
		enableUI(game);
	}
	else
	{
		disableUI();
	}
}

void MainWindow::enableUI(std::shared_ptr<Game> game)
{
	try
	{
		m_game = game;
		m_randomizerTabWidget->enableUI(m_game);
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
		m_game.reset();
	}
}

void MainWindow::saveSettings()
{
	nlohmann::ordered_json json;
	std::ofstream jsonFile(GuiPath::jcrGuiSettingsFilename);
	m_guiSettings.saveSettings(&json);
	jsonFile << std::setw(4) << json;
}

void MainWindow::loadPresets()
{
	const auto presetsPathQStr{ QFileDialog::getOpenFileName(this, "Open Presets File", QString{}, "*.json", nullptr) };
	if (!presetsPathQStr.isEmpty())
	{
		const std::filesystem::path presetsPath{ QtUtility::qStrToPlatformStr(presetsPathQStr) };
		m_randomizerTabWidget->loadPresets(presetsPath);
	}	
}

void MainWindow::savePresets()
{
	const auto presetsPathQStr{ QFileDialog::getSaveFileName(this, "Save Presets File", QString{}, "*.json", nullptr)};
	if (!presetsPathQStr.isEmpty())
	{
		const std::filesystem::path presetsPath{ QtUtility::qStrToPlatformStr(presetsPathQStr) };
		m_randomizerTabWidget->savePresets(presetsPath);
	}
}

void MainWindow::onFileOpen()
{
	const auto filePathQStr{ QFileDialog::getOpenFileName(this, "Open Jade Cocoon Binary File", QString{}, "*.bin *.cue", nullptr)};
	if (!filePathQStr.isEmpty())
	{
		std::filesystem::path filePath{ QtUtility::qStrToPlatformStr(filePathQStr) };
		enableUI(&filePath);
	}
}

void MainWindow::onFileSaveAs()
{
	try
	{
		if (!m_randomizerTabWidget->isVanilla())
		{
			QMessageBox::critical(this, "Error",
				"This iso is already randomized, it is not allowed to re-randomize it,\nuse a vanilla iso instead.");
			return;
		}
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(this, "Error", QString::fromStdString(std::format("An error occured, Reason: {}", e.what())));
		disableUI();
		return;
	}

	const QString filePathQStr{ QFileDialog::getSaveFileName(this, "Save Jade Cocoon Binary File", QString{}, "*.bin", nullptr)};
	if (filePathQStr.isEmpty())
	{
		return;
	}
	
	const bool isSeedEnabled{ m_topInfoWidget->isSeedEnabled() };
	const auto firstSeed
	{
		isSeedEnabled ? m_topInfoWidget->seed() : Random::get().generate(Random::limit)
	};

	Random::get().setSeed(firstSeed);

	SaveGameDialog saveGameDialog(this);
	auto future{ std::async(std::launch::async, &MainWindow::saveGame, this, filePathQStr, &saveGameDialog) };
	saveGameDialog.exec();

	future.wait();
	if (!future.get())
	{
		disableUI();
	}
	else
	{
		const std::filesystem::path filePath{ QtUtility::qStrToPlatformStr(filePathQStr) };

		if (!isSeedEnabled)
		{
			m_topInfoWidget->setSeed(firstSeed);
		}

		auto filePathCue{ filePath };
		filePathCue.replace_extension(".cue");

		if (!std::filesystem::is_regular_file(filePathCue) &&
			QMessageBox::question(this, "Save", "Generate the associated .cue file?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			Game::generateCue(filePath);
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