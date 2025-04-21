#pragma once

#include "Backend/File.hpp"
#include "Backend/GameTree.hpp"
#include "Backend/Offset.hpp"
#include "Backend/Version.hpp"
#include "Common/RawFile.hpp"
#include "Common/RawFileReadOnly.hpp"
#include "Common/Random.hpp"
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

	Game(const std::filesystem::path& exePath, std::filesystem::path&& directoryPath, Version version);
	
	std::unique_ptr<RawFile> file(File file) const;
	RawFile executable() const;
	std::unique_ptr<RawFileReadOnly> staticFile(File file) const;
	RawFileReadOnly staticExecutable() const;
	const char* filePathByIndex(File file) const;

	void expandExecutable();
	Game::CustomCodeOffset customCodeOffset(u32 size);
	u32 heapRandomizerBegin() const;
	u32 gameToFileTextSectionShift() const;
	bool isVanilla() const;
	bool removeStaticDirectory() const;
	void createBuilderDirectory() const;

	template <SameAs<Version>... Args>
	bool isVersion(Args... versions) const
	{
		return ((m_version == versions) || ...);
	}
	bool isNtsc() const;
	bool isNtscJ() const;

	const char* versionText() const;
	const char* serialText() const;
	Version version() const;
	const Offset& offset() const;
	const GameTree& builderTree() const;
	Random* random();

	static std::unique_ptr<Game> createGame(const std::filesystem::path& isoPath, std::filesystem::path&& gameDirectory);
	static std::unique_ptr<Game> createGame(std::filesystem::path&& gameDirectory);
private:
	File fileByVersion(File file) const;

	GameTree m_staticTree;
	GameTree m_builderTree;
	Random m_random;
	std::vector<const char*> m_data001FilesPath;
	Version m_version;
	Offset m_offset;
	u32 m_ccShift{};
};