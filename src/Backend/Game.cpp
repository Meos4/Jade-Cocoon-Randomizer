#include "Game.hpp"

#include "Backend/File.hpp"
#include "Backend/Iso.hpp"
#include "Backend/Mips.hpp"
#include "Backend/MipsFn.hpp"
#include "Backend/Path.hpp"
#include "Backend/Util.hpp"
#include "Backend/VersionUtil.hpp"
#include "Common/Buffer.hpp"
#include "Common/FileSystem.hpp"
#include "Common/JcrException.hpp"

#include "JCExe.hpp"
#include "JCTools.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <fstream>
#include <type_traits>
#include <utility>

Game::Game(const std::filesystem::path& exeFilename, std::filesystem::path&& directoryPath, Version version)
	: m_staticTree(std::move(directoryPath), exeFilename),
	m_builderTree(Path::builderDirectory(m_staticTree.directory()), exeFilename),
	m_version(version),
	m_offset(version),
	m_data001FilesPath(std::move(JCExe{ VersionUtil::gameToJcExeVersion(version) }.data001FilesPath()))
{
	m_staticTree.removeDATA001();
}

std::unique_ptr<RawFile> Game::file(File file) const
{
	return m_builderTree.file(filePathByIndex(file));
}

RawFile Game::executable() const
{
	return m_builderTree.executable();
}

std::unique_ptr<RawFileReadOnly> Game::staticFile(File file) const
{
	return m_staticTree.fileReadOnly(filePathByIndex(file));
}

RawFileReadOnly Game::staticExecutable() const
{
	return m_staticTree.executableReadOnly();
}

const char* Game::filePathByIndex(File file) const
{
	return m_data001FilesPath[static_cast<File_t>(fileByVersion(file))];
}

void Game::expandExecutable()
{
	m_ccShift = 0;
	auto executable{ this->executable() };
	const auto offsetGHvb{ m_offset.game.heapVanillaBegin };

	const auto heapShift{ heapRandomizerBegin() - offsetGHvb };
	auto newExecutableSize{ (offsetGHvb + heapShift - gameToFileTextSectionShift()) & ~0x80000000 };
	const auto sectorRemainer{ newExecutableSize % Iso::sectorSize };
	if (sectorRemainer)
	{
		newExecutableSize += Iso::sectorSize - sectorRemainer;
	}

	executable.write(0x1C, newExecutableSize - Iso::sectorSize);
	std::filesystem::resize_file(m_builderTree.exePath(), newExecutableSize);

	const auto offsetFMFn{ m_offset.file.executable.mainFn };
	auto instructions{ executable.read<std::array<Mips_t, 2>>(offsetFMFn + 0x34) };
	auto& [luiHeap, addiuHeap]{ instructions };

	const u32 newBeginHeap{ (luiHeap << 16) + static_cast<s16>(addiuHeap) + heapShift };
	const bool isLower16Positive{ static_cast<s16>(newBeginHeap) >= 0 };

	luiHeap = ((luiHeap >> 16) << 16);
	luiHeap += isLower16Positive ? (newBeginHeap >> 16) : (newBeginHeap >> 16) + 1;
	addiuHeap = ((addiuHeap >> 16) << 16) + static_cast<u16>(newBeginHeap);

	executable.write(offsetFMFn + 0x34, instructions);
}

Game::CustomCodeOffset Game::customCodeOffset(u32 size)
{
	const auto fileHeapVanillaBegin{ (m_offset.game.heapVanillaBegin - gameToFileTextSectionShift()) & ~0x80000000 };
	const Game::CustomCodeOffset ccOffset
	{
		.file = fileHeapVanillaBegin + m_ccShift,
		.game = m_offset.game.heapVanillaBegin + m_ccShift
	};
	m_ccShift += size;

	return ccOffset;
}

u32 Game::heapRandomizerBegin() const
{
	static constexpr u32 customCodeSize
	{
		sizeof(MipsFn::GenerateValidMinion) + // Story
		sizeof(MipsFn::GenerateValidMinion) + // EC

		sizeof(MipsFn::RandomEternalCorridorOst) +

		sizeof(MipsFn::NuzlockeDefinitiveDeath) +
		sizeof(MipsFn::WriteUsedMapEOBData) +
		sizeof(MipsFn::WriteUsedMapEOB) +
		sizeof(MipsFn::WriteUsedMapCapture) +
		sizeof(MipsFn::ReadUsedMap) +
		sizeof(MipsFn::ResetFromNowhere) +

		sizeof(MipsFn::DifficultyModeStats) +

		sizeof(MipsFn::AfterTutorialStateData) +
		sizeof(MipsFn::WriteAfterTutorialState) +

		sizeof(MipsFn::DrawHiddenStats) +
		sizeof(MipsFn::CriticalRateFromStats) +
		sizeof(MipsFn::DrawCriticalRate) +
		sizeof(MipsFn::DrawCriticalRateMerge) +

		sizeof(MipsFn::ToggleX2Framerate) +

		sizeof(MipsFn::SetChestNewItemQuantityLimit) +

		sizeof(MipsFn::FixEntityEnhancement) +
		
		sizeof(MipsFn::PreviewHeal)
	};

	return m_offset.game.heapVanillaBegin + customCodeSize;
}

