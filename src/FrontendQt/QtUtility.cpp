#include "QtUtility.hpp"

#include "Common/Types.hpp"

#include <QMessageBox>
#include <QRegularExpression>

#include <limits>

namespace QtUtility
{
	void qStrU64Filter(QString* qStr)
	{
		static constexpr auto u64Limit{ std::numeric_limits<u64>::max() };
		static constexpr auto maxStringDigits{ std::numeric_limits<u64>::digits10 + 1 };

		if (qStr->isEmpty())
		{
			*qStr = '0';
			return;
		}

		qStr->remove(QRegularExpression("\\D"));

		if (qStr->size() > maxStringDigits)
		{
			*qStr = QString::number(u64Limit);
			return;
		}
		
		if (qStr->size() == maxStringDigits)
		{
			bool isAValidU64Number{ false };
			const auto number{ qStr->toULongLong(&isAValidU64Number) };

			if (!isAValidU64Number)
			{
				*qStr = QString::number(u64Limit);
			}
		}
	}

	QString jsonErrorMessage(const std::filesystem::path& path, const Json::Exception& e)
	{
		QString errorMessage
		{
		#ifdef _WIN32
			QString::fromStdWString(std::format(L"\"{}\" is not a valid json file, ", path.wstring()))
		#else
			QString::fromStdString(std::format("\"{}\" is not a valid json file, ", path.string()))
		#endif
		};
		errorMessage += QString::fromStdString(std::format("Reason:\n{}", e.what()));
		return errorMessage;
	}
}