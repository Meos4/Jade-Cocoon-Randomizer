#include "RawFile.hpp"

static constexpr auto openMode{ std::fstream::binary | std::fstream::in | std::fstream::out };

RawFile::RawFile(const char* pathFile)
	: RawFile(std::filesystem::path(pathFile))
{
}

RawFile::RawFile(const std::string& pathFile)
	: RawFile(std::filesystem::path(pathFile))
{
}

RawFile::RawFile(const std::filesystem::path& pathFile)
	: RawFileBase(pathFile, openMode)
{
	throwOnError("open");
}

void RawFile::resize(std::uintmax_t newSize)
{
	m_stream.flush();
	m_stream.close();
	std::filesystem::resize_file(m_path, newSize);
	m_stream.open(m_path, openMode);
	throwOnError("resize");
}