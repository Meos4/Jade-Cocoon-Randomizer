#include "GameTree.hpp"

#include "JCTools.hpp"

#include "Backend/Iso.hpp"
#include "Backend/Path.hpp"
#include "Common/FileSystem.hpp"
#include "Common/JcrException.hpp"

static void checkRegularFile(const std::filesystem::path& file)
{
	static constexpr auto fileExceptionFormat{ "\"{}\" file doesn't exist in \"{}\"" };

	if (!std::filesystem::is_regular_file(file))
	{
		throw JcrException{ fileExceptionFormat, file.filename().string(), file.parent_path().string() };
	}
}

GameTree::GameTree(std::filesystem::path&& directory, const std::filesystem::path& exeFilename)
	: m_directory(directory), m_exePath(Path::executablePath(m_directory, exeFilename))
{
}

std::unique_ptr<RawFile> GameTree::file(const char* file) const
{
	const auto filePath{ m_directory / file };
	checkRegularFile(filePath);
	return std::make_unique<RawFile>(filePath);
}

RawFile GameTree::executable() const
{
	checkRegularFile(m_exePath);
	return RawFile{ m_exePath };
}

void GameTree::repackDATA001() const
{
	const auto filesPath{ Path::filesDirectoryPath(m_directory) };
	JCTools::repacker(filesPath, m_directory, filesPath, Path::dataDirectoryPath(filesPath));
}

bool GameTree::removeDATA001() const
{
	return FileSystem::remove(Path::dataDirectoryPath(Path::filesDirectoryPath(m_directory)) / "DATA.001");
}

void GameTree::createIso(const std::filesystem::path* destPath) const
{
	const auto configXml{ Path::configXmlPath(m_directory) };
	if (!Iso::make(destPath, &configXml))
	{
		throw JcrException{ "Unable to repack iso" };
	}
}

bool GameTree::exists() const
{
	return std::filesystem::exists(m_directory);
}

void GameTree::copy(const std::filesystem::path& dst) const
{
	FileSystem::copyTree(m_directory, dst);
}

void GameTree::copyFile(const char* one, const char* two) const
{
	std::filesystem::copy_file(m_directory / one, m_directory / two, std::filesystem::copy_options::overwrite_existing);
}

bool GameTree::remove() const
{
	return FileSystem::remove(m_directory);
}

const std::filesystem::path& GameTree::directory() const
{
	return m_directory;
}

const std::filesystem::path& GameTree::exePath() const
{
	return m_exePath;
}