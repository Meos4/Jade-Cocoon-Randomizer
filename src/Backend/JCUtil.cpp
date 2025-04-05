#include "JCUtil.hpp"

namespace JCUtil
{
	void rotateCLUT(std::span<u16> clut, s32 rotation)
	{
		for (auto& clr : clut)
		{
			clr = JCUtil::rotateHueClr(clr, rotation);
		}
	}

	void blackAndWhiteCLUT(std::span<u16> clut, JCUtil::BlackAndWhiteMethod method)
	{
		for (auto& clr : clut)
		{
			clr = JCUtil::blackAndWhiteClr(clr, method);
		}
	}
}