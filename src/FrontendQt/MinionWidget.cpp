#include "MinionWidget.hpp"

#include "Backend/Entity.hpp"
#include "Common/JcrException.hpp"
#include "FrontendQt/GuiSettings.hpp"
#include "FrontendQt/SpecialMagicDialog.hpp"

#include <QAbstractItemView>

MinionWidget::MinionWidget(QWidget* parent)
	: RandomizerWidget(parent)
{
	m_ui.setupUi(this);
	
	setStyleSheet("font-weight: bold;");

	m_qCheckBox =
	{
		{ SETTINGS(m_ui.spawnStoryRandomRealtime) },
		{ SETTINGS(m_ui.spawnStoryRandomPremade) },
		{ SETTINGS(m_ui.spawnEternalCorridorRandom) },
		{ SETTINGS(m_ui.spawnDreamRandom) },
		{ SETTINGS(m_ui.spawnDreamCustom) },
		{ SETTINGS(m_ui.spawnKorisRandom) },
		{ SETTINGS(m_ui.spawnKorisCustom) },
		{ SETTINGS(m_ui.statsShuffleBetweenMinions) },
		{ SETTINGS(m_ui.statsShuffleStats) },
		{ SETTINGS(m_ui.specialMagicRandom) },
		{ SETTINGS(m_ui.appearanceRandomNewMinion) },
		{ SETTINGS(m_ui.appearanceModelRandom) },
		{ SETTINGS(m_ui.appearanceTextureRandom) },
		{ SETTINGS(m_ui.appearanceTextureIncludeCompatible) },
		{ SETTINGS(m_ui.appearanceTextureRandomColor) },
		{ SETTINGS(m_ui.appearanceGrowthSizeShuffle) },
		{ SETTINGS(m_ui.appearanceGrowthSizeInvert) }
	};

	m_qComboBox =
	{
		{ SETTINGS(m_ui.spawnDreamCombo) },
		{ SETTINGS(m_ui.spawnKorisCombo) }
	};

	GuiSettings::setToolTip(m_ui.spawnStoryRandomRealtime, "Randomize minion spawns during the story in real time.");
	GuiSettings::setToolTip(m_ui.spawnStoryRandomPremade,
		"Randomize minion spawns during the story in a premade way."
		"\n\nEx: Once randomized Pataimel is an Arpatron, it will be an Arpatron each time."
	);

	GuiSettings::setToolTip(m_ui.spawnEternalCorridorRandom, "Randomize minion spawns in the Eternal Corridor.");
	GuiSettings::setToolTip(m_ui.spawnDreamRandom, "Randomize the minion from Levant's second dream.");
	static constexpr auto customDream{ "Allows to choose the minion of Levant's second dream." };
	GuiSettings::setToolTip(m_ui.spawnDreamCustom, customDream);
	GuiSettings::setToolTip(m_ui.spawnDreamCombo, customDream);
	GuiSettings::setToolTip(m_ui.spawnKorisRandom, "Randomize Arpatron from the Koris tutorial.");
	static constexpr auto customKoris{ "Allows to choose the minion from the Koris tutorial." };
	GuiSettings::setToolTip(m_ui.spawnKorisCustom, customKoris);
	GuiSettings::setToolTip(m_ui.spawnKorisCombo, customKoris);
	GuiSettings::setToolTip(m_ui.statsShuffleBetweenMinions,
		"Shuffle stats between minions."
		"\n\nEx: Pataimel can get Arpatron stats, Arpatron get Marrdreg stats..."
		"\n\n*Body Enhancements are also shuffled."
		"\nBipedal = +10 Special Attack Damage."
		"\nWinged = +10 Magic Damage + Dodge \"All Enemies\" Special Attack."
	);

	GuiSettings::setToolTip(m_ui.statsShuffleStats, "Shuffle stats randomly.\n\nEx: Attack = Speed, Speed = Magic Attack...");
	GuiSettings::setToolTip(m_ui.specialMagicRandom, "Randomize special attacks and magics according on the element.");
	GuiSettings::setToolTip(m_ui.specialMagicPool, "Pool of specials and magic.\n\nChecked = Enabled, Unchecked = Disabled.");
	GuiSettings::setToolTip(m_ui.appearanceRandomNewMinion, "Create new randomly generated minions appearance.");
	GuiSettings::setToolTip(m_ui.appearanceModelRandom, "Randomize minions model.");
	GuiSettings::setToolTip(m_ui.appearanceTextureRandom, "Randomize minions texture.");
	GuiSettings::setToolTip(m_ui.appearanceTextureIncludeCompatible, "Include Eternal Corridor Bosses + Minion (Dark Arpatron) textures in the pool.");
	GuiSettings::setToolTip(m_ui.appearanceTextureRandomColor, "Randomize minion texture colors.");
	GuiSettings::setToolTip(m_ui.appearanceGrowthSizeShuffle, "Shuffle growth level size.");
	GuiSettings::setToolTip(m_ui.appearanceGrowthSizeInvert, "Invert growth level size.");

	m_ui.specialMagicPool->setStyleSheet("font-weight: normal;");
	m_ui.spawnDreamCombo->setStyleSheet("font-weight: normal; combobox-popup: 0;");
	m_ui.spawnDreamCombo->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_ui.spawnKorisCombo->setStyleSheet("font-weight: normal; combobox-popup: 0;");
	m_ui.spawnKorisCombo->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	m_ui.spawnDreamCombo->setEnabled(false);
	m_ui.spawnKorisCombo->setEnabled(false);
	m_ui.specialMagicPool->setEnabled(false);
	m_ui.appearanceTextureIncludeCompatible->setEnabled(false);

	m_specialMagicDialog = new SpecialMagicDialog(this);

	m_specialMagicDialog->setSpecials
	({
		SPECIAL_NEUTRAL, SPECIAL_EARTH_ATTACK, SPECIAL_WATER_ATTACK, SPECIAL_FIRE_ATTACK,
		SPECIAL_WIND_ATTACK, SPECIAL_POISON, SPECIAL_SLEEP, SPECIAL_FLESH_TO_STONE,
		SPECIAL_DESTROY_MANA, SPECIAL_ABSORB_HP, SPECIAL_ABSORB_MANA, SPECIAL_ALL_ENEMIES,
		SPECIAL_CRITICAL, SPECIAL_BREAK_LEGS, SPECIAL_REND_WINGS
	});

	m_specialMagicDialog->setMagics
	(
		MAGIC_AGNI | MAGIC_AGNIS | MAGIC_AGNIA | MAGIC_VAHLI |
		MAGIC_VAHLIS | MAGIC_VAHLIA | MAGIC_ULVI | MAGIC_ULVIS |
		MAGIC_ULVIA | MAGIC_MALTI | MAGIC_MALTIS | MAGIC_MALTIA |
		MAGIC_SELAHM_VENON | MAGIC_MEDINA | MAGIC_SELAHM | MAGIC_SEL_SELAHM |
		MAGIC_AD_VENON | MAGIC_AD_SLAHM | MAGIC_AD_ROQUA | MAGIC_AD_MUMULS |
		MAGIC_DEVA_AGNI | MAGIC_DEVA_VALI | MAGIC_DEVA_ULVI | MAGIC_DEVA_MALTI
	);

	connect(m_ui.spawnStoryRandomRealtime, &QAbstractButton::toggled, this, &MinionWidget::updateSpawnStoryRandomRealtime);
	connect(m_ui.spawnStoryRandomPremade, &QAbstractButton::toggled, this, &MinionWidget::updateSpawnStoryRandomPremade);
	connect(m_ui.spawnDreamRandom, &QAbstractButton::toggled, this, &MinionWidget::updateSpawnDreamRandom);
	connect(m_ui.spawnDreamCustom , &QAbstractButton::toggled, this, &MinionWidget::updateSpawnDreamCustom);
	connect(m_ui.spawnKorisRandom, &QAbstractButton::toggled, this, &MinionWidget::updateSpawnKorisRandom);
	connect(m_ui.spawnKorisCustom, &QAbstractButton::toggled, this, &MinionWidget::updateSpawnKorisCustom);
	connect(m_ui.specialMagicRandom, &QAbstractButton::toggled, m_ui.specialMagicPool, &QWidget::setEnabled);
	connect(m_ui.specialMagicPool, &QAbstractButton::clicked, m_specialMagicDialog, &QDialog::exec);
	connect(m_ui.appearanceTextureRandom, &QAbstractButton::toggled, m_ui.appearanceTextureIncludeCompatible, &QWidget::setEnabled);
	connect(m_ui.appearanceGrowthSizeShuffle, &QAbstractButton::toggled, this, &MinionWidget::updateAppearanceGrowthSizeShuffle);
	connect(m_ui.appearanceGrowthSizeInvert, &QAbstractButton::toggled, this, &MinionWidget::updateAppearanceGrowthSizeInvert);
}

