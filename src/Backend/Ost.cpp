#include "Ost.hpp"

#include <algorithm>
#include <array>

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

	std::vector<s16> idWithoutDuplicate()
	{
		auto ostId{ Ost::id() };

		auto isADuplicateOst = [](auto ost)
		{
			// Legend of Arcana ~ Clandestine Meeting duplicate
			static constexpr std::array<s16, 3> duplicateOsts{ 30, 126, 144 };
			for (const auto& duplicateOst : duplicateOsts)
			{
				if (ost == duplicateOst)
				{
					return true;
				}
			}
			return false;
		};

		const auto [begin, end] { std::ranges::remove_if(ostId, isADuplicateOst) };
		ostId.erase(begin, end);
		return ostId;
	}
}