#include "Ost.hpp"

namespace Ost
{
	std::vector<s16> id()
	{
		std::vector<s16> ostId;
		ostId.reserve(Ost::names.size());

		for (const auto& [id, name]: Ost::names)
		{
			ostId.emplace_back(id);
		}

		return ostId;
	}
}