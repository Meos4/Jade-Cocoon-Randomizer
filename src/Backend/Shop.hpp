#pragma once

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"
#include "Common/Types.hpp"

#include <memory>

class Shop final
{
public:
	Shop(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData);

	void setWeapon() const;
	void setArmor() const;
	void setOther() const;
	void setItem() const;
	void setEternalCorridorUnlockAll() const;
private:
	std::shared_ptr<Game> m_game;
	std::shared_ptr<SharedData> m_sharedData;
};