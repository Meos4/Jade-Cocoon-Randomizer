#include "Misc.hpp"

#include "Backend/File.hpp"
#include "Backend/Random.hpp"

#include <numeric>
#include <limits>
#include <utility>
#include <vector>

Misc::Misc(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
	: m_game(std::move(game)), m_sharedData(std::move(sharedData))
{
}

Misc::HudColorArray Misc::hudColor() const
{
	return m_game->staticExecutable().read<Misc::HudColorArray>(m_game->offset().file.executable.hudColors);
}

void Misc::setHudColor() const
{
	Misc::HudColorArray hudColor;
	for (auto& color : hudColor)
	{
		color = Random::get().generate(std::numeric_limits<u32>::max()) & 0x00FFFFFF;
	}
	setHudColor(hudColor);
}

void Misc::setHudColor(const Misc::HudColorArray& hud) const
{
	m_game->executable().write(m_game->offset().file.executable.hudColors, hud);
}

void Misc::setNPCsVoice() const
{
	struct VoiceBehavior
	{
		u8 dialogueId, subDialogueId;
	};

	struct Voice
	{
		s16 xaPosition, xaPosition2, duration;
	};

	struct FileVoiceInfo
	{
		std::unique_ptr<RawFile> file;
		u32 offset;
		u32 nbVoices;
	};

	const auto& offsetF{ m_game->offset().file };

	const std::array<FileVoiceInfo, 78> filesInfo
	{{
		{ m_game->file(File::SCENE_PSYRUS2_LOOKOUT_SCE01_SBH), offsetF.scene_psyrus2_lookout_sce01_sbh.tableOfVoices, 15 },
		{ m_game->file(File::SCENE_PSYRUS2_LOOKOUT_SCE00A_SBH), offsetF.scene_psyrus2_lookout_sce00a_sbh.tableOfVoices, 11 },
		{ m_game->file(File::SCENE_PSYRUS2_LEBANT_SCE00_SBH), offsetF.scene_psyrus2_lebant_sce00_sbh.tableOfVoices, 7 },
		{ m_game->file(File::SCENE_OTHER_DREAM_SCE00_SBH), offsetF.scene_other_dream_sce00_sbh.tableOfVoices, 11 },
		{ m_game->file(File::SCENE_PSYRUS1_LEBANT_SCE00_SBH), offsetF.scene_psyrus1_lebant_sce00_sbh.tableOfVoices, 16 },
		{ m_game->file(File::SCENE_PSYRUS1_ZOKUCHO_SCE01_SBH), offsetF.scene_psyrus1_zokucho_sce01_sbh.tableOfVoices, 44 },
		{ m_game->file(File::SCENE_PSYRUS1_ZOKUCHO_SCE05_SBH), offsetF.scene_psyrus1_zokucho_sce05_sbh.tableOfVoices, 30 },
		{ m_game->file(File::SCENE_PSYRUS2_LOOKOUT_SCE01A_SBH), offsetF.scene_psyrus2_lookout_sce01a_sbh.tableOfVoices, 18 },
		{ m_game->file(File::SCENE_OTHER_DREAM_SCE00A_SBH), offsetF.scene_other_dream_sce00a_sbh.tableOfVoices, 7 },
		{ m_game->file(File::SCENE_PSYRUS1_LEBANT_SCE00A_SBH), offsetF.scene_psyrus1_lebant_sce00a_sbh.tableOfVoices, 37 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE00_SBH), offsetF.scene_psyrus1_garai_sce00_sbh.tableOfVoices, 25 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE03_SBH), offsetF.scene_psyrus1_garai_sce03_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_PSYRUS1_KAJIYA_SCE00_SBH), offsetF.scene_psyrus1_kajiya_sce00_sbh.tableOfVoices, 26 },
		{ m_game->file(File::SCENE_PSYRUS1_LOOKOUT_SCE00_SBH), offsetF.scene_psyrus1_lookout_sce00_sbh.tableOfVoices, 28 },
		{ m_game->file(File::SCENE_PSYRUS1_CEMETERY_SCE00_SBH), offsetF.scene_psyrus1_cemetery_sce00_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD1_GATE_SCE01_SBH), offsetF.scene_field1_gate_sce01_sbh.tableOfVoices, 5 },
		{ m_game->file(File::SCENE_FIELD1_GATE_SCE00A_SBH), offsetF.scene_field1_gate_sce00a_sbh.tableOfVoices, 80 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE01_SBH), offsetF.scene_psyrus1_garai_sce01_sbh.tableOfVoices, 92 },
		{ m_game->file(File::SCENE_FIELD1_FOREST1_SCE06A_SBH), offsetF.scene_field1_forest1_sce06a_sbh.tableOfVoices, 7 },
		{ m_game->file(File::SCENE_FIELD1_FOREST1_SCE09_SBH), offsetF.scene_field1_forest1_sce09_sbh.tableOfVoices, 44 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE01A_SBH), offsetF.scene_psyrus1_garai_sce01a_sbh.tableOfVoices, 51 },
		{ m_game->file(File::SCENE_PSYRUS1_KAJIYA_SCE00A_SBH), offsetF.scene_psyrus1_kajiya_sce00a_sbh.tableOfVoices, 11 },
		{ m_game->file(File::SCENE_PSYRUS1_LOOKOUT_SCE00A_SBH), offsetF.scene_psyrus1_lookout_sce00a_sbh.tableOfVoices, 21 },
		{ m_game->file(File::SCENE_PSYRUS1_CEMETERY_SCE00A_SBH), offsetF.scene_psyrus1_cemetery_sce00a_sbh.tableOfVoices, 22 },
		{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE13A_SBH), offsetF.scene_field1_forest2_sce13a_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE14A_SBH), offsetF.scene_field1_forest2_sce14a_sbh.tableOfVoices, 5 },
		{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE07_SBH), offsetF.scene_field1_forest2_sce07_sbh.tableOfVoices, 81 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE01B_SBH), offsetF.scene_psyrus1_garai_sce01b_sbh.tableOfVoices, 56 },
		{ m_game->file(File::SCENE_PSYRUS1_LOOKOUT_SCE01_SBH), offsetF.scene_psyrus1_lookout_sce01_sbh.tableOfVoices, 40 },
		{ m_game->file(File::SCENE_PSYRUS1_LEBANT_SCE00C_SBH), offsetF.scene_psyrus1_lebant_sce00c_sbh.tableOfVoices, 39 },
		{ m_game->file(File::SCENE_PSYRUS1_KAJIYA_SCE00B_SBH), offsetF.scene_psyrus1_kajiya_sce00b_sbh.tableOfVoices, 33 },
		{ m_game->file(File::SCENE_PSYRUS1_LOOKOUT_SCE00B_SBH), offsetF.scene_psyrus1_lookout_sce00b_sbh.tableOfVoices, 27 },
		{ m_game->file(File::SCENE_PSYRUS1_CEMETERY_SCE00B_SBH), offsetF.scene_psyrus1_cemetery_sce00b_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE11_SBH), offsetF.scene_field1_forest3_sce11_sbh.tableOfVoices, 12 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE12_SBH), offsetF.scene_field1_forest3_sce12_sbh.tableOfVoices, 13 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE03A_SBH), offsetF.scene_field1_forest3_sce03a_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE13_SBH), offsetF.scene_field1_forest3_sce13_sbh.tableOfVoices, 19 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE14_SBH), offsetF.scene_field1_forest3_sce14_sbh.tableOfVoices, 40 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE01C_SBH), offsetF.scene_psyrus1_garai_sce01c_sbh.tableOfVoices, 54 },
		{ m_game->file(File::SCENE_PSYRUS1_ZOKUCHO_SCE01A_SBH), offsetF.scene_psyrus1_zokucho_sce01a_sbh.tableOfVoices, 29 },
		{ m_game->file(File::SCENE_PSYRUS1_LEBANT_SCE00D_SBH), offsetF.scene_psyrus1_lebant_sce00d_sbh.tableOfVoices, 27 },
		{ m_game->file(File::SCENE_PSYRUS1_KAJIYA_SCE00C_SBH), offsetF.scene_psyrus1_kajiya_sce00c_sbh.tableOfVoices, 17 },
		{ m_game->file(File::SCENE_PSYRUS1_LOOKOUT_SCE00C_SBH), offsetF.scene_psyrus1_lookout_sce00c_sbh.tableOfVoices, 8 },
		{ m_game->file(File::SCENE_PSYRUS1_CEMETERY_SCE00C_SBH), offsetF.scene_psyrus1_cemetery_sce00c_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE11A_SBH), offsetF.scene_field1_forest3_sce11a_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE12A_SBH), offsetF.scene_field1_forest3_sce12a_sbh.tableOfVoices, 5 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE13A_SBH), offsetF.scene_field1_forest3_sce13a_sbh.tableOfVoices, 9 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE14A_SBH), offsetF.scene_field1_forest3_sce14a_sbh.tableOfVoices, 9 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE20_SBH), offsetF.scene_field1_forest3_sce20_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_PSYRUS2_GARAI_SCE00_SBH), offsetF.scene_psyrus2_garai_sce00_sbh.tableOfVoices, 20 },
		{ m_game->file(File::SCENE_PSYRUS2_KAJIYA_SCE00_SBH), offsetF.scene_psyrus2_kajiya_sce00_sbh.tableOfVoices, 11 },
		{ m_game->file(File::SCENE_PSYRUS2_ZOKUCHO_SCE03_SBH), offsetF.scene_psyrus2_zokucho_sce03_sbh.tableOfVoices, 19 },
		{ m_game->file(File::SCENE_PSYRUS2_GARAI_SCE03_SBH), offsetF.scene_psyrus2_garai_sce03_sbh.tableOfVoices, 24 },
		{ m_game->file(File::SCENE_FIELD1_GATE_SCE06_SBH), offsetF.scene_field1_gate_sce06_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE32_SBH), offsetF.scene_field1_forest3_sce32_sbh.tableOfVoices, 2 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE33_SBH), offsetF.scene_field1_forest3_sce33_sbh.tableOfVoices, 2 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE34_SBH), offsetF.scene_field1_forest3_sce34_sbh.tableOfVoices, 2 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE09A_SBH), offsetF.scene_field1_forest4_sce09a_sbh.tableOfVoices, 6 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE12_SBH), offsetF.scene_field1_forest4_sce12_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE17_SBH), offsetF.scene_field1_forest4_sce17_sbh.tableOfVoices, 2 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE18_SBH), offsetF.scene_field1_forest4_sce18_sbh.tableOfVoices, 33 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE19_SBH), offsetF.scene_field1_forest4_sce19_sbh.tableOfVoices, 17 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE16_SBH), offsetF.scene_field1_forest4_sce16_sbh.tableOfVoices, 36 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE15_SBH), offsetF.scene_field1_forest4_sce15_sbh.tableOfVoices, 16 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE14_SBH), offsetF.scene_field1_forest4_sce14_sbh.tableOfVoices, 14 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE16A_SBH), offsetF.scene_field1_forest4_sce16a_sbh.tableOfVoices, 39 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE16B_SBH), offsetF.scene_field1_forest4_sce16b_sbh.tableOfVoices, 36 },
		{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE07_SBH), offsetF.scene_field2_forest1_sce07_sbh.tableOfVoices, 22 },
		{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE07_SBH), offsetF.scene_field2_forest2_sce07_sbh.tableOfVoices, 24 },
		{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE11A_SBH), offsetF.scene_field2_forest3_sce11a_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE12A_SBH), offsetF.scene_field2_forest3_sce12a_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE13A_SBH), offsetF.scene_field2_forest3_sce13a_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE20_SBH), offsetF.scene_field2_forest3_sce20_sbh.tableOfVoices, 24 },
		{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE22A_SBH), offsetF.scene_field2_forest3_sce22a_sbh.tableOfVoices, 22 },
		{ m_game->file(File::SCENE_OTHER_CLEAR_SCE00_SBH), offsetF.scene_other_clear_sce00_sbh.tableOfVoices, 8 },
		{ m_game->file(File::SCENE_OTHER_CLEAR_SCE00_SBH), offsetF.scene_other_clear_sce00_sbh.tableOfVoices + 0x48, 7 }, // Multiplayer
		{ m_game->file(File::SCENE_OTHER_CLEAR_SCE03_SBH), offsetF.scene_other_clear_sce03_sbh.tableOfVoices, 3 },
		{ m_game->file(File::SCENE_OTHER_CLEAR_SCE04_SBH), offsetF.scene_other_clear_sce04_sbh.tableOfVoices, 14 }
	}};		

	const auto totalVoices{ std::accumulate(filesInfo.begin(), filesInfo.end(), 0u,
		[](auto sum, const auto& fvi) { return sum + fvi.nbVoices; }) };
		
	std::vector<Voice> voices;
	voices.reserve(totalVoices);

	for (const auto& [file, offset, nbVoices] : filesInfo)
	{
		for (u32 i{}; i < nbVoices; ++i)
		{
			voices.emplace_back(file->read<Voice>(offset + i * (sizeof(Voice) + sizeof(VoiceBehavior)) + sizeof(VoiceBehavior)));
		}
	}

	for (const auto& [file, offset, nbVoices] : filesInfo)
	{
		for (u32 i{}; i < nbVoices; ++i)
		{
			const auto rngVoice{ Random::get().generate(voices.size() - 1) };
			file->write(offset + i * (sizeof(Voice) + sizeof(VoiceBehavior)) + sizeof(VoiceBehavior), voices[rngVoice]);
			voices.erase(voices.begin() + rngVoice);
		}
	}
}