void MinionWidget::enableUI(Game* game, std::shared_ptr<SharedData> sharedData)
{
	const bool isNtscJ{ game->isVersion(Version::NtscJ1, Version::NtscJ2) };
	m_minion = std::make_unique<Minion>(game, sharedData);

	auto minionForCombobox = [isNtscJ](Id_Entity_t id) -> s32
	{
		if (id >= ID_DREAM_MAN)
		{
			if (isNtscJ)
			{
				throw JcrException{ "Attempt to assign an invalid minion in a Japanese version : {}", id };
			}
			else if (id <= ID_CUSHIDRA)
			{
				throw JcrException{ "Attempt to assign a boss to a minion : {}", id };
			}
			else if (id >= ID_PALOOJA)
			{
				throw JcrException{ "Minion id overflow : {}", id };
			}
			return id - Entity::totalStoryBosses;
		}
		else
		{
			return id;
		}
	};

	auto addStoryMinions = [this]()
	{
		for (s32 i{}; i < Entity::totalStoryMinions; ++i)
		{
			m_ui.spawnDreamCombo->addItem(Entity::names[i]);
			m_ui.spawnKorisCombo->addItem(Entity::names[i]);
		}
	};

	auto addECMinions = [this]() 
	{
		for (s32 i{}; i < Entity::totalECMinions; ++i)
		{
			const auto index{ Entity::totalStoryMinions + Entity::totalStoryBosses + i };
			m_ui.spawnDreamCombo->addItem(Entity::names[index]);
			m_ui.spawnKorisCombo->addItem(Entity::names[index]);
		}

		m_isSpawnStoryRandomRealtimeBanned = false;
		if (!m_ui.spawnStoryRandomPremade->isChecked())
		{
			m_ui.spawnStoryRandomRealtime->setEnabled(true);
		}
	};

	if (isNtscJ)
	{
		m_isSpawnStoryRandomRealtimeBanned = true;
		m_ui.spawnStoryRandomRealtime->setEnabled(false);
		m_ui.spawnStoryRandomRealtime->setChecked(false);
	}

	const auto comboCount{ m_ui.spawnDreamCombo->count() };

	if (comboCount == Entity::totalStoryMinions)
	{
		if (!isNtscJ)
		{
			addECMinions();
		}
	}
	else if (comboCount == Entity::totalStoryMinions + Entity::totalECMinions)
	{
		if (isNtscJ)
		{
			m_ui.spawnDreamCombo->clear();
			m_ui.spawnKorisCombo->clear();

			addStoryMinions();

			m_ui.spawnDreamCombo->setCurrentIndex(minionForCombobox(m_minion->dreamMinion()));
			m_ui.spawnKorisCombo->setCurrentIndex(minionForCombobox(m_minion->korisMinion()));
		}
	}
	else
	{
		addStoryMinions();

		if (!isNtscJ)
		{
			addECMinions();
		}

		m_ui.spawnDreamCombo->setCurrentIndex(minionForCombobox(m_minion->dreamMinion()));
		m_ui.spawnKorisCombo->setCurrentIndex(minionForCombobox(m_minion->korisMinion()));
	}

	setEnabled(true);
}

