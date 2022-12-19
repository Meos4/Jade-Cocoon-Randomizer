#pragma once

#include "Common/Buffer.hpp"
#include "Common/Types.hpp"

#include <filesystem>
#include <fstream>
#include <string>

class RawFile
{
public:
	explicit RawFile(const char* pathFile);
	explicit RawFile(const std::string& pathFile);
	explicit RawFile(const std::filesystem::path& pathFile);

	virtual ~RawFile() = default;
	
	template <typename T>
	T read(u64 offset, std::size_t size = sizeof(T))
	{
		static_assert(sizeof(T) < 1024, "Size limit reached, should read using heap");
		T hook;
		m_stream.seekg(offset);
		m_stream.read((char*)&hook, size);
		return hook;
	}

	template <typename T>
	void read(u64 offset, T* store, std::size_t size = sizeof(T))
	{
		m_stream.seekg(offset);
		m_stream.read((char*)store, size);
	}

	template <typename T>
	void write(u64 offset, const T& content, std::size_t size = sizeof(T))
	{
		m_stream.seekp(offset);
		m_stream.write((const char*)&content, size);
	}

	std::uintmax_t size() const;
	Buffer readFile();
private:
	std::fstream m_stream;
	std::filesystem::path m_path;
};