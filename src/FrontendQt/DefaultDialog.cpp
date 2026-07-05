#include "DefaultDialog.hpp"

#include "FrontendQt/HelpConsoleWidget.hpp"

DefaultDialog::DefaultDialog(QWidget* parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	setModal(true);

	m_ui.scrollArea->setContentsMargins(0, 0, 0, 0);
	m_ui.scrollArea->viewport()->setContentsMargins(0, 0, 0, 0);

	m_ui.fixesGroupBox->setStyleSheet("QGroupBox { font-weight: bold; }");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.x2Framerate) },
		{ SETTINGS(m_ui.canPauseTheGame) },
		{ SETTINGS(m_ui.skipOpeningLogos) },
		{ SETTINGS(m_ui.showHiddenStats) },
		{ SETTINGS(m_ui.autoHealInSafeArea) },
		{ SETTINGS(m_ui.turboModeInDialogues) },
		{ SETTINGS(m_ui.analogMode) },
		{ SETTINGS(m_ui.bugFixesHpMpBars) },
		{ SETTINGS(m_ui.bugFixesBodyEnhancement) },
		{ SETTINGS(m_ui.bugFixesAutumnMoonVisualAttackEffect) },
		{ SETTINGS(m_ui.bugFixesSpecialAttackModifiersDisplay) },
	};

	m_helpConsole = new HelpConsoleWidget(this);
	m_ui.mainLayout->insertWidget(2, m_helpConsole);
	m_ui.mainLayout->setStretch(1, 1);
	m_ui.mainLayout->setStretch(2, 1);

	m_helpConsole->addFeatureTitleOnly(m_ui.x2Framerate, QStringLiteral("x2 Framerate"),
		"By pressing Select you can switch to a x2 framerate speed, if press again, switch back to x1."
		"\n\nIt is recommended to use this feature using emulator that supports CPU overclocking, otherwise "
		"if there are too many 3D models on the screen, the game will not fully support the x2."
	);

	m_helpConsole->addFeatureTitleOnly(m_ui.canPauseTheGame, QStringLiteral("Can Pause The Game"),
		"Press Start to pause the game, press again to resume."
	);

	m_helpConsole->addFeatureTitleOnly(m_ui.skipOpeningLogos, QStringLiteral("Skip Opening Logos"),
		"Skip the opening logos."
	);

	m_helpConsole->addFeatureTitleOnly(m_ui.showHiddenStats, QStringLiteral("Show Hidden Stats"),
		"Show Critical and Body Enhancement of minions in the stats frame."
		"\n\nBipedal = +10 Special Attack Damage."
		"\nWinged = +10 Magic Damage + Dodge \"All Enemies\" Special Attack."
	);

	m_helpConsole->addFeatureTitleOnly(m_ui.autoHealInSafeArea, QStringLiteral("Auto Heal In Safe Area"),
		"Automatically heal in safe areas."
	);

	m_helpConsole->addFeatureTitleOnly(m_ui.turboModeInDialogues, QStringLiteral("Turbo Mode In Dialogues"),
		"Allows holding the confirmation button to skip dialogues."
	);

	m_helpConsole->addFeatureTitleOnly(m_ui.analogMode, QStringLiteral("Analog Mode"),
		"Enables 360° movement with the left analog stick."
		"\n\nThis feature is present by default on the PAL and NTSC-J2 versions, but missing on "
		"NTSC-U and NTSC-J1. This ports it to those two versions."
	);

	const QString bugFixesTitle{ m_ui.fixesGroupBox->title() };

	m_helpConsole->addFeature(m_ui.bugFixesHpMpBars, bugFixesTitle,
		"Bug may occur on hp / mp bars when playing on unofficial hardware, this fixes the issue."
	);

	m_helpConsole->addFeature(m_ui.bugFixesBodyEnhancement, bugFixesTitle,
		"Due to a programming error, body enhancements on Eternal Corridor minions and Eternal Corridor "
		"bosses do not work, this solves the issue."
		"\n\nWhen fixed, Eternal Corridor minions will use body enhancements from story minions "
		"and Eternal Corridor bosses will use their supposed body enhancements."
	);

	m_helpConsole->addFeature(m_ui.bugFixesAutumnMoonVisualAttackEffect, bugFixesTitle,
		"Autumn Moon was supposed to have a unique visual attack effect. Unfortunately due to an oversight "
		"from the developers, the weapon file does not include the effect and use none, this solves this issue."
	);

	m_helpConsole->addFeature(m_ui.bugFixesSpecialAttackModifiersDisplay, bugFixesTitle,
		"Due to a programming error, on the French version the special attack modifiers "
		"(+Power, +Accuracy and +Critical) are not displayed, this solves the issue."
	);
}

const char* DefaultDialog::name() const
{
	return nameKey;
}

void DefaultDialog::loadPresets(const Json::Read& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}
}

void DefaultDialog::savePresets(Json::Write* json) const
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}
}

const Ui::DefaultDialog& DefaultDialog::Ui() const
{
	return m_ui;
}