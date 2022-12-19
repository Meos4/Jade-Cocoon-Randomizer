#include "JCTools.hpp"
#include "JCExe.hpp"
#include "Types.hpp"

#include "fmt/format.h"

#include <stdexcept>
#include <vector>
#include <fstream>
#include <limits>

static constexpr auto sectorSize{ 2048u };
static constexpr auto data001Filename{ "DATA.001" };

struct DslLOC
{
	u8 minutes;
	u8 seconds;
	u8 sectors;
	u8 track;
};

struct FileInfo
{
	DslLOC position;
	u32 nbSectors;
	u32 size;
};

static s32 DsPosToInt(DslLOC* p)
{
	s32 v0, v1, a0, a1, a2;
	v1 = p->minutes;
	a2 = p->seconds;
	a1 = v1 >> 4;
	v0 = a1 << 2;
	v0 += a1;
	v0 <<= 1;
	v1 &= 0xF;
	v0 += v1;
	a1 = v0 << 4;
	a1 -= v0;
	a1 <<= 2;
	v1 = a2 >> 4;
	v0 = v1 << 2;
	v0 += v1;
	v0 <<= 1;
	a2 &= 0xF;
	v0 += a2;
	a1 += v0;
	v1 = a1 << 2;
	v1 += a1;
	v0 = v1 << 4;
	a1 = p->sectors;
	v0 -= v1;
	a0 = a1 >> 4;
	v1 = a0 << 2;
	v1 += a0;
	v1 <<= 1;
	a1 &= 0xF;
	v1 += a1;
	v0 += v1;
	return v0 - 0x96;
}

static DslLOC* DsIntToPos(s32 i, DslLOC* p)
{
	s32 v1, a0, a1, a2, a3, t0, t1, t3, hi;
	v1 = 0x1B4E81B5;
	a0 = i + 0x96;
	hi = (static_cast<u64>(a0) * v1) >> 32;
	a1 = 0x88888889;
	v1 = hi;
	a3 = v1 >> 3;
	v1 = a0 >> 31;
	a3 -= v1;
	hi = (static_cast<u64>(a3) * a1) >> 32;
	t1 = 0x66666667;
	a1 = a3 << 2;
	a1 += a3;
	v1 = a1 << 4;
	a2 = hi;
	v1 -= a1;
	a0 -= v1;
	hi = (static_cast<u64>(a0) * t1) >> 32;
	v1 = a3 >> 31;
	t0 = a2 + a3;
	t0 >>= 5;
	t0 -= v1;
	v1 = t0 << 4;
	v1 -= t0;
	a1 = hi;
	v1 <<= 2;
	a3 -= v1;
	hi = (static_cast<u64>(a3) * t1) >> 32;
	v1 = a0 >> 31;
	a1 >>= 2;
	a1 -= v1;
	a2 = a1 << 4;
	v1 = a1 << 2;
	v1 += a1;
	v1 <<= 1;
	a0 -= v1;
	t3 = hi;
	a2 += a0;
	v1 = a3 >> 31;
	hi = (static_cast<u64>(t0) * t1) >> 32;
	p->sectors = static_cast<u8>(a2);
	a0 = t3 >> 2;
	a0 -= v1;
	a1 = a0 << 4;
	v1 = a0 << 2;
	v1 += a0;
	v1 <<= 1;
	a3 -= v1;
	a1 += a3;
	v1 = t0 >> 31;
	p->seconds = static_cast<u8>(a1);
	t1 = hi;
	a0 = t1 >> 2;
	a0 -= v1;
	a1 = a0 << 4;
	v1 = a0 << 2;
	v1 += a0;
	v1 <<= 1;
	t0 -= v1;
	a1 += t0;
	p->minutes = static_cast<u8>(a1);
	return p;
}

namespace JCTools
{
	void unpacker(const std::filesystem::path& srcExe, const std::filesystem::path& srcData, const std::filesystem::path& dest)
	{
		const std::filesystem::path data001Path{ fmt::format("{}/{}", srcData.string(), data001Filename) };

		if (!std::filesystem::is_regular_file(data001Path))
		{
			throw std::runtime_error{ fmt::format("Can't find \"{}\" in \"{}\"", data001Path.filename().string(), data001Path.parent_path().string()) };
		}

		const auto constructorInfo{ JCExe::findFilenamePathAndVersion(srcExe) };

		if (!constructorInfo.has_value())
		{
			throw std::runtime_error{ fmt::format("Can't find compatible playstation executable in \"{}\"", srcExe.string()) };
		}

		const JCExe exeInfo{ constructorInfo.value().version };

		fmt::print("{} version found\n", exeInfo.toString());

		std::ifstream
			data001{ data001Path, std::ifstream::binary },
			executable{ constructorInfo.value().path, std::ifstream::binary };

		std::filesystem::create_directories(dest);

		fmt::print("Unpacking files...\n");

		DslLOC data001Loc;
		executable.seekg(exeInfo.offset().data001FileInfoBegin);
		executable.read((char*)&data001Loc, sizeof(data001Loc));

		const auto data001Sector{ DsPosToInt(&data001Loc) };
		const auto data001FilesPath{ exeInfo.data001FilesPath() };
		executable.seekg(exeInfo.offset().data001FileInfoBegin);

		for (const auto& filePathStr : data001FilesPath)
		{
			const std::filesystem::path filePath{ fmt::format("{}/{}", dest.string(), filePathStr) };
			std::filesystem::create_directories(filePath.parent_path());
			FileInfo fileInfo;
			executable.read((char*)&fileInfo, sizeof(fileInfo));

			std::vector<char> buffer(fileInfo.size);
			data001.seekg((DsPosToInt(&fileInfo.position) - data001Sector) * sectorSize);
			data001.read(buffer.data(), buffer.size());

			std::ofstream file{ filePath, std::ofstream::binary };
			file.write((const char*)buffer.data(), buffer.size());
		}

		fmt::print("{} Files unpacked\n", data001FilesPath.size());
	}

