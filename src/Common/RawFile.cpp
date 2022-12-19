#include "RawFile.hpp"

static constexpr auto openMode{ std::fstream::binary | std::fstream::in | std::fstream::out };

RawFile::RawFile(const char* pathFile)
	: m_stream(pathFile, openMode), m_path(pathFile) {};

RawFile::RawFile(const std::string& pathFile)
	: m_stream(pathFile, openMode), m_path(pathFile) {};

RawFile::RawFile(const std::filesystem::path& pathFile)
	: m_stream(pathFile, openMode), m_path(pathFile) {};

std::uintmax_t RawFile::size() const
{
	return std::filesystem::file_size(m_path);
}

Buffer RawFile::readFile()
{
	const auto fileSize{ size() };
	Buffer buffer(fileSize);
	read(0, buffer.data(), fileSize);
	return buffer;
}