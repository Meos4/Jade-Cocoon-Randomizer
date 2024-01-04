#include "AddonsWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/GuiSettings.hpp"

AddonsWidget::AddonsWidget(QWidget* parent)
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

	GuiSettings::setToolTip(m_ui.nuzlockeOneCapturePerArea,
		"Allows the capture of only one minion from the first encounter per area."
		"\n\nAreas:"
		"\nKoris Minion"
		"\nBeetle Forest"
		"\nDragonfly Forest"
		"\nSpider Forest"
		"\nMoth Forest"
		"\nNetherworld Beetle Forest"
		"\nNetherworld Dragonfly Forest"
		"\nNetherworld Spider Forest"
		"\nEternal Corridor"
	);

	GuiSettings::setToolTip(m_ui.nuzlockeDefinitiveMinionDeath, "The death of one of your minions is definitive and will be automatically released.");
	GuiSettings::setToolTip(m_ui.nuzlockeDefinitiveLevantDeath, "When Levant dies, the game will automatically restart.");
	GuiSettings::setToolTip(m_ui.difficultyHard,
		"Increase boss stats by:"
		"\nAttack +33.33%"
		"\nDefense +33.33%"
		"\nMagic Attack +33.33%"
		"\nMagic Defense +33.33%"
		"\nSpeed +20%"
		"\nCritical +20%"
		"\n\n*Does not apply to Eternal Corridor bosses."
	);

	GuiSettings::setToolTip(m_ui.difficultyExtreme,
		"Increase boss stats by:"
		"\nAttack +50%"
		"\nDefense +50%"
		"\nMagic Attack +50%"
		"\nMagic Defense +50%"
		"\nSpeed +33.33%"
		"\nCritical +33.33%"
		"\n\n*Does not apply to Eternal Corridor bosses."
	);

	GuiSettings::setToolTip(m_ui.skipTutorialEnable, "Skip new game tutorial.");
	GuiSettings::setToolTip(m_ui.skipTutorialSkipKoris, "Skip Koris tutorial in the Beetle Forest.");
	GuiSettings::setToolTip(m_ui.x2FramerateEnable,
		"By pressing Select you can switch to a x2 framerate speed, if press again, switch back to x1."
		"\n\n*It is recommended to use this feature using emulator that supports CPU overclocking, otherwise"
		"\nif there are too many 3D models on the screen, the game will not fully support the x2."
	);

	GuiSettings::setToolTip(m_ui.showHiddenStatsEnable,
		"Show Critical and Body Enhancement of minions in the stats frame."
		"\n\nBipedal = +10 Special Attack Damage."
		"\nWinged = +10 Magic Damage + Dodge \"All Enemies\" Special Attack."
	);

	GuiSettings::setToolTip(m_ui.itemQuantityLimitSlider, "Changes item quantity limit.\n\nDefault = 20");
	GuiSettings::setToolTip(m_ui.eternalCorridorLevelCapSlider, "Change the level cap of the Eternal Corridor.\n\nDefault = 26");
	GuiSettings::setToolTip(m_ui.palToNtscEnable, "Turn pal versions in ntsc.\n\n*Does not work on some emulators.");

	m_ui.itemQuantityLimitValue->setStyleSheet("font-weight: normal;");
	m_ui.eternalCorridorLevelCapValue->setStyleSheet("font-weight: normal;");

	m_ui.skipTutorialSkipKoris->setEnabled(false);

	connect(m_ui.difficultyHard, &QAbstractButton::toggled, this, &AddonsWidget::updateDifficultyHard);
	connect(m_ui.difficultyExtreme, &QAbstractButton::toggled, this, &AddonsWidget::updateDifficultyExtreme);
	connect(m_ui.skipTutorialEnable, &QAbstractButton::toggled, m_ui.skipTutorialSkipKoris, &QWidget::setEnabled);
	connect(m_ui.itemQuantityLimitSlider, &QAbstractSlider::valueChanged, this, &AddonsWidget::setItemQuantityText);
	connect(m_ui.eternalCorridorLevelCapSlider, &QAbstractSlider::valueChanged, this, &AddonsWidget::setEternalCorridorLevelCapText);
}

void AddonsWidget::enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
{
	m_ui.palToNtscBox->setEnabled(!game->isNtsc());
	m_addons = std::make_unique<Addons>(game, sharedData);
	setEnabled(true);
}

void AddonsWidget::disableUI()
{
	setDisabled(true);
	if (m_addons)
	{
		m_addons.reset();
	}
}

void AddonsWidget::write() const
{
	if (!m_addons)
	{
		throw JcrException{ "Game is uninitialized" };
	}

	Addons::Nuzlocke_t nuzlocke{};
	if (m_ui.nuzlockeOneCapturePerArea->isChecked())
	{
		nuzlocke |= Addons::NUZLOCKE_ONE_CAPTURE;
	}
	if (m_ui.nuzlockeDefinitiveMinionDeath->isChecked())
	{
		nuzlocke |= Addons::NUZLOCKE_DEFINITIVE_DEATH;
	}
	if (m_ui.nuzlockeDefinitiveLevantDeath->isChecked())
	{
		nuzlocke |= Addons::NUZLOCKE_DEFINITIVE_LEVANT_DEATH;
	}
	if (nuzlocke)
	{
		m_addons->setNuzlocke(nuzlocke);
	}

	if (m_ui.difficultyHard->isChecked())
	{
		m_addons->setDifficulty(Addons::Difficulty::Hard);
	}
	else if (m_ui.difficultyExtreme->isChecked())
	{
		m_addons->setDifficulty(Addons::Difficulty::Extreme);
	}

	if (m_ui.skipTutorialEnable->isChecked())
	{
		m_addons->setSkipTutorial(m_ui.skipTutorialSkipKoris->isChecked());
	}

	if (m_ui.x2FramerateEnable->isChecked())
	{
		m_addons->setX2Framerate();
	}

	if (m_ui.showHiddenStatsEnable->isChecked())
	{
		m_addons->setShowHiddenStats();
	}

	m_addons->setItemQuantityLimit(static_cast<u8>(m_ui.itemQuantityLimitSlider->value()));
	m_addons->setLevelCapEC(static_cast<u8>(m_ui.eternalCorridorLevelCapSlider->value()));

	if (m_ui.palToNtscBox->isEnabled() && m_ui.palToNtscEnable->isChecked())
	{
		m_addons->setPalToNtsc();
	}
}

const char* AddonsWidget::name() const
{
	return "Addons";
}

void AddonsWidget::loadPresets(const nlohmann::json& json)
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

void AddonsWidget::savePresets(nlohmann::ordered_json* json)
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