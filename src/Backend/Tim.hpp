#pragma once

#include "Common/TemplateTypes.hpp"
#include "Common/Types.hpp"

#include <optional>

class RawFile;

namespace Tim
{
	void rotateBPP8(RawFile* file, s32 rotation, u32 offset);

	template <Integral... Args>
	void rotateBPP8(RawFile* file, s32 rotation, Args... offsets)
	{
		(Tim::rotateBPP8(file, rotation, static_cast<u32>(offsets)), ...);
	};

	void rotateBPP16(RawFile* file, s32 rotation, u32 offset, std::optional<u32> sizeLimit = std::nullopt);
}