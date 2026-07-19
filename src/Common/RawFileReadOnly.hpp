#pragma once

#include "Common/RawFileBase.hpp"

#include <filesystem>
#include <fstream>
#include <string>

class RawFileReadOnly final : public RawFileBase<std::ifstream>
{
public:
	explicit RawFileReadOnly(const char* pathFile);
	explicit RawFileReadOnly(const std::string& pathFile);
	explicit RawFileReadOnly(const std::filesystem::path& pathFile);

	using RawFileBase<std::ifstream>::isOpen;
};