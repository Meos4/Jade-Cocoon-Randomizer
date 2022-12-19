#include "QtUtility.hpp"

#include "Common/Types.hpp"

#include <limits>
#include <string>

namespace QtUtility
{
	void qStrToQStrU64(QString* qStr)
	{
		static constexpr auto u64Limit{ std::numeric_limits<u64>::max() };

		if (qStr->isEmpty())
		{
			*qStr = '0';
			return;
		}

		for (const auto& ch : *qStr)
		{
			if (ch < '0' || ch > '9')
			{
				*qStr = '0';
				return;
			}
		}

		if (qStr->size() > 20)
		{
			*qStr = QString::number(u64Limit);
			return;
		}
		
		if (qStr->size() == 20)
		{
			std::string str{ qStr->toStdString() }, val1, val2;
			for (std::size_t i{}; i != str.size() / 2; ++i)
			{
				val1 += str[i];
				val2 += str[i + 10];
			}
			if (std::stoull(val1) > 1844674407 ||
				(std::stoull(val1) == 1844674407 && std::stoull(val2) > 3709551615))
			{
				*qStr = QString::fromStdString(std::to_string(u64Limit));
				return;
			}
		}
	}
}