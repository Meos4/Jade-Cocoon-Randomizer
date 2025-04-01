#include "JCUtility.hpp"

namespace JCUtility
{
	void rotateCLUT(std::span<u16> clut, s32 rotation)
	{
		for (auto& clr : clut)
		{
			clr = JCUtility::rotateHueClr(clr, rotation);
		}
	}

	void blackAndWhiteCLUT(std::span<u16> clut, JCUtility::BlackAndWhiteMethod method)
	{
		for (auto& clr : clut)
		{
			clr = JCUtility::blackAndWhiteClr(clr, method);
		}
	}
}