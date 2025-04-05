#include "Boss.hpp"

#include "Backend/Entity.hpp"
#include "Backend/File.hpp"
#include "Backend/JCUtil.hpp"
#include "Backend/Merge.hpp"
#include "Backend/Mips.hpp"
#include "Backend/Model.hpp"
#include "Backend/Util.hpp"
#include "Common/JcrException.hpp"
#include "Common/TemplateTypes.hpp"

#include <array>
#include <algorithm>
#include <bit>
#include <cstring>
#include <type_traits>
#include <ranges>
#include <unordered_set>
#include <utility>

struct DamageModifiers
{
	s8 fire;
	s8 air;
	s8 earth;
	s8 water;
	s8 poison;
	s8 sleep;
	s8 fleshToStone;
	s8 deathCritical;
	s8 absorption;
	s8 unk;
	s8 standardAttacks;
	s8 unk2;
};

static void setDamageModifiers(DamageModifiers* damageModifiers, Element_t element)
{
	const auto
		min{ std::min({ damageModifiers->fire, damageModifiers->air, damageModifiers->earth, damageModifiers->water }) },
		max{ std::max({ damageModifiers->fire, damageModifiers->air, damageModifiers->earth, damageModifiers->water }) };

	auto mid{ min };
	
	auto setMid = [min, &mid, max](u8 elem)
	{
		if (elem != min && elem != max)
		{
			mid = elem;
		}
	};

	setMid(damageModifiers->fire);
	setMid(damageModifiers->air);
	setMid(damageModifiers->earth);
	setMid(damageModifiers->water);

	switch (element)
	{
	case ELEMENT_NONE:
		damageModifiers->fire = mid;
		damageModifiers->air = mid;
		damageModifiers->earth = mid;
		damageModifiers->water = mid;
		break;
	case ELEMENT_FIRE:
		damageModifiers->fire = mid;
		damageModifiers->air = min;
		damageModifiers->earth = mid;
		damageModifiers->water = max;
		break;
	case ELEMENT_AIR:
		damageModifiers->fire = max;
		damageModifiers->air = mid;
		damageModifiers->earth = min;
		damageModifiers->water = mid;
		break;
	case ELEMENT_EARTH:
		damageModifiers->fire = mid;
		damageModifiers->air = max;
		damageModifiers->earth = mid;
		damageModifiers->water = min;
		break;
	case ELEMENT_WATER:
		damageModifiers->fire = min;
		damageModifiers->air = mid;
		damageModifiers->earth = max;
		damageModifiers->water = mid;
		break;
	}
}

static void setDamageModifiersNone(DamageModifiers* damageModifiers, Element_t element)
{
	const auto
		max{ std::max({ damageModifiers->fire, damageModifiers->air, damageModifiers->earth, damageModifiers->water }) },
		mid{ static_cast<s8>(max / 2) };

	switch (element)
	{
	case ELEMENT_NONE:
		break;
	case ELEMENT_FIRE:
		damageModifiers->fire = mid;
		damageModifiers->air = mid;
		damageModifiers->earth = mid;
		damageModifiers->water = max;
		break;
	case ELEMENT_AIR:
		damageModifiers->fire = max;
		damageModifiers->air = mid;
		damageModifiers->earth = mid;
		damageModifiers->water = mid;
		break;
	case ELEMENT_EARTH:
		damageModifiers->fire = mid;
		damageModifiers->air = max;
		damageModifiers->earth = mid;
		damageModifiers->water = mid;
		break;
	case ELEMENT_WATER:
		damageModifiers->fire = mid;
		damageModifiers->air = mid;
		damageModifiers->earth = max;
		damageModifiers->water = mid;
		break;
	}
}

