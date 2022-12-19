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

	enum class Ost
	{
		Random,
		Shuffle
	};

	Forest(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData);

	Forest::OstArray ost() const;

	void setPaletteColor() const;
	void setBattleMaps() const;
	void setOst(Forest::Ost state) const;
	void setOst(const Forest::OstArray& ostsId) const;
private:
	std::shared_ptr<Game> m_game;
	std::shared_ptr<SharedData> m_sharedData;
};