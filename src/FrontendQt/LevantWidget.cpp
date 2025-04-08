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

void LevantWidget::enableUI(Game* game, std::shared_ptr<SharedData> sharedData)
{
	m_levant = std::make_unique<Levant>(game, sharedData);
	setEnabled(true);
}

void LevantWidget::disableUI()
{
	setDisabled(true);
	if (m_levant)
	{
		m_levant.reset();
	}
}

void LevantWidget::write() const
{
	if (!m_levant)
	{
		throw JcrException{ "Game is uninitialized" };
	}

	if (m_ui.baseStatsRandom->isChecked())
	{
		m_levant->setBaseStats();
	}

	Levant::Animation_t animation{};
	if (m_ui.animationBetaUsingItem->isChecked())
	{
		animation |= Levant::ANIMATION_BETA_USING_ITEM;
	}
	if (m_ui.animationRandomWeaponsPosture->isChecked())
	{
		animation |= Levant::ANIMATION_RANDOM_WEAPONS_POSTURE;
	}
	if (animation)
	{
		m_levant->setAnimation(animation);
	}

	if (m_ui.fluteStylingShuffle->isChecked())
	{
		m_levant->setFluteStyling();
	}
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