Boss::Boss(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
	: m_game(std::move(game)), m_sharedData(std::move(sharedData))
{
}

void Boss::setElement(Boss::Element state, bool allowNoneElement) const
{
	const u32 statsOffset{ m_game->offset().file.executable.entityStats + ID_DREAM_MAN * sizeof(StatsStruct) };

	static constexpr auto bossCounter{ Entity::totalStoryBosses - 2 }; // -2 no Chosen One and Cushidra
	std::array<Element_t, bossCounter> elements;

	auto executable{ m_game->executable() };

	for (std::size_t i{}; i < elements.size(); ++i)
	{
		executable.read(statsOffset + i * sizeof(StatsStruct) + 6, &elements[i]);
	}

	const u32 damageModifiersOffset{ m_game->offset().file.executable.entityDamageModifiers + 0x1FC };
	auto damageModifiers{ executable.read<std::array<DamageModifiers, bossCounter>>(damageModifiersOffset) };

	const Element_t minElement{ allowNoneElement ? ELEMENT_NONE : ELEMENT_FIRE };

	for (std::size_t i{}; i < elements.size(); ++i)
	{
		if (!(state == Boss::Element::RandomElemental && elements[i] == ELEMENT_NONE))
		{
			const auto rngElement{ m_game->random()->generate(minElement, Element_t(ELEMENT_COUNT - 1)) };

			if (rngElement != elements[i])
			{
				elements[i] == ELEMENT_NONE ?
					setDamageModifiersNone(&damageModifiers[i], rngElement) :
					setDamageModifiers(&damageModifiers[i], rngElement);

				elements[i] = rngElement;
			}
		}
	}

	for (std::size_t i{}; i < elements.size(); ++i)
	{
		executable.write(statsOffset + i * sizeof(StatsStruct) + 6, elements[i]);
	}

	executable.write(damageModifiersOffset, damageModifiers);

	const auto
		fireMapBonus{ Mips::li(Mips::Register::s0, elements[ID_TUTURIS - ID_DREAM_MAN]) },
		airMapBonus{ Mips::li(Mips::Register::s0, elements[ID_SETERIAN - ID_DREAM_MAN]) },
		earthMapBonus{ Mips::li(Mips::Register::s0, elements[ID_DELFANEL - ID_DREAM_MAN]) };

	const auto over_game_bin{ m_game->file(File::OVER_GAME_BIN) };

	over_game_bin->write(m_game->offset().file.over_game_bin.fireMapBonus, fireMapBonus);
	over_game_bin->write(m_game->offset().file.over_game_bin.fireMapBonus + 8, airMapBonus);
	over_game_bin->write(m_game->offset().file.over_game_bin.fireMapBonus + 0xC, earthMapBonus);
}

void Boss::setSpecialMagic() const
{
	const std::unordered_map<Element_t, Special_t> availableSpecial
	{
		{ ELEMENT_NONE, { SPECIAL_NEUTRAL0 } },
		{ ELEMENT_FIRE, { SPECIAL_FIRE_ATTACK } },
		{ ELEMENT_AIR, { SPECIAL_WIND_ATTACK } },
		{ ELEMENT_EARTH, { SPECIAL_EARTH_ATTACK } },
		{ ELEMENT_WATER, { SPECIAL_WATER_ATTACK } }
	};

	auto generateSpecial = [&](SpecialStruct* special, Element_t element, bool allowStatus = true)
	{
		static constexpr std::array<Special_t, 3> 
			availableSpecialStatus{ SPECIAL_POISON, SPECIAL_FLESH_TO_STONE, SPECIAL_SLEEP };

		static constexpr auto elemProba{ 75.f };

		special->isEnabled = 1;

		if (!allowStatus || m_game->random()->generateProba(elemProba))
		{
			const auto elementToUse
			{ 
				element == ELEMENT_NONE ? m_game->random()->generate(element, Element_t(ELEMENT_COUNT - 1)) : element
			};

			special->specialAttackId = availableSpecial.at(elementToUse);
			return allowStatus;
		}
		else
		{
			special->specialAttackId = availableSpecialStatus[m_game->random()->generate(availableSpecialStatus.size() - 1)];
			return false;
		}
	};

	auto generateMagic = [&](Mips::Register rgt, Element_t element)
	{
		static constexpr std::array<Magic_t, ELEMENT_COUNT> availableMagics
		{
			MAGIC_AGNI | MAGIC_AGNIS | MAGIC_AGNIA | MAGIC_FIRE_BOSS,
			MAGIC_MALTI | MAGIC_MALTIS | MAGIC_MALTIA | MAGIC_AIR_BOSS,
			MAGIC_ULVI | MAGIC_ULVIS | MAGIC_ULVIA | MAGIC_EARTH_BOSS,
			MAGIC_VAHLI | MAGIC_VAHLIS | MAGIC_VAHLIA | MAGIC_WATER_BOSS
		};

		const auto elementToUse{ element == ELEMENT_NONE ? m_game->random()->generate(Element_t(ELEMENT_COUNT - 1)) : element };
		return Mips::li(rgt, static_cast<u16>(Utility::bitToInt(m_game->random()->generateBit(availableMagics[elementToUse]))));
	};

	auto executable{ m_game->executable() };

	const u32 statsOffset{ m_game->offset().file.executable.entityStats + ID_DREAM_MAN * sizeof(StatsStruct) };
	const auto stats{ executable.read<std::array<StatsStruct, Entity::totalStoryBosses>>(statsOffset) };

	const u32 specialsOffset{ m_game->offset().file.executable.entitySpecials + ID_DREAM_MAN * sizeof(SpecialsArray) };
	auto specials{ executable.read<std::array<SpecialsArray, Entity::totalStoryBosses>>(specialsOffset) };

	// Dream Man

	// Minion
	// Special: Horn (Neutral)
	// Magic: Agnia (a0)

	static constexpr auto minionId{ ID_MINION - ID_DREAM_MAN };
	generateSpecial(&specials[minionId][BODYPART_HORN], stats[minionId].element);
	const auto minionAgniaId{ generateMagic(Mips::Register::a0, stats[minionId].element) };

	// Delfanel
	// Special: Horn (Waiting), Fang (Flesh To Stone), Claw (All Enemies), Tail (Flesh To Stone), Foot (Waiting)
	// Magic: Earth Boss (v0)
	// Note: Tail is unused

	static constexpr auto delfanelId{ ID_DELFANEL - ID_DREAM_MAN };
	specials[delfanelId][BODYPART_FANG].specialAttackId =
		m_game->random()->generate({ SPECIAL_DELFANEL_FLESH_TO_STONE, SPECIAL_TUTURIS_POISON, SPECIAL_SETERIAN_SLEEP });
	generateSpecial(&specials[delfanelId][BODYPART_CLAW], stats[delfanelId].element, false);
	const auto delfanelEarthBossId{ generateMagic(Mips::Register::v0, stats[delfanelId].element) };

	// Earth Boss

	// Tuturis
	// Special: Fang (Absorb Mana), Tail (Poison), Foot (All Enemies)
	// Magic: Fire Boss (v0)
	// Note: Foot is unused and doesn't work well (bug anim)

	static constexpr auto tuturisId{ ID_TUTURIS - ID_DREAM_MAN };
	specials[tuturisId][BODYPART_TAIL].specialAttackId =
		m_game->random()->generate({ SPECIAL_DELFANEL_FLESH_TO_STONE, SPECIAL_TUTURIS_POISON, SPECIAL_SETERIAN_SLEEP });
	const auto tuturisFireBossId{ generateMagic(Mips::Register::v0, stats[tuturisId].element) };

	// Fire Boss

	// Seterian
	// Special: Horn (Heal), Fang (Sleep), Claw (Air), Tail (Air)
	// Magic: Air Boss (v0)
	// Note: Claw is unused but works well

	static constexpr auto seterianId{ ID_SETERIAN - ID_DREAM_MAN };
	const auto seterianFang{ m_game->random()->generate({ SPECIAL_DELFANEL_FLESH_TO_STONE, SPECIAL_SETERIAN_SLEEP }) };
	const auto li_a1_seterianStatus{ Mips::li(Mips::Register::a1, seterianFang == SPECIAL_SETERIAN_SLEEP ? 1 << 1 : 0x0038u) };
	specials[seterianId][BODYPART_FANG].specialAttackId = seterianFang;
	generateSpecial(&specials[seterianId][BODYPART_TAIL], stats[seterianId].element, false);
	const auto seterianAirBossId{ generateMagic(Mips::Register::v0, stats[seterianId].element) };

	// Wind Boss

	// Kikinak
	// Special: Fang (Air), Claw (Neutral), Tail (Neutral)
	// Magic:

	static constexpr auto kikinakId{ ID_KIKINAK - ID_DREAM_MAN };
	bool kikinakAllowStatus{ true };
	kikinakAllowStatus = generateSpecial(&specials[kikinakId][BODYPART_FANG], stats[kikinakId].element, kikinakAllowStatus);
	kikinakAllowStatus = generateSpecial(&specials[kikinakId][BODYPART_CLAW], stats[kikinakId].element, kikinakAllowStatus);
	generateSpecial(&specials[kikinakId][BODYPART_TAIL], stats[kikinakId].element, kikinakAllowStatus);

	// Masked Boy
	// Special: Fang (Neutral), Claw (Taunt), Tail (Neutral)
	// Magic:
	// Note:

	static constexpr auto maskedBoyId{ ID_MASKED_BOY - ID_DREAM_MAN };
	bool maskedBoyAllowStatus{ true };
	maskedBoyAllowStatus = generateSpecial(&specials[maskedBoyId][BODYPART_FANG], stats[maskedBoyId].element, maskedBoyAllowStatus);
	generateSpecial(&specials[maskedBoyId][BODYPART_TAIL], stats[maskedBoyId].element, maskedBoyAllowStatus);

	// Poacher

	// Goat
	// Special: Claw (Fire)
	// Magic:
	// Note: Specials use priority

	static constexpr auto goatId{ ID_GOAT - ID_DREAM_MAN };
	const auto goatBodyPart{ m_game->random()->generate(BodyPart_t(BODYPART_COUNT - 1)) };
	specials[goatId][BODYPART_CLAW] = {};
	generateSpecial(&specials[goatId][goatBodyPart], stats[goatId].element);

	// Lagoat
	// Special: Horn (Rend Wings)
	// Magic:
	// Note: Specials use priority

	static constexpr auto lagoatId{ ID_LAGOAT - ID_DREAM_MAN };
	const auto lagoatBodyPart{ m_game->random()->generate(BodyPart_t(BODYPART_COUNT - 1)) };
	specials[lagoatId][BODYPART_HORN] = {};
	generateSpecial(&specials[lagoatId][lagoatBodyPart], stats[lagoatId].element);

	// Gigoat
	// Special: Fang (Neutral)
	// Magic:
	// Note: Specials use priority 

	static constexpr auto gigoatId{ ID_GIGOAT - ID_DREAM_MAN };
	const auto gigoatBodyPart{ m_game->random()->generate(BodyPart_t(BODYPART_COUNT - 1)) };
	specials[gigoatId][BODYPART_FANG] = {};
	generateSpecial(&specials[gigoatId][gigoatBodyPart], stats[gigoatId].element);

	// Chosen One

	// Cushidra
	// Special:
	// Magic: Fire Boss (a2) Air Boss (a2) Earth Boss (a2)
	// Note: Specials are generated in real time based on element

	static constexpr auto cushidraId{ ID_CUSHIDRA - ID_DREAM_MAN };
	const auto cushidraFireBossId{ generateMagic(Mips::Register::a2, ELEMENT_FIRE) };
	const auto cushidraAirBossId{ generateMagic(Mips::Register::a2, ELEMENT_AIR) };
	const auto cushidraEarthBossId{ generateMagic(Mips::Register::a2, ELEMENT_EARTH) };

	executable.write(specialsOffset, specials);

	if (m_game->isVersion(Version::NtscJ1)) 
	{
		const auto over_encom_bin{ m_game->file(File::OVER_ENCOM_BIN) };

		over_encom_bin->write(m_game->offset().file.over_encom_bin.minionAgniaId, minionAgniaId);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.minionAgniaId + 0x13C, minionAgniaId);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.tuturisFireBossId, tuturisFireBossId);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.delfanelEarthBossId, delfanelEarthBossId);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.seterianAirBossId, seterianAirBossId);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.seterianAirBossId - 0xC4, li_a1_seterianStatus);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.cushidraFireBossId, cushidraFireBossId);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.cushidraFireBossId + 0x4C, cushidraAirBossId);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.cushidraFireBossId + 0xD0, cushidraEarthBossId);

		// Minion in NTSCJ1 use Agni instead of Agnia
		static constexpr auto
			sw_a0_4_v0{ Mips_t(0xAC440004) },
			li_v0_1{ Mips_t(0x24020001) };

		over_encom_bin->write(m_game->offset().file.over_encom_bin.minionAgniaId + 4, sw_a0_4_v0);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.minionAgniaId + 0x1C, li_v0_1);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.minionAgniaId + 0x140, sw_a0_4_v0);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.minionAgniaId + 0x158, li_v0_1);
	}
	else
	{
		const auto over_battle_bin{ m_game->file(File::OVER_BATTLE_BIN) };

		over_battle_bin->write(m_game->offset().file.over_battle_bin.minionAgniaId, minionAgniaId);
		over_battle_bin->write(m_game->offset().file.over_battle_bin.minionAgniaId + 0x144, minionAgniaId);
		over_battle_bin->write(m_game->offset().file.over_battle_bin.tuturisFireBossId, tuturisFireBossId);
		over_battle_bin->write(m_game->offset().file.over_battle_bin.delfanelEarthBossId, delfanelEarthBossId);
		over_battle_bin->write(m_game->offset().file.over_battle_bin.seterianAirBossId, seterianAirBossId);
		over_battle_bin->write(m_game->offset().file.over_battle_bin.seterianAirBossId - 0xC4, li_a1_seterianStatus);
		over_battle_bin->write(m_game->offset().file.over_battle_bin.cushidraFireBossId, cushidraFireBossId);
		over_battle_bin->write(m_game->offset().file.over_battle_bin.cushidraFireBossId + 0x4C, cushidraAirBossId);
		over_battle_bin->write(m_game->offset().file.over_battle_bin.cushidraFireBossId + 0xD0, cushidraEarthBossId);
	}
}

