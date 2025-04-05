#include "Json.hpp"

#include <fstream>
#include <iomanip>

namespace Json
{
	std::optional<Json::Read> read(const std::filesystem::path& path)
	{
		if (!std::filesystem::is_regular_file(path))
		{
			return std::nullopt;
		}

		std::ifstream jsonFile{ path };
		Json::Read json;
		jsonFile >> json;
		return json;
	}

	bool overwrite(const Json::Write& json, const std::filesystem::path& path)
	{
		const auto parentPath{ path.parent_path() };

		if (!parentPath.empty() && !std::filesystem::exists(parentPath) && !std::filesystem::create_directories(parentPath))
		{
			return false;
		}

		std::ofstream jsonFile{ path };
		jsonFile << std::setw(4) << json;
		return true;
	}
}