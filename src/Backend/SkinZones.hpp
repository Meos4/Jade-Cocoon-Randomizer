#pragma once

#include "Backend/Entity.hpp"
#include "Backend/TimPalette.hpp"
#include "Common/RawFile.hpp"
#include "Common/Types.hpp"

#include <array>
#include <bitset>

namespace SkinZones
{
	struct Rearrangement
	{
		std::array<u16, TimPalette::clutSize> palette;
		std::bitset<TimPalette::clutSize> protectedSlots;
	};

	Rearrangement rearrangeCLUT(RawFile* file, Model_t model, u32 clutOffset);
}