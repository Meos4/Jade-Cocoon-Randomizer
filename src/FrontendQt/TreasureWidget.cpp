#include "TreasureWidget.hpp"

#include "Common/JcrException.hpp"

TreasureWidget::TreasureWidget(QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.itemsRandom) },
		{ SETTINGS(m_ui.itemsRandomByCategory) },
		{ SETTINGS(m_ui.itemsRandomSkeletonKeys) },
		{ SETTINGS(m_ui.valuablesRandom) },
		{ SETTINGS(m_ui.valuablesRandomByCategory) },
		{ SETTINGS(m_ui.valuablesRandomUsableValuables) },
		{ SETTINGS(m_ui.equipmentsRandom) },
		{ SETTINGS(m_ui.equipmentsRandomByCategory) },
		{ SETTINGS(m_ui.battleShuffle) },
		{ SETTINGS(m_ui.battleShuffleSkeletonKeys) }
	};

	m_ui.itemsRandom->setToolTip
	(
		"Randomize items to any treasure."
		"\n\n*Mugwort from the Koris tutorial is not random."
		"\n*Treasures are randomized by chapter pool."
	);
	m_ui.itemsRandomByCategory->setToolTip
	(
		"Randomize items to other items."
		"\n\n*Mugwort from the Koris tutorial is not random."
		"\n*Treasures are randomized by chapter pool."
	);
	m_ui.itemsRandomSkeletonKeys->setToolTip("Randomize skeleton keys.");
	m_ui.valuablesRandom->setToolTip
	(
		"Randomize valuables to any treasures."
		"\n\n*Valuables needed to progress the game are not randomized."
		"\n*Treasures are randomized by chapter pool."
	);
	m_ui.valuablesRandomByCategory->setToolTip
	(
		"Randomize valuables to other valuables."
		"\n\n*Valuables needed to progress the game are not randomized."
		"\n*Treasures are randomized by chapter pool."
	);
	m_ui.valuablesRandomUsableValuables->setToolTip
	(
		"Include forest warp keys and Kikinak ocarina in the pool."
	);
	m_ui.equipmentsRandom->setToolTip("Randomize equipments to any treasure.\n\n*Treasures are randomized by chapter pool.");
	m_ui.equipmentsRandomByCategory->setToolTip("Randomize equipments to other equipments.\n\n*Treasures are randomized by chapter pool.");
	m_ui.battleShuffle->setToolTip("Shuffle battle treasures.");
	m_ui.battleShuffleSkeletonKeys->setToolTip("Include battle skeleton keys.");

	m_ui.itemsRandomSkeletonKeys->setEnabled(false);
	m_ui.valuablesRandomUsableValuables->setEnabled(false);
	m_ui.battleShuffleSkeletonKeys->setEnabled(false);

	connect(m_ui.itemsRandom, &QAbstractButton::toggled, this, &TreasureWidget::updateItems);
	connect(m_ui.itemsRandomByCategory, &QAbstractButton::toggled, this, &TreasureWidget::updateItems);
	connect(m_ui.valuablesRandom, &QAbstractButton::toggled, this, &TreasureWidget::updateValuables);
	connect(m_ui.valuablesRandomByCategory, &QAbstractButton::toggled, this, &TreasureWidget::updateValuables);
	connect(m_ui.equipmentsRandom, &QAbstractButton::toggled, this, &TreasureWidget::updateEquipmentsRandom);
	connect(m_ui.equipmentsRandomByCategory, &QAbstractButton::toggled, this, &TreasureWidget::updateEquipmentsRandomByCategory);
	connect(m_ui.battleShuffle, &QAbstractButton::toggled, m_ui.battleShuffleSkeletonKeys, &QWidget::setEnabled);
}

void TreasureWidget::enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
{
	m_treasure = std::make_unique<Treasure>(game, sharedData);
	setEnabled(true);
}

void TreasureWidget::disableUI()
{
	setDisabled(true);
	if (m_treasure)
	{
		m_treasure.reset();
	}
}