u32 Game::gameToFileTextSectionShift() const
{
	// Text section in NTSC-J 2 starts at 0x80018000
	return m_version == Version::NtscJ2 ? 0x17800 : 0xF800;
}

bool Game::isVanilla() const
{
	const auto [lui, addiu]{ staticExecutable().read<std::array<Mips_t, 2>>(m_offset.file.executable.mainFn + 0x34) };
	return (lui << 16) + static_cast<s16>(addiu) == m_offset.game.heapVanillaBegin;
}

bool Game::removeStaticDirectory() const
{
	return m_staticTree.remove();
}

void Game::createBuilderDirectory() const
{
	if (m_builderTree.exists() && !m_builderTree.remove())
	{
		throw JcrException{ "\"{}\" directory cannot be removed", m_builderTree.directory().string() };
	}

	m_staticTree.copy(m_builderTree.directory());
}

bool Game::isNtsc() const
{
	return isVersion(Version::NtscJ1, Version::NtscJ2, Version::NtscU);
}

bool Game::isNtscJ() const
{
	return isVersion(Version::NtscJ1, Version::NtscJ2);
}

const char* Game::versionText() const
{
	return VersionUtil::text(m_version);
}

const char* Game::serialText() const
{
	return VersionUtil::serial(m_version);
}

Version Game::version() const
{
	return m_version;
}

const Offset& Game::offset() const
{
	return m_offset;
}

const GameTree& Game::builderTree() const
{
	return m_builderTree;
}

Random* Game::random()
{
	return &m_random;
}

std::unique_ptr<Game> Game::createGame(const std::filesystem::path& isoPath, std::filesystem::path&& gameDirectory)
{
	if (Iso::findVersion(isoPath) == std::nullopt)
	{
		throw JcrException{ "\"{}\" is not a Jade Cocoon binary file", isoPath.filename().string() };
	}

	if (std::filesystem::is_directory(gameDirectory) && !FileSystem::remove(gameDirectory))
	{
		throw JcrException{ "\"{}\" directory cannot be removed", gameDirectory.string() };
	}

	std::filesystem::create_directories(gameDirectory);

	const auto
		configXmlPath{ Path::configXmlPath(gameDirectory) },
		filesDirectoryPath{ Path::filesDirectoryPath(gameDirectory) };

	Iso::dump(&isoPath, &configXmlPath, &filesDirectoryPath);
	JCTools::unpacker(filesDirectoryPath, Path::dataDirectoryPath(filesDirectoryPath), gameDirectory);

	const auto exeInfo{ JCExe::findFilenamePathAndVersion(filesDirectoryPath) };

	if (!exeInfo.has_value())
	{
		throw JcrException{ "Can't find compatible playstation executable in \"{}\"", filesDirectoryPath.string() };
	}
	else if (exeInfo.value().version == JCExe::Version::Prototype_D05_M08_Y1999_15H48)
	{
		throw JcrException{ "This version is not supported" };
	}

	return std::make_unique<Game>(exeInfo.value().path.filename(), std::move(gameDirectory), VersionUtil::jcExeToGameVersion(exeInfo.value().version));
}

std::unique_ptr<Game> Game::createGame(std::filesystem::path&& gameDirectory)
{
	const auto filesDirectoryPath{ Path::filesDirectoryPath(gameDirectory) };

	if (!std::filesystem::exists(filesDirectoryPath))
	{
		return nullptr;
	}

	const auto exeInfo{ JCExe::findFilenamePathAndVersion(filesDirectoryPath) };

	if (!exeInfo.has_value() || exeInfo.value().version == JCExe::Version::Prototype_D05_M08_Y1999_15H48)
	{
		return nullptr;
	}

	return std::make_unique<Game>(exeInfo.value().path.filename(), std::move(gameDirectory), VersionUtil::jcExeToGameVersion(exeInfo.value().version));
}

File Game::fileByVersion(File file) const
{
	const auto fileUnderlying{ static_cast<File_t>(file) };
	
	return fileUnderlying >= static_cast<File_t>(File::CRAVE_CRAVE_TIM) && isNtscJ() 
	? static_cast<File>(fileUnderlying - 2) : file;
};