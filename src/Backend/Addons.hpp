#pragma once

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"
#include "Common/Types.hpp"

#include <memory>

class Addons final
{
public:
	using Nuzlocke_t = u32;

	enum : Nuzlocke_t
	{
		NUZLOCKE_ONE_CAPTURE = 1 << 0,
		NUZLOCKE_DEFINITIVE_DEATH = 1 << 1
	};

	enum class Difficulty
	{
		Hard,
		Extreme
	};

	Addons(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData);

	void setNuzlocke(Addons::Nuzlocke_t state) const;
	void setDifficulty(Addons::Difficulty state) const;
	void setSkipTutorial(bool skipKoris) const;
	void setX2Framerate() const;
	void setItemQuantityLimit(u8 limit) const;
	void setLevelCapEC(u8 levelCap) const;
	void setShowHiddenStats() const;
	void setPalToNtsc() const;
private:
	std::shared_ptr<Game> m_game;
	std::shared_ptr<SharedData> m_sharedData;
};