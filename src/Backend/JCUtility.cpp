#include "JCUtility.hpp"

#include <algorithm>

namespace JCUtility
{
	void rotateCLUT(std::span<u16> clut, s32 rotation)
	{
		rotation %= clutRotationLimit;

		for (auto& clr : clut)
		{
			auto [r, g, b, flag] { JCUtility::rgbf(clr) };

			const auto maxRGB{ std::max({r, g, b}) };

			if (!maxRGB)
			{
				continue;
			}

			const auto
				minRGB{ std::min({r, g, b}) },
				difMin{ static_cast<u16>(maxRGB - minRGB) },
				quo1{ static_cast<u16>(((difMin << 5) - difMin) / maxRGB) };

			if (!quo1)
			{
				continue;
			}

			const auto
				difR{ static_cast<u16>(maxRGB - r) },
				difG{ static_cast<u16>(maxRGB - g) },
				difB{ static_cast<u16>(maxRGB - b) };

			enum
			{
				MODE_B, MODE_R, MODE_G
			} mode{ MODE_B };

			if (b < r)
			{
				mode = MODE_R;

				if (r < g)
				{
					mode = MODE_G;
				}
			}
			else if (b < g)
			{
				mode = MODE_G;
			}

			u16 quo2;

			if (mode == MODE_R)
			{
				quo2 = ((difB - difG) << 5) / difMin;
			}
			else if (mode == MODE_G)
			{
				quo2 = (((difR - difB) << 5) / difMin) + 0x40;
			}
			else // MODE_B
			{
				quo2 = (((difG - difR) << 5) / difMin) + 0x80;
			}

			if (static_cast<s16>(quo2) < 0)
			{
				quo2 += 0xC0;
			}

			if (quo2 == u16(-1))
			{
				continue;
			}

			quo2 += static_cast<u16>(rotation / 2 + rotation / 32);
			quo2 %= 0xC0;

			auto unk{ static_cast<s32>(quo2) }, unk2{ static_cast<s32>(quo1) << 5 };

			unk >>= 5;
			unk <<= 5;
			unk = quo2 - unk;
			unk <<= 0x10;
			unk >>= 0x10;

			auto get = [maxRGB](s32 val) -> u16
			{
				val = maxRGB * (0x3E0 - val) + 0x1F0;
				return static_cast<u16>(((((static_cast<s64>(val) * signed(0x84210843)) >> 32) + val) >> 9) - (val >> 0x1F));
			};

			const auto
				one{ get(unk2) },
				two{ get(quo1 * (0x20 - unk)) },
				three{ get(quo1 * unk) };

			switch (quo2 >> 5)
			{
			case 0:
				r = maxRGB;
				g = two;
				b = one;
				break;
			case 1:
				r = three;
				g = maxRGB;
				b = one;
				break;
			case 2:
				r = one;
				g = maxRGB;
				b = two;
				break;
			case 3:
				r = one;
				g = three;
				b = maxRGB;
				break;
			case 4:
				r = two;
				g = one;
				b = maxRGB;
				break;
			case 5:
				r = maxRGB;
				g = one;
				b = three;
				break;
			default:
				continue;
			}

			clr = flag | r | (g << 5) | (b << 10);
		}
	}
}