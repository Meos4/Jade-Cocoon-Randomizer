#pragma once

#include "Common/JcrException.hpp"
#include "Common/TemplateTypes.hpp"
#include "Common/Types.hpp"

#include <algorithm>
#include <limits>
#include <random>
#include <type_traits>
#include <vector>

class Random final
{
public:
	using Type = std::mt19937_64::result_type;
	static constexpr auto limit{ std::numeric_limits<Random::Type>::max() };

	Random();

	template <Arithmetic T>
	T generate(T min, T max)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			std::uniform_real_distribution<T> distribute{ min, max };
			return distribute(m_mte);
		}
		else if constexpr (sizeof(T) == 1)
		{
			using ByteTypeRandom = std::conditional_t<std::is_unsigned_v<T>, Random::Type, std::make_signed_t<Random::Type>>;
			std::uniform_int_distribution<ByteTypeRandom> distribute{ static_cast<ByteTypeRandom>(min), static_cast<ByteTypeRandom>(max) };
			return static_cast<T>(distribute(m_mte));
		}
		else
		{
			std::uniform_int_distribution<T> distribute{ min, max };
			return distribute(m_mte);
		}
	}

	template <Arithmetic T>
	T generate(T max)
	{
		return generate<T>(static_cast<T>(0), max);
	}

	template <typename T>
	T generate(std::initializer_list<T> list)
	{
		const auto size{ list.size() };
		if (!size)
		{
			throw JcrException{ "Attempt to generate in an empty list" };
		}
		return *(list.begin() + generate(size - 1));
	}

	template <Integral T>
	T generateBit(T bits)
	{
		if (bits)
		{
			std::vector<T> availableBits;
			for (T i{}; i < std::numeric_limits<T>::digits; ++i)
			{
				const T binaryBit{ bits & (1 << i) };
				if (binaryBit)
				{
					availableBits.emplace_back(binaryBit);
				}
			}
			return availableBits.at(generate(availableBits.size() - 1));
		}
		return 0;
	}

	template <Integral T>
	T generateBitAndErase(T* bits)
	{
		const auto rngBit{ generateBit(*bits) };
		if (rngBit)
		{
			*bits &= ~rngBit;
		}
		return rngBit;
	}

	template <std::ranges::random_access_range T>
	void shuffle(T* container)
	{
		std::ranges::shuffle(*container, m_mte);
	}

	template <std::ranges::random_access_range T>
	[[nodiscard]] T shuffle(const T& container)
	{
		T newContainer{ container };
		std::ranges::shuffle(newContainer, m_mte);
		return newContainer;
	}

	[[nodiscard]] bool generateProba(float percent);
	float generatePercent();

	void setSeed(Random::Type seed);
private:
	std::mt19937_64 m_mte;
};