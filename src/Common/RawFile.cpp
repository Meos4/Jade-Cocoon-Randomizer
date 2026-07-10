#include "RawFile.hpp"

#include "Common/JcrException.hpp"

static constexpr auto openMode{ std::fstream::binary | std::fstream::in | std::fstream::out };

RawFile::RawFile(const char* pathFile)
	: m_stream(pathFile, openMode), m_path(pathFile)
{
	throwOnError("open");
}

RawFile::RawFile(const std::string& pathFile)
	: m_stream(pathFile, openMode), m_path(pathFile)
{
	throwOnError("open");
}

RawFile::RawFile(const std::filesystem::path& pathFile)
	: m_stream(pathFile, openMode), m_path(pathFile)
{
	throwOnError("open");
}

void RawFile::throwOnError(const char* operation) const
{
	if (!m_stream)
	{
		throw JcrException{ "RawFile {} failed: {}", operation, m_path.string() };
	}
}

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

void RawFile::resize(std::uintmax_t newSize)
{
	m_stream.flush();
	m_stream.close();
	std::filesystem::resize_file(m_path, newSize);
	m_stream.open(m_path, openMode);
	throwOnError("resize");
}