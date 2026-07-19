#pragma once

#include "Common/RawFileBase.hpp"

#include <filesystem>
#include <fstream>
#include <string>

class RawFile final : public RawFileBase<std::fstream>
{
public:
	explicit RawFile(const char* pathFile);
	explicit RawFile(const std::string& pathFile);
	explicit RawFile(const std::filesystem::path& pathFile);

	template <typename T>
	void write(u64 offset, const T& content, std::size_t size = sizeof(T))
	{
		m_stream.seekp(offset);
		m_stream.write((const char*)&content, size);
		throwOnError("write");
	}

	void resize(std::uintmax_t newSize);
};