#pragma once

#include "nlohmann/json.hpp"

namespace Json
{
	template <typename TVal, typename TKey, typename TFn>
	void set(const nlohmann::json& json, const TKey& key, TFn&& callback)
	{
		if (json.contains(key))
		{
			callback(json.at(key).get<TVal>());
		}
	}
}