void Boss::setAppearance(Boss::Appearance state) const
{
	auto rotate = [](RawFile* file, u32 offset, s32 rotation)
	{
		static constexpr auto clutSize{ 0x100u };
		auto clut{ file->read<std::array<u16, clutSize>>(offset) };
		JCUtil::rotateCLUT(clut, rotation);
		file->write(offset, clut);
	};

	struct ModelFileOffset
	{
		Model_t model;
		FileOffset fileOffset;
	};

	static constexpr std::array<ModelFileOffset, 11> mfo
	{
		MODEL_YUME, { File::MODEL_YUME_MDL, 0x2248 },
		MODEL_DMYB, { File::MODEL_DMYB_MDL, 0x840 },
		MODEL_BSGS, { File::MODEL_BSGS_MDL, 0x820 },
		MODEL_BSGE, { File::MODEL_BSGE_MDL, 0x2248 },
		MODEL_BSFS, { File::MODEL_BSFS_MDL, 0x824 },
		MODEL_BSFE, { File::MODEL_BSFE_MDL, 0x2248 },
		MODEL_BSWS, { File::MODEL_BSWS_MDL, 0x1020 },
		MODEL_BSWE, { File::MODEL_BSWE_MDL, 0x2248 },
		MODEL_KIKI, { File::MODEL_KIKI_MDL, 0x2A48 },
		MODEL_LUI_A, { File::MODEL_LUI_A_MDL, 0x2248 },
		MODEL_NAGK, { File::MODEL_NAGK_MDL, 0x2248 }
	};

	const std::unordered_set<Model_t> portraits
	{
		MODEL_YUME, MODEL_BSGE, MODEL_BSFE, MODEL_BSWE,
		MODEL_KIKI, MODEL_LUI_A, MODEL_NAGK
	};

	static constexpr ModelFileOffset mfoCushidra{ MODEL_BSZZ, { File::MODEL_BSZZ_MDL, 0x1024 } };

	auto executable{ m_game->executable() };

	const u32 goatModelsBehaviorOffset
	{
		m_game->offset().file.executable.entityModelsBehavior +
		(Entity::totalStoryMinions + static_cast<u32>(mfo.size())) * sizeof(ModelBehavior)
	};
	auto goatModelsBehavior{ executable.read<std::array<ModelBehavior, 3>>(goatModelsBehaviorOffset) };

	if (state == Boss::Appearance::ColorBasedOnElement)
	{
		const u32 statsOffset{ m_game->offset().file.executable.entityStats + ID_DREAM_MAN * sizeof(StatsStruct) };
		std::array<Element_t, Entity::totalStoryBosses - 2> elements; // -2 no Chosen One and Cushidra

		for (std::size_t i{}; i < elements.size(); ++i)
		{
			executable.read(statsOffset + i * sizeof(StatsStruct) + 6, &elements[i]);
		}

		const auto& modelsRotation{ m_sharedData->rotations() };

		// Boss
		for (std::size_t i{}; i < mfo.size(); ++i)
		{
			if (elements[i] != ELEMENT_NONE)
			{
				const auto file{ m_game->file(mfo[i].fileOffset.first) };
				const auto rotation{ static_cast<s32>(modelsRotation.at(mfo[i].model).rotation.at(elements[i])) * 2 };

				rotate(file.get(), mfo[i].fileOffset.second, rotation);

				if (portraits.contains(mfo[i].model))
				{
					rotate(file.get(), mfo[i].fileOffset.second - 0x1220, rotation);
				}
			}
		}

		// Goat
		const auto goatTextureModelId{ m_sharedData->goatTextureModelId() };
		for (std::size_t i{}; i < goatModelsBehavior.size(); ++i)
		{
			const auto element{ elements[11 + i] };
			const auto rotation
			{
				element == ELEMENT_NONE ?
				modelsRotation.at(goatTextureModelId).storyRotation :
				modelsRotation.at(goatTextureModelId).rotation.at(element)
			};
			goatModelsBehavior[i].colorRotation = rotation;
		}

		// Cushidra
		const auto rngElement{ m_game->random()->generate(Element_t(ELEMENT_COUNT - 1)) };
		const auto cushidraRotation{ static_cast<s32>(modelsRotation.at(mfoCushidra.model).rotation[rngElement]) * 2 };
		rotate(m_game->file(mfoCushidra.fileOffset.first).get(), mfoCushidra.fileOffset.second, cushidraRotation);
	}
	else // Random Color
	{
		// Boss
		for (const auto& [model, fileoffset] : mfo)
		{
			const auto file{ m_game->file(fileoffset.first) };
			const auto rng{ m_game->random()->generate(JCUtil::clutRotationLimit) };

			rotate(file.get(), fileoffset.second, rng);

			if (portraits.contains(model))
			{
				rotate(file.get(), fileoffset.second - 0x1220, rng);
			}
		}

		// Goat
		for (auto& modelBehavior : goatModelsBehavior)
		{
			modelBehavior.colorRotation = m_game->random()->generate<u8>(255);
		}

		// Cushidra
		rotate(m_game->file(mfoCushidra.fileOffset.first).get(), mfoCushidra.fileOffset.second, m_game->random()->generate(JCUtil::clutRotationLimit));
	}

	executable.write(goatModelsBehaviorOffset, goatModelsBehavior);
}

