#pragma once

#include "Backend/Version.hpp"

#include <filesystem>
#include <optional>

namespace Iso
{
    bool generateCue(const std::filesystem::path& isoPath);
	std::optional<Version> findVersion(const std::filesystem::path& isoPath);
    void dump(const std::filesystem::path* iso, const std::filesystem::path* configXml, const std::filesystem::path* files);
    bool make(const std::filesystem::path* iso, const std::filesystem::path* configXml);
}