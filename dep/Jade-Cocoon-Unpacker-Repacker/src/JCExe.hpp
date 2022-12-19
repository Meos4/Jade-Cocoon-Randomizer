#pragma once

#include "Types.hpp"

#include <filesystem>
#include <vector>
#include <optional>
#include <array>
#include <unordered_map>

class JCExe final
{
public:
	enum class Version
	{
		NtscJ1,
		NtscJ2,
		NtscU,
		PalEn,
		PalFr,
		PalDe,
		PalEs,
		PalIt,
		Prototype_D05_M08_Y1999_15H48
	};

	struct Offset
	{
		u32 pattern,
			data001FileInfoBegin,
			data001FileInfoEnd,
			voiceXaFileInfoBegin,
			movieStrFileInfoBegin,
			movieStrFileInfoEnd;
	};

	struct FilenamePathVersion
	{
		std::filesystem::path path;
		JCExe::Version version;
	};

	JCExe(JCExe::Version version);

	u32 nbData001Files() const;
	u32 nbMovieStrFiles() const;
	const JCExe::Offset& offset() const;
	const char* toString() const;
	std::vector<const char*> data001FilesPath() const;

	static std::optional<FilenamePathVersion> findFilenamePathAndVersion(const std::filesystem::path& path);
private:
	struct VersionInfo
	{
		const char* text;
		std::array<u8, 11> pattern;
		JCExe::Offset offset;
	};

	JCExe::Version m_version;

	static const std::array<const char*, 1079> s_data001FilesPath;
	static const std::unordered_map<JCExe::Version, JCExe::VersionInfo> s_versionInfo;
};