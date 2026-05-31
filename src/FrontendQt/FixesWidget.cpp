#include "FixesWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/HelpConsoleWidget.hpp"

FixesWidget::FixesWidget(HelpConsoleWidget* helpConsole, QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.hpMpBarsFix) },
	};

	helpConsole->addFeature(m_ui.hpMpBarsFix, m_ui.hpMpBarsBox->title(),
		"Bug may occur on hp / mp bars when playing on unofficial hardware, this fixes the issue."
	);
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
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}
}

void FixesWidget::savePresets(Json::Write* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}
}

const Ui::FixesWidget& FixesWidget::Ui() const
{
	return m_ui;
}