#include "FileSystem.hpp"

namespace FileSystem
{
	void copyTree(const std::filesystem::path& src, const std::filesystem::path& dst)
	{
		std::filesystem::create_directories(dst);

		for (const auto& entry : std::filesystem::recursive_directory_iterator(src))
		{
			const auto& srcPath{ entry.path() };
			const auto srcRelative{ std::filesystem::relative(srcPath, src) };
			const auto destPath{ dst / srcRelative };

			if (std::filesystem::is_directory(entry.status()))
			{
				std::filesystem::create_directories(destPath);
			}
			else
			{
				std::filesystem::copy(srcPath, destPath, std::filesystem::copy_options::overwrite_existing);
			}
		}
	}

	bool remove(const std::filesystem::path& path)
	{
		std::error_code err;
		const auto nbRemoved{ std::filesystem::remove_all(path, err) };
		return nbRemoved != -1 ? true : false;
	}
}