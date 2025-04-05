#pragma once

#include "Common/Json.hpp"

#include <QString>

#include <filesystem>

#define UI_NAME(Variable) QtUtil::m_uiName(#Variable)

namespace QtUtil
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
	QString jsonErrorMessage(const std::filesystem::path& path, const Json::Exception& e);
}