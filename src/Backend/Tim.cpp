#include "Tim.hpp"

#include "Backend/JCUtil.hpp"
#include "Common/JcrException.hpp"
#include "Common/RawFile.hpp"

#include <format>

namespace Tim
{
	static constexpr auto sizeHeader{ 0x14u };

	void rotateBPP8(RawFile* file, s32 rotation, u32 offset)
	{
		const auto nbClut{ file->read<u16>(offset + 0x12) };

		for (u16 i{}; i < nbClut; ++i)
		{
			static constexpr auto clutSize{ 0x100u };
			const u32 clutOffset{ offset + Tim::sizeHeader + clutSize * sizeof(u16) * i };

			auto clut{ file->read<std::array<u16, clutSize>>(clutOffset) };
			JCUtil::rotateCLUT(clut, rotation);
			file->write(clutOffset, clut);
		}
	}

	void rotateBPP16(RawFile* file, s32 rotation, u32 offset, std::optional<u32> clutSizeLimit)
	{
		const u32 clutOffset{ offset + Tim::sizeHeader };
		const auto clutSize{ file->read<u32>(offset + 8) - 0xC };

		if (clutSizeLimit.has_value() && clutSize > clutSizeLimit.value())
		{
			throw JcrException{ "Clut size exceeded max: {} size: {}", clutSizeLimit.value(), clutSize };
		}

		std::vector<u16> clut(clutSize / sizeof(u16));
		auto* const clutPtr{ clut.data() };

		file->read(clutOffset, clutPtr, clutSize);
		JCUtil::rotateCLUT(clut, rotation);
		file->write(clutOffset, *clutPtr, clutSize);
	}
}