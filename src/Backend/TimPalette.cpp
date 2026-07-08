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

	void blackAndWhiteCLUT(std::span<u16> clut, TimPalette::BlackAndWhiteMethod method, const std::bitset<TimPalette::clutSize>& skip)
	{
		for (std::size_t i{}; i < clut.size(); ++i)
		{
			if (!skip[i])
			{
				clut[i] = TimPalette::blackAndWhiteClr(clut[i], method);
			}
		}
	}

	void setBlackOpaque(std::span<u16> clut, std::span<const u16> original)
	{
		for (std::size_t i{}; i < clut.size(); ++i)
		{
			if (clut[i] == 0 && original[i] != 0)
			{
				clut[i] = 0x8000;
			}
		}
	}
}