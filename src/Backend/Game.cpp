#include "Game.hpp"

#include "Backend/File.hpp"
#include "Backend/Mips.hpp"
#include "Backend/Path.hpp"
#include "Backend/Utility.hpp"
#include "Common/Buffer.hpp"

#include "JCExe.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <fstream>
#include <stdexcept>
#include <type_traits>
#include <utility>

const std::unordered_map<Version, Game::VersionSerialText> Game::s_versionSerial
{
	{ Version::NtscJ1,	{ "NTSC-J v1", "SLPS-01729" } },
	{ Version::NtscJ2,	{ "NTSC-J v2", "SLPS-91154" } },
	{ Version::NtscU,	{ "NTSC-U", "SLUS-00854" } },
	{ Version::PalEn,	{ "PAL-EN", "SLES-02201" } },
	{ Version::PalFr,	{ "PAL-FR", "SLES-02202" } },
	{ Version::PalDe,	{ "PAL-DE", "SLES-02203" } },
	{ Version::PalEs,	{ "PAL-ES", "SLES-02205" } },
	{ Version::PalIt,	{ "PAL-IT", "SLES-02206" } }
};

Game::Game(const std::filesystem::path& isoPath, const std::filesystem::path& exePath, Version version)
	: m_isoPath(isoPath), m_exePath(exePath), m_version(version), m_offset(version),
	m_data001FilesPath(std::move(JCExe{ Utility::gameToJcExeVersion(version) }.data001FilesPath()))
{
}

std::unique_ptr<RawFile> Game::file(s32 file) const
{
	const std::filesystem::path filePath{ std::format("{}/{}", Path::jcrTempDirectory, m_data001FilesPath.at(fileByVersion(file))) };

	if (!std::filesystem::is_regular_file(filePath))
	{
		throw std::runtime_error
		{
			std::format("\"{}\" file doesn't exist in \"{}\"", filePath.filename().string(), filePath.parent_path().string())
		};
	}
	return std::make_unique<RawFile>(filePath);
}

RawFile Game::executable() const
{
	if (!std::filesystem::is_regular_file(m_exePath))
	{
		throw std::runtime_error
		{
			std::format("\"{}\" file doesn't exist in \"{}\"", m_exePath.filename().string(), m_exePath.parent_path().string())
		};
	}
	return RawFile{ m_exePath };
}

const char* Game::filePathByIndex(s32 file) const
{
	return m_data001FilesPath.at(fileByVersion(file));
}

void Game::expandExecutable(u32 toAdd) const
{
	auto executable{ this->executable() };
	const auto executableSize{ executable.size() };

	const u32 rest{ (executableSize + toAdd) % Game::sectorSize };
	if (rest)
	{
		toAdd += Game::sectorSize - rest;
	}
	const auto newExecutableSize{ executableSize + toAdd };

	executable.write(0x1C, newExecutableSize - Game::sectorSize);
	std::filesystem::resize_file(m_exePath, newExecutableSize);

	auto
		luiBeginHeap{ executable.read<Mips_t>(offset().file.executable.mainFn + 0x34) },
		addiuBeginHeap{ executable.read<Mips_t>(offset().file.executable.mainFn + 0x38) };

	const u32
		heapShift{ isVersion(Version::NtscJ1) ? toAdd - NtscJ1::customCodeShift : toAdd },
		newBeginHeap{ (luiBeginHeap << 16) + static_cast<s16>(addiuBeginHeap) + heapShift };

	const bool isRightPartPositive{ static_cast<s16>(newBeginHeap) >= 0 };

	luiBeginHeap = ((luiBeginHeap >> 16) << 16);
	luiBeginHeap += isRightPartPositive ? (newBeginHeap >> 16) : (newBeginHeap >> 16) + 1;
	addiuBeginHeap = ((addiuBeginHeap >> 16) << 16) + static_cast<u16>(newBeginHeap);

	executable.write(offset().file.executable.mainFn + 0x34, luiBeginHeap);
	executable.write(offset().file.executable.mainFn + 0x38, addiuBeginHeap);
}

Game::CustomCodeOffset Game::customCodeOffset(u32 size)
{
	const Game::CustomCodeOffset ccOffset
	{
		.file = offset().file.executable.cc_begin + m_ccShift,
		.game = offset().game.cc_begin + m_ccShift
	};
	m_ccShift += size;

	return ccOffset;
}

bool Game::isNtsc() const
{
	return isVersion(Version::NtscJ1, Version::NtscJ2, Version::NtscU);
}

const char* Game::versionText() const
{
	return s_versionSerial.at(m_version).version;
}

const char* Game::serialText() const
{
	return s_versionSerial.at(m_version).serial;
}

std::filesystem::path Game::isoPath() const
{
	return m_isoPath;
}

std::filesystem::path Game::isoFilename() const
{
	return m_isoPath.filename();
}

Version Game::version() const
{
	return m_version;
}

const Offset& Game::offset() const
{
	return m_offset;
}

void Game::setIsoPath(const std::filesystem::path& isoPath)
{
	m_isoPath = isoPath;
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

bool Game::isAJCIso(const std::filesystem::path& isoPath)
{
	if (!std::filesystem::is_regular_file(isoPath))
	{
		return false;
	}

	RawFile iso{ isoPath };

	using OffsetPattern = std::pair<u32, SBuffer<11>>;
	static constexpr std::array<OffsetPattern, 8> ver
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
		return false;
	}

	for (const auto& [offset, pattern] : ver)
	{
		if (iso.read<std::remove_const_t<decltype(pattern)>>(offset) == pattern)
		{
			return true;
		}
	}

	return false;
}