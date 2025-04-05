#pragma once

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"

#include <memory>

class Shop final
{
public:
	Shop(Game* game, std::shared_ptr<SharedData> sharedData);

	void setWeapon() const;
	void setArmor() const;
	void setOther() const;
	void setItem() const;
	void setEternalCorridorUnlockAll() const;
private:
	Game* m_game;
	std::shared_ptr<SharedData> m_sharedData;
};