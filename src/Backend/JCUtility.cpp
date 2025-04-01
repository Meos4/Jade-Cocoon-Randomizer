#include "JCUtility.hpp"

#include <algorithm>

namespace JCUtility
{
	void rotateCLUT(std::span<u16> clut, s32 rotation)
	{
		for (auto& clr : clut)
		{
			clr = JCUtility::rotateHueClr(clr, rotation);
		}
	}
}