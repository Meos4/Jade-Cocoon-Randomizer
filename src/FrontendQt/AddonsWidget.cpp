#include "AddonsWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/HelpConsoleWidget.hpp"

AddonsWidget::AddonsWidget(HelpConsoleWidget* helpConsole, QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.nuzlockeOneCapturePerArea) },
		{ SETTINGS(m_ui.nuzlockeDefinitiveMinionDeath) },
		{ SETTINGS(m_ui.nuzlockeDefinitiveLevantDeath) },
		{ SETTINGS(m_ui.difficultyHard) },
		{ SETTINGS(m_ui.difficultyExtreme) },
		{ SETTINGS(m_ui.skipTutorialEnable) },
		{ SETTINGS(m_ui.skipTutorialSkipKoris) },
		{ SETTINGS(m_ui.x2FramerateEnable) },
		{ SETTINGS(m_ui.showHiddenStatsEnable) },
		{ SETTINGS(m_ui.palToNtscEnable) }
	};
	
	m_qSlider =
	{
		{ SETTINGS(m_ui.itemQuantityLimitSlider) },
		{ SETTINGS(m_ui.eternalCorridorLevelCapSlider) }
	};

	const QString _Nuzlocke{ m_ui.nuzlockeBox->title() };

	helpConsole->addFeature(m_ui.nuzlockeOneCapturePerArea, _Nuzlocke, 
		"Allows the capture of only one minion from the first encounter per area."
		"\n\nAreas:"
		"\nKoris Minion, Beetle Forest, Dragonfly Forest, Spider Forest, Moth Forest"
		"\nNetherworld Beetle Forest, Netherworld Dragonfly Forest, Netherworld Spider Forest, Eternal Corridor"
	);

	helpConsole->addFeature(m_ui.nuzlockeDefinitiveMinionDeath, _Nuzlocke, 
		"The death of one of your minions is definitive and will be automatically released."
	);

	helpConsole->addFeature(m_ui.nuzlockeDefinitiveLevantDeath, _Nuzlocke, 
		"When Levant dies, the game will automatically restart."
	);

	const QString _Difficulty{ m_ui.difficultyBox->title() };

	helpConsole->addFeature(m_ui.difficultyHard, _Difficulty, 
		"Increase boss stats by: "
		"Attack - Defense - Magic Attack - Magic Defense +33.33% | Speed - Critical +20%"
		"\n\nDoes not apply to Eternal Corridor bosses."
	);

	helpConsole->addFeature(m_ui.difficultyExtreme, _Difficulty, 
		"Increase boss stats by: "
		"Attack - Defense - Magic Attack - Magic Defense +50% | Speed - Critical +33.33%"
		"\n\nDoes not apply to Eternal Corridor bosses."
	);

	const QString _SkipTutorial{ m_ui.skipTutorialBox->title() };

	helpConsole->addFeature(m_ui.skipTutorialEnable, _SkipTutorial,
		"Skip the new game tutorial."
	);

	helpConsole->addFeature(m_ui.skipTutorialSkipKoris, _SkipTutorial,
		"Skip Koris tutorial in the Beetle Forest."
	);

	helpConsole->addFeature(m_ui.x2FramerateEnable, m_ui.x2FramerateBox->title(),
		"By pressing Select you can switch to a x2 framerate speed, if press again, switch back to x1."
		"\n\nIt is recommended to use this feature using emulator that supports CPU overclocking, otherwise "
		"if there are too many 3D models on the screen, the game will not fully support the x2."
	);

	helpConsole->addFeature(m_ui.showHiddenStatsEnable, m_ui.showHiddenStatsBox->title(),
		"Show Critical and Body Enhancement of minions in the stats frame."
		"\n\nBipedal = +10 Special Attack Damage."
		"\nWinged = +10 Magic Damage + Dodge \"All Enemies\" Special Attack."
	);

	const QString _Slider{ "Slider" };

	helpConsole->addFeature(m_ui.itemQuantityLimitSlider, m_ui.itemQuantityLimitBox->title(), _Slider,
		"Changes item quantity limit."
		"\n\nDefault = 20"
	);

	helpConsole->addFeature(m_ui.eternalCorridorLevelCapSlider, m_ui.eternalCorridorLevelCapBox->title(), _Slider,
		"Change the level cap of the Eternal Corridor."
		"\n\nDefault = 26"
	);

	helpConsole->addFeature(m_ui.palToNtscEnable, m_ui.palToNtscBox->title(),
		"Turn pal versions in ntsc."
		"\n\nDoes not work on some emulators."
	);

	m_ui.itemQuantityLimitValue->setStyleSheet("font-weight: normal;");
	m_ui.eternalCorridorLevelCapValue->setStyleSheet("font-weight: normal;");

	m_ui.skipTutorialSkipKoris->setEnabled(false);

	connect(m_ui.difficultyHard, &QAbstractButton::toggled, this, &AddonsWidget::updateDifficultyHard);
	connect(m_ui.difficultyExtreme, &QAbstractButton::toggled, this, &AddonsWidget::updateDifficultyExtreme);
	connect(m_ui.skipTutorialEnable, &QAbstractButton::toggled, m_ui.skipTutorialSkipKoris, &QWidget::setEnabled);
	connect(m_ui.itemQuantityLimitSlider, &QAbstractSlider::valueChanged, this, &AddonsWidget::setItemQuantityText);
	connect(m_ui.eternalCorridorLevelCapSlider, &QAbstractSlider::valueChanged, this, &AddonsWidget::setEternalCorridorLevelCapText);
}

void AddonsWidget::enableUI(Randomizer* randomizer)
{
	m_ui.palToNtscBox->setEnabled(!randomizer->game().isNtsc());
	setEnabled(true);
}

void AddonsWidget::disableUI()
{
	setDisabled(true);
}

const char* AddonsWidget::name() const
{
	return "Addons";
}

void AddonsWidget::loadPresets(const Json::Read& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}

	for (auto& slider : m_qSlider)
	{
		slider.load(json);
	}
}

void AddonsWidget::savePresets(Json::Write* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}

	for (const auto& slider : m_qSlider)
	{
		slider.save(json);
	}
}

const Ui::AddonsWidget& AddonsWidget::Ui() const
{
	return m_ui;
}

void AddonsWidget::updateDifficultyHard()
{
	const bool isChecked{ m_ui.difficultyHard->isChecked() };
	if (isChecked)
	{
		m_ui.difficultyExtreme->setChecked(false);
	}
	m_ui.difficultyExtreme->setEnabled(!isChecked);
}

void AddonsWidget::updateDifficultyExtreme()
{
	const bool isChecked{ m_ui.difficultyExtreme->isChecked() };
	if (isChecked)
	{
		m_ui.difficultyHard->setChecked(false);
	}
	m_ui.difficultyHard->setEnabled(!isChecked);
}

void AddonsWidget::setItemQuantityText(s32 value)
{
	m_ui.itemQuantityLimitValue->setText(QString::number(value));
}

void AddonsWidget::setEternalCorridorLevelCapText(s32 value)
{
	m_ui.eternalCorridorLevelCapValue->setText(QString::number(value));
}