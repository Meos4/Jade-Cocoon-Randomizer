#pragma once

#include "Backend/Entity.hpp"
#include "Backend/Game.hpp"
#include "Backend/MipsFn.hpp"
#include "Backend/SharedData.hpp"
#include "Common/Types.hpp"

#include <memory>
#include <unordered_set>

class Minion final
{
public:
	enum class SpawnStory
	{
		RandomRealtime,
		RandomPremade
	};

	using Stats_t = u32;

	enum : Stats_t
	{
		STATS_SHUFFLE_BETWEEN_MINIONS = 1 << 0,
		STATS_SHUFFLE_STATS = 1 << 1
	};

	using Appearance_t = u32;

	enum : Appearance_t
	{
		APPEARANCE_RANDOM_NEW_MINION = 1 << 0,
		APPEARANCE_MODEL_RANDOM = 1 << 1,
		APPEARANCE_TEXTURE_RANDOM = 1 << 2,
		APPEARANCE_TEXTURE_INCLUDE_COMPATIBLE = 1 << 3,
		APPEARANCE_TEXTURE_RANDOM_COLOR = 1 << 4,
		APPEARANCE_GROWTH_SIZE_SHUFFLE = 1 << 5,
		APPEARANCE_GROWTH_SIZE_INVERT = 1 << 6
	};

	Minion(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData);

	Id_Entity_t dreamMinion() const;
	Id_Entity_t korisMinion() const;

	void setSpawnStory(Minion::SpawnStory state) const;
	void setSpawnEC() const;
	void setDreamMinion() const;
	void setDreamMinion(Id_Entity_t id) const;
	void setKorisMinion() const;
	void setKorisMinion(Id_Entity_t id) const;
	void setStats(Minion::Stats_t state) const;
	void setSpecialMagic(const std::unordered_set<Special_t>& special, Magic_t magic) const;
	void setAppearance(Minion::Appearance_t state) const;
private:
	Id_Entity_t generateMinion(bool eternalCorridor) const;
	MipsFn::GenerateValidMinion generateValidMinionFn() const;
	bool isValidMinionForStory(Id_Entity_t id) const;

	std::shared_ptr<Game> m_game;
	std::shared_ptr<SharedData> m_sharedData;
};