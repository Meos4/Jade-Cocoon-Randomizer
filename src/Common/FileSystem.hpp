#pragma once

#include <filesystem>

namespace FileSystem
{
	void copyTree(const std::filesystem::path& src, const std::filesystem::path& dst);
	bool remove(const std::filesystem::path& path);
}