void Boss::setElementEC(bool allowNoneElement) const
{
	const auto scene_other_hunting_sce00_sbh{ m_game->file(File::SCENE_OTHER_HUNTING_SCE00_SBH) };

	static constexpr auto bossCounter{ Entity::totalECBosses - 2 }; // -2 no TX_08 and TX_09
	const auto damageModifiersOffset{ m_game->offset().file.scene_other_hunting_sce00_sbh.bossDamageModifiersEC };
	auto damageModifiers{ scene_other_hunting_sce00_sbh->read<std::array<DamageModifiers, bossCounter>>(damageModifiersOffset) };

	auto executable{ m_game->executable() };

	const u32 modelsBehaviorOffset
	{
		m_game->offset().file.executable.entityModelsBehavior +
		(Entity::totalEntity - Entity::totalECBosses) * sizeof(ModelBehavior)
	};
	auto modelsBehavior{ executable.read<std::array<ModelBehavior, bossCounter>>(modelsBehaviorOffset) };

	const Element_t minElement{ allowNoneElement ? ELEMENT_NONE : ELEMENT_FIRE };
	const auto statsOffset{ m_game->offset().file.scene_other_hunting_sce00_sbh.statsEC };

	for (s32 i{}; i < bossCounter; ++i)
	{
		const auto baseElement{ scene_other_hunting_sce00_sbh->read<Element_t>(statsOffset + i * sizeof(StatsStruct) + 6) };
		const auto rngElement{ m_game->random()->generate(minElement, Element_t(ELEMENT_COUNT - 1)) };

		if (baseElement != rngElement)
		{
			setDamageModifiers(&damageModifiers[i], rngElement);
			scene_other_hunting_sce00_sbh->write(statsOffset + i * sizeof(StatsStruct) + 6, rngElement);

			if (rngElement != ELEMENT_NONE)
			{
				const auto& rotation{ m_sharedData->rotation(MODEL_TX00 + i) };
				modelsBehavior[i].colorRotation = rotation.rotation[rngElement];
			}
		}
	}

	scene_other_hunting_sce00_sbh->write(damageModifiersOffset, damageModifiers);
	executable.write(modelsBehaviorOffset, modelsBehavior);
}

