#pragma once

#include <array>
#include <filesystem>
#include <type_traits>

namespace Path
{
	inline constexpr auto defaultGameDirectory{ "JCR_Temp" };

	using CStringPlatform =
	#ifdef _WIN32
		const wchar_t*;
	#else
		const char*;
	#endif

	std::filesystem::path configXmlPath(const std::filesystem::path& gameDirectory);
	std::filesystem::path filesDirectoryPath(const std::filesystem::path& gameDirectory);
	std::filesystem::path dataDirectoryPath(const std::filesystem::path& filesDirectory);
	std::filesystem::path executablePath(const std::filesystem::path& gameDirectory, const std::filesystem::path& executableName);
	std::filesystem::path builderDirectory(const std::filesystem::path& gameDirectory);

	using CStringPlatformPtr = std::remove_cv_t<std::remove_pointer_t<Path::CStringPlatform>>**;
}