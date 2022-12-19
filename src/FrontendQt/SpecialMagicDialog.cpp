#include "SpecialMagicDialog.hpp"

static constexpr auto magicLimit{ MAGIC_FIRE_BOSS };
static constexpr auto specialLimit{ SPECIAL_DELFANEL_FLESH_TO_STONE };

SpecialMagicDialog::SpecialMagicDialog(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	setStyleSheet("font-weight: normal;");

	m_ui.sSpecialLabel->setStyleSheet("font-weight: bold;");
	m_ui.mMagicLabel->setStyleSheet("font-weight: bold;");

	const QPixmap
		noneIcon{ ":Img/None13.png" },
		fireIcon{ ":Img/Fire13.png" },
		airIcon{ ":Img/Air13.png" },
		earthIcon{ ":Img/Earth13.png" },
		waterIcon{ ":Img/Water13.png" };

	m_ui.sNoneLabel->setPixmap(noneIcon);
	m_ui.sFireLabel->setPixmap(fireIcon);
	m_ui.sAirLabel->setPixmap(airIcon);
	m_ui.sEarthLabel->setPixmap(earthIcon);
	m_ui.sWaterLabel->setPixmap(waterIcon);

	m_ui.mFireLabel->setPixmap(fireIcon);
	m_ui.mAirLabel->setPixmap(airIcon);
	m_ui.mEarthLabel->setPixmap(earthIcon);
	m_ui.mWaterLabel->setPixmap(waterIcon);

	m_specialsBox =
	{
		{ SPECIAL_NEUTRAL, m_ui.sNeutral },
		{ SPECIAL_EARTH_ATTACK, m_ui.sEarthAttack },
		{ SPECIAL_WATER_ATTACK, m_ui.sWaterAttack },
		{ SPECIAL_FIRE_ATTACK, m_ui.sFireAttack },
		{ SPECIAL_WIND_ATTACK, m_ui.sAirAttack },
		{ SPECIAL_POISON, m_ui.sPoison },
		{ SPECIAL_SLEEP, m_ui.sSleep },
		{ SPECIAL_FLESH_TO_STONE, m_ui.sFleshToStone },
		{ SPECIAL_DESTROY_MANA, m_ui.sDestroyMana },
		{ SPECIAL_ABSORB_HP, m_ui.sAbsorbHP },
		{ SPECIAL_ABSORB_MANA, m_ui.sAbsorbMana },
		{ SPECIAL_ALL_ENEMIES, m_ui.sAllEnemies },
		{ SPECIAL_CRITICAL, m_ui.sCritical },
		{ SPECIAL_BREAK_LEGS, m_ui.sBreakLegs },
		{ SPECIAL_REND_WINGS, m_ui.sRendWings },
	};

	m_magicsBox =
	{
		{ MAGIC_AGNI, m_ui.mAgni },
		{ MAGIC_AGNIS, m_ui.mAgnis },
		{ MAGIC_AGNIA, m_ui.mAgnia },
		{ MAGIC_VAHLI, m_ui.mVahli },
		{ MAGIC_VAHLIS, m_ui.mVahlis },
		{ MAGIC_VAHLIA, m_ui.mVahlia },
		{ MAGIC_ULVI, m_ui.mUlvi },
		{ MAGIC_ULVIS, m_ui.mUlvis },
		{ MAGIC_ULVIA, m_ui.mUlvia },
		{ MAGIC_MALTI, m_ui.mMalti },
		{ MAGIC_MALTIS, m_ui.mMaltis },
		{ MAGIC_MALTIA, m_ui.mMaltia },
		{ MAGIC_SELAHM_VENON, m_ui.mSelahmVenon },
		{ MAGIC_MEDINA, m_ui.mMedina },
		{ MAGIC_MEDINIA, m_ui.mMedinia },
		{ MAGIC_SELAHM, m_ui.mSelahm },
		{ MAGIC_SEL_SELAHM, m_ui.mSelSelahm },
		{ MAGIC_AD_VENON, m_ui.mAdVenon },
		{ MAGIC_AD_SLAHM, m_ui.mAdSlahm },
		{ MAGIC_AD_ROQUA, m_ui.mAdRoqua },
		{ MAGIC_AD_MUMULS, m_ui.mAdMumuls },
		{ MAGIC_DEVA_AGNI, m_ui.mDevaAgni },
		{ MAGIC_DEVA_VALI, m_ui.mDevaVali },
		{ MAGIC_DEVA_ULVI, m_ui.mDevaUlvi },
		{ MAGIC_DEVA_MALTI, m_ui.mDevaMalti }
	};
}

std::unordered_set<Special_t> SpecialMagicDialog::specials() const
{
	std::unordered_set<Special_t> specials;
	for (const auto& [key, box] : m_specialsBox)
	{
		if (box->isChecked())
		{
			specials.insert(key);
		}
	}
	return specials;
}

Magic_t SpecialMagicDialog::magics() const
{
	Magic_t magics{};
	for (const auto& [key, box] : m_magicsBox)
	{
		if (box->isChecked())
		{
			magics |= key;
		}
	}
	return magics;
}

void SpecialMagicDialog::setSpecials(const std::unordered_set<Special_t>& specials)
{
	for (Special_t i{ SPECIAL_NEUTRAL }; i < specialLimit; ++i)
	{
		m_specialsBox.at(i)->setChecked(specials.contains(i));
	}
}

void SpecialMagicDialog::setMagics(Magic_t magics)
{
	for (Magic_t i{ MAGIC_AGNI }; i < magicLimit; i <<= 1)
	{
		m_magicsBox.at(i)->setChecked(magics & i);
	}
}