#include "Game.hpp"

#include "Backend/File.hpp"
#include "Backend/Mips.hpp"
#include "Backend/MipsFn.hpp"
#include "Backend/Path.hpp"
#include "Backend/Utility.hpp"
#include "Common/Buffer.hpp"
#include "Common/JcrException.hpp"

#include "JCExe.hpp"
#include "JCTools.hpp"
#include "dumpsxiso/dumpsxiso.h"
#include "mkpsxiso/mkpsxiso.h"

#include <algorithm>
#include <array>
#include <format>
#include <fstream>
#include <type_traits>
#include <utility>

Game::Game(const std::filesystem::path& exeFilename, std::filesystem::path&& directoryPath, Version version)
	: m_exePath(Path::executablePath(directoryPath, exeFilename)),
	m_gameDirectory(directoryPath),
	m_version(version), 
	m_offset(version),
	m_data001FilesPath(std::move(JCExe{ Utility::gameToJcExeVersion(version) }.data001FilesPath()))
{
}

std::unique_ptr<RawFile> Game::file(s32 file) const
{
	const std::filesystem::path filePath{ std::format("{}/{}", m_gameDirectory.string(), m_data001FilesPath.at(fileByVersion(file)))};
	if (!std::filesystem::is_regular_file(filePath))
	{
		throw JcrException{ "\"{}\" file doesn't exist in \"{}\"", filePath.filename().string(), filePath.parent_path().string() };
	}
	return std::make_unique<RawFile>(filePath);
}

RawFile Game::executable() const
{
	if (!std::filesystem::is_regular_file(m_exePath))
	{
		throw JcrException{ "\"{}\" file doesn't exist in \"{}\"", m_exePath.filename().string(), m_exePath.parent_path().string() };
	}
	return RawFile{ m_exePath };
}

const char* Game::filePathByIndex(s32 file) const
{
	return m_data001FilesPath.at(fileByVersion(file));
}

void Game::expandExecutable() const
{
	auto executable{ this->executable() };
	const auto offsetGHvb{ m_offset.game.heapVanillaBegin };

	const auto heapShift{ heapRandomizerBegin() - offsetGHvb };
	auto newExecutableSize{ (offsetGHvb + heapShift - gameToFileTextSectionShift()) & ~0x80000000 };
	const auto sectorRemainer{ newExecutableSize % Game::sectorSize };
	if (sectorRemainer)
	{
		newExecutableSize += Game::sectorSize - sectorRemainer;
	}

	executable.write(0x1C, newExecutableSize - Game::sectorSize);
	std::filesystem::resize_file(m_exePath, newExecutableSize);

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
	const auto [lui, addiu]{ executable().read<std::array<Mips_t, 2>>(m_offset.file.executable.mainFn + 0x34) };
	return (lui << 16) + static_cast<s16>(addiu) == m_offset.game.heapVanillaBegin;
}

void Game::repackFilesToDATA001()
{
	const auto filesDirectoryPath{ Path::filesDirectoryPath(m_gameDirectory) };

	JCTools::repacker(filesDirectoryPath, m_gameDirectory, filesDirectoryPath, Path::dataDirectoryPath(filesDirectoryPath));
}

void Game::createIsoFromFiles(const std::filesystem::path* destPath)
{
	const auto configXmlPath{ Path::configXmlPath(m_gameDirectory) };

	const auto makeArgs{ Path::makeIsoArgs(destPath, &configXmlPath) };
	if (mkpsxiso(static_cast<int>(makeArgs.size()), (Path::CStringPlatformPtr)makeArgs.data()) == EXIT_FAILURE)
	{
		throw JcrException{ "Unable to repack iso" };
	}
}

bool Game::isNtsc() const
{
	return isVersion(Version::NtscJ1, Version::NtscJ2, Version::NtscU);
}

const char* Game::versionText() const
{
	static constexpr std::array<const char*, static_cast<std::size_t>(Version::Count)> texts
	{
		"NTSC-J v1", "NTSC-J v2", "NTSC-U", "PAL-EN",
		"PAL-FR", "PAL-DE", "PAL-ES", "PAL-IT"
	};

	return texts[static_cast<std::size_t>(m_version)];
}

const char* Game::serialText() const
{
	static constexpr std::array<const char*, static_cast<std::size_t>(Version::Count)> serials
	{
		"SLPS-01729", "SLPS-91154", "SLUS-00854", "SLES-02201",
		"SLES-02202", "SLES-02203", "SLES-02205", "SLES-02206"
	};

	return serials[static_cast<std::size_t>(m_version)];
}

Version Game::version() const
{
	return m_version;
}

const Offset& Game::offset() const
{
	return m_offset;
}

const std::filesystem::path& Game::gameDirectory() const
{
	return m_gameDirectory;
}

