#pragma once

#include <filesystem>

namespace JCTools
{
	void unpacker(const std::filesystem::path& srcExe, const std::filesystem::path& srcData, const std::filesystem::path& dest);
	void repacker(const std::filesystem::path& srcExe, const std::filesystem::path& srcData,
		const std::filesystem::path& destExe, const std::filesystem::path& destData);
}