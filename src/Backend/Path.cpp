#include "Path.hpp"

namespace Path
{
	std::array<Path::CStringPlatform, 7> dumpIsoArgs(
		const std::filesystem::path* isoPath, 
		const std::filesystem::path* configXmlPath, 
		const std::filesystem::path* filesPath)
	{
		return
		{
			#ifdef _WIN32
				L"",
				isoPath->c_str(),
				L"-s",
				configXmlPath->c_str(),
				L"-x",
				filesPath->c_str(),
				nullptr
			#else
				"",
				isoPath->c_str(),
				"-s",
				configXmlPath->c_str(),
				"-x",
				filesPath->c_str(),
				nullptr
			#endif	
		};	
	}

	std::array<Path::CStringPlatform, 7> makeIsoArgs(const std::filesystem::path* isoPath, const std::filesystem::path* configXmlPath)
	{
		return
		{
			#ifdef _WIN32
				L"",
				L"-y",
				L"-q",
				L"-o",
				isoPath->c_str(),
				configXmlPath->c_str(),
				nullptr
			#else
				"",
				"-y",
				"-q",
				"-o",
				isoPath->c_str(),
				configXmlPath->c_str(),
				nullptr
			#endif
		};
	}
}