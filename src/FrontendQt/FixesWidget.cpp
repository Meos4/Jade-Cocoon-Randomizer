#include "FixesWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/HelpConsoleWidget.hpp"

FixesWidget::FixesWidget(HelpConsoleWidget* helpConsole, QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qSlider =
	{
		{ SETTINGS(m_ui.hpMpBarsSizeSlider) },
	};

	m_ui.hpMpBarsSizeValue->setStyleSheet("font-weight: normal;");

	helpConsole->addFeature(m_ui.hpMpBarsSizeSlider, m_ui.hpMpBarsSizeBox->title(), "Slider", 
		"Bug may occur on hp / mp bars when playing on unofficial hardware, this will reduce its size."
		"\n\nMost of the time, reducing the size to 24 or 23 is fine, but it can differ and needs to be gauged."
	);

	connect(m_ui.hpMpBarsSizeSlider, &QSlider::valueChanged, this, &FixesWidget::setHpMpBarsSizeText);
}

void FixesWidget::enableUI(Randomizer* randomizer)
{
	setEnabled(true);
}

void FixesWidget::disableUI()
{
	setDisabled(true);
}

const char* FixesWidget::name() const
{
	return "Fixes";
}

void FixesWidget::loadPresets(const Json::Read& json)
{
	for (auto& slider : m_qSlider)
	{
		slider.load(json);
	}
}

void FixesWidget::savePresets(Json::Write* json)
{
	for (const auto& slider : m_qSlider)
	{
		slider.save(json);
	}
}

const Ui::FixesWidget& FixesWidget::Ui() const
{
	return m_ui;
}

void FixesWidget::setHpMpBarsSizeText(s32 value)
{
	m_ui.hpMpBarsSizeValue->setText(QString::number(value));
}