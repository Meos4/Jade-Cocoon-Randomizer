#pragma once

#include "Common/RawFile.hpp"
#include "Common/RawFileReadOnly.hpp"

#include <filesystem>
#include <memory>

/*
* Directory/
* |- DATA/
* |  |- ...
* |- Files/
* |  |- DATA/
* |     |- DATA.001
* |     |- VOICE.XA
* |  |- MOVIE/
* |     |- ...
* |  |- license_data.dat
* |  |- ExeFilename
* |  |- SYSTEM.CNF
* |- config.xml
*/

class GameTree final
{
public:
	GameTree(std::filesystem::path&& directory, const std::filesystem::path& exeFilename);

	std::unique_ptr<RawFile> file(const char* file) const;
	RawFile executable() const;
	std::unique_ptr<RawFileReadOnly> fileReadOnly(const char* file) const;
	RawFileReadOnly executableReadOnly() const;
	void repackDATA001() const;
	bool removeDATA001() const;
	void createIso(const std::filesystem::path* destPath) const;
	bool exists() const;
	void copy(const std::filesystem::path& dst) const;
	void copyFile(const char* one, const char* two) const;
	bool remove() const;

	const std::filesystem::path& directory() const;
	const std::filesystem::path& exePath() const;
private:
	std::filesystem::path m_directory;
	std::filesystem::path m_exePath;
};