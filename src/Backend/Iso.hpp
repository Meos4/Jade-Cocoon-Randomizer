#pragma once

#include "Backend/Version.hpp"

#include <filesystem>
#include <optional>

namespace Iso
{
    bool generateCue(const std::filesystem::path& isoPath);
	std::optional<Version> findVersion(const std::filesystem::path& isoPath);
}