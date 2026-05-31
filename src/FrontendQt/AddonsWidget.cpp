#include "AddonsWidget.hpp"

#include "Common/JcrException.hpp"
#include "FrontendQt/HelpConsoleWidget.hpp"

AddonsWidget::AddonsWidget(HelpConsoleWidget* helpConsole, QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.skipTutorialEnable) },
		{ SETTINGS(m_ui.skipTutorialSkipKoris) },
		{ SETTINGS(m_ui.palToNtscEnable) }
	};
	
	m_qSlider =
	{
		{ SETTINGS(m_ui.itemQuantityLimitSlider) },
		{ SETTINGS(m_ui.eternalCorridorLevelCapSlider) }
	};

	const QString _SkipTutorial{ m_ui.skipTutorialBox->title() };

	helpConsole->addFeature(m_ui.skipTutorialEnable, _SkipTutorial,
		"Skip the new game tutorial."
	);

	helpConsole->addFeature(m_ui.skipTutorialSkipKoris, _SkipTutorial,
		"Skip Koris tutorial in the Beetle Forest."
	);

	const QString _Slider{ "Slider" };

	helpConsole->addFeature(m_ui.itemQuantityLimitSlider, m_ui.itemQuantityLimitBox->title(), _Slider,
		"Changes item quantity limit."
		"\n\nDefault = 20"
	);

	helpConsole->addFeature(m_ui.eternalCorridorLevelCapSlider, m_ui.eternalCorridorLevelCapBox->title(), _Slider,
		"Change the level cap of the Eternal Corridor."
		"\n\nDefault = 26"
	);

	helpConsole->addFeature(m_ui.palToNtscEnable, m_ui.palToNtscBox->title(),
		"Turn pal versions in ntsc."
		"\n\nDoes not work on some emulators."
	);

	m_ui.itemQuantityLimitValue->setStyleSheet("font-weight: normal;");
	m_ui.eternalCorridorLevelCapValue->setStyleSheet("font-weight: normal;");

	m_ui.skipTutorialSkipKoris->setEnabled(false);

	connect(m_ui.skipTutorialEnable, &QAbstractButton::toggled, m_ui.skipTutorialSkipKoris, &QWidget::setEnabled);
	connect(m_ui.itemQuantityLimitSlider, &QAbstractSlider::valueChanged, this, &AddonsWidget::setItemQuantityText);
	connect(m_ui.eternalCorridorLevelCapSlider, &QAbstractSlider::valueChanged, this, &AddonsWidget::setEternalCorridorLevelCapText);
}

void AddonsWidget::enableUI(Randomizer* randomizer)
{
	m_ui.palToNtscBox->setEnabled(!randomizer->game().isNtsc());
	setEnabled(true);
}

void AddonsWidget::disableUI()
{
	setDisabled(true);
}

const char* AddonsWidget::name() const
{
	return "Addons";
}

void AddonsWidget::loadPresets(const Json::Read& json)
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

void AddonsWidget::savePresets(Json::Write* json)
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

const Ui::AddonsWidget& AddonsWidget::Ui() const
{
	return m_ui;
}

void AddonsWidget::setItemQuantityText(s32 value)
{
	m_ui.itemQuantityLimitValue->setText(QString::number(value));
}

void AddonsWidget::setEternalCorridorLevelCapText(s32 value)
{
	m_ui.eternalCorridorLevelCapValue->setText(QString::number(value));
}