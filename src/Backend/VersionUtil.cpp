#include "VersionUtil.hpp"

#include <array>

namespace VersionUtil
{
	const char* text(Version version)
	{
		static constexpr std::array<const char*, static_cast<std::size_t>(Version::Count)> texts
		{
			"NTSC-J v1", "NTSC-J v2", "NTSC-U", "PAL-EN",
			"PAL-FR", "PAL-DE", "PAL-ES", "PAL-IT"
		};

		return texts[static_cast<std::size_t>(version)];
	}

	const char* serial(Version version)
	{
		static constexpr std::array<const char*, static_cast<std::size_t>(Version::Count)> serials
		{
			"SLPS-01729", "SLPS-91154", "SLUS-00854", "SLES-02201",
			"SLES-02202", "SLES-02203", "SLES-02205", "SLES-02206"
		};

		return serials[static_cast<std::size_t>(version)];
	}
}