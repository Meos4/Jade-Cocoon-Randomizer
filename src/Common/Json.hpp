#pragma once

#include "nlohmann/json.hpp"

#include <filesystem>
#include <optional>

namespace Json
{
	using Read = nlohmann::json;
	using Write = nlohmann::ordered_json;
	using Exception = nlohmann::json::exception;

	std::optional<Json::Read> read(const std::filesystem::path& path);
	bool overwrite(const Json::Write& json, const std::filesystem::path& path);

	template <typename TVal, typename TKey, typename TFn>
	void set(const nlohmann::json& json, const TKey& key, TFn&& callback)
	{
		if (json.contains(key))
		{
			callback(json.at(key).get<TVal>());
		}
	}
}