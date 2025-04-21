#include "LevantWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/HelpConsoleWidget.hpp"

LevantWidget::LevantWidget(HelpConsoleWidget* helpConsole, QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.baseStatsRandom) },
		{ SETTINGS(m_ui.animationBetaUsingItem) },
		{ SETTINGS(m_ui.animationRandomWeaponsPosture) },
		{ SETTINGS(m_ui.fluteStylingShuffle) }
	};

	helpConsole->addFeature(m_ui.baseStatsRandom, m_ui.baseStatsBox->title(),
		"Randomly shuffle the total of Levant stats at the new game."
		"\n\nA stat cannot be lower than 18 for balancing reasons."
	);

	const auto _Animation{ m_ui.animationBox->title() };

	helpConsole->addFeature(m_ui.animationBetaUsingItem, _Animation,
		"Replaces the item use animation with a beta/unused magic animation."
	);

	helpConsole->addFeature(m_ui.animationRandomWeaponsPosture, _Animation,
		"Random battle weapons postures."
		"\n\nEx: a dagger can be held like a sword."
	);

	helpConsole->addFeature(m_ui.fluteStylingShuffle, m_ui.fluteStylingBox->title(),
		"Shuffle the sound effects played when Levant summon / capture a minion."
		"\n\nA summon sound effect can be remplaced by a capture effect and vice versa."
	);
}

void LevantWidget::enableUI(Randomizer* randomizer)
{
	setEnabled(true);
}

void LevantWidget::disableUI()
{
	setDisabled(true);
}

const char* LevantWidget::name() const
{
	return "Levant";
}

void LevantWidget::loadPresets(const Json::Read& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}
}

void LevantWidget::savePresets(Json::Write* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}
}

const Ui::LevantWidget& LevantWidget::Ui() const
{
	return m_ui;
}