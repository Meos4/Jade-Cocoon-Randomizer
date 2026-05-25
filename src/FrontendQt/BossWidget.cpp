#include "BossWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/HelpConsoleWidget.hpp"

BossWidget::BossWidget(HelpConsoleWidget* helpConsole, QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");
	
	m_qCheckBox =
	{
		{ SETTINGS(m_ui.storyElementRandomAll) },
		{ SETTINGS(m_ui.storyElementRandomElemental) },
		{ SETTINGS(m_ui.storySpecialMagicRandom) },
		{ SETTINGS(m_ui.storyAppearanceTextureColorBasedOnElement) },
		{ SETTINGS(m_ui.storyAppearanceTextureRandomColor) },
		{ SETTINGS(m_ui.eternalCorridorElementRandom) },
		{ SETTINGS(m_ui.eternalCorridorSpecialMagicRandom) },
		{ SETTINGS(m_ui.eternalCorridorRandomNewAppearance) },
		{ SETTINGS(m_ui.eternalCorridorAppearanceTextureRandom) },
		{ SETTINGS(m_ui.eternalCorridorAppearanceTextureRandomColor) },
		{ SETTINGS(m_ui.eternalCorridorAppearanceTextureIncludeCompatible) }
	};

	m_qSlider =
	{
		{ SETTINGS(m_ui.eternalCorridorCrazinessSlider) }
	};

	const QString _Story{ m_ui.storyBox->title() };
	const auto _StoryElement{ _Story + " " + m_ui.storyElementBox->title() };

	helpConsole->addFeature(m_ui.storyElementRandomAll, _StoryElement, 
		"Randomize the element of all story bosses."
		"\n\nElemental battle maps will be adapted to the summon element (Tuturis, Seterian and Delfanel.)"
	);

	helpConsole->addFeature(m_ui.storyElementRandomElemental, _StoryElement, 
		"Randomize the element only of story bosses that have an element."
		"\n\nElemental battle maps will be adapted to the summon element (Tuturis, Seterian and Delfanel.)"
	);

	helpConsole->addFeature(m_ui.storySpecialMagicRandom, _Story + " " + m_ui.storySpecialMagicBox->title(),
		"Randomize specials and magics based on boss elements."
		"\n\nDoes not randomize heals/taunts and some AI-based attacks."
	);

	const auto _StoryAppearanceTexture{ _Story + " " + m_ui.storyAppearanceTextureBox->title() };

	helpConsole->addFeature(m_ui.storyAppearanceTextureColorBasedOnElement, _StoryAppearanceTexture,
		"Colorize the textures of the story bosses according to the elements."
		"\n\n\"None\" bosses will use the default color and Cushidra will randomly use an elemental color."
	);

	helpConsole->addFeature(m_ui.storyAppearanceTextureRandomColor, _StoryAppearanceTexture,
		"Randomize the texture colors of bosses."
	);

	const QString _EternalCorridor{ m_ui.eternalCorridorBox->title() };

	helpConsole->addFeature(m_ui.eternalCorridorElementRandom, _EternalCorridor + " " + m_ui.eternalCorridorElementBox->title(),
		"Randomize Eternal Corridor boss elements."
	);

	helpConsole->addFeature(m_ui.eternalCorridorSpecialMagicRandom, _EternalCorridor + " " + m_ui.eternalCorridorSpecialMagicBox->title(),
		"Randomize specials and magics based on Eternal Corridor boss elements."
		"\n\nDoes not randomize AI-based attacks."
	);

	const auto _EternalCorridorAppearance{ _EternalCorridor + " " + m_ui.eternalCorridorAppearanceBox->title() };
	const QString _ECCraziness{ _EternalCorridorAppearance + " " + m_ui.eternalCorridorCrazinessBox->title() };

	helpConsole->addFeature(m_ui.eternalCorridorRandomNewAppearance, _ECCraziness,
		"Create new randomly generated appearances."
	);

	helpConsole->addFeature(m_ui.eternalCorridorCrazinessSlider, _ECCraziness, "Craziness",
		"The higher the percentage, the more bizarre the generated appearances will be."
	);

	const auto _EternalCorridorAppearanceTexture{ _EternalCorridorAppearance + " " + m_ui.eternalCorridorAppearanceTextureBox->title() };

	helpConsole->addFeature(m_ui.eternalCorridorAppearanceTextureRandom, _EternalCorridorAppearanceTexture,
		"Randomize the texture of Eternal Corridor bosses."
	);

	helpConsole->addFeature(m_ui.eternalCorridorAppearanceTextureRandomColor, _EternalCorridorAppearanceTexture,
		"Randomize the texture colors of Eternal Corridor bosses."
	);

	helpConsole->addFeature(m_ui.eternalCorridorAppearanceTextureIncludeCompatible, _EternalCorridorAppearanceTexture,
		"Includes minions + Minion (Dark Arpatron) textures in the pool."
	);

	m_ui.eternalCorridorAppearanceTextureIncludeCompatible->setEnabled(false);
	m_ui.eternalCorridorCrazinessValue->setEnabled(false);

	connect(m_ui.storyElementRandomAll, &QAbstractButton::toggled, this, &BossWidget::updateStoryElement);
	connect(m_ui.storyElementRandomElemental, &QAbstractButton::toggled, this, &BossWidget::updateStoryElement);
	connect(m_ui.storySpecialMagicRandom, &QAbstractButton::toggled, this, &BossWidget::updateStoryElement);
	connect(m_ui.storyAppearanceTextureColorBasedOnElement, &QAbstractButton::toggled, this, &BossWidget::updateStoryAppearanceTextureBasedOnElement);
	connect(m_ui.storyAppearanceTextureRandomColor, &QAbstractButton::toggled, this, &BossWidget::updateStoryAppearanceTextureRandomColor);
	connect(m_ui.eternalCorridorElementRandom, &QAbstractButton::toggled, this, &BossWidget::updateEternalCorridorElementRandom);
	connect(m_ui.eternalCorridorSpecialMagicRandom, &QAbstractButton::toggled, this, &BossWidget::updateEternalCorridorElementRandom);
	connect(m_ui.eternalCorridorAppearanceTextureRandom, &QAbstractButton::toggled, m_ui.eternalCorridorAppearanceTextureIncludeCompatible, &QWidget::setEnabled);
	connect(m_ui.eternalCorridorRandomNewAppearance, &QAbstractButton::toggled, m_ui.eternalCorridorCrazinessSlider, &QWidget::setEnabled);
	connect(m_ui.eternalCorridorRandomNewAppearance, &QAbstractButton::toggled, m_ui.eternalCorridorCrazinessValue, &QWidget::setEnabled);
	connect(m_ui.eternalCorridorCrazinessSlider, &QAbstractSlider::valueChanged, this, &BossWidget::updateECCraziness);
}