void Boss::setSpecialMagicEC() const
{
	const std::unordered_map<Element_t, Special_t> availableSpecial
	{
		{ ELEMENT_NONE, { SPECIAL_NEUTRAL0 } },
		{ ELEMENT_FIRE, { SPECIAL_FIRE_ATTACK } },
		{ ELEMENT_AIR, { SPECIAL_WIND_ATTACK } },
		{ ELEMENT_EARTH, { SPECIAL_EARTH_ATTACK } },
		{ ELEMENT_WATER, { SPECIAL_WATER_ATTACK } }
	};

	auto generateSpecial = [&](SpecialStruct* special, Element_t element, bool allowStrong = true)
	{
		static constexpr std::array<Special_t, 5> availableSpecialStrong
		{
			SPECIAL_POISON, SPECIAL_SLEEP, SPECIAL_FLESH_TO_STONE, SPECIAL_ABSORB_HP,
			SPECIAL_CRITICAL
		};

		static constexpr auto elemProba{ 75.f };

		if (!allowStrong || m_game->random()->generateProba(elemProba))
		{
			const auto elementToUse
			{
				element == ELEMENT_NONE ? m_game->random()->generate(element, Element_t(ELEMENT_COUNT - 1)) : element
			};

			special->specialAttackId = availableSpecial.at(elementToUse);
			return allowStrong;
		}
		else
		{
			special->specialAttackId = availableSpecialStrong[m_game->random()->generate(availableSpecialStrong.size() - 1)];
			return false;
		}
	};

	bool allowAdMumuls{ true };

	auto generateMagic = [&](Mips::Register rgt, Element_t element)
	{
		static constexpr std::array<Magic_t, ELEMENT_COUNT> availableMagics
		{
			MAGIC_AGNI | MAGIC_AGNIS | MAGIC_AGNIA | MAGIC_FIRE_BOSS,
			MAGIC_MALTI | MAGIC_MALTIS | MAGIC_MALTIA | MAGIC_AIR_BOSS,
			MAGIC_ULVI | MAGIC_ULVIS | MAGIC_ULVIA | MAGIC_EARTH_BOSS,
			MAGIC_VAHLI | MAGIC_VAHLIS | MAGIC_VAHLIA | MAGIC_WATER_BOSS
		};

		static constexpr auto adMumulsRate{ 1.f / 3.f * 100.f };

		if (allowAdMumuls && m_game->random()->generateProba(adMumulsRate))
		{
			allowAdMumuls = false;
			return Mips::li(rgt, static_cast<u16>(Utility::bitToInt(Magic_t(MAGIC_AD_MUMULS))));
		}
		else
		{
			const auto elementToUse{ element == ELEMENT_NONE ? m_game->random()->generate(Element_t(ELEMENT_COUNT - 1)) : element };
			return Mips::li(rgt, static_cast<u16>(Utility::bitToInt(m_game->random()->generateBit(availableMagics[elementToUse]))));
		}
	};

	const auto scene_other_hunting_sce00_sbh{ m_game->file(File::SCENE_OTHER_HUNTING_SCE00_SBH) };

	static constexpr auto bossCounter{ Entity::totalECBosses - 2 }; // -2 no TX_08 and TX_09

	const auto statsOffset{ m_game->offset().file.scene_other_hunting_sce00_sbh.statsEC };
	const auto stats{ scene_other_hunting_sce00_sbh->read<std::array<StatsStruct, bossCounter>>(statsOffset) };

	const auto specialsOffset{ m_game->offset().file.scene_other_hunting_sce00_sbh.bossSpecialsEC };
	auto specials{ scene_other_hunting_sce00_sbh->read<std::array<SpecialsArray, bossCounter>>(specialsOffset) };

	// Palooja
	// Special: Horn (Neutral) + Fang (Critical)
	// Magic:

	static constexpr auto paloojaId{ ID_PALOOJA - ID_PALOOJA };
	bool paloojaAllowStrong{ true };
	paloojaAllowStrong = generateSpecial(&specials[paloojaId][BODYPART_HORN], stats[paloojaId].element, paloojaAllowStrong);
	generateSpecial(&specials[paloojaId][BODYPART_FANG], stats[paloojaId].element, paloojaAllowStrong);
		
	// Ticker
	// Special:
	// Magic: Ulvi (a2), Ulvia (a2)

	static constexpr auto tickerId{ ID_TICKER - ID_PALOOJA };
	const auto tickerUlvi{ generateMagic(Mips::Register::a2, stats[tickerId].element) };
	const auto tickerUlvia{ generateMagic(Mips::Register::a2, stats[tickerId].element) };

	// Jeechwo
	// Special: Horn (Rend Wings), Fang (Break Legs), Claw (Neutral)
	// Magic:
	// Note: Unused Horn & Fang

	static constexpr auto jeechwoId{ ID_JEECHWO - ID_PALOOJA };
	generateSpecial(&specials[jeechwoId][BODYPART_CLAW], stats[jeechwoId].element, false);

	// Kolna
	// Special:
	// Magic: Ad Mumuls (a2)

	static constexpr auto kolnaId{ ID_KOLNA - ID_PALOOJA };
	const auto kolnaAdMumuls{ generateMagic(Mips::Register::a2, stats[kolnaId].element) };

	// Sicanjal
	// Special: Tail (Neutral)
	// Magic:

	static constexpr auto sicanjalId{ ID_SICANJAL - ID_PALOOJA };
	generateSpecial(&specials[sicanjalId][BODYPART_TAIL], stats[sicanjalId].element, false);

	// Timaios
	// Special: Fang (Absorb Mana) + Claw (Absorb HP)
	// Magic:

	static constexpr auto timaiosId{ ID_TIMAIOS - ID_PALOOJA };
	bool timaiosAllowStrong{ true };
	timaiosAllowStrong = generateSpecial(&specials[timaiosId][BODYPART_FANG], stats[timaiosId].element, timaiosAllowStrong);
	generateSpecial(&specials[timaiosId][BODYPART_CLAW], stats[timaiosId].element, timaiosAllowStrong);

	// Yolga

	// Klarrgas
	// Special: Horn (Flesh To Stone), Fang (Sleep), Claw (Poison)
	// Magic:

	scene_other_hunting_sce00_sbh->write(specialsOffset, specials);

	if (m_game->isVersion(Version::NtscJ1))
	{
		const auto over_encom_bin{ m_game->file(File::OVER_ENCOM_BIN) };

		over_encom_bin->write(m_game->offset().file.over_encom_bin.tickerUlvi, tickerUlvi);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.tickerUlvi - 8, tickerUlvia);
		over_encom_bin->write(m_game->offset().file.over_encom_bin.kolnaAdMumuls, kolnaAdMumuls);
	}
	else
	{
		const auto over_battle_bin{ m_game->file(File::OVER_BATTLE_BIN) };

		over_battle_bin->write(m_game->offset().file.over_battle_bin.tickerUlvi, tickerUlvi);
		over_battle_bin->write(m_game->offset().file.over_battle_bin.tickerUlvi - 8, tickerUlvia);
		over_battle_bin->write(m_game->offset().file.over_battle_bin.kolnaAdMumuls, kolnaAdMumuls);
	}
}

