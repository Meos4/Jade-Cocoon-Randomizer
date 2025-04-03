#pragma once

#include <QString>

#define UI_NAME(Variable) QtUtility::m_uiName(#Variable)

namespace QtUtility
{
	consteval auto m_uiName(const char* var)
	{
		return var + 5;
	}

	inline auto qStrToPlatformStr(const QString& qStr)
	{
		return
		#ifdef _WIN32
			qStr.toStdWString();
		#else
			qStr.toStdString();
		#endif
	}

	void qStrU64Filter(QString* qStr);
}