void BossWidget::enableUI(Randomizer* randomizer)
{
	setEnabled(true);
}

void BossWidget::disableUI()
{
	setDisabled(true);
}

const char* BossWidget::name() const
{
	return "Boss";
}

void BossWidget::loadPresets(const Json::Read& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}

	for (auto& slider : m_qSlider)
	{
		slider.load(json);
	}

	updateECCraziness(m_ui.eternalCorridorCrazinessSlider->value());
}

void BossWidget::savePresets(Json::Write* json)
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

const Ui::BossWidget& BossWidget::Ui() const
{
	return m_ui;
}

void BossWidget::updateStoryElement()
{
	const bool
		isAllChecked{ m_ui.storyElementRandomAll->isChecked() },
		isElementalChecked{ m_ui.storyElementRandomElemental->isChecked() };

	if (isAllChecked && isElementalChecked)
	{
		m_ui.storyElementRandomAll->setChecked(false);
		return;
	}

	const bool isOneChecked{ isAllChecked || isElementalChecked };

	if (isOneChecked)
	{
		m_ui.storySpecialMagicRandom->setChecked(true);

		if (isAllChecked)
		{
			m_ui.storyElementRandomElemental->setChecked(false);
		}
		else // Elemental
		{
			m_ui.storyElementRandomAll->setChecked(false);
		}
	}

	m_ui.storySpecialMagicRandom->setEnabled(!isOneChecked);
	m_ui.storyElementRandomAll->setEnabled(!isElementalChecked);
	m_ui.storyElementRandomElemental->setEnabled(!isAllChecked);
}

void BossWidget::updateStoryAppearanceTextureBasedOnElement()
{
	const bool isChecked{ m_ui.storyAppearanceTextureColorBasedOnElement->isChecked() };
	if (isChecked)
	{
		m_ui.storyAppearanceTextureRandomColor->setChecked(false);
	}
	m_ui.storyAppearanceTextureRandomColor->setEnabled(!isChecked);
}

void BossWidget::updateStoryAppearanceTextureRandomColor()
{
	const bool isChecked{ m_ui.storyAppearanceTextureRandomColor->isChecked() };
	if (isChecked)
	{
		m_ui.storyAppearanceTextureColorBasedOnElement->setChecked(false);
	}
	m_ui.storyAppearanceTextureColorBasedOnElement->setEnabled(!isChecked);
}

void BossWidget::updateECCraziness(s32 value)
{
	m_ui.eternalCorridorCrazinessValue->setText(QString::number(value) + "%");
}

void BossWidget::updateEternalCorridorElementRandom()
{
	const bool isChecked{ m_ui.eternalCorridorElementRandom->isChecked() };
	if (isChecked)
	{
		m_ui.eternalCorridorSpecialMagicRandom->setChecked(true);
	}
	m_ui.eternalCorridorSpecialMagicRandom->setEnabled(!isChecked);
}