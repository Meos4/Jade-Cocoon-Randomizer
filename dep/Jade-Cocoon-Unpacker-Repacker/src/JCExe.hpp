#pragma once

#include "Types.hpp"

#include <filesystem>
#include <optional>
#include <vector>

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
		Prototype_D05_M08_Y1999_15H48,
		Count
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

	static std::optional<FilenamePathVersion> findFilenamePathAndVersion(const std::filesystem::path& path);

	u32 nbData001Files() const;
	u32 nbMovieStrFiles() const;
	const JCExe::Offset& offset() const;
	const char* toString() const;
	std::vector<const char*> data001FilesPath() const;
private:
	JCExe::Version m_version;
};