void MinionWidget::disableUI()
{
	setDisabled(true);
	if (m_minion)
	{
		m_minion.reset();
	}
}

void MinionWidget::write() const
{
	auto getMinionFromCombobox = [](s32 id) -> Id_Entity_t
	{
		return id >= ID_DREAM_MAN ? id + Entity::totalStoryBosses : id;
	};

	if (!m_minion)
	{
		throw JcrException{ "Game is uninitialized" };
	}

	if (m_ui.spawnStoryRandomRealtime->isChecked())
	{
		m_minion->setSpawnStory(Minion::SpawnStory::RandomRealtime);
	}
	else if (m_ui.spawnStoryRandomPremade->isChecked())
	{
		m_minion->setSpawnStory(Minion::SpawnStory::RandomPremade);
	}

	if (m_ui.spawnEternalCorridorRandom->isChecked())
	{
		m_minion->setSpawnEC();
	}

	if (m_ui.spawnDreamRandom->isChecked())
	{
		m_minion->setDreamMinion();
	}
	else if (m_ui.spawnDreamCustom->isChecked())
	{
		m_minion->setDreamMinion(getMinionFromCombobox(m_ui.spawnDreamCombo->currentIndex()));
	}

	if (m_ui.spawnKorisRandom->isChecked())
	{
		m_minion->setKorisMinion();
	}
	else if (m_ui.spawnKorisCustom->isChecked())
	{
		m_minion->setKorisMinion(getMinionFromCombobox(m_ui.spawnKorisCombo->currentIndex()));
	}

	Minion::Stats_t stats{};
	if (m_ui.statsShuffleBetweenMinions->isChecked())
	{
		stats |= Minion::STATS_SHUFFLE_BETWEEN_MINIONS;
	}
	if (m_ui.statsShuffleStats->isChecked())
	{
		stats |= Minion::STATS_SHUFFLE_STATS;
	}
	if (stats)
	{
		m_minion->setStats(stats);
	}

	if (m_ui.specialMagicRandom->isChecked())
	{
		m_minion->setSpecialMagic(m_specialMagicDialog->specials(), m_specialMagicDialog->magics());
	}

	Minion::Appearance_t appearance{};
	if (m_ui.appearanceRandomNewMinion->isChecked())
	{
		appearance |= Minion::APPEARANCE_RANDOM_NEW_MINION;
	}
	if (m_ui.appearanceModelRandom->isChecked())
	{
		appearance |= Minion::APPEARANCE_MODEL_RANDOM;
	}
	if (m_ui.appearanceTextureRandom->isChecked())
	{
		appearance |= Minion::APPEARANCE_TEXTURE_RANDOM;

		if (m_ui.appearanceTextureIncludeCompatible->isChecked())
		{
			appearance |= Minion::APPEARANCE_TEXTURE_INCLUDE_COMPATIBLE;
		}
	}
	if (m_ui.appearanceTextureRandomColor->isChecked())
	{
		appearance |= Minion::APPEARANCE_TEXTURE_RANDOM_COLOR;
	}
	if (m_ui.appearanceGrowthSizeShuffle->isChecked())
	{
		appearance |= Minion::APPEARANCE_GROWTH_SIZE_SHUFFLE;
	}
	else if (m_ui.appearanceGrowthSizeInvert->isChecked())
	{
		appearance |= Minion::APPEARANCE_GROWTH_SIZE_INVERT;
	}
	if (appearance)
	{
		m_minion->setAppearance(appearance);
	}
}

