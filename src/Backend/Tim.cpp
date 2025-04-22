#include "Tim.hpp"

#include "Backend/TimPalette.hpp"
#include "Common/JcrException.hpp"
#include "Common/RawFile.hpp"

#include <format>

namespace Tim
{
	static constexpr auto sizeHeader{ sizeof(Tim::Header) };

	bool isValid(const Tim::Header& header)
	{
		return header.magic == 0x10 && 
			header.type == TYPE_4BPP || header.type == TYPE_8BPP || header.type == TYPE_16BPP &&
			header.paletteX < 1024 && header.paletteY < 512;
	}

	void rotateBPP8(RawFile* file, s32 rotation, u32 offset)
	{
		const auto header{ file->read<Tim::Header>(offset) };

		if (!Tim::isValid(header) || header.type != TYPE_8BPP)
		{
			throw JcrException{ "Invalid 8BPP TIM file" };
		}

		for (u16 i{}; i < header.nbPalettes; ++i)
		{
			static constexpr auto clutSize{ 0x100u };
			const u32 clutOffset{ offset + Tim::sizeHeader + clutSize * sizeof(u16) * i };

			auto clut{ file->read<std::array<u16, clutSize>>(clutOffset) };
			TimPalette::rotateCLUT(clut, rotation);
			file->write(clutOffset, clut);
		}
	}

	void rotateBPP16(RawFile* file, s32 rotation, u32 offset, std::optional<u32> clutSizeLimit)
	{
		const auto header{ file->read<Tim::Header>(offset) };

		if (!Tim::isValid(header) || header.type != TYPE_16BPP)
		{
			throw JcrException{ "Invalid 16BPP TIM file" };
		}

		const auto clutSize{ header.offset - 0xC };

		if (clutSizeLimit.has_value() && clutSize > clutSizeLimit.value())
		{
			throw JcrException{ "Clut size exceeded max: {} size: {}", clutSizeLimit.value(), clutSize };
		}

		std::vector<u16> clut(clutSize / sizeof(u16));
		auto* const clutPtr{ clut.data() };
		const u32 clutOffset{ offset + Tim::sizeHeader };

		file->read(clutOffset, clutPtr, clutSize);
		TimPalette::rotateCLUT(clut, rotation);
		file->write(clutOffset, *clutPtr, clutSize);
	}
}