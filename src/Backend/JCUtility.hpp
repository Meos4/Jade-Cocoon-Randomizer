#pragma once

#include "Common/Types.hpp"

#include <span>

namespace JCUtility
{
	inline constexpr auto clutRotationLimit{ 360 };

	void rotateCLUT(std::span<u16> clut, s32 rotation);
};