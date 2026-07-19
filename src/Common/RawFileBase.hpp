#pragma once

#include "Common/Buffer.hpp"
#include "Common/JcrException.hpp"
#include "Common/Types.hpp"

#include <filesystem>
#include <fstream>
#include <string>

template <typename Stream>
class RawFileBase
{
public:
	template <typename T>
	T read(u64 offset, std::size_t size = sizeof(T))
	{
		T hook;
		m_stream.seekg(offset);
		m_stream.read((char*)&hook, size);
		throwOnError("read");
		return hook;
	}

	template <typename T>
	void read(u64 offset, T* store, std::size_t size = sizeof(T))
	{
		m_stream.seekg(offset);
		m_stream.read((char*)store, size);
		throwOnError("read");
	}

	std::uintmax_t size() const
	{
		return std::filesystem::file_size(m_path);
	}

	Buffer readFile()
	{
		const auto fileSize{ size() };
		Buffer buffer(fileSize);
		read(0, buffer.data(), fileSize);
		return buffer;
	}
protected:
	RawFileBase(const std::filesystem::path& path, std::ios_base::openmode mode)
		: m_stream(path, mode), m_path(path)
	{
	}

	void throwOnError(const char* operation) const
	{
		if (!m_stream)
		{
			throw JcrException{ "Raw file {} failed: {}", operation, m_path.string() };
		}
	}

	bool isOpen() const
	{
		return m_stream.is_open();
	}

	Stream m_stream;
	std::filesystem::path m_path;
};
