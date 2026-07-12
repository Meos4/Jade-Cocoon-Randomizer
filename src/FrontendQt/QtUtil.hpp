#pragma once

#include "Common/Json.hpp"

#include <QString>

#include <filesystem>

#define UI_NAME(Variable) QtUtil::m_uiName(#Variable)

namespace QtUtil
{
	consteval auto m_uiName(const char* var)
	{
		constexpr char prefix[]{ "m_ui." };

		for (const char* p{ prefix }; *p != '\0'; ++p, ++var)
		{
			if (*var != *p)
			{
				throw "UI_NAME expects an \"m_ui.\" prefixed variable";
			}
		}

		return var;
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
	QString fileSaveErrorMessage(const std::filesystem::path& path);
}