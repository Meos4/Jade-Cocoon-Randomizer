#include "ChallengeWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/HelpConsoleWidget.hpp"

ChallengeWidget::ChallengeWidget(HelpConsoleWidget* helpConsole, QWidget* parent)
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
		{ SETTINGS(m_ui.difficultyExtreme) }
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

	connect(m_ui.difficultyHard, &QAbstractButton::toggled, this, &ChallengeWidget::updateDifficultyHard);
	connect(m_ui.difficultyExtreme, &QAbstractButton::toggled, this, &ChallengeWidget::updateDifficultyExtreme);
}

void ChallengeWidget::enableUI(Randomizer* randomizer)
{
	setEnabled(true);
}

void ChallengeWidget::disableUI()
{
	setDisabled(true);
}

const char* ChallengeWidget::name() const
{
	return "Challenge";
}

void ChallengeWidget::loadPresets(const Json::Read& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}
}

void ChallengeWidget::savePresets(Json::Write* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}
}

const Ui::ChallengeWidget& ChallengeWidget::Ui() const
{
	return m_ui;
}

void ChallengeWidget::updateDifficultyHard()
{
	const bool isChecked{ m_ui.difficultyHard->isChecked() };
	if (isChecked)
	{
		m_ui.difficultyExtreme->setChecked(false);
	}
	m_ui.difficultyExtreme->setEnabled(!isChecked);
}

void ChallengeWidget::updateDifficultyExtreme()
{
	const bool isChecked{ m_ui.difficultyExtreme->isChecked() };
	if (isChecked)
	{
		m_ui.difficultyHard->setChecked(false);
	}
	m_ui.difficultyHard->setEnabled(!isChecked);
}