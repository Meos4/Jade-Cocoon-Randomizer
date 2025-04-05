#pragma once

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"
#include "Common/Types.hpp"

#include <array>
#include <memory>

class Misc final
{
public:
	struct Color
	{
		u8 red;
		u8 green;
		u8 blue;
		u8 alpha;

		constexpr auto& operator=(u32 rgba)
		{
			red = static_cast<u8>(rgba);
			green = static_cast<u8>(rgba >> 8);
			blue = static_cast<u8>(rgba >> 16);
			alpha = static_cast<u8>(rgba >> 24);
			return *this;
		}
	};

	using HudColorArray = std::array<Misc::Color, 15>;

	Misc(Game* game, std::shared_ptr<SharedData> sharedData);

	HudColorArray hudColor() const;

	void setHudColor() const;
	void setHudColor(const Misc::HudColorArray& hud) const;
	void setNPCsVoice() const;
private:
	Game* m_game;
	std::shared_ptr<SharedData> m_sharedData;
};