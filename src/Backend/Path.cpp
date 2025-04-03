#include "Path.hpp"

#include <format>

namespace Path
{
	inline constexpr auto
		configXmlFilename{ "config.xml" },
		dataDirectory{ "DATA" },
		filesDirectory{ "Files" };

	std::filesystem::path configXmlPath(const std::filesystem::path& temp)
	{
		return std::format("{}/{}", temp.string(), Path::configXmlFilename);
	}

	std::filesystem::path filesDirectoryPath(const std::filesystem::path& temp)
	{
		return std::format("{}/{}", temp.string(), Path::filesDirectory);
	}

	std::filesystem::path dataDirectoryPath(const std::filesystem::path& filesDirectory)
	{
		return std::format("{}/{}", filesDirectory.string(), Path::dataDirectory);
	}

	std::filesystem::path executablePath(const std::filesystem::path& gameDirectory, const std::filesystem::path& exeFilename)
	{
		return Path::filesDirectoryPath(gameDirectory) / exeFilename;
	}

	std::array<Path::CStringPlatform, 7> dumpIsoArgs(
		const std::filesystem::path* isoPath, 
		const std::filesystem::path* configXmlPath, 
		const std::filesystem::path* filesPath)
	{
		return
		{
			#ifdef _WIN32
				L"",
				isoPath->c_str(),
				L"-s",
				configXmlPath->c_str(),
				L"-x",
				filesPath->c_str(),
				nullptr
			#else
				"",
				isoPath->c_str(),
				"-s",
				configXmlPath->c_str(),
				"-x",
				filesPath->c_str(),
				nullptr
			#endif	
		};	
	}

	std::array<Path::CStringPlatform, 7> makeIsoArgs(const std::filesystem::path* isoPath, const std::filesystem::path* configXmlPath)
	{
		return
		{
			#ifdef _WIN32
				L"",
				L"-y",
				L"-q",
				L"-o",
				isoPath->c_str(),
				configXmlPath->c_str(),
				nullptr
			#else
				"",
				"-y",
				"-q",
				"-o",
				isoPath->c_str(),
				configXmlPath->c_str(),
				nullptr
			#endif
		};
	}
}