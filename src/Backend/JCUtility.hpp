#pragma once

#include "Common/Types.hpp"

#include <span>

namespace JCUtility
{
	struct RGBF
	{
		u16 r;
		u16 g;
		u16 b;
		u16 flag;
	};

	inline constexpr auto clutRotationLimit{ 360 };

	constexpr JCUtility::RGBF rgbf(u16 clr)
	{
		constexpr u8 timRGBLimit{ 0x1F };

		return
		{
			static_cast<u16>(clr & timRGBLimit),
			static_cast<u16>((clr >> 5) & timRGBLimit),
			static_cast<u16>((clr >> 10) & timRGBLimit),
			static_cast<u16>(clr & 0x8000)
		};
	}

	void rotateCLUT(std::span<u16> clut, s32 rotation);
};