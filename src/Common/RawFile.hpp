#pragma once

#include "Common/Buffer.hpp"
#include "Common/Types.hpp"

#include <filesystem>
#include <fstream>
#include <string>

class RawFile final
{
public:
	explicit RawFile(const char* pathFile);
	explicit RawFile(const std::string& pathFile);
	explicit RawFile(const std::filesystem::path& pathFile);

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

	template <typename T>
	void write(u64 offset, const T& content, std::size_t size = sizeof(T))
	{
		m_stream.seekp(offset);
		m_stream.write((const char*)&content, size);
		throwOnError("write");
	}

	std::uintmax_t size() const;
	Buffer readFile();
	void resize(std::uintmax_t newSize);
private:
	void throwOnError(const char* operation) const;

	std::fstream m_stream;
	std::filesystem::path m_path;
};