#pragma once

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"
#include "Common/Types.hpp"

#include <memory>

class Boss final
{
public:
	enum class Element
	{
		RandomAll,
		RandomElemental
	};

	enum class Appearance
	{
		ColorBasedOnElement,
		RandomColor
	};

	using AppearanceEC_t = u32;

	enum : AppearanceEC_t
	{
		APPEARANCE_EC_RANDOM_NEW_APPEARANCE = 1 << 0,
		APPEARANCE_EC_TEXTURE_RANDOM = 1 << 1,
		APPEARANCE_EC_TEXTURE_INCLUDE_COMPATIBLE = 1 << 2,
		APPEARANCE_EC_TEXTURE_RANDOM_COLOR = 1 << 3
	};

	Boss(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData);

	void setElement(Boss::Element state, bool allowNoneElement) const;
	void setSpecialMagic() const;
	void setAppearance(Appearance state) const;
	void setElementEC(bool allowNoneElement) const;
	void setSpecialMagicEC() const;
	void setAppearanceEC(Boss::AppearanceEC_t state) const;
private:
	std::shared_ptr<Game> m_game;
	std::shared_ptr<SharedData> m_sharedData;
};