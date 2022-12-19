#include "LevantWidget.hpp"

LevantWidget::LevantWidget(QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.baseStatsRandom) },
		{ SETTINGS(m_ui.animationBetaUsingItem) },
		{ SETTINGS(m_ui.animationRandomWeaponsPosture) }
	};

	m_ui.baseStatsRandom->setToolTip
	(
		"Randomly shuffle the total of Levant stats at the new game."
		"\n\n*A stat cannot be lower than 18 for balancing reasons."
	);
	m_ui.animationBetaUsingItem->setToolTip("Replaces the item use animation with a beta/unused magic animation.");
	m_ui.animationRandomWeaponsPosture->setToolTip
	(
		"Random battle weapons postures."
		"\n\nEx: a dagger can be held like a sword."
	);
}

void LevantWidget::enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
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
		throw std::runtime_error{ "Game is uninitialized" };
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
}

const char* LevantWidget::name() const
{
	return "Levant";
}

void LevantWidget::loadSettings(const nlohmann::json& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}
}

void LevantWidget::saveSettings(nlohmann::ordered_json* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}
}