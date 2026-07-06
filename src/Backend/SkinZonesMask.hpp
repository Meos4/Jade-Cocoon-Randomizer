#pragma once

#include "Backend/Entity.hpp"
#include "Common/Types.hpp"

namespace SkinZonesMask
{
	const u8* protectMask(Model_t model, u32 clutOffset, u32 texture);
	const u8* levantMask(Model_t model, u32 shape, u32 slot);
}