const char* MinionWidget::name() const
{
	return "Minion";
}

void MinionWidget::loadPresets(const Json::Read& json)
{
	for (auto& checkBox : m_qCheckBox)
	{
		checkBox.load(json);
	}

	for (auto& comboBox : m_qComboBox)
	{
		comboBox.load(json);
	}

	Json::set<std::unordered_set<Special_t>>(json, "specials", [this](const auto& v) { m_specialMagicDialog->setSpecials(v); });
	Json::set<Magic_t>(json, "magics", [this](auto v) { m_specialMagicDialog->setMagics(v); });
}

void MinionWidget::savePresets(Json::Write* json)
{
	for (const auto& checkBox : m_qCheckBox)
	{
		checkBox.save(json);
	}

	for (const auto& comboBox : m_qComboBox)
	{
		comboBox.save(json);
	}

	(*json)["specials"] = m_specialMagicDialog->specials();
	(*json)["magics"] = m_specialMagicDialog->magics();
}

void MinionWidget::updateSpawnStoryRandomRealtime()
{
	if (m_isSpawnStoryRandomRealtimeBanned)
	{
		m_ui.spawnStoryRandomRealtime->setChecked(false);
	}

	const bool isChecked{ m_ui.spawnStoryRandomRealtime->isChecked() };
	if (isChecked)
	{
		m_ui.spawnStoryRandomPremade->setChecked(false);
	}
	m_ui.spawnStoryRandomPremade->setEnabled(!isChecked);
}

