#pragma once

#include "Common/TemplateTypes.hpp"
#include "Common/Types.hpp"

#include <optional>

class RawFile;

namespace Tim
{
	struct Header
	{
		u32 magic;
		u32 type;
		u32 offset;
		s16 paletteX;
		s16 paletteY;
		s16 paletteColors;
		s16 nbPalettes; 
	};

	enum : u32
	{
		TYPE_4BPP = 0x08,
		TYPE_8BPP = 0x09,
		TYPE_16BPP = 0x02,
	};

	bool isValid(const Tim::Header& header);	
	void rotateBPP8(RawFile* file, s32 rotation, u32 offset);
	void rotateBPP16(RawFile* file, s32 rotation, u32 offset, std::optional<u32> sizeLimit = std::nullopt);

	template <Integral... Args>
	void rotateBPP8(RawFile* file, s32 rotation, Args... offsets)
	{
		(Tim::rotateBPP8(file, rotation, static_cast<u32>(offsets)), ...);
	};
}