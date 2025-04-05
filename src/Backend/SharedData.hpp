#pragma once

#include "Backend/Entity.hpp"
#include "Backend/Game.hpp"
#include "Common/Buffer.hpp"

#include <memory>
#include <unordered_map>

struct RotationElement
{
	std::array<u8, ELEMENT_COUNT> rotation{ 0xFF, 0xFF, 0xFF, 0xFF };
	u8 storyRotation{ 0xFF };
	Element_t element{ ELEMENT_NONE };
};

class SharedData final
{
public:
	SharedData(Game* game);

	void read();

	const Buffer& model(Model_t id) const;
	const std::unordered_map<Model_t, RotationElement>& rotations() const;
	const RotationElement& rotation(Model_t id) const;
	Model_t goatTextureModelId() const;

	void setGoatTextureModelId(Model_t model);
private:
	Game* m_game;

	std::unordered_map<Model_t, Buffer> m_models;
	std::unordered_map<Model_t, RotationElement> m_rotations;

	Model_t m_goatTextureModelId{ MODEL_ZIRA };
};