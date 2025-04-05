#pragma once

#include "Common/Types.hpp"

using Version_t = s32;
enum class Version : Version_t
{
	NtscJ1,
	NtscJ2,
	NtscU,
	PalEn,
	PalFr,
	PalDe,
	PalEs,
	PalIt,
	Count
};