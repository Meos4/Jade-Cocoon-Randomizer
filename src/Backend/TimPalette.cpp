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

	void rotateCLUT(std::span<u16> clut, s32 rotation, const std::bitset<TimPalette::clutSize>& skip)
	{
		for (std::size_t i{}; i < clut.size(); ++i)
		{
			if (!skip[i])
			{
				clut[i] = TimPalette::rotateHueClr(clut[i], rotation);
			}
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