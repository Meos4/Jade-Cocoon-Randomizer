#pragma once

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"
#include "Common/Types.hpp"

#include <memory>

class Fixes final
{
public:
	Fixes(Game* game, std::shared_ptr<SharedData> sharedData);

	void setBodyEnhancement() const;
	void setAutumnMoonEffect() const;
	void setHpMpBarsSize(u16 value) const;
	void setSpecialsModifiers() const;
private:
	Game* m_game;
	std::shared_ptr<SharedData> m_sharedData;
};