void TreasureWidget::write() const
{
	if (!m_treasure)
	{
		throw JcrException{ "Game is uninitialized" };
	}

	if (m_ui.itemsRandom->isChecked())
	{
		m_treasure->setItems(Treasure::Items::Random, m_ui.itemsRandomSkeletonKeys->isChecked());
	}
	else if (m_ui.itemsRandomByCategory->isChecked())
	{
		m_treasure->setItems(Treasure::Items::RandomByCategory, m_ui.itemsRandomSkeletonKeys->isChecked());
	}

	if (m_ui.valuablesRandom->isChecked())
	{
		m_treasure->setValuables(Treasure::Valuables::Random, m_ui.valuablesRandomUsableValuables->isChecked());
	}
	else if (m_ui.valuablesRandomByCategory->isChecked())
	{
		m_treasure->setValuables(Treasure::Valuables::RandomByCategory, m_ui.valuablesRandomUsableValuables->isChecked());
	}

	if (m_ui.equipmentsRandom->isChecked())
	{
		m_treasure->setEquipments(Treasure::Equipments::Random);
	}
	else if (m_ui.equipmentsRandomByCategory->isChecked())
	{
		m_treasure->setEquipments(Treasure::Equipments::RandomByCategory);
	}

	if (m_ui.battleShuffle->isChecked())
	{
		m_treasure->setBattle(m_ui.battleShuffleSkeletonKeys->isChecked());
	}
}

const char* TreasureWidget::name() const
{
	return "Treasure";
}

void TreasureWidget::loadSettings(const nlohmann::json& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}
}

void TreasureWidget::saveSettings(nlohmann::ordered_json* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}
}

void TreasureWidget::updateItems()
{
	const bool
		isRandomChecked{ m_ui.itemsRandom->isChecked() },
		isRandomByCategoryChecked{ m_ui.itemsRandomByCategory->isChecked() };

	if (isRandomChecked && isRandomByCategoryChecked)
	{
		m_ui.itemsRandom->setChecked(false);
		return;
	}

	if (isRandomChecked)
	{
		m_ui.itemsRandomByCategory->setChecked(false);
	}
	else if (isRandomByCategoryChecked)
	{
		m_ui.itemsRandom->setChecked(false);
	}

	m_ui.itemsRandomSkeletonKeys->setEnabled(isRandomChecked || isRandomByCategoryChecked);
	m_ui.itemsRandom->setEnabled(!isRandomByCategoryChecked);
	m_ui.itemsRandomByCategory->setEnabled(!isRandomChecked);
}

void TreasureWidget::updateValuables()
{
	const bool
		isValuablesChecked{ m_ui.valuablesRandom->isChecked() },
		isValuablesByCategoryChecked{ m_ui.valuablesRandomByCategory->isChecked() };

	if (isValuablesChecked && isValuablesByCategoryChecked)
	{
		m_ui.valuablesRandom->setChecked(false);
		return;
	}

	if (isValuablesChecked)
	{
		m_ui.valuablesRandomByCategory->setChecked(false);
	}
	else if (isValuablesByCategoryChecked)
	{
		m_ui.valuablesRandom->setChecked(false);
	}

	m_ui.valuablesRandomUsableValuables->setEnabled(isValuablesChecked || isValuablesByCategoryChecked);
	m_ui.valuablesRandom->setEnabled(!isValuablesByCategoryChecked);
	m_ui.valuablesRandomByCategory->setEnabled(!isValuablesChecked);
}

void TreasureWidget::updateEquipmentsRandom()
{
	const bool isChecked{ m_ui.equipmentsRandom->isChecked() };
	if (isChecked)
	{
		m_ui.equipmentsRandomByCategory->setChecked(false);
	}
	m_ui.equipmentsRandomByCategory->setEnabled(!isChecked);
}

void TreasureWidget::updateEquipmentsRandomByCategory()
{
	const bool isChecked{ m_ui.equipmentsRandomByCategory->isChecked() };
	if (isChecked)
	{
		m_ui.equipmentsRandom->setChecked(false);
	}
	m_ui.equipmentsRandom->setEnabled(!isChecked);
}