void MinionWidget::updateSpawnStoryRandomPremade()
{
	const bool isChecked{ m_ui.spawnStoryRandomPremade->isChecked() };
	if (isChecked)
	{
		m_ui.spawnStoryRandomRealtime->setChecked(false);
	}
	m_ui.spawnStoryRandomRealtime->setEnabled(!isChecked && !m_isSpawnStoryRandomRealtimeBanned);
}

void MinionWidget::updateSpawnDreamRandom()
{
	const bool isChecked{ m_ui.spawnDreamRandom->isChecked() };
	if (isChecked)
	{
		m_ui.spawnDreamCustom->setChecked(false);
	}
	m_ui.spawnDreamCustom->setEnabled(!isChecked);
}

void MinionWidget::updateSpawnDreamCustom()
{
	const bool isChecked{ m_ui.spawnDreamCustom->isChecked() };
	if (isChecked)
	{
		m_ui.spawnDreamRandom->setChecked(false);
	}
	m_ui.spawnDreamCombo->setEnabled(isChecked);
	m_ui.spawnDreamRandom->setEnabled(!isChecked);
}

void MinionWidget::updateSpawnKorisRandom()
{
	const bool isChecked{ m_ui.spawnKorisRandom->isChecked() };
	if (isChecked)
	{
		m_ui.spawnKorisCustom->setChecked(false);
	}
	m_ui.spawnKorisCustom->setEnabled(!isChecked);
}

void MinionWidget::updateSpawnKorisCustom()
{
	const bool isChecked{ m_ui.spawnKorisCustom->isChecked() };
	if (isChecked)
	{
		m_ui.spawnKorisRandom->setChecked(false);
	}
	m_ui.spawnKorisCombo->setEnabled(isChecked);
	m_ui.spawnKorisRandom->setEnabled(!isChecked);
}

void MinionWidget::updateAppearanceGrowthSizeShuffle()
{
	const bool isChecked{ m_ui.appearanceGrowthSizeShuffle->isChecked() };
	if (isChecked)
	{
		m_ui.appearanceGrowthSizeInvert->setChecked(false);
	}
	m_ui.appearanceGrowthSizeInvert->setEnabled(!isChecked);
}

void MinionWidget::updateAppearanceGrowthSizeInvert()
{
	const bool isChecked{ m_ui.appearanceGrowthSizeInvert->isChecked() };
	if (isChecked)
	{
		m_ui.appearanceGrowthSizeShuffle->setChecked(false);
	}
	m_ui.appearanceGrowthSizeShuffle->setEnabled(!isChecked);
}