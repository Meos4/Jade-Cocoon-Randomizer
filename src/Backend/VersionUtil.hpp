#pragma once

#include "Backend/Version.hpp"
#include "Common/JcrException.hpp"

#include "JCExe.hpp"

namespace VersionUtil
{
	const char* text(Version version);
	const char* serial(Version version);

	constexpr JCExe::Version gameToJcExeVersion(Version version)
	{
		switch (version)
		{
			case Version::NtscJ1: return JCExe::Version::NtscJ1;
			case Version::NtscJ2: return JCExe::Version::NtscJ2;
			case Version::NtscU: return JCExe::Version::NtscU;
			case Version::PalEn: return JCExe::Version::PalEn;
			case Version::PalFr: return JCExe::Version::PalFr;
			case Version::PalDe: return JCExe::Version::PalDe;
			case Version::PalEs: return JCExe::Version::PalEs;
			case Version::PalIt: return JCExe::Version::PalIt;
			default: throw JcrException{ "Invalid game version : {}", static_cast<Version_t>(version) };
		}
	}

	constexpr Version jcExeToGameVersion(JCExe::Version version)
	{
		switch (version)
		{
			case JCExe::Version::NtscJ1: return Version::NtscJ1;
			case JCExe::Version::NtscJ2: return Version::NtscJ2;
			case JCExe::Version::NtscU: return Version::NtscU;
			case JCExe::Version::PalEn: return Version::PalEn;
			case JCExe::Version::PalFr: return Version::PalFr;
			case JCExe::Version::PalDe: return Version::PalDe;
			case JCExe::Version::PalEs: return Version::PalEs;
			case JCExe::Version::PalIt: return Version::PalIt;
			default: throw JcrException{ "Invalid game version : {}", static_cast<Version_t>(version) };
		}
	}
}