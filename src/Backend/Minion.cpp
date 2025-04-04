#include "Minion.hpp"

#include "Backend/File.hpp"
#include "Backend/JCUtility.hpp"
#include "Backend/Merge.hpp"
#include "Backend/Mips.hpp"
#include "Backend/Model.hpp"
#include "Common/Buffer.hpp"
#include "Common/JcrException.hpp"
#include "Common/Random.hpp"

#include <array>
#include <limits>
#include <map>
#include <numeric>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <vector>
#include <numbers>

enum : std::size_t
{
	GLOBAL_GROWTH_SIZE_X,
	GLOBAL_GROWTH_SIZE_X_FLAG,
	GLOBAL_GROWTH_SIZE_Y,
	GLOBAL_GROWTH_SIZE_Y_FLAG,
	GLOBAL_GROWTH_SIZE_Z,
	GLOBAL_GROWTH_SIZE_Z_FLAG,
	GLOBAL_GROWTH_SIZE_COUNT
};

enum : std::size_t
{
	GROWTH_SIZE_X_SHADOW,
	GROWTH_SIZE_Y_COORDINATE,
	GROWTH_SIZE_Y_SHADOW,
	GROWTH_SIZE_SHADOW_FLAG,

	GROWTH_SIZE_UNK,
	GROWTH_SIZE_UNK2,
	GROWTH_SIZE_UNK3,
	GROWTH_SIZE_UNK_FLAG,

	GROWTH_SIZE_TORSO_X,
	GROWTH_SIZE_TORSO_Z,
	GROWTH_SIZE_TORSO_Y,
	GROWTH_SIZE_TORSO_FLAG,

	GROWTH_SIZE_RIGHT_TRICEPS_X,
	GROWTH_SIZE_RIGHT_TRICEPS_Z,
	GROWTH_SIZE_RIGHT_TRICEPS_Y,
	GROWTH_SIZE_RIGHT_TRICEPS_FLAG,

	GROWTH_SIZE_RIGHT_FOREARM_X,
	GROWTH_SIZE_RIGHT_FOREARM_Z,
	GROWTH_SIZE_RIGHT_FOREARM_Y,
	GROWTH_SIZE_RIGHT_FOREARM_FLAG,

	GROWTH_SIZE_RIGHT_HAND_X,
	GROWTH_SIZE_RIGHT_HAND_Z,
	GROWTH_SIZE_RIGHT_HAND_Y,
	GROWTH_SIZE_RIGHT_HAND_FLAG,

	GROWTH_SIZE_LEFT_TRICEPS_X,
	GROWTH_SIZE_LEFT_TRICEPS_Z,
	GROWTH_SIZE_LEFT_TRICEPS_Y,
	GROWTH_SIZE_LEFT_TRICEPS_FLAG,

	GROWTH_SIZE_LEFT_FOREARM_X,
	GROWTH_SIZE_LEFT_FOREARM_Z,
	GROWTH_SIZE_LEFT_FOREARM_Y,
	GROWTH_SIZE_LEFT_FOREARM_FLAG,

	GROWTH_SIZE_LEFT_HAND_X,
	GROWTH_SIZE_LEFT_HAND_Z,
	GROWTH_SIZE_LEFT_HAND_Y,
	GROWTH_SIZE_LEFT_HAND_FLAG,

	GROWTH_SIZE_NECK_X,
	GROWTH_SIZE_NECK_Z,
	GROWTH_SIZE_NECK_Y,
	GROWTH_SIZE_NECK_FLAG,

	GROWTH_SIZE_HEAD_X,
	GROWTH_SIZE_HEAD_Y,
	GROWTH_SIZE_HEAD_Z,
	GROWTH_SIZE_HEAD_FLAG,

	GROWTH_SIZE_MOUTH_X,
	GROWTH_SIZE_MOUTH_Y,
	GROWTH_SIZE_MOUTH_Z,
	GROWTH_SIZE_MOUTH_FLAG,

	GROWTH_SIZE_RIGHT_WING_X,
	GROWTH_SIZE_RIGHT_WING_Z,
	GROWTH_SIZE_RIGHT_WING_Y,
	GROWTH_SIZE_RIGHT_WING_FLAG,

	GROWTH_SIZE_RIGHT_WING2_X,
	GROWTH_SIZE_RIGHT_WING2_Z,
	GROWTH_SIZE_RIGHT_WING2_Y,
	GROWTH_SIZE_RIGHT_WING2_FLAG,

	GROWTH_SIZE_LEFT_WING_X,
	GROWTH_SIZE_LEFT_WING_Z,
	GROWTH_SIZE_LEFT_WING_Y,
	GROWTH_SIZE_LEFT_WING_FLAG,

	GROWTH_SIZE_LEFT_WING2_X,
	GROWTH_SIZE_LEFT_WING2_Z,
	GROWTH_SIZE_LEFT_WING2_Y,
	GROWTH_SIZE_LEFT_WING2_FLAG,

	GROWTH_SIZE_PELVIS_X,
	GROWTH_SIZE_PELVIS_Z,
	GROWTH_SIZE_PELVIS_Y,
	GROWTH_SIZE_PELVIS_FLAG,

	GROWTH_SIZE_RIGHT_HIP_X,
	GROWTH_SIZE_RIGHT_HIP_Z,
	GROWTH_SIZE_RIGHT_HIP_Y,
	GROWTH_SIZE_RIGHT_HIP_FLAG,

	GROWTH_SIZE_RIGHT_LEG_X,
	GROWTH_SIZE_RIGHT_LEG_Z,
	GROWTH_SIZE_RIGHT_LEG_Y,
	GROWTH_SIZE_RIGHT_LEG_FLAG,

	GROWTH_SIZE_RIGHT_FOOT_X,
	GROWTH_SIZE_RIGHT_FOOT_Y,
	GROWTH_SIZE_RIGHT_FOOT_Z,
	GROWTH_SIZE_RIGHT_FOOT_FLAG,

	GROWTH_SIZE_LEFT_HIP_X,
	GROWTH_SIZE_LEFT_HIP_Z,
	GROWTH_SIZE_LEFT_HIP_Y,
	GROWTH_SIZE_LEFT_HIP_FLAG,

	GROWTH_SIZE_LEFT_LEG_X,
	GROWTH_SIZE_LEFT_LEG_Z,
	GROWTH_SIZE_LEFT_LEG_Y,
	GROWTH_SIZE_LEFT_LEG_FLAG,

	GROWTH_SIZE_LEFT_FOOT_X,
	GROWTH_SIZE_LEFT_FOOT_Y,
	GROWTH_SIZE_LEFT_FOOT_Z,
	GROWTH_SIZE_LEFT_FOOT_FLAG,

	GROWTH_SIZE_TAIL_X,
	GROWTH_SIZE_TAIL_Z,
	GROWTH_SIZE_TAIL_Y,
	GROWTH_SIZE_TAIL_FLAG,

	GROWTH_SIZE_TAIL_TIP_X,
	GROWTH_SIZE_TAIL_TIP_Z,
	GROWTH_SIZE_TAIL_TIP_Y,
	GROWTH_SIZE_TAIL_TIP_FLAG,

	GROWTH_SIZE_COUNT
};

struct MinionModelProperties
{
	std::array<s32, 6> unk;
	std::array<s16, GLOBAL_GROWTH_SIZE_COUNT> xyz;
	std::array<s16, GROWTH_SIZE_COUNT> properties;
	std::array<u16, 4> propertiesFlag;
	std::array<u16, 4> propertiesTransparency;
	std::array<u8, 100> empty;
};

Minion::Minion(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
	: m_game(std::move(game)), m_sharedData(std::move(sharedData))
{
}

Id_Entity_t Minion::dreamMinion() const
{
	const auto scene_other_dream_sce00a_sbh{ m_game->staticFile(File::SCENE_OTHER_DREAM_SCE00A_SBH) };
	return scene_other_dream_sce00a_sbh->read<Id_Entity_t>(m_game->offset().file.scene_other_dream_sce00a_sbh.dreamMinionId);
}

Id_Entity_t Minion::korisMinion() const
{
	const auto scene_field1_gate_sce00a_sbh{ m_game->staticFile(File::SCENE_FIELD1_GATE_SCE00A_SBH) };
	return scene_field1_gate_sce00a_sbh->read<Id_Entity_t>(m_game->offset().file.scene_field1_gate_sce00a_sbh.korisMinionId);
}

void Minion::setSpawnStory(Minion::SpawnStory state) const
{
	const bool isNtscJ{ m_game->isVersion(Version::NtscJ1, Version::NtscJ2) };
	const auto over_game_bin{ m_game->file(File::OVER_GAME_BIN) };

	if (state == Minion::SpawnStory::RandomRealtime && !isNtscJ)
	{
		const auto generateValidMinionOffset{ m_game->customCodeOffset(sizeof(MipsFn::GenerateValidMinion)) };

		m_game->executable().write(generateValidMinionOffset.file, generateValidMinionFn());
		over_game_bin->write(m_game->offset().file.over_game_bin.spawnStory, Mips::jal(generateValidMinionOffset.game));
	}
	else if (state == Minion::SpawnStory::RandomPremade)
	{
		std::array<Id_Entity_t, 51> minionsId;

		for (auto& id : minionsId)
		{
			id = generateMinion(!isNtscJ);
		}

		over_game_bin->write(m_game->offset().file.over_game_bin.tableOfStoryMinionsId, minionsId);
	}

	if (!isNtscJ)
	{
		static constexpr auto li_v0_1{ Mips::li(Mips::Register::v0, 1) };
		static constexpr std::array<Mips_t, 2> li_v0_1_nop{ li_v0_1, 0x00000000 };

		over_game_bin->write(m_game->offset().file.over_game_bin.readFlagRotateSpawn, li_v0_1_nop);
		over_game_bin->write(m_game->offset().file.over_game_bin.readFlagRotateSpawn + 0x98, li_v0_1);
	}
}

void Minion::setSpawnEC() const
{
	const auto generateValidMinionOffset{ m_game->customCodeOffset(sizeof(MipsFn::GenerateValidMinion)) };

	const std::array<Mips_t, 2> jal_generateValidMinionEC
	{
		Mips::jal(generateValidMinionOffset.game),
		0x00000000 // nop
	};

	const auto scene_other_hunting_sce00_sbh{ m_game->file(File::SCENE_OTHER_HUNTING_SCE00_SBH) };

	m_game->executable().write(generateValidMinionOffset.file, generateValidMinionFn());
	scene_other_hunting_sce00_sbh->write(m_game->offset().file.scene_other_hunting_sce00_sbh.neutralSpawnEC, jal_generateValidMinionEC);
	// Elemental Spawn
	scene_other_hunting_sce00_sbh->write(m_game->offset().file.scene_other_hunting_sce00_sbh.neutralSpawnEC + 0x80, jal_generateValidMinionEC);
}

void Minion::setDreamMinion() const
{
	const bool isNtscJ{ m_game->isVersion(Version::NtscJ1, Version::NtscJ2) };
	const auto scene_other_dream_sce00a_sbh{ m_game->file(File::SCENE_OTHER_DREAM_SCE00A_SBH) };
	scene_other_dream_sce00a_sbh->write(m_game->offset().file.scene_other_dream_sce00a_sbh.dreamMinionId, generateMinion(!isNtscJ));
}

void Minion::setDreamMinion(Id_Entity_t id) const
{
	if (!isValidMinionForStory(id))
	{
		throw JcrException{ "Attempt to use an invalid dream minion : {}", id };
	}
	const auto scene_other_dream_sce00a_sbh{ m_game->file(File::SCENE_OTHER_DREAM_SCE00A_SBH) };
	scene_other_dream_sce00a_sbh->write(m_game->offset().file.scene_other_dream_sce00a_sbh.dreamMinionId, id);
}

void Minion::setKorisMinion() const
{
	const bool isNtscJ{ m_game->isVersion(Version::NtscJ1, Version::NtscJ2) };
	const auto scene_field1_gate_sce00a_sbh{ m_game->file(File::SCENE_FIELD1_GATE_SCE00A_SBH) };
	scene_field1_gate_sce00a_sbh->write(m_game->offset().file.scene_field1_gate_sce00a_sbh.korisMinionId, generateMinion(!isNtscJ));
}

void Minion::setKorisMinion(Id_Entity_t id) const
{
	if (!isValidMinionForStory(id))
	{
		throw JcrException{ "Attempt to use an invalid koris minion : {}", id };
	}
	const auto scene_field1_gate_sce00a_sbh{ m_game->file(File::SCENE_FIELD1_GATE_SCE00A_SBH) };
	scene_field1_gate_sce00a_sbh->write(m_game->offset().file.scene_field1_gate_sce00a_sbh.korisMinionId, id);
}

void Minion::setStats(Minion::Stats_t state) const
{
	auto executable{ m_game->executable() };
	auto stats{ executable.read<std::array<StatsStruct, Entity::totalStoryMinions>>(m_game->offset().file.executable.entityStats) };

	if (state & Minion::STATS_SHUFFLE_BETWEEN_MINIONS)
	{
		const auto statsConst{ stats };
		m_game->random()->shuffle(&stats);

		for (std::size_t i{}; i < stats.size(); ++i)
		{
			stats[i].element = statsConst[i].element;
		}
	}

	if (state & Minion::STATS_SHUFFLE_STATS)
	{
		for (auto& id : stats)
		{
			std::array<u8, 6> availableStats 
			{ 
				id.attack, id.magicAttack, id.defense,
				id.magicDefense, id.speed, id.critical 
			};

			m_game->random()->shuffle(&availableStats);

			id.attack = availableStats[0];
			id.magicAttack = availableStats[1];
			id.defense = availableStats[2];
			id.magicDefense = availableStats[3];
			id.speed = availableStats[4];
			id.critical = availableStats[5];
		}
	}

	executable.write(m_game->offset().file.executable.entityStats, stats);
}

void Minion::setSpecialMagic(const std::unordered_set<Special_t>& special, Magic_t magic) const
{
	const std::unordered_map<Element_t, std::unordered_set<Special_t>> specialsAllowed
	{
		{
			ELEMENT_NONE,
		{
			SPECIAL_NEUTRAL, SPECIAL_POISON, SPECIAL_SLEEP, SPECIAL_FLESH_TO_STONE,
			SPECIAL_DESTROY_MANA, SPECIAL_ABSORB_HP, SPECIAL_ABSORB_MANA, SPECIAL_ALL_ENEMIES,
			SPECIAL_CRITICAL, SPECIAL_BREAK_LEGS, SPECIAL_REND_WINGS
		}
		},
		{ ELEMENT_FIRE, { SPECIAL_FIRE_ATTACK } },
		{ ELEMENT_AIR, { SPECIAL_WIND_ATTACK } },
		{ ELEMENT_EARTH, { SPECIAL_EARTH_ATTACK } },
		{ ELEMENT_WATER, { SPECIAL_WATER_ATTACK } }
	};

	const std::unordered_map<Element_t, Magic_t> magicsAllowed
	{
		{ ELEMENT_FIRE, MAGIC_AGNI | MAGIC_AGNIS | MAGIC_AGNIA },
		{ ELEMENT_AIR, MAGIC_MALTI | MAGIC_MALTI | MAGIC_MALTIA },
		{ ELEMENT_EARTH, MAGIC_ULVI | MAGIC_ULVIS | MAGIC_ULVIA },
		{ ELEMENT_WATER, MAGIC_VAHLI | MAGIC_VAHLIS | MAGIC_VAHLIA }
	};

	const std::unordered_map<Element_t, Magic_t> magicsStatusAllowed
	{
		{ ELEMENT_FIRE, MAGIC_AD_MUMULS | MAGIC_DEVA_AGNI },
		{ ELEMENT_AIR, MAGIC_SEL_SELAHM | MAGIC_AD_SLAHM | MAGIC_DEVA_MALTI },
		{ ELEMENT_EARTH, MAGIC_SELAHM_VENON | MAGIC_AD_VENON | MAGIC_AD_ROQUA | MAGIC_DEVA_ULVI },
		{ ELEMENT_WATER, MAGIC_MEDINA | MAGIC_MEDINIA | MAGIC_SELAHM | MAGIC_DEVA_VALI }
	};

	std::unordered_map<Element_t, std::vector<Special_t>> availableSpecials;
	std::unordered_map<Element_t, Magic_t> availableMagics, availableMagicsStatus;

	for (const auto& [elem, specials] : specialsAllowed)
	{
		for (const auto& spe : specials)
		{
			if (special.contains(spe))
			{
				availableSpecials[elem].emplace_back(spe);
			}
		}
	}

	for (const auto& [elem, magics] : magicsAllowed)
	{
		const auto availableElemMagics{ magics & magic };
		if (availableElemMagics)
		{
			availableMagics[elem] = availableElemMagics;
		}
	}

	for (const auto& [elem, magics] : magicsStatusAllowed)
	{
		const auto availableElemMagicsStatus{ magics & magic };
		if (availableElemMagicsStatus)
		{
			availableMagicsStatus[elem] = availableElemMagicsStatus;
		}	
	}

	auto generateSpecialMagic = [&](SpecialsArray* specials, Magic_t* magics, const StatsStruct& stats, Element_t element)
	{
		static constexpr auto rateDamage{ 2.f / 3.f * 100.f };

		std::vector<BodyPart_t> availableBodyParts
		{
			BODYPART_HORN, BODYPART_FANG, BODYPART_CLAW,
			BODYPART_TAIL, BODYPART_FOOT
		};

		Magic_t magicDamage{}, magicStatus{};

		if (availableMagics.contains(element))
		{
			magicDamage = availableMagics.at(element);
		}

		if (availableMagicsStatus.contains(element))
		{
			magicStatus = availableMagicsStatus.at(element);
		}

		auto generateSpecial = [&]()
		{
			if (availableBodyParts.empty())
			{
				return false;
			}

			const auto bodyPartRng{ m_game->random()->generate(availableBodyParts.size() - 1) };
			auto& specialBody{ (*specials)[availableBodyParts[bodyPartRng]] };

			if (!availableSpecials[element].empty() &&
				(availableSpecials[ELEMENT_NONE].empty() || m_game->random()->generateProba(rateDamage)))
			{
				const auto id{ availableSpecials[element][m_game->random()->generate(availableSpecials[element].size() - 1)] };
				specialBody.specialAttackId = id;
			}
			else if (!availableSpecials[ELEMENT_NONE].empty())
			{
				const auto id{ availableSpecials[ELEMENT_NONE][m_game->random()->generate(availableSpecials[ELEMENT_NONE].size() - 1)] };
				specialBody.specialAttackId = id;
			}
			else
			{
				return false;
			}

			specialBody.isEnabled = 1;
			availableBodyParts.erase(availableBodyParts.begin() + bodyPartRng);

			static constexpr auto enhancementRate{ 65.f / 221.f * 100.f };
			if (m_game->random()->generateProba(enhancementRate))
			{
				specialBody.isEnhanced = 1;
				specialBody.enhancement = m_game->random()->generate<EnhancementBodyPart_t>(ENHANCEMENT_BODYPART_POWER, ENHANCEMENT_BODYPART_CRITICAL);
			}

			return true;
		};

		auto generateMagic = [&]()
		{
			if (magicDamage && (!magicStatus || m_game->random()->generateProba(rateDamage)))
			{
				*magics |= m_game->random()->generateBitAndErase(&magicDamage);
			}
			else if (magicStatus)
			{
				*magics |= m_game->random()->generateBitAndErase(&magicStatus);
			}
			else
			{
				return false;
			}

			return true;
		};
	
		if (stats.defense > stats.magicDefense)
		{
			if (!generateSpecial())
			{
				generateMagic();
			}
		}
		else
		{
			if (!generateMagic())
			{
				generateSpecial();
			}
		}

		const auto attackRate{ static_cast<float>(stats.attack) - static_cast<float>(stats.magicAttack) };
		const auto specialRate{ 50.f + attackRate * 0.25f };
		auto nbRate{ (600.f / stats.statsTotal()) * 50.f };

		for (s32 i{}; i < 9; ++i)
		{
			if (m_game->random()->generateProba(nbRate))
			{
				if (m_game->random()->generateProba(specialRate))
				{
					if (!generateSpecial())
					{
						generateMagic();
					}
				}
				else
				{
					if (!generateMagic())
					{
						generateSpecial();
					}
				}
			}
			nbRate /= 2.f;
		}
	};

	auto executable{ m_game->executable() };

	std::array<SpecialsArray, Entity::totalStoryMinions> specialsStory{};
	std::array<SpecialsArray, Entity::totalECMinions> specialsEC{};
	std::array<Magic_t, Entity::totalStoryMinions> magicsStory{};
	std::array<Magic_t, Entity::totalECMinions> magicsEC{};
	const auto statsStory{ executable.read<std::array<StatsStruct, Entity::totalStoryMinions>>(m_game->offset().file.executable.entityStats) };

	for (s32 i{}; i < Entity::totalStoryMinions; ++i)
	{
		generateSpecialMagic(&specialsStory[i], &magicsStory[i], statsStory[i], statsStory[i].element);
	}

	for (s32 i{}; i < Entity::totalECMinions; ++i)
	{
		generateSpecialMagic(&specialsEC[i], &magicsEC[i], statsStory[i / 4], i % 4);
	}

	const auto
		offsetSpecialsStory{ m_game->offset().file.executable.entitySpecials },
		offsetMagicsStory{ m_game->offset().file.executable.entityMagics };

	executable.write(offsetSpecialsStory, specialsStory);
	executable.write(offsetMagicsStory, magicsStory);

	if (m_game->isVersion(Version::NtscJ1, Version::NtscJ2))
	{
		const auto scene_other_hunting_sce00_sbh{ m_game->file(File::SCENE_OTHER_HUNTING_SCE00_SBH) };

		scene_other_hunting_sce00_sbh->write(m_game->offset().file.scene_other_hunting_sce00_sbh.specialsEC, specialsEC);
		scene_other_hunting_sce00_sbh->write(m_game->offset().file.scene_other_hunting_sce00_sbh.magicsEC, magicsEC);
	}
	else
	{
		const u32
			offsetSpecialsEC{ offsetSpecialsStory + (Entity::totalStoryMinions + Entity::totalStoryBosses) * sizeof(SpecialsArray) },
			offsetMagicsEC{ offsetMagicsStory + (Entity::totalStoryMinions + Entity::totalStoryBosses) * sizeof(Magic_t) };

		executable.write(offsetSpecialsEC, specialsEC);
		executable.write(offsetMagicsEC, magicsEC);
	}

	static constexpr auto b_3{ Mips_t(0x10000003) };

	if (m_game->isVersion(Version::NtscJ1))
	{
		m_game->file(File::OVER_ENCOM_BIN)->write(m_game->offset().file.over_encom_bin.storyMinionAI, b_3);
	}
	else
	{
		m_game->file(File::OVER_BATTLE_BIN)->write(m_game->offset().file.over_battle_bin.storyMinionAI, b_3);
	}

	// Vatolka specials are set directly in the game code
	const auto branchVatolkaSpecials{ m_game->isVersion(Version::NtscJ1) ? Mips_t(0x10000026) : Mips_t(0x10000027) };
	executable.write(m_game->offset().file.executable.branchSetVatolkaSpecials, branchVatolkaSpecials);
}

void Minion::setAppearance(Minion::Appearance_t state) const
{
	std::unordered_map<Model_t, std::unique_ptr<RawFile>> minions;

	for (std::size_t i{}; i < Model::Minion::models.size(); ++i)
	{
		minions[Model::Minion::models[i]] = m_game->file(Model::Minion::files[i]);
	}
	
	auto executable{ m_game->executable() };

	if (state & Minion::APPEARANCE_RANDOM_NEW_MINION)
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

	if (state & (Minion::APPEARANCE_GROWTH_SIZE_SHUFFLE | Minion::APPEARANCE_GROWTH_SIZE_INVERT))
	{
		auto propertiesBegin = [](RawFile* file)
		{
			return file->read<u32>(0xC) + 0xC;
		};

		static constexpr auto nbProperties{ 5u };

		if (state & Minion::APPEARANCE_GROWTH_SIZE_SHUFFLE)
		{
			for (const auto& [model, file] : minions)
			{
				std::vector<u32> availableProperties(nbProperties);
				std::iota(availableProperties.begin(), availableProperties.end(), 0);

				const auto& modelConst{ m_sharedData->model(model) };
				const auto fileBegin{ propertiesBegin(file.get()) };
				const auto* const propertiesBeginPtr{ modelConst.data() + fileBegin };
				
				for (u32 i{}; i < nbProperties; ++i)
				{
					const auto rngProperties{ m_game->random()->generate(availableProperties.size() - 1) };

					file->write
					(
						fileBegin + i * sizeof(MinionModelProperties),
						*(propertiesBeginPtr + availableProperties[rngProperties] * sizeof(MinionModelProperties)),
						sizeof(MinionModelProperties)
					);

					availableProperties.erase(availableProperties.begin() + rngProperties);
				}
			}
		}
		else // Invert
		{
			for (const auto& [model, file] : minions)
			{
				const auto& modelConst{ m_sharedData->model(model) };
				const auto fileBegin{ propertiesBegin(file.get()) };
				const auto* const propertiesBeginPtr{ modelConst.data() + fileBegin };

				for (u32 i{}; i < nbProperties; ++i)
				{
					file->write
					(
						fileBegin + i * sizeof(MinionModelProperties),
						*(propertiesBeginPtr + (nbProperties - 1 - i) * sizeof(MinionModelProperties)),
						sizeof(MinionModelProperties)
					);
				}
			}
		}	
	}

	if (state & (Minion::APPEARANCE_MODEL_RANDOM | Minion::APPEARANCE_TEXTURE_RANDOM))
	{
		const auto statsStory
		{
			executable.read<std::array<StatsStruct, Entity::totalStoryMinions + Entity::totalStoryBosses>>
			(m_game->offset().file.executable.entityStats)
		};

		const auto& modelsRotation{ m_sharedData->rotations() };

		auto modelsBehavior
		{
			executable.read<std::array<ModelBehavior, Entity::totalEntity - 2>>
			(m_game->offset().file.executable.entityModelsBehavior)
		};

		auto throwIfUninitialized = [](Model_t model, u8 rotation)
		{
			if (rotation == 0xFF)
			{
				throw JcrException{ "Model {} rotate color is uninitialized", model };
			}
		};

		if (state & Minion::APPEARANCE_MODEL_RANDOM)
		{
			for (s32 i{}; i < Entity::totalStoryMinions; ++i)
			{
				const auto rngModel{ Model::Minion::models[m_game->random()->generate(Model::Minion::models.size() - 1)] };
				const auto& modelRotation{ modelsRotation.at(rngModel) };
				const auto rotation
				{
					statsStory[i].element == modelRotation.element ? 
					modelRotation.storyRotation : 
					modelRotation.rotation.at(statsStory[i].element)
				};
				throwIfUninitialized(rngModel, rotation);

				modelsBehavior[i].id = rngModel;
				modelsBehavior[i].colorRotation = rotation;
			}

			for (s32 i{}; i < Entity::totalECMinions; ++i)
			{
				const auto rngModel{ Model::Minion::models[m_game->random()->generate(Model::Minion::models.size() - 1)] };
				const auto rotation{ modelsRotation.at(rngModel).rotation[i % 4] };
				throwIfUninitialized(rngModel, rotation);

				const auto minionId{ ID_PATARAID + i };
				modelsBehavior[minionId].id = rngModel;
				modelsBehavior[minionId].colorRotation = rotation;
			}
		}

		if (state & Minion::APPEARANCE_TEXTURE_RANDOM)
		{
			const auto includeCompatible{ state & Minion::APPEARANCE_TEXTURE_INCLUDE_COMPATIBLE };
			const auto totalTextures
			{
				includeCompatible ?
				Model::Minion::models.size() + Model::Minion::modelsEC.size() + 1 :
				Model::Minion::models.size()
			};

			std::vector<Model_t> texturesModels;
			texturesModels.reserve(totalTextures);

			for (const auto& model : Model::Minion::models)
			{
				texturesModels.emplace_back(model);
			}

			if (includeCompatible)
			{
				for (const auto& model : Model::Minion::modelsEC)
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

				if (model == MODEL_ZIRA)
				{
					m_sharedData->setGoatTextureModelId(rngModel);
				}

				for (s32 i{}; i < Entity::totalStoryMinions; ++i)
				{
					if (modelsBehavior[i].id == model)
					{
						const auto& modelRotation{ modelsRotation.at(rngModel) };
						const auto rotation
						{
							statsStory[i].element == modelRotation.element ?
							modelRotation.storyRotation : 
							modelRotation.rotation.at(statsStory[i].element)
						};
						throwIfUninitialized(modelsBehavior[i].id, rotation);
						modelsBehavior[i].colorRotation = rotation;
					}
				}

				for (s32 i{}; i < Entity::totalECMinions; ++i)
				{
					const auto minionId{ ID_PATARAID + i };
					if (modelsBehavior[minionId].id == model)
					{
						const auto rotation{ modelsRotation.at(rngModel).rotation[i % 4] };
						throwIfUninitialized(modelsBehavior[minionId].id, rotation);
						modelsBehavior[minionId].colorRotation = rotation;
					}
				}
			}
		}

		executable.write(m_game->offset().file.executable.entityModelsBehavior, modelsBehavior);	
	}

	if (state & Minion::APPEARANCE_TEXTURE_RANDOM_COLOR)
	{
		for (const auto& [model, file] : minions)
		{
			auto clut{ file->read<std::array<u16, Model::Minion::Texture::clutSize>>(Model::Minion::Texture::clutBegin) };
			JCUtility::rotateCLUT(clut, m_game->random()->generate(JCUtility::clutRotationLimit));
			file->write(Model::Minion::Texture::clutBegin, clut);
		}
	}
}

Id_Entity_t Minion::generateMinion(bool eternalCorridor) const
{
	auto validMinionId = [](Id_Entity_t id) -> Id_Entity_t
	{
		return id > ID_VATOLKA_W ? id + Entity::totalStoryBosses : id;
	};

	return validMinionId(m_game->random()->generate(eternalCorridor ? ID_TYTON - Entity::totalStoryBosses : ID_VATOLKA_W));
}

MipsFn::GenerateValidMinion Minion::generateValidMinionFn() const
{
	const auto randOffset{ m_game->offset().game.randSeed };
	const auto 
		right{ static_cast<u16>(randOffset) }, 
		left{ static_cast<s16>(randOffset) >= 0 ? static_cast<u16>(randOffset >> 16) : static_cast<u16>((randOffset >> 16) + 1) };

	MipsFn::GenerateValidMinion generateValidMinionFn
	{
		0x3C0341C6, // lui v1, 0x41C6
		Mips_t(0x3C020000 + left), // lui v0, 0xXXXX
		Mips_t(0x8C420000 + right), // lw v0, 0xXXXX
		0x34634E6D, // ori v1, 0x4E6D
		0x00430018, // mult v0, v1
		0x00002012, // mflo a0
		0x24823039, // addiu v0, a0, 0x3039
		Mips_t(0x3C010000 + left), // lui at, 0xXXXX
		Mips_t(0xAC220000 + right), // sw v0, 0xXXXX(at)
		0x00021602, // srl v0, 0x18
		0x2C430029, // sltiu v1, v0, 0x29
		0x1C600005, // bgtz v1, 5
		0x2C430039, // sltiu v1, v0, 0x39
		0x1C60FFF2, // bgtz v1, -14
		0X2C4300C9, // sltiu v1, v0, 0xC9
		0x1860FFF0, // bgtz v1, -16
		0x00000000, // nop
		0x03E00008, // jr ra
		0x00000000  // nop
	};

	return generateValidMinionFn;
}

bool Minion::isValidMinionForStory(Id_Entity_t id) const
{
	const bool 
		isNtscJ{ m_game->isVersion(Version::NtscJ1, Version::NtscJ2) },
		firstPortion{ id >= ID_MARRDREG && id < ID_DREAM_MAN };

	if (isNtscJ)
	{
		return firstPortion;
	}
	else
	{
		return firstPortion || (id > ID_CUSHIDRA && id < ID_PALOOJA);
	}
}