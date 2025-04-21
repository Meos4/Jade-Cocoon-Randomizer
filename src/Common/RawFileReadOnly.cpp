#include "RawFileReadOnly.hpp"

static constexpr auto openMode{ std::ifstream::binary };

RawFileReadOnly::RawFileReadOnly(const char* pathFile)
	: m_stream(pathFile, openMode), m_path(pathFile) {};

RawFileReadOnly::RawFileReadOnly(const std::string& pathFile)
	: m_stream(pathFile, openMode), m_path(pathFile) {};

RawFileReadOnly::RawFileReadOnly(const std::filesystem::path& pathFile)
	: m_stream(pathFile, openMode), m_path(pathFile) {};

std::uintmax_t RawFileReadOnly::size() const
{
	return std::filesystem::file_size(m_path);
}

Buffer RawFileReadOnly::readFile()
{
	const auto fileSize{ size() };
	Buffer buffer(fileSize);
	read(0, buffer.data(), fileSize);
	return buffer;
}