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

void Game::expandTo(u32 newAllocatedSize)
{
	const auto fileHeapVanillaBegin{ (m_offset.game.heapVanillaBegin - gameToFileTextSectionShift()) & ~0x80000000 };
	auto requiredFileSize{ fileHeapVanillaBegin + newAllocatedSize };

	const auto sectorRemainder{ requiredFileSize % Iso::sectorSize };
	if (sectorRemainder)
	{
		requiredFileSize += Iso::sectorSize - sectorRemainder;
	}

	const auto currentFileSize{ std::filesystem::file_size(m_builderTree.exePath()) };
	auto newFileSize{ requiredFileSize };
	if (newFileSize < currentFileSize)
	{
		newFileSize = static_cast<u32>(currentFileSize);
	}

	auto executable{ this->executable() };

	executable.write(0x1C, requiredFileSize - Iso::sectorSize);
	if (newFileSize > currentFileSize)
	{
		std::filesystem::resize_file(m_builderTree.exePath(), newFileSize);
	}

	const auto offsetFMFn{ m_offset.file.executable.mainFn };
	auto instructions{ executable.read<std::array<Mips_t, 2>>(offsetFMFn + 0x34) };
	auto& [luiHeap, addiuHeap]{ instructions };

	const u32 newBeginHeap{ m_offset.game.heapVanillaBegin + newAllocatedSize };
	const bool isLower16Positive{ static_cast<s16>(newBeginHeap) >= 0 };

	luiHeap = ((luiHeap >> 16) << 16);
	luiHeap += isLower16Positive ? (newBeginHeap >> 16) : (newBeginHeap >> 16) + 1;
	addiuHeap = ((addiuHeap >> 16) << 16) + static_cast<u16>(newBeginHeap);

	executable.write(offsetFMFn + 0x34, instructions);
}

Game::CustomCodeOffset Game::customCodeOffset(u32 size)
{
	const auto allocatedSize{ (m_ccShift + Iso::sectorSize - 1) / Iso::sectorSize * Iso::sectorSize };
	if (m_ccShift + size > allocatedSize)
	{
		auto newAllocatedSize{ allocatedSize };
		do
		{
			newAllocatedSize += Iso::sectorSize;
		} while (m_ccShift + size > newAllocatedSize);
		expandTo(newAllocatedSize);
	}

	const auto fileHeapVanillaBegin{ (m_offset.game.heapVanillaBegin - gameToFileTextSectionShift()) & ~0x80000000 };
	const Game::CustomCodeOffset ccOffset
	{
		.file = fileHeapVanillaBegin + m_ccShift,
		.game = m_offset.game.heapVanillaBegin + m_ccShift
	};
	m_ccShift += size;

	return ccOffset;
}

void Game::addFrameFn(u32 fnGameOffset)
{
	m_frameFns.emplace_back(fnGameOffset);
}

void Game::writeFrameFns()
{
	if (m_frameFns.empty())
	{
		return;
	}

	m_frameFns.emplace_back(0);

	const auto tableSize{ static_cast<u32>(m_frameFns.size() * sizeof(u32)) };
	const auto dispatcherOffset{ customCodeOffset(sizeof(MipsFn::FrameFnsDispatcher) + tableSize) };
	const auto li32_table{ Mips::li32(Mips::Register::s0, dispatcherOffset.game + sizeof(MipsFn::FrameFnsDispatcher)) };

	const MipsFn::FrameFnsDispatcher dispatcherFn
	{
		0x27BDFFB0, // addiu sp, -0x50
		0xAFA10000, // sw at, 0(sp)
		0xAFA20004, // sw v0, 4(sp)
		0xAFA30008, // sw v1, 8(sp)
		0xAFA4000C, // sw a0, 0xC(sp)
		0xAFA50010, // sw a1, 0x10(sp)
		0xAFA60014, // sw a2, 0x14(sp)
		0xAFA70018, // sw a3, 0x18(sp)
		0xAFA8001C, // sw t0, 0x1C(sp)
		0xAFA90020, // sw t1, 0x20(sp)
		0xAFAA0024, // sw t2, 0x24(sp)
		0xAFAB0028, // sw t3, 0x28(sp)
		0xAFAC002C, // sw t4, 0x2C(sp)
		0xAFAD0030, // sw t5, 0x30(sp)
		0xAFAE0034, // sw t6, 0x34(sp)
		0xAFAF0038, // sw t7, 0x38(sp)
		0xAFB8003C, // sw t8, 0x3C(sp)
		0xAFB90040, // sw t9, 0x40(sp)
		0xAFB00044, // sw s0, 0x44(sp)
		0xAFBF0048, // sw ra, 0x48(sp)
		li32_table[0], // lui s0, 0xXXXX
		li32_table[1], // ori s0, 0xXXXX
		0x8E020000, // loop: lw v0, 0(s0)
		0x00000000, // nop
		0x10400005, // beqz v0, done
		0x00000000, // nop
		0x0040F809, // jalr v0
		0x26100004, // addiu s0, 4 (branch delay slot)
		0x1000FFF9, // beq zero, zero, loop
		0x00000000, // nop (branch delay slot)
		0x8FA10000, // done: lw at, 0(sp)
		0x8FA20004, // lw v0, 4(sp)
		0x8FA30008, // lw v1, 8(sp)
		0x8FA4000C, // lw a0, 0xC(sp)
		0x8FA50010, // lw a1, 0x10(sp)
		0x8FA60014, // lw a2, 0x14(sp)
		0x8FA70018, // lw a3, 0x18(sp)
		0x8FA8001C, // lw t0, 0x1C(sp)
		0x8FA90020, // lw t1, 0x20(sp)
		0x8FAA0024, // lw t2, 0x24(sp)
		0x8FAB0028, // lw t3, 0x28(sp)
		0x8FAC002C, // lw t4, 0x2C(sp)
		0x8FAD0030, // lw t5, 0x30(sp)
		0x8FAE0034, // lw t6, 0x34(sp)
		0x8FAF0038, // lw t7, 0x38(sp)
		0x8FB8003C, // lw t8, 0x3C(sp)
		0x8FB90040, // lw t9, 0x40(sp)
		0x8FB00044, // lw s0, 0x44(sp)
		0x8FBF0048, // lw ra, 0x48(sp)
		0x27BD0050, // addiu sp, 0x50
		Mips::j(m_offset.game.frameLimiter + 0x14),
		0x00000000  // nop
	};

	auto executable{ this->executable() };

	executable.write(dispatcherOffset.file, dispatcherFn);
	executable.write(dispatcherOffset.file + sizeof(MipsFn::FrameFnsDispatcher), m_frameFns.front(), tableSize);
	executable.write(m_offset.file.executable.afterFramerateLimiter, Mips::j(dispatcherOffset.game));

	m_frameFns.clear();
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

void Game::createBuilderDirectory()
{
	m_ccShift = 0;
	m_frameFns.clear();

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