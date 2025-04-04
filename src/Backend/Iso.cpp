#include "Iso.hpp"

#include "Common/RawFile.hpp"

namespace Iso
{
	bool generateCue(const std::filesystem::path& isoPath)
	{
		auto cue{ isoPath };
		cue.replace_extension(".cue");
		try
		{
			std::ofstream stream{ cue };
			stream << "FILE " << isoPath.filename() << " BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n";
			return true;
		}
		catch ([[maybe_unused]] const std::exception&) // Using some chars catches an exception
		{
			if (std::filesystem::exists(cue))
			{
				std::filesystem::remove(cue);
			}
			return false;
		}
	}

	std::optional<Version> findVersion(const std::filesystem::path& isoPath)
	{
		if (!std::filesystem::is_regular_file(isoPath))
		{
			return std::nullopt;
		}

		RawFile iso{ isoPath };

		using OffsetPattern = std::pair<u32, SBuffer<11>>;
		static constexpr std::array<OffsetPattern, static_cast<std::size_t>(Version::Count)> ver
		{{
			{ 0x0007DA33, {0x53, 0x4C, 0x50, 0x53, 0x5F, 0x30, 0x31, 0x37, 0x2E, 0x32, 0x39} },
			{ 0x0007CF8F, {0x53, 0x4C, 0x50, 0x53, 0x5F, 0x39, 0x31, 0x31, 0x2E, 0x35, 0x34} },
			{ 0x0007DF47, {0x53, 0x4C, 0x55, 0x53, 0x5F, 0x30, 0x30, 0x38, 0x2E, 0x35, 0x34} },
			{ 0x0007DE6B, {0x53, 0x4C, 0x45, 0x53, 0x5F, 0x30, 0x32, 0x32, 0x2E, 0x30, 0x31} },
			{ 0x0007DF1F, {0x53, 0x4C, 0x45, 0x53, 0x5F, 0x30, 0x32, 0x32, 0x2E, 0x30, 0x32} },
			{ 0x0007DEC3, {0x53, 0x4C, 0x45, 0x53, 0x5F, 0x30, 0x32, 0x32, 0x2E, 0x30, 0x33} },
			{ 0x0007DEDB, {0x53, 0x4C, 0x45, 0x53, 0x5F, 0x30, 0x32, 0x32, 0x2E, 0x30, 0x35} },
			{ 0x0007DED3, {0x53, 0x4C, 0x45, 0x53, 0x5F, 0x30, 0x32, 0x32, 0x2E, 0x30, 0x36} }
		}};

		static constexpr auto maxOffset{ (*std::max_element(ver.begin(), ver.end(),
			[](const auto& a, const auto& b) { return a.first < b.first; })).first };

		if (iso.size() < maxOffset + sizeof(OffsetPattern::second))
		{
			return std::nullopt;
		}

		for (std::size_t i{}; i < ver.size(); ++i)
		{
			const auto& [offset, pattern]{ ver[i] };
			if (iso.read<std::remove_const_t<decltype(pattern)>>(offset) == pattern)
			{
				return static_cast<Version>(i);
			}
		}

		return std::nullopt;
	}
}