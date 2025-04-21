#pragma once

#include "Common/Buffer.hpp"
#include "Common/Types.hpp"

#include <filesystem>
#include <fstream>
#include <string>

class RawFileReadOnly final
{
public:
	explicit RawFileReadOnly(const char* pathFile);
	explicit RawFileReadOnly(const std::string& pathFile);
	explicit RawFileReadOnly(const std::filesystem::path& pathFile);
	
	template <typename T>
	T read(u64 offset, std::size_t size = sizeof(T))
	{
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

	std::uintmax_t size() const;
	Buffer readFile();
private:
	std::ifstream m_stream;
	std::filesystem::path m_path;
};