#pragma once

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"
#include "Common/Types.hpp"

#include <memory>

class Treasure final
{
public:
	enum class Items
	{
		Random,
		RandomByCategory
	};

	enum class Valuables
	{
		Random,
		RandomByCategory
	};

	enum class Equipments
	{
		Random,
		RandomByCategory
	};

	Treasure(Game* game, std::shared_ptr<SharedData> sharedData);

	void setItems(Treasure::Items state, bool randomSkeletonKeys) const;
	void setValuables(Treasure::Valuables state, bool randomUsableValuables) const;
	void setEquipments(Treasure::Equipments state) const;
	void setBattle(bool shuffleSkeletonKeys) const;
private:
	Game* m_game;
	std::shared_ptr<SharedData> m_sharedData;
};