	void repacker(const std::filesystem::path& srcExe, const std::filesystem::path& srcData,
		const std::filesystem::path& destExe, const std::filesystem::path& destData)
	{
		const auto constructorInfo{ JCExe::findFilenamePathAndVersion(srcExe) };

		if (!constructorInfo.has_value())
		{
			throw std::runtime_error{ fmt::format("Can't find compatible playstation executable in \"{}\"", srcExe.string()) };
		}

		const JCExe exeInfo{ constructorInfo.value().version };

		fmt::print("{} version found\n", exeInfo.toString());

		const auto data001FilesPath{ exeInfo.data001FilesPath() };
		u64 totalFilesSize{};

		for (const auto& filePathStr : data001FilesPath)
		{
			const std::filesystem::path filePath{ fmt::format("{}/{}", srcData.string(), filePathStr) };

			if (!std::filesystem::is_regular_file(filePath))
			{
				throw std::runtime_error{ fmt::format("\"{}\" file is missing in \"{}\"", filePath.filename().string(), filePath.parent_path().string()) };
			}

			totalFilesSize += std::filesystem::file_size(filePath);
		}

		if (totalFilesSize > std::numeric_limits<u32>::max())
		{
			throw std::runtime_error{ fmt::format("\"{}\" can't be repacked because files exceed the size limit", data001Filename) };
		}

		std::filesystem::create_directories(destData);
		const std::filesystem::path exePath{ fmt::format("{}/{}", destExe.string(), constructorInfo.value().path.filename().string()) };

		if (srcExe != destExe)
		{
			std::filesystem::create_directories(destExe);
			std::filesystem::copy_file(constructorInfo.value().path, exePath, std::filesystem::copy_options::overwrite_existing);
		}

		std::ofstream data001{ fmt::format("{}/{}", destData.string(), data001Filename), std::ofstream::binary };
		std::fstream executable{ exePath, std::fstream::binary | std::fstream::in | std::fstream::out };

		fmt::print("Repacking files...\n");

		DslLOC data001Loc{};
		executable.seekg(exeInfo.offset().data001FileInfoBegin);
		executable.read((char*)&data001Loc, sizeof(data001Loc));

		auto sectorPosition{ static_cast<unsigned>(DsPosToInt(&data001Loc)) };
		executable.seekg(exeInfo.offset().data001FileInfoBegin);

		for (const auto& filePathStr : data001FilesPath)
		{
			const std::filesystem::path filePath{ fmt::format("{}/{}", srcData.string(), filePathStr) };
			const auto fileSize{ static_cast<u32>(std::filesystem::file_size(filePath)) };

			DslLOC fileLoc{};
			DsIntToPos(sectorPosition, &fileLoc);
			const FileInfo fileInfo
			{
				.position = fileLoc,
				.nbSectors = (fileSize + sectorSize - 1) >> 0xB,
				.size = fileSize
			};

			const auto rest{ fileSize % sectorSize };
			std::vector<char> buffer(rest ? fileSize + sectorSize - rest : fileSize);
			std::ifstream file{ filePath, std::ifstream::binary };

			file.read(buffer.data(), fileSize);
			data001.write(buffer.data(), buffer.size());
			executable.write((char*)&fileInfo, sizeof(fileInfo));

			sectorPosition += static_cast<u32>(buffer.size()) / sectorSize;
		}

		// DATA/VOICE.XA MODE 2336
		sectorPosition += 1;

		FileInfo voiceXaFileInfo;
		executable.seekg(exeInfo.offset().voiceXaFileInfoBegin);
		executable.read((char*)&voiceXaFileInfo, sizeof(voiceXaFileInfo));
		DsIntToPos(sectorPosition, &voiceXaFileInfo.position);

		executable.seekg(exeInfo.offset().voiceXaFileInfoBegin);
		executable.write((char*)&voiceXaFileInfo, sizeof(voiceXaFileInfo));

		sectorPosition += voiceXaFileInfo.nbSectors;

		// MOVIE/.STR MODE 2336
		sectorPosition += 1;

		std::vector<FileInfo> movieStrFilesInfo(exeInfo.nbMovieStrFiles());

		executable.read((char*)movieStrFilesInfo.data(), movieStrFilesInfo.size() * sizeof(FileInfo));
		executable.seekg(exeInfo.offset().movieStrFileInfoBegin);

		for (auto& strFileInfo : movieStrFilesInfo)
		{
			DsIntToPos(sectorPosition, &strFileInfo.position);
			executable.write((char*)&strFileInfo, sizeof(strFileInfo));

			sectorPosition += strFileInfo.nbSectors;
		}

		fmt::print("Done\n");
	}
}