s32 Game::fileByVersion(s32 file) const
{
	return file >= File::CRAVE_CRAVE_TIM && isVersion(Version::NtscJ1, Version::NtscJ2) ? file - 2 : file;
};

bool Game::generateCue(const std::filesystem::path& isoPath)
{
	auto cue{ isoPath };
	cue.replace_extension(".cue");
	try
	{
		std::ofstream stream{ cue };
		stream << "FILE " << isoPath.filename() << " BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n";
		return true;
	}
	catch ([[maybe_unused]] const std::exception&) // Using some chars catches an exception
	{
		if (std::filesystem::exists(cue))
		{
			std::filesystem::remove(cue);
		}
		return false;
	}
}

std::optional<Version> Game::versionFromIso(const std::filesystem::path& isoPath)
{
	if (!std::filesystem::is_regular_file(isoPath))
	{
		return std::nullopt;
	}

	RawFile iso{ isoPath };

	using OffsetPattern = std::pair<u32, SBuffer<11>>;
	static constexpr std::array<OffsetPattern, static_cast<std::size_t>(Version::Count)> ver
	{{
		{ 0x0007DA33, {0x53, 0x4C, 0x50, 0x53, 0x5F, 0x30, 0x31, 0x37, 0x2E, 0x32, 0x39} },
		{ 0x0007CF8F, {0x53, 0x4C, 0x50, 0x53, 0x5F, 0x39, 0x31, 0x31, 0x2E, 0x35, 0x34} },
		{ 0x0007DF47, {0x53, 0x4C, 0x55, 0x53, 0x5F, 0x30, 0x30, 0x38, 0x2E, 0x35, 0x34} },
		{ 0x0007DE6B, {0x53, 0x4C, 0x45, 0x53, 0x5F, 0x30, 0x32, 0x32, 0x2E, 0x30, 0x31} },
		{ 0x0007DF1F, {0x53, 0x4C, 0x45, 0x53, 0x5F, 0x30, 0x32, 0x32, 0x2E, 0x30, 0x32} },
		{ 0x0007DEC3, {0x53, 0x4C, 0x45, 0x53, 0x5F, 0x30, 0x32, 0x32, 0x2E, 0x30, 0x33} },
		{ 0x0007DEDB, {0x53, 0x4C, 0x45, 0x53, 0x5F, 0x30, 0x32, 0x32, 0x2E, 0x30, 0x35} },
		{ 0x0007DED3, {0x53, 0x4C, 0x45, 0x53, 0x5F, 0x30, 0x32, 0x32, 0x2E, 0x30, 0x36} }
	}};

	static constexpr auto maxOffset{ (*std::max_element(ver.begin(), ver.end(),
		[](const auto& a, const auto& b) { return a.first < b.first; })).first };

	if (iso.size() < maxOffset + sizeof(OffsetPattern::second))
	{
		return std::nullopt;
	}

	for (std::size_t i{}; i < ver.size(); ++i)
	{
		const auto& [offset, pattern]{ ver[i] };
		if (iso.read<std::remove_const_t<decltype(pattern)>>(offset) == pattern)
		{
			return static_cast<Version>(i);
		}
	}

	return std::nullopt;
}

Game Game::createGame(const std::filesystem::path& isoPath, std::filesystem::path&& gameDirectory)
{
	if (!std::filesystem::is_regular_file(isoPath))
	{
		if (std::filesystem::is_directory(isoPath))
		{
			throw JcrException{ "\"{}\" is a directory", isoPath.filename().string() };
		}
		else
		{
			throw JcrException{ "\"{}\" file does not exist", isoPath.filename().string() };
		}
	}

	if (Game::versionFromIso(isoPath) == std::nullopt)
	{
		throw JcrException{ "\"{}\" is not a Jade Cocoon binary file", isoPath.filename().string() };
	}

	if (std::filesystem::is_directory(gameDirectory))
	{
		std::error_code err;
		const auto nbRemoved{ std::filesystem::remove_all(gameDirectory, err) };
		if (nbRemoved == -1)
		{
			throw JcrException{ "\"{}\" directory cannot be removed", gameDirectory.string()};
		}
	}

	std::filesystem::create_directory(gameDirectory);

	const std::filesystem::path
		configXmlPath{ Path::configXmlPath(gameDirectory) },
		filesDirectoryPath{ Path::filesDirectoryPath(gameDirectory) };

	const auto dumpArgs{ Path::dumpIsoArgs(&isoPath, &configXmlPath, &filesDirectoryPath) };

	dumpsxiso(static_cast<int>(dumpArgs.size()), (Path::CStringPlatformPtr)dumpArgs.data());
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

	return { exeInfo.value().path.filename(), std::move(gameDirectory), Utility::jcExeToGameVersion(exeInfo.value().version)};
}