void Boss::setAppearanceEC(Boss::AppearanceEC_t state) const
{
	std::unordered_map<Model_t, std::unique_ptr<RawFile>> minions;

	for (std::size_t i{}; i < Model::Minion::modelsEC.size(); ++i)
	{
		minions[Model::Minion::modelsEC[i]] = m_game->file(Model::Minion::filesEC[i]);
	}

	auto executable{ m_game->executable() };

	if (state & Boss::APPEARANCE_EC_RANDOM_NEW_APPEARANCE)
	{
		for (const auto& [model, file] : minions)
		{
			std::vector<Merge::ModelInterp> modelsInterp;

			static constexpr s16 maxInterpolation{ 3072 }, iterationRate{ 512 };

			const auto rate{ m_game->random()->generate(0.2f, 1.f) * maxInterpolation };
			const auto iteration{ maxInterpolation / iterationRate + 1 - static_cast<s32>(rate / iterationRate) };
			modelsInterp.reserve(iteration);

			for (s32 i{}; i < iteration; ++i)
			{
				const auto rngModel{ m_game->random()->generate(Model::Minion::models.size() - 1) };

				modelsInterp.emplace_back
				(
					m_sharedData->model(Model::Minion::models[rngModel]),
					m_game->random()->generate(static_cast<s16>(rate / 1.5f), static_cast<s16>(rate))
				);
			}

			auto mainModel{ m_sharedData->model(model) };
			const auto firstAnimPtr{ executable.read<u32>(m_game->offset().file.executable.tableOfModelAnimationsPtr) };
			const auto animPtr{ executable.read<u32>(m_game->offset().file.executable.tableOfModelAnimationsPtr + sizeof(u32) * model) };
			const auto offset{ (animPtr - firstAnimPtr + m_game->offset().file.executable.tableOfModelAnimations) & ~0x80000000 };

			Model::Minion::Animation::UnpackedOffsetSize unpackedOffsetSize
			{
				executable.read<std::array<std::pair<u16, u16>, Model::Minion::Animation::nbUnpacked>>(offset + 0x3E)
			};

			Merge::Model(&mainModel, unpackedOffsetSize, modelsInterp);
			file->write(0, *mainModel.data(), mainModel.size());
		}
	}

	if (state & Boss::APPEARANCE_EC_TEXTURE_RANDOM)
	{
		const auto scene_other_hunting_sce00_sbh{ m_game->file(File::SCENE_OTHER_HUNTING_SCE00_SBH) };

		static constexpr auto bossCounter{ Entity::totalECBosses - 2 }; // -2 no TX_08 and TX_09

		const auto statsEC
		{
			scene_other_hunting_sce00_sbh->read<std::array<StatsStruct, bossCounter>>
			(m_game->offset().file.scene_other_hunting_sce00_sbh.statsEC)
		};

		const auto& modelsRotation{ m_sharedData->rotations() };

		const u32 modelsBehaviorOffset
		{
			m_game->offset().file.executable.entityModelsBehavior +
			(Entity::totalEntity - Entity::totalECBosses) * sizeof(ModelBehavior)
		};
		auto modelsBehavior{ executable.read<std::array<ModelBehavior, bossCounter>>(modelsBehaviorOffset)};

		const auto includeCompatible{ state & Boss::APPEARANCE_EC_TEXTURE_INCLUDE_COMPATIBLE };
		const auto totalTextures
		{
			includeCompatible ?
			Model::Minion::modelsEC.size() + Model::Minion::models.size() + 1 :
			Model::Minion::modelsEC.size()
		};

		std::vector<Model_t> texturesModels;
		texturesModels.reserve(totalTextures);

		for (const auto& model : Model::Minion::modelsEC)
		{
			texturesModels.emplace_back(model);
		}

		if (includeCompatible)
		{
			for (const auto& model : Model::Minion::models)
			{
				texturesModels.emplace_back(model);
			}
			texturesModels.emplace_back(MODEL_DMYB);
		}

		for (const auto& [model, file] : minions)
		{
			const auto rngModel{ texturesModels[m_game->random()->generate(texturesModels.size() - 1)] };
			const auto& modelConst{ m_sharedData->model(rngModel) };
			file->write
			(
				Model::Minion::Texture::begin, 
				*(modelConst.data() + (rngModel == MODEL_DMYB ? 0x82C : Model::Minion::Texture::begin)),
				Model::Minion::Texture::size
			);

			for (s32 i{}; i < bossCounter; ++i)
			{
				if (modelsBehavior[i].id == model)
				{
					const auto& modelRotation{ modelsRotation.at(rngModel) };
					const auto rotation 
					{
						statsEC[i].element == ELEMENT_NONE ? 
						modelRotation.storyRotation : 
						modelRotation.rotation.at(statsEC[i].element)
					};

					if (rotation == 0xFF)
					{
						throw JcrException{ "Model {} rotate color is uninitialized", model };
					};

					modelsBehavior[i].colorRotation = rotation;
				}
			}
		}

		executable.write(modelsBehaviorOffset, modelsBehavior);
	}

	if (state & Boss::APPEARANCE_EC_TEXTURE_RANDOM_COLOR)
	{
		for (const auto& [model, file] : minions)
		{
			auto clut{ file->read<std::array<u16, Model::Minion::Texture::clutSize>>(Model::Minion::Texture::clutBegin) };
			JCUtil::rotateCLUT(clut, m_game->random()->generate(JCUtil::clutRotationLimit));
			file->write(Model::Minion::Texture::clutBegin, clut);
		}
	}
}