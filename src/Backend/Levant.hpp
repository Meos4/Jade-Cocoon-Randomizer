#pragma once

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"
#include "Common/Types.hpp"

#include <memory>

class Levant final
{
public:
	using Animation_t = u32;

	enum : Animation_t
	{
		ANIMATION_BETA_USING_ITEM = 1 << 0,
		ANIMATION_RANDOM_WEAPONS_POSTURE = 1 << 1
	};

	Levant(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData);

	void setBaseStats() const;
	void setAnimation(Levant::Animation_t state) const;
	void setFluteStyling() const;
private:
	std::shared_ptr<Game> m_game;
	std::shared_ptr<SharedData> m_sharedData;
};