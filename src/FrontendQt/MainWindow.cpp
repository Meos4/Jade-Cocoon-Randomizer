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

#include "JCTools.hpp"
#include "dumpsxiso/dumpsxiso.h"
#include "mkpsxiso/mkpsxiso.h"

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

void MainWindow::enableUI(std::filesystem::path* isoPath)
{
	ExtractGameDialog extractGameDialog(this);

	auto extractGame = [&]()
	{
		try
		{
			emit extractGameDialog.onStateChanged("Extracting game, please wait");

			if (isoPath->extension() == ".cue")
			{
				isoPath->replace_extension(".bin");
			}

			if (!std::filesystem::is_regular_file(*isoPath))
			{
				if (std::filesystem::is_directory(*isoPath))
				{
					#ifdef _WIN32 
						throw QString{ QString::fromStdWString(std::format(L"\"{}\" is a directory", isoPath->filename().wstring())) };
					#else
						throw JcrException{ "\"{}\" is a directory", isoPath->filename().string() };
					#endif
				}
				else
				{
					#ifdef _WIN32 
						throw QString{ QString::fromStdWString(std::format(L"\"{}\" file does not exist", isoPath->filename().wstring())) };
					#else
						throw JcrException{ "\"{}\" file does not exist", isoPath->filename().string() };
					#endif
				}
			}

			if (!Game::isAJCIso(*isoPath))
			{
				#ifdef _WIN32 
					throw QString{ QString::fromStdWString(std::format(L"\"{}\" is not a Jade Cocoon binary file", isoPath->filename().wstring())) };
				#else
					throw JcrException{ "\"{}\" is not a Jade Cocoon binary file", isoPath->filename().string() };
				#endif
			}

			if (std::filesystem::is_directory(Path::jcrTempDirectory))
			{
				std::error_code err;
				const auto nbRemoved{ std::filesystem::remove_all(Path::jcrTempDirectory, err) };
				if (nbRemoved == -1)
				{
					throw JcrException{ "\"{}\" directory cannot be removed", Path::jcrTempDirectory };
				}
			}

			std::filesystem::create_directory(Path::jcrTempDirectory);

			const std::filesystem::path
				configPath{ std::format("{}/{}", Path::jcrTempDirectory, Path::configXmlFilename) },
				filesPath{ std::format("{}/{}", Path::jcrTempDirectory, Path::filesDirectory) };

			const auto dumpArgs{ Path::dumpIsoArgs(isoPath, &configPath, &filesPath) };

			dumpsxiso(static_cast<int>(dumpArgs.size()), (Path::CStringPlatformPtr)dumpArgs.data());
			JCTools::unpacker(filesPath, std::format("{}/{}", filesPath.string(), Path::dataDirectory), Path::jcrTempDirectory);

			const auto exeInfo{ JCExe::findFilenamePathAndVersion(filesPath) };

			if (!exeInfo.has_value())
			{
				throw JcrException{ "Can't find compatible playstation executable in \"{}\"", filesPath.string() };
			}
			else if (exeInfo.value().version == JCExe::Version::Prototype_D05_M08_Y1999_15H48)
			{
				throw JcrException{ "This version is not supported" };
			}

			m_game = std::make_shared<Game>(*isoPath, exeInfo.value().path, Utility::jcExeToGameVersion(exeInfo.value().version));

			emit extractGameDialog.shouldClose();

			return true;
		}
		catch (const std::exception& e)
		{
			emit extractGameDialog.onStateError(QString::fromStdString(std::format("An error occured, Reason: {}", e.what())));
		}
		catch (const QString& e)
		{
			const QString error
			{
				#ifdef _WIN32 
					QString::fromStdWString(std::format(L"An error occured, Reason: {}", QtUtility::qStrToPlatformStr(e)))
				#else
					QString::fromStdString(std::format("An error occured, Reason: {}", QtUtility::qStrToPlatformStr(e)))
				#endif
			};
			emit extractGameDialog.onStateError(error);
		}

		emit extractGameDialog.taskCompleted();
		emit extractGameDialog.onOkButtonVisibilityChanged(true);
		return false;
	};

	std::future<bool> future{ std::async(std::launch::async, extractGame) };
	extractGameDialog.exec();

	future.wait();
	if (!future.get())
	{
		disableUI();
	}
	else
	{
		try
		{
			m_randomizerTabWidget->enableUI(m_game);
			const QString verSerial{ QString::fromStdString(std::format("{} [{}]", m_game->versionText(), m_game->serialText())) };
			const QString filename
			{
				#ifdef _WIN32 
					QString::fromStdWString(m_game->isoFilename().wstring())
				#else
					QString::fromStdString(m_game->isoFilename().string())
				#endif
			};

			const auto state
			{
				m_randomizerTabWidget->isVanilla() ?
				TopInfoWidget::State::Vanilla :
				TopInfoWidget::State::Randomized
			};

			m_topInfoWidget->enableUI(verSerial, filename, state);

			m_ui.actionFileClose->setEnabled(true);
			m_ui.actionFileSaveAs->setEnabled(true);
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(this, "Error", QString::fromStdString(std::format("An error occured, Reason: {}", e.what())));
			disableUI();
		}
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
	else if (const auto isoPath{ m_game->isoPath() };
		QtUtility::qStrToPlatformStr(filePathQStr) == isoPath && std::filesystem::is_regular_file(isoPath))
	{
		QMessageBox::critical(this, "Error", "You can't save on your vanilla iso.");
		return;
	}

	SaveGameDialog saveGameDialog(this);

	auto saveGame = [&]()
	{
		try
		{
			emit saveGameDialog.onStateChanged("Randomizing game...");

			m_randomizerTabWidget->write();

			emit saveGameDialog.progressBarChanged(33);
			emit saveGameDialog.onStateChanged("Repack game files...");

			const std::filesystem::path filesPath{ std::format("{}/{}", Path::jcrTempDirectory, Path::filesDirectory) };
			JCTools::repacker(filesPath, Path::jcrTempDirectory, filesPath, std::format("{}/{}", filesPath.string(), Path::dataDirectory));

			emit saveGameDialog.progressBarChanged(66);
			emit saveGameDialog.onStateChanged("Repack iso...");

			const std::filesystem::path 
				configPath{ std::format("{}/{}", Path::jcrTempDirectory, Path::configXmlFilename) },
				filePath{ QtUtility::qStrToPlatformStr(filePathQStr) };

			const auto makeArgs{ Path::makeIsoArgs(&filePath, &configPath) };
			if (mkpsxiso(static_cast<int>(makeArgs.size()), (Path::CStringPlatformPtr)makeArgs.data()) == EXIT_FAILURE)
			{
				throw JcrException{ "Unable to repack iso" };
			}

			emit saveGameDialog.progressBarChanged(100);
			emit saveGameDialog.onStateChanged("Done");
			emit saveGameDialog.taskCompleted();
			return true;
		}
		catch (const std::exception& e)
		{
			emit saveGameDialog.onStateError(QString::fromStdString(std::format("An error occured, Reason: {}", e.what())));
			emit saveGameDialog.taskCompleted();
			return false;
		}
	};
	
	const bool isSeedEnabled{ m_topInfoWidget->isSeedEnabled() };
	const auto firstSeed
	{
		isSeedEnabled ? m_topInfoWidget->seed() : Random::get().generate(Random::limit)
	};

	Random::get().setSeed(firstSeed);

	std::future<bool> future{ std::async(std::launch::async, saveGame) };
	saveGameDialog.exec();

	future.wait();
	if (!future.get())
	{
		disableUI();
	}
	else
	{
		const std::filesystem::path filePath{ QtUtility::qStrToPlatformStr(filePathQStr) };
		m_game->setIsoPath(filePath);
		
		const QString filename
		{
			#ifdef _WIN32
				QString::fromStdWString(m_game->isoFilename().wstring())
			#else
				QString::fromStdString(m_game->isoFilename().string())
			#endif
		};

		m_topInfoWidget->setFilename(filename);
		m_topInfoWidget->setState(TopInfoWidget::State::Randomized);
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