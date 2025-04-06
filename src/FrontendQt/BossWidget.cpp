#include "BossWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/GuiSettings.hpp"

BossWidget::BossWidget(QWidget* parent)
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

	GuiSettings::setToolTip(m_ui.storyElementRandomAll,
		"Randomize the element of all story bosses."
		"\n\n*Elemental battle maps will be adapted to the summon element (Tuturis, Seterian and Delfanel.)"
	);
	GuiSettings::setToolTip(m_ui.storyElementRandomElemental,
		"Randomize the element only of story bosses that have an element."
		"\n\n*Elemental battle maps will be adapted to the summon element (Tuturis, Seterian and Delfanel.)"
	);
	GuiSettings::setToolTip(m_ui.storySpecialMagicRandom,
		"Randomize specials and magics based on boss elements."
		"\n\n*Does not randomize heals/taunts and some AI-based attacks."
	);
	GuiSettings::setToolTip(m_ui.storyAppearanceTextureColorBasedOnElement,
		"Colorize the textures of the story bosses according to the elements."
		"\n\n*\"None\" bosses will use the default color and Cushidra will randomly use an elemental color."
	);
	GuiSettings::setToolTip(m_ui.storyAppearanceTextureRandomColor, "Randomize the texture colors of bosses.");
	GuiSettings::setToolTip(m_ui.eternalCorridorElementRandom, "Randomize Eternal Corridor boss elements.");
	GuiSettings::setToolTip(m_ui.eternalCorridorSpecialMagicRandom,
		"Randomize specials and magics based on Eternal Corridor boss elements."
		"\n\n*Does not randomize AI-based attacks."
	);
	GuiSettings::setToolTip(m_ui.eternalCorridorRandomNewAppearance, "Create new randomly generated appearances.");
	GuiSettings::setToolTip(m_ui.eternalCorridorAppearanceTextureRandom, "Randomize the texture of Eternal Corridor bosses.");
	GuiSettings::setToolTip(m_ui.eternalCorridorAppearanceTextureRandomColor, "Randomize the texture colors of Eternal Corridor bosses.");
	GuiSettings::setToolTip(m_ui.eternalCorridorAppearanceTextureIncludeCompatible, "Includes minions + Minion (Dark Arpatron) textures in the pool.");

	m_ui.eternalCorridorAppearanceTextureIncludeCompatible->setEnabled(false);

	connect(m_ui.storyElementRandomAll, &QAbstractButton::toggled, this, &BossWidget::updateStoryElement);
	connect(m_ui.storyElementRandomElemental, &QAbstractButton::toggled, this, &BossWidget::updateStoryElement);
	connect(m_ui.storySpecialMagicRandom, &QAbstractButton::toggled, this, &BossWidget::updateStoryElement);
	connect(m_ui.storyAppearanceTextureColorBasedOnElement, &QAbstractButton::toggled, this, &BossWidget::updateStoryAppearanceTextureBasedOnElement);
	connect(m_ui.storyAppearanceTextureRandomColor, &QAbstractButton::toggled, this, &BossWidget::updateStoryAppearanceTextureRandomColor);
	connect(m_ui.eternalCorridorElementRandom, &QAbstractButton::toggled, this, &BossWidget::updateEternalCorridorElementRandom);
	connect(m_ui.eternalCorridorSpecialMagicRandom, &QAbstractButton::toggled, this, &BossWidget::updateEternalCorridorElementRandom);
	connect(m_ui.eternalCorridorAppearanceTextureRandom, &QAbstractButton::toggled, m_ui.eternalCorridorAppearanceTextureIncludeCompatible, &QWidget::setEnabled);
}

void BossWidget::enableUI(Game* game, std::shared_ptr<SharedData> sharedData)
{
	m_boss = std::make_unique<Boss>(game, sharedData);
	setEnabled(true);
}

void BossWidget::disableUI()
{
	setDisabled(true);
	if (m_boss)
	{
		m_boss.reset();
	}
}

void BossWidget::write() const
{
	if (!m_boss)
	{
		throw JcrException{ "Game is uninitialized" };
	}

	if (m_ui.storyElementRandomAll->isChecked())
	{
		m_boss->setElement(Boss::Element::RandomAll);
	}
	else if (m_ui.storyElementRandomElemental->isChecked())
	{
		m_boss->setElement(Boss::Element::RandomElemental);
	}

	if (m_ui.storySpecialMagicRandom->isChecked())
	{
		m_boss->setSpecialMagic();
	}

	if (m_ui.storyAppearanceTextureColorBasedOnElement->isChecked())
	{
		m_boss->setAppearance(Boss::Appearance::ColorBasedOnElement);
	}
	else if (m_ui.storyAppearanceTextureRandomColor->isChecked())
	{
		m_boss->setAppearance(Boss::Appearance::RandomColor);
	}

	if (m_ui.eternalCorridorElementRandom->isChecked())
	{
		m_boss->setElementEC();
	}

	if (m_ui.eternalCorridorSpecialMagicRandom->isChecked())
	{
		m_boss->setSpecialMagicEC();
	}

	Boss::AppearanceEC_t appearanceEC{};
	if (m_ui.eternalCorridorRandomNewAppearance->isChecked())
	{
		appearanceEC |= Boss::APPEARANCE_EC_RANDOM_NEW_APPEARANCE;
	}
	if (m_ui.eternalCorridorAppearanceTextureRandom->isChecked())
	{
		appearanceEC |= Boss::APPEARANCE_EC_TEXTURE_RANDOM;

		if (m_ui.eternalCorridorAppearanceTextureIncludeCompatible->isChecked())
		{
			appearanceEC |= Boss::APPEARANCE_EC_TEXTURE_INCLUDE_COMPATIBLE;
		}
	}
	if (m_ui.eternalCorridorAppearanceTextureRandomColor->isChecked())
	{
		appearanceEC |= Boss::APPEARANCE_EC_TEXTURE_RANDOM_COLOR;
	}
	if (appearanceEC)
	{
		m_boss->setAppearanceEC(appearanceEC);
	}
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
}

void BossWidget::savePresets(Json::Write* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}
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

void BossWidget::updateEternalCorridorElementRandom()
{
	const bool isChecked{ m_ui.eternalCorridorElementRandom->isChecked() };
	if (isChecked)
	{
		m_ui.eternalCorridorSpecialMagicRandom->setChecked(true);
	}
	m_ui.eternalCorridorSpecialMagicRandom->setEnabled(!isChecked);
}