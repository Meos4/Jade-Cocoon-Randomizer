#pragma once

#include "Backend/Version.hpp"

namespace VersionUtil
{
	const char* text(Version version);
	const char* serial(Version version);
}