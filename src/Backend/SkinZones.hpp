#pragma once

#include "Backend/Entity.hpp"
#include "Backend/TimPalette.hpp"
#include "Common/RawFile.hpp"
#include "Common/Types.hpp"

#include <array>
#include <bitset>
#include <span>

namespace SkinZones
{
	struct Rearrangement
	{
		std::array<u16, TimPalette::clutSize> palette;
		std::bitset<TimPalette::clutSize> protectedSlots;
	};

	struct MaskedTexture
	{
		u32 pixelsOffset;
		u16 width;
		u16 height;
		const u8* mask;
	};

	Rearrangement rearrangeCLUT(RawFile* file, Model_t model, u32 clutOffset);
	Rearrangement rearrangeTextures(RawFile* file, u32 clutOffset, std::span<const MaskedTexture> textures);
}