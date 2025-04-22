#include "TimPalette.hpp"

namespace TimPalette
{
	void rotateCLUT(std::span<u16> clut, s32 rotation)
	{
		for (auto& clr : clut)
		{
			clr = TimPalette::rotateHueClr(clr, rotation);
		}
	}

	void blackAndWhiteCLUT(std::span<u16> clut, TimPalette::BlackAndWhiteMethod method)
	{
		for (auto& clr : clut)
		{
			clr = TimPalette::blackAndWhiteClr(clr, method);
		}
	}
}