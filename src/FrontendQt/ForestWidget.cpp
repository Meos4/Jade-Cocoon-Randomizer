#include "ForestWidget.hpp"

#include "Backend/Ost.hpp"
#include "Common/JcrException.hpp"
#include "FrontendQt/GuiSettings.hpp"

#include <QAbstractItemView>
#include <QStringList>

#include <array>
#include <cstring>
#include <string>

static s16 textToId(const char* text)
{
	for (const auto& [id, name] : Ost::names)
	{
		if (std::strcmp(text, name) == 0)
		{
			return id;
		}
	}
	return Forest::randomOstVal;
}

ForestWidget::ForestWidget(QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.paletteColorRandom) },
		{ SETTINGS(m_ui.battleMapsRandom) },
		{ SETTINGS(m_ui.ostRandom) },
		{ SETTINGS(m_ui.ostShuffle) },
		{ SETTINGS(m_ui.ostCustom) },
		{ SETTINGS(m_ui.ostRandomEternalCorridorOstPerCorridor) }
	};

	GuiSettings::setToolTip(m_ui.paletteColorRandom, "Randomize the colors of the forest palettes.");
	GuiSettings::setToolTip(m_ui.battleMapsRandom,
		"Randomize battle maps."
		"\n\n*Elemental battle maps are not randomized (Tuturis, Seterian and Delfanel.)"
	);

	GuiSettings::setToolTip(m_ui.ostRandom, "Randomize osts.");
	GuiSettings::setToolTip(m_ui.ostShuffle, "Shuffle ost between forests.");
	static constexpr auto customOst{ "Allows to choose the ost by forest." };
	GuiSettings::setToolTip(m_ui.ostCustom, customOst);
	GuiSettings::setToolTip(m_ui.ostCustomForestCombo, customOst);
	GuiSettings::setToolTip(m_ui.ostCustomChoiceCombo, customOst);
	GuiSettings::setToolTip(m_ui.ostRandomEternalCorridorOstPerCorridor,
		"Randomly change the ost in real time during Eternal Corridor with each new corridor.");

	m_ui.ostCustomForestCombo->setStyleSheet("font-weight: normal;");
	m_ui.ostCustomChoiceCombo->setStyleSheet("font-weight: normal;");

	m_ui.ostCustomForestCombo->setEnabled(false);
	m_ui.ostCustomChoiceCombo->setEnabled(false);

	static constexpr std::array<const char*, 8> forestNames
	{
		"Beetle Forest", "Dragonfly Forest", "Spider Forest" , "Moth Forest", 
		"Netherworld Beetle Forest", "Netherworld Dragonfly Forest", "Netherworld Spider Forest" , "Eternal Corridor"
	};

	for (const auto name : forestNames)
	{
		m_ui.ostCustomForestCombo->addItem(name);
	}

	m_ui.ostCustomChoiceCombo->setStyleSheet("font-weight: normal; combobox-popup: 0;");
	m_ui.ostCustomChoiceCombo->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	QStringList ostNames;

	for (const auto& [id, name] : Ost::names)
	{
		ostNames.append(name);
	}

	ostNames.sort();
	ostNames.insert(0, "[Random]");

	for (const auto& name : ostNames)
	{
		m_ui.ostCustomChoiceCombo->addItem(name);
	}

	m_ui.ostCustomForestCombo->setMaximumWidth(m_ui.ostCustomForestCombo->minimumSizeHint().width());
	m_ui.ostCustomChoiceCombo->setMaximumWidth(m_ui.ostCustomChoiceCombo->minimumSizeHint().width());

	connect(m_ui.ostRandom, &QAbstractButton::toggled, this, &ForestWidget::updateOstRandom);
	connect(m_ui.ostShuffle, &QAbstractButton::toggled, this, &ForestWidget::updateOstShuffle);
	connect(m_ui.ostCustom, &QAbstractButton::toggled, this, &ForestWidget::updateOstCustom);
	connect(m_ui.ostCustomForestCombo, &QComboBox::currentIndexChanged, this, &ForestWidget::updateOst);
}

void ForestWidget::enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
{
	m_forest = std::make_unique<Forest>(game, sharedData);

	if (m_isFirstEnableUI)
	{
		const auto forestOst{ m_forest->ost() };
		const auto nbOst{ m_ui.ostCustomChoiceCombo->count() };
		for (std::size_t i{}; i < forestOst.size(); ++i)
		{
			const auto forestStr{ Ost::names.at(forestOst[i]) };
			for (s32 j{}; j < nbOst; ++j)
			{
				const auto choiceStr{ m_ui.ostCustomChoiceCombo->itemText(j).toStdString() };
				if (std::strcmp(forestStr, choiceStr.c_str()) == 0)
				{
					m_ostIndex[i] = j;
					break;
				}
			}
		}
		m_previousForestIndex = m_ui.ostCustomForestCombo->currentIndex();
		ostToUI(m_previousForestIndex);
		m_isFirstEnableUI = false;
	}

	setEnabled(true);
}

void ForestWidget::disableUI()
{
	setDisabled(true);
	if (m_forest)
	{
		m_forest.reset();
	}
}

void ForestWidget::write() const
{
	if (!m_forest)
	{
		throw JcrException{ "Game is uninitialized" };
	}

	if (m_ui.paletteColorRandom->isChecked())
	{
		m_forest->setPaletteColor();
	}

	if (m_ui.battleMapsRandom->isChecked())
	{
		m_forest->setBattleMaps();
	}

	if (m_ui.ostRandom->isChecked())
	{
		m_forest->setOst(Forest::Ost::Random);
	}
	else if (m_ui.ostShuffle->isChecked())
	{
		m_forest->setOst(Forest::Ost::Shuffle);
	}
	else if (m_ui.ostCustom->isChecked())
	{
		m_forest->setOst(ostArray());
	}

	if (m_ui.ostRandomEternalCorridorOstPerCorridor->isChecked())
	{
		m_forest->setRandomEternalCorridorOstPerCorridor();
	}
}

const char* ForestWidget::name() const
{
	return "Forest";
}

void ForestWidget::loadPresets(const Json::Read& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}

	auto setIndex = [&](auto&& v) 
	{
		for (auto&& index : v)
		{
			const auto count{ m_ui.ostCustomChoiceCombo->count() };
			if (index < 0)
			{
				index = 0;
			}
			else if (index >= count)
			{
				index = count - 1;
			}
		}
		m_ostIndex = v;
	};

	Json::set<decltype(m_ostIndex)>(json, "ost", setIndex);
	ostToUI(m_ui.ostCustomForestCombo->currentIndex());
}

void ForestWidget::savePresets(Json::Write* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}

	UIToOst(m_previousForestIndex);
	(*json)["ost"] = m_ostIndex;
}

Forest::OstArray ForestWidget::ostArray() const
{
	Forest::OstArray ost;
	auto ostIndex{ m_ostIndex };
	ostIndex[m_ui.ostCustomForestCombo->currentIndex()] = m_ui.ostCustomChoiceCombo->currentIndex();

	for (std::size_t i{}; i < ost.size(); ++i)
	{
		const auto ostStr{ m_ui.ostCustomChoiceCombo->itemText(ostIndex[i]).toStdString() };
		ost[i] = textToId(ostStr.c_str());
	}
	return ost;
}

void ForestWidget::updateOstRandom()
{
	const bool isChecked{ m_ui.ostRandom->isChecked() };
	if (isChecked)
	{
		m_ui.ostShuffle->setChecked(false);
		m_ui.ostCustom->setChecked(false);
	}
	m_ui.ostShuffle->setEnabled(!isChecked);
	m_ui.ostCustom->setEnabled(!isChecked);
}

void ForestWidget::updateOstShuffle()
{
	const bool isChecked{ m_ui.ostShuffle->isChecked() };
	if (isChecked)
	{
		m_ui.ostRandom->setChecked(false);
		m_ui.ostCustom->setChecked(false);
	}
	m_ui.ostRandom->setEnabled(!isChecked);
	m_ui.ostCustom->setEnabled(!isChecked);
}

void ForestWidget::updateOstCustom()
{
	const bool isChecked{ m_ui.ostCustom->isChecked() };
	if (isChecked)
	{
		m_ui.ostRandom->setChecked(false);
		m_ui.ostShuffle->setChecked(false);
	}
	m_ui.ostRandom->setEnabled(!isChecked);
	m_ui.ostShuffle->setEnabled(!isChecked);
	m_ui.ostCustomForestCombo->setEnabled(isChecked);
	m_ui.ostCustomChoiceCombo->setEnabled(isChecked);
}

void ForestWidget::updateOst()
{
	UIToOst(m_previousForestIndex);
	m_previousForestIndex = m_ui.ostCustomForestCombo->currentIndex();
	ostToUI(m_previousForestIndex);
}

void ForestWidget::ostToUI(s32 forest)
{
	m_ui.ostCustomChoiceCombo->setCurrentIndex(m_ostIndex[forest]);
}

void ForestWidget::UIToOst(s32 forest)
{
	m_ostIndex[forest] = m_ui.ostCustomChoiceCombo->currentIndex();
}