#pragma once

#include "Backend/Mips.hpp"
#include "Common/Types.hpp"

#include <array>

namespace MipsFn
{
	// Minion
	using GenerateValidMinion = std::array<Mips_t, 19>;

	// Addons
	using NuzlockeDefinitiveDeath = std::array<Mips_t, 23>;
	using WriteUsedMapEOBData = std::array<u8, 20>;
	using WriteUsedMapEOB = std::array<Mips_t, 44>;
	using WriteUsedMapCapture = std::array<Mips_t, 8>;
	using ReadUsedMap = std::array<Mips_t, 52>;

	using DifficultyModeStats = std::array<Mips_t, 33>;

	using AfterTutorialStateData = std::array<u8, 360>;
	using WriteAfterTutorialState = std::array<Mips_t, 38>;

	using DrawHiddenStats = std::array<Mips_t, 62>;
	using CriticalRateFromStats = std::array<Mips_t, 13>;
	using DrawCriticalRate = std::array<Mips_t, 15>;
	using DrawCriticalRateMerge = std::array<Mips_t, 14>;

	using ToggleX2Framerate = std::array<Mips_t, 20>;

	// Fixes
	using FixEntityEnhancement = std::array<Mips_t, 20>;

	using PreviewHeal = std::array<Mips_t, 3>;
}