#include "Path.hpp"

namespace Path
{
	inline constexpr auto
		configXmlFilename{ "config.xml" },
		dataDirectory{ "DATA" },
		filesDirectory{ "Files" };

	std::filesystem::path configXmlPath(const std::filesystem::path& gameDirectory)
	{
		return gameDirectory / Path::configXmlFilename;
	}

	std::filesystem::path filesDirectoryPath(const std::filesystem::path& gameDirectory)
	{
		return gameDirectory / Path::filesDirectory;
	}

	std::filesystem::path dataDirectoryPath(const std::filesystem::path& filesDirectory)
	{
		return filesDirectory / Path::dataDirectory;
	}

	std::filesystem::path executablePath(const std::filesystem::path& gameDirectory, const std::filesystem::path& exeFilename)
	{
		return Path::filesDirectoryPath(gameDirectory) / exeFilename;
	}

	std::filesystem::path builderDirectory(const std::filesystem::path& gameDirectory)
	{
		return gameDirectory.parent_path() / gameDirectory.filename() += "_Builder";
	}
}