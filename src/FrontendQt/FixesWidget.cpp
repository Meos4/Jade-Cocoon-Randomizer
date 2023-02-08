#include "FixesWidget.hpp"

#include "Common/JcrException.hpp"

FixesWidget::FixesWidget(QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.bodyEnhancementFix) },
		{ SETTINGS(m_ui.autumnMoonEffectFix) },
		{ SETTINGS(m_ui.specialAttackModifiersDisplayFix) },
	};

	m_qSlider =
	{
		{ SETTINGS(m_ui.hpMpBarsSizeSlider) },
	};

	m_ui.hpMpBarsSizeValue->setStyleSheet("font-weight: normal;");

	m_ui.bodyEnhancementFix->setToolTip
	(
		"Due to a programming error, body enhancements on Eternal Corridor minions and Eternal Corridor"
		"\nbosses do not work, this solves the issue."
		"\n\n*When fixed, Eternal Corridor minions will use body enhancements from story minions,"
		"\nand Eternal Corridor bosses will use their supposed body enhancements."
	);
	m_ui.autumnMoonEffectFix->setToolTip
	(
		"Autumn Moon was supposed to have a unique attack effect, unfortunately due to an oversight"
		"\nfrom the developers, the weapon file does not include the effect and use none, this solves this issue."
	);
	m_ui.hpMpBarsSizeSlider->setToolTip
	(
		"Bug may occur on hp / mp bars when playing on unofficial hardware, this will reduce its size."
		"\n\n*Most of the time, reducing the size to 24 or 23 is fine, but it can differ and needs to be gauged."
	);
	m_ui.specialAttackModifiersDisplayFix->setToolTip
	(
		"Due to a programming error, on the French version the special attack modifiers"
		"\n(+Power, +Accuracy and +Critical) are not displayed, this solves the issue."
	);

	connect(m_ui.hpMpBarsSizeSlider, &QSlider::valueChanged, this, &FixesWidget::setHpMpBarsSizeText);
}

void FixesWidget::enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
{
	m_ui.specialAttackModifiersDisplayBox->setEnabled(game->isVersion(Version::PalFr));
	m_fixes = std::make_unique<Fixes>(game, sharedData);
	setEnabled(true);
}

void FixesWidget::disableUI()
{
	setDisabled(true);
	if (m_fixes)
	{
		m_fixes.reset();
	}
}

void FixesWidget::write() const
{
	if (!m_fixes)
	{
		throw JcrException{ "Game is uninitialized" };
	}

	if (m_ui.bodyEnhancementFix->isChecked())
	{
		m_fixes->setBodyEnhancement();
	}

	if (m_ui.autumnMoonEffectFix->isChecked())
	{
		m_fixes->setAutumnMoonEffect();
	}

	m_fixes->setHpMpBarsSize(static_cast<u16>(m_ui.hpMpBarsSizeSlider->value()));

	if (m_ui.specialAttackModifiersDisplayBox->isEnabled() && 
		m_ui.specialAttackModifiersDisplayFix->isChecked())
	{
		m_fixes->setSpecialsModifiers();
	}
}

const char* FixesWidget::name() const
{
	return "Fixes";
}

void FixesWidget::loadSettings(const nlohmann::json& json)
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

void FixesWidget::saveSettings(nlohmann::ordered_json* json)
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

void FixesWidget::setHpMpBarsSizeText(s32 value)
{
	m_ui.hpMpBarsSizeValue->setText(QString::number(value));
}