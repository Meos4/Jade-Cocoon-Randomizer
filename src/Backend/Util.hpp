#pragma once

#include "Common/TemplateTypes.hpp"

#include <limits>
#include <span>
#include <type_traits>

namespace Utility
{
	template <Integral T>
	constexpr T bitToInt(T bit)
	{
		for (T i{}; i < std::numeric_limits<T>::digits; ++i)
		{
			if (T(1 << i) == bit)
			{
				return i;
			}
		}
		return 0;
	};

	template <typename T>
	constexpr bool areSame(std::span<const T> sequence)
	{
		if (sequence.empty())
		{
			return false;
		}

		for (std::size_t i{ 1 }; i < sequence.size(); ++i)
		{
			if (sequence[0] != sequence[i])
			{
				return false;
			}
		}

		return true;
	}
}