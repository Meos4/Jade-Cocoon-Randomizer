#pragma once

enum class Version
{
	NtscJ1,
	NtscJ2,
	NtscU,
	PalEn,
	PalFr,
	PalDe,
	PalEs,
	PalIt
};

namespace NtscJ1
{
	// Necessary otherwise during battle PLCOM.BIN will exceed
	// the size of executable and overwrite the custom code
	inline constexpr auto customCodeShift{ 0x3C78u };
}