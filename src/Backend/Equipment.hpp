#pragma once

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"
#include "Common/Types.hpp"

#include <memory>

class Equipment final
{
public:
	using Weapons_t = u32;

	enum : Weapons_t
	{
		WEAPONS_RANDOM_STATS_AND_ELEMENT = 1 << 0,
		WEAPONS_RANDOM_APPEARANCE = 1 << 1
	};

	using Armors_t = u32;

	enum : Armors_t
	{
		ARMORS_RANDOM_STATS = 1 << 0,
		ARMORS_RANDOM_APPEARANCE = 1 << 1
	};

	Equipment(Game* game, std::shared_ptr<SharedData> sharedData);

	void setWeapons(Equipment::Weapons_t state) const;
	void setArmors(Equipment::Armors_t state) const;
	void setOthers() const;

	static void setDamageEffectFromWeaponIdFn(const Game& game, bool setAutumnMoonEffect);
private:
	Game* m_game;
	std::shared_ptr<SharedData> m_sharedData;
};