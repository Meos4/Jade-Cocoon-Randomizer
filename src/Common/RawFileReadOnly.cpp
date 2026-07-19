#include "RawFileReadOnly.hpp"

static constexpr auto openMode{ std::ifstream::binary };

RawFileReadOnly::RawFileReadOnly(const char* pathFile)
	: RawFileReadOnly(std::filesystem::path(pathFile))
{
}

RawFileReadOnly::RawFileReadOnly(const std::string& pathFile)
	: RawFileReadOnly(std::filesystem::path(pathFile))
{
}

RawFileReadOnly::RawFileReadOnly(const std::filesystem::path& pathFile)
	: RawFileBase(pathFile, openMode)
{
}