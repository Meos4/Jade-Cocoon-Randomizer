#pragma once

#include <array>
#include <filesystem>
#include <type_traits>

namespace Path
{
	inline constexpr auto
		jcrTempDirectory{ "JCR_Temp" },
		configXmlFilename{ "config.xml" },
		dataDirectory{ "DATA" },
		filesDirectory{ "Files" };

	using CStringPlatform =
	#ifdef _WIN32
		const wchar_t*;
	#else
		const char*;
	#endif

	using CStringPlatformPtr = std::remove_cv_t<std::remove_pointer_t<Path::CStringPlatform>>**;

	std::array<Path::CStringPlatform, 7> dumpIsoArgs(
		const std::filesystem::path& isoPath, 
		const std::filesystem::path& configXmlPath, 
		const std::filesystem::path& filesPath);
	std::array<Path::CStringPlatform, 7> makeIsoArgs(const std::filesystem::path& isoPath, const std::filesystem::path& configXmlPath);
}