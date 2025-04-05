#pragma once

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"
#include "Common/Types.hpp"

#include <memory>

class Forest final
{
public:
	static constexpr auto nbForest{ 8u };
	using OstArray = std::array<s16, Forest::nbForest>;

	static constexpr s16 randomOstVal{ -1 };

	enum class Ost
	{
		Random,
		Shuffle
	};

	Forest(Game* game, std::shared_ptr<SharedData> sharedData);

	Forest::OstArray ost() const;

	void setPaletteColor() const;
	void setBattleMaps() const;
	void setOst(Forest::Ost state) const;
	void setOst(const Forest::OstArray& ostsId) const;
	void setRandomEternalCorridorOstPerCorridor() const;
private:
	Game* m_game;
	std::shared_ptr<SharedData> m_sharedData;
};