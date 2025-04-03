#pragma once

#include "Backend/Offset.hpp"
#include "Backend/Version.hpp"
#include "Common/RawFile.hpp"
#include "Common/TemplateTypes.hpp"
#include "Common/Types.hpp"

#include <filesystem>
#include <memory>
#include <optional>

class Game final
{
public:
	struct CustomCodeOffset
	{
		u32 file;
		u32 game;
	};

	static constexpr auto sectorSize{ 2048u };

	Game(const std::filesystem::path& exePath, std::filesystem::path&& directoryPath, Version version);
	
	std::unique_ptr<RawFile> file(s32 file) const;
	RawFile executable() const;
	const char* filePathByIndex(s32 file) const;

	void expandExecutable() const;
	Game::CustomCodeOffset customCodeOffset(u32 size);
	u32 heapRandomizerBegin() const;
	u32 gameToFileTextSectionShift() const;
	bool isVanilla() const;
	void repackFilesToDATA001();
	void createIsoFromFiles(const std::filesystem::path* destPath);

	template <SameAs<Version>... Args>
	bool isVersion(Args... versions) const
	{
		return ((m_version == versions) || ...);
	}
	bool isNtsc() const;

	const char* versionText() const;
	const char* serialText() const;
	Version version() const;
	const Offset& offset() const;
	const std::filesystem::path& gameDirectory() const;

	static bool generateCue(const std::filesystem::path& isoPath);
	static std::optional<Version> versionFromIso(const std::filesystem::path& isoPath);
	static Game createGame(const std::filesystem::path& isoPath, std::filesystem::path&& gameDirectory);
	static std::optional<Game> createGame(std::filesystem::path&& gameDirectory);
private:
	s32 fileByVersion(s32 file) const;

	std::filesystem::path m_exePath;
	std::filesystem::path m_gameDirectory;
	std::vector<const char*> m_data001FilesPath;
	Version m_version;
	Offset m_offset;
	u32 m_ccShift{};
};