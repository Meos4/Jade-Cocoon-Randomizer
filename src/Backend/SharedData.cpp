#include "SharedData.hpp"

#include "Backend/File.hpp"
#include "Backend/Model.hpp"
#include "Common/JcrException.hpp"

SharedData::SharedData(Game* game)
	: m_game(game)
{
}

void SharedData::read()
{
	// .MDL
	for (u32 i{}; i < Model::Minion::nbUniqueModel; ++i)
	{
		m_models[Model::Minion::models[i]] = m_game->file(Model::Minion::files[i])->readFile();
	}

	for (s32 i{}; i < Entity::totalECBosses - 2; ++i)
	{
		m_models[Model::Minion::modelsEC[i]] = m_game->file(Model::Minion::filesEC[i])->readFile();
	}

	m_models[MODEL_DMYB] = m_game->file(File::MODEL_DMYB_MDL)->readFile();

	// Palette Rotation
	auto executable{ m_game->executable() };

	const auto statsStory
	{
		executable.read<std::array<StatsStruct, Entity::totalStoryMinions + Entity::totalStoryBosses>>
		(m_game->offset().file.executable.entityStats)
	};

	auto modelsBehavior
	{
		executable.read<std::array<ModelBehavior, Entity::totalEntity>>
		(m_game->offset().file.executable.entityModelsBehavior)
	};

	for (const auto& model : Model::Minion::models)
	{
		m_rotations[model] = RotationElement();
	}

	m_rotations.at(MODEL_ALPA).rotation = { 64, 96, 128, 192 };
	m_rotations.at(modelsBehavior[ID_VATOLKA_F].id).storyRotation = modelsBehavior[ID_VATOLKA_F].colorRotation;
	m_rotations.at(modelsBehavior[ID_VATOLKA_F].id).element = ELEMENT_FIRE;

	m_rotations[MODEL_YUME] = { { 145, 16, 48, 96 }, 0, ELEMENT_NONE };
	m_rotations[MODEL_DMYB] = { { 0, 24, 50, 96 }, 0, ELEMENT_FIRE };
	m_rotations[MODEL_BSGS] = { { 150, 0, 40, 96 }, 0,  ELEMENT_EARTH };
	m_rotations[MODEL_BSGE] = { { 150, 15, 32, 96 }, 0, ELEMENT_NONE };
	m_rotations[MODEL_BSFS] = { { 0, 32, 64, 110 }, 0, ELEMENT_FIRE };
	m_rotations[MODEL_BSFE] = { { 160, 20, 60, 100 }, 0, ELEMENT_NONE };
	m_rotations[MODEL_BSWS] = { { 75, 105, 140, 20 }, 0, ELEMENT_AIR };
	m_rotations[MODEL_BSWE] = { { 70, 110, 145, 20 }, 0, ELEMENT_NONE };
	m_rotations[MODEL_KIKI] = { { 145, 32, 60, 95 }, 0, ELEMENT_NONE };
	m_rotations[MODEL_LUI_A] = { { 145, 24, 48, 96 }, 0, ELEMENT_NONE };
	m_rotations[MODEL_NAGK] = { { 145, 24, 48, 96 }, 0, ELEMENT_NONE };
	m_rotations[MODEL_BSZZ] = { { 115, 155, 24, 65 }, 0, ELEMENT_NONE };

	m_rotations[MODEL_TX00] = { { 0, 192, 64, 96 }, 0, ELEMENT_FIRE };
	m_rotations[MODEL_TX01] = { { 128, 160, 0, 64 }, 0, ELEMENT_EARTH };
	m_rotations[MODEL_TX02] = { { 160, 0, 32, 96 }, 0, ELEMENT_WATER };
	m_rotations[MODEL_TX03] = { { 160, 0, 32, 96 }, 0, ELEMENT_AIR };
	m_rotations[MODEL_TX04] = { { 160, 0, 32, 96 }, 0, ELEMENT_AIR };
	m_rotations[MODEL_TX05] = { { 0, 32, 64, 96 }, 0, ELEMENT_WATER };
	m_rotations[MODEL_TX06] = { { 64, 96, 128, 0 }, 0, ELEMENT_WATER };
	m_rotations[MODEL_TX07] = { { 64, 96, 128, 160 }, 0, ELEMENT_FIRE };

	if (!m_game->isNtscJ())
	{
		m_rotations[MODEL_ASHA] = { { 155, 20, 65, 110 }, 0, ELEMENT_FIRE };
	}

	auto throwIfInvalidModelId = [this](Model_t id)
	{
		if (!m_rotations.contains(id))
		{
			throw JcrException{ "Rotations doesn't contain model id : {}", id };
		}
	};

	// -4 No Vatolka
	for (s32 i{}; i < Entity::totalStoryMinions - 4; ++i)
	{
		throwIfInvalidModelId(modelsBehavior[i].id);
		m_rotations.at(modelsBehavior[i].id).storyRotation = modelsBehavior[i].colorRotation;
		m_rotations.at(modelsBehavior[i].id).element = statsStory[i].element;
	}

	// Vatolka
	for (s32 i{}; i < 4; ++i)
	{
		const auto vatolkaId{ ID_VATOLKA_F + i };
		const auto vatolkaModelId{ modelsBehavior[vatolkaId].id };
		throwIfInvalidModelId(vatolkaModelId);
		m_rotations.at(vatolkaModelId).rotation[i] = modelsBehavior[vatolkaId].colorRotation;
	}

	for (s32 i{}; i < Entity::totalECMinions; ++i)
	{
		const auto minionId{ ID_PATARAID + i };
		const auto minionModelId{ modelsBehavior[minionId].id };
		throwIfInvalidModelId(minionModelId);
		m_rotations.at(minionModelId).rotation[i % 4] = modelsBehavior[minionId].colorRotation;
	}
}

const Buffer& SharedData::model(Model_t id) const
{
	if (!m_models.contains(id))
	{
		throw JcrException{ "Models doesn't contain model id : {}", id };
	}
	return m_models.at(id);
}

const std::unordered_map<Model_t, RotationElement>& SharedData::rotations() const
{
	return m_rotations;
}

const RotationElement& SharedData::rotation(Model_t id) const
{
	if (!m_rotations.contains(id))
	{
		throw JcrException{ "Rotations doesn't contain model id : {}", id };
	}
	return m_rotations.at(id);
}

Model_t SharedData::goatTextureModelId() const
{
	return m_goatTextureModelId;
}

void SharedData::setGoatTextureModelId(Model_t id)
{
	m_goatTextureModelId = id;
}