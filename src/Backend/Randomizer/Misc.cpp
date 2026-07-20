#include "Backend/Randomizer.hpp"

#include "Backend/File.hpp"
#include "Backend/Resource.hpp"
#include "Backend/Mips.hpp"
#include "Backend/MipsFn.hpp"
#include "Backend/Version.hpp"

#include <array>
#include <numeric>
#include <limits>
#include <utility>
#include <vector>

Randomizer::HudColorArray Randomizer::hudColor() const
{
	return m_game->staticExecutable().read<Randomizer::HudColorArray>(m_game->offset().file.executable.hudColors);
}

void Randomizer::miscHudColor() const
{
	Randomizer::HudColorArray hudColor;
	for (auto& color : hudColor)
	{
		color = m_game->random()->generate(std::numeric_limits<u32>::max()) & 0x00FFFFFF;
	}
	miscHudColor(hudColor);
}

void Randomizer::miscHudColor(const Randomizer::HudColorArray& hud) const
{
	m_game->executable().write(m_game->offset().file.executable.hudColors, hud);
}

void Randomizer::miscNPCsVoice() const
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
			const auto rngVoice{ m_game->random()->generate(voices.size() - 1) };
			file->write(offset + i * (sizeof(Voice) + sizeof(VoiceBehavior)) + sizeof(VoiceBehavior), voices[rngVoice]);
			voices.erase(voices.begin() + rngVoice);
		}
	}
}

void Randomizer::miscBetaBattleTheme() const
{
	const auto pubtst2{ m_game->file(File::SOUND_PUBTST2_SND) };
	pubtst2->write(0, Resource::bBattleSnd);
	pubtst2->resize(Resource::bBattleSnd.size());

	const auto& game{ m_game->offset().game };
	const auto& exeOff{ m_game->offset().file.executable };
	const auto& battleOff{ m_game->offset().file.over_battle_bin };

	const auto fadeOffset{ m_game->customCodeOffset(sizeof(MipsFn::BattleThemeFade)) };

	const u16 sptrHi{ static_cast<u16>((game.soundStatePtr + 0x8000u) >> 16) };
	const u16 sptrLo{ static_cast<u16>(game.soundStatePtr) };
	const MipsFn::BattleThemeFade fadeFn
	{
		Mips_t(0x27BDFFF8),                          // addiu sp, sp, -8
		Mips_t(0xAFBF0000),                          // sw    ra, 0(sp)
		Mips::jal(game.fadeSlotFn),                  // jal   fadeSlotFn
		Mips_t(0x00000000),                          // nop
		Mips::lui(Mips::Register::v0, sptrHi),       // lui   v0, HI
		Mips_t(0x8C420000u | sptrLo),                // lw    v0, LO(v0)
		Mips_t(0x00000000),                          // nop
		Mips_t(0x8C420024),                          // lw    v0, 0x24(v0)
		Mips_t(0x00000000),                          // nop
		Mips_t(0x8C430004),                          // lw    v1, 4(v0)
		Mips_t(0x00000000),                          // nop
		Mips_t(0x84650004),                          // lh    a1, 4(v1)
		Mips_t(0x8464000C),                          // lh    a0, 0xc(v1)
		Mips_t(0x04A00003),                          // bltz  a1, +3
		Mips::li(Mips::Register::a2, 0x7f),          // addiu a2, zero, 0x7f
		Mips::jal(game.fadeSeqFn),                   // jal   fadeSeqFn
		Mips::li(Mips::Register::a3, 0x78),          // addiu a3, zero, 0x78
		Mips_t(0x8FBF0000),                          // lw    ra, 0(sp)
		Mips_t(0x27BD0008),                          // addiu sp, sp, 8
		Mips_t(0x03E00008),                          // jr    ra
		Mips_t(0x00000000)                           // nop
	};

	auto executable{ m_game->executable() };
	executable.write(exeOff.ostTable + 65u * 8u + 4u, u8{ 1 });
	executable.write(fadeOffset.file, fadeFn);

	const auto battleBin{ m_game->file(File::OVER_BATTLE_BIN) };
	battleBin->write(battleOff.battleMusicPlay, Mips::li(Mips::Register::a0, 65));
	battleBin->write(battleOff.battleMusicPlay + 4u, Mips::jal(game.playOstFn));

	const auto jalFade{ Mips::jal(fadeOffset.game) };
	battleBin->write(battleOff.battleExitFade, jalFade);
	battleBin->write(battleOff.battleExitFade + 0xC0u, jalFade);
	battleBin->write(battleOff.battleExitFade + 0x1E0u, jalFade);
}

void Randomizer::miscSkipPrologue(bool skipKoris) const
{
	static constexpr MipsFn::AfterTutorialStateData afterTutorialStateData
	{
		0xC8, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0E, 0x01, 0x0C, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x02, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00,
		0x00, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x00,
		0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x4E, 0x00,
		0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x53, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x02, 0x00, 0x07, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x02, 0x00, 0x08, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x02, 0x00, 0x09, 0x00, 0x00, 0x00, 0x5E, 0x00, 0x02, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x60, 0x00,
		0x02, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x62, 0x00, 0x02, 0x00, 0x04, 0x00, 0x01, 0x00, 0x50, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0A, 0x00,
		0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x14, 0x00,
		0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x18, 0x00, 0x01, 0x00, 0x05, 0x00, 0x00, 0x00, 0x20, 0x00,
		0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x25, 0x00, 0x01, 0x00, 0x07, 0x00, 0x00, 0x00, 0x32, 0x00,
		0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x36, 0x00, 0x01, 0x00, 0x09, 0x00, 0x00, 0x00, 0x3A, 0x00,
		0x01, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x01, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x42, 0x00,
		0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x04, 0x00, 0x03, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x00, 0x01, 0x00, 0x1C, 0x00,
		0x01, 0x00, 0x06, 0x00, 0x03, 0x00, 0x29, 0x00, 0x01, 0x00, 0x06, 0x00, 0x04, 0x00, 0x2D, 0x00,
		0x04, 0x00, 0x01, 0x00, 0x06, 0x00, 0x71, 0x00
	};
	
	const auto afterTutorialStateOffset{ m_game->customCodeOffset(sizeof(MipsFn::AfterTutorialStateData)) };

	const auto
		li32_afterTutorialState{ Mips::li32(Mips::Register::a0, afterTutorialStateOffset.game) },
		li32_gameStateStruct{ Mips::li32(Mips::Register::a1, m_game->offset().game.gameStateStruct) };

	const MipsFn::WriteAfterTutorialState writeAfterTutorialStateFn
	{
		0x27BDFFF0, // addiu sp, -0x10
		0xAFA2000C, // sw v0, 0xC(sp)
		0xAFA30008, // sw v1, 8(sp)
		0xAFA40004, // sw a0, 4(sp)
		0xAFA50000, // sw a1, 0(sp)

		li32_afterTutorialState[0], // lui a0, 0xXXXX
		li32_gameStateStruct[0], // lui a1, 0xXXXX
		li32_afterTutorialState[1], // ori a0, 0xXXXX
		li32_gameStateStruct[1], // ori a1, 0xXXXX

		// Give 200 Yan, 3 Mugwort and 1 Valerian Podwer
		0x00001821, // move v1, zero
		0x8C8B0000, // lw t3, 0(a0)
		0x24630001, // addiu v1, 1
		0xACAB0000, // sw t3, 0(a1)
		0x24840004, // addiu a0, 4
		0x2C680002, // sltiu t0, v1, 2
		0x1500FFFA, // bnez t0, -6
		0x24A50004, // addiu a1, 4

		// Give Dagger
		0x24A5007C, // addiu a1, 0x7C
		0x3C0900FF, // lui t1, 0x00FF
		0x35290101, // ori t1, 0x0101
		0xACA90000, // sw t1, 0(a1)

		// Set Story Flags and Events
		0x24A50200, // addiu a1, 0x200
		0x00001821, // move v1, zero
		0x8C8B0000, // lw t3, 0(a0)
		0x24630001, // addiu v1, 1
		0xACAB0000, // sw t3, 0(a1)
		0x24840004, // addiu a0, 4
		0x2C680058, // sltiu t0, v1, 58
		0x1500FFFA, // bnez t0, -6
		0x24A50004, // addiu a1, 4

		0x8FA2000C, // lw v0, 0xC(sp)
		0x8FA30008, // lw v1, 8(sp)
		0x8FA40004, // lw a0, 4(sp)
		0x8FA50000, // lw a1, 0(sp)
		0x03E00008, // jr ra
		0x27BD0010  // addiu sp, 0x10
	};

	auto executable{ m_game->executable() };

	const auto writeAfterTutorialStateOffset{ m_game->customCodeOffset(sizeof(MipsFn::WriteAfterTutorialState)) };

	executable.write(afterTutorialStateOffset.file, afterTutorialStateData);
	executable.write(writeAfterTutorialStateOffset.file, writeAfterTutorialStateFn);

	if (skipKoris)
	{
		executable.write(afterTutorialStateOffset.file + 0x16, u8(1));

		// Set the FOREST1/SCE00 entry event script as already played, (event bit 2 + base bit 1)
		executable.write(afterTutorialStateOffset.file + 0x14, u8(6));

		// Reset the previous map triple (4,1,0) != (-1,-1,-1) otherwise
		// it considers the zone unchanged and keeps the minions spawn pool empty
		executable.write(afterTutorialStateOffset.file + 0x46, u16(0xFFFF));
		executable.write(afterTutorialStateOffset.file + 0x4A, u16(0xFFFF));
		executable.write(afterTutorialStateOffset.file + 0x4E, u16(0xFFFF));
		executable.write(afterTutorialStateOffset.file + 0x52, u16(0xFFFF));

		// Entry door index = -1 to match the engine "no door transition" state,
		// otherwise the scene object spawner never runs on the arrival map
		// and its doors + Knowledge 1 pickup don't spawn
		executable.write(afterTutorialStateOffset.file + 0x50, u16(0xFFFF));

		executable.write(afterTutorialStateOffset.file + 0xCE, u8(0x0B));
		executable.write(afterTutorialStateOffset.file + 0x126, u8(0x64));
		executable.write(afterTutorialStateOffset.file + 0x12E, u8(0x6A));
		executable.write(afterTutorialStateOffset.file + 0x12F, u8(0));

		// Spawn at the Beetle Forest, (4,1,0)
		m_game->file(File::OVER_CHAPTER_BIN)->write(m_game->offset().file.over_chapter_bin.chapter2StartMapId, u16(0x6A));
	}
	else
	{
		// Spawn at the Gate, (4,0,1)
		m_game->file(File::OVER_CHAPTER_BIN)->write(m_game->offset().file.over_chapter_bin.chapter2StartMapId, u16(0x65));
	}

	static constexpr u32 chapterFsmStatesTable{ 4 };

	const auto
		state0Fn{ m_game->staticFile(File::OVER_CHAPTER_BIN)->read<u32>(chapterFsmStatesTable) },
		lastStateFn{ m_game->staticFile(File::OVER_CHAPTER_BIN)->read<u32>(chapterFsmStatesTable + 0x14) };

	const u32 currentChapter{ m_game->offset().game.gameStateStruct + 0x2B8 };

	const u16
		currentChapterHi{ static_cast<u16>((currentChapter + 0x8000) >> 16) },
		currentChapterLo{ static_cast<u16>(currentChapter) };

	const MipsFn::SkipChapter2Cinematic skipChapter2CinematicFn
	{
		Mips::lui(Mips::Register::v0, currentChapterHi),
		Mips_t(0x84430000 | currentChapterLo), // lh v1, currentChapter(v0)
		Mips::li(Mips::Register::v0, 1),
		Mips_t(0x14620003), // bne v1, v0, +3
		Mips_t(0x00000000), // nop
		Mips::j(lastStateFn),
		Mips_t(0xAE400004), // sw zero, 4(s2), the NTSC-J entry does not initialize the last state timer
		Mips::j(state0Fn),
		Mips_t(0x00000000)  // nop
	};

	const auto skipChapter2CinematicOffset{ m_game->customCodeOffset(sizeof(MipsFn::SkipChapter2Cinematic)) };

	executable.write(skipChapter2CinematicOffset.file, skipChapter2CinematicFn);
	m_game->file(File::OVER_CHAPTER_BIN)->write(chapterFsmStatesTable, skipChapter2CinematicOffset.game);

	const auto over_title_bin{ m_game->file(File::OVER_TITLE_BIN) };

	const auto
		sb_setLevantGarb{ m_game->isNtscJ() ? Mips_t(0xA04007DD) : Mips_t(0xA04007E5)}; // sb zero, 0x7DD/0x7E5(v0)

	over_title_bin->write(m_game->offset().file.over_title_bin.initMapNewGame, Mips::jal(writeAfterTutorialStateOffset.game));
	over_title_bin->write(m_game->offset().file.over_title_bin.initMapNewGame - 8, Mips_t(0));
	over_title_bin->write(m_game->offset().file.over_title_bin.setLevantGarb, sb_setLevantGarb);
}

void Randomizer::miscItemQuantityLimit(u8 limit) const
{
	const auto
		over_game_bin{ m_game->file(File::OVER_GAME_BIN) },
		over_wpnshop_bin{ m_game->file(File::OVER_WPNSHOP_BIN) };

	const auto
		slti_v0_v0{ Mips_t(0x28420000 + limit + 1) },
		slti_v1_v1{ Mips_t(0x28630000 + limit) },
		slti_v0_s0{ Mips_t(0x2A020000 + limit + 1) },
		li_s0{ Mips_t(0x24100000 + limit) },
		li_v0{ Mips_t(0x24020000 + limit) },
		li_v1{ Mips_t(0x24030000 + limit) };

	over_game_bin->write(m_game->offset().file.over_game_bin.isQuantityLimitReachedFn + 8, slti_v0_v0);
	over_game_bin->write(m_game->offset().file.over_game_bin.itemShopBuyFn + 0x74, slti_v1_v1);

	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.itemShopQuantityLimitFn + 0x58, slti_v0_s0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.itemShopQuantityLimitFn + 0x64, li_s0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.itemShopQuantityLimitFn + 0x6C, slti_v0_v0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.itemShopQuantityLimitFn + 0x78, li_v0);

	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.equipmentShopQuantityLimitFn + 0x88, slti_v0_s0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.equipmentShopQuantityLimitFn + 0x94, li_s0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.equipmentShopQuantityLimitFn + 0x9C, slti_v0_v0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.equipmentShopQuantityLimitFn + 0xA8, li_v0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.equipmentShopQuantityLimitFn + 0xF4, li_v1);

	const MipsFn::SetChestNewItemQuantityLimit setChestNewItemQuantityLimitFn
	{
		// If quantity is > than the limit
		0x2C620000 + limit + 1u, // sltiu v0, v1, limit + 1
		0x14400002, // bnez v0, +2
		0x00000000, // nop

		// Set quantity to limit
		Mips::li(Mips::Register::v1, limit),

		0x03E00008, // jr ra
		0xA0900000  // sb s0, 0(a0)
	};

	const auto setChestNewItemQuantityLimitOffset{ m_game->customCodeOffset(sizeof(MipsFn::SetChestNewItemQuantityLimit)) };

	m_game->executable().write(setChestNewItemQuantityLimitOffset.file, setChestNewItemQuantityLimitFn);
	over_game_bin->write(m_game->offset().file.over_game_bin.setItemQuantityFromChestFn + 0x34, Mips::jal(setChestNewItemQuantityLimitOffset.game));
}

void Randomizer::miscLevelCapEC(u8 levelCap) const
{
	if (levelCap != 26 && levelCap >= 19 && levelCap < 64)
	{
		const auto
			slti_v0_v1{ Mips_t(0x28620000 + levelCap - 18) },
			li_v1{ Mips_t(0x24030000 + levelCap - 19) };

		const auto over_game_bin{ m_game->file(File::OVER_GAME_BIN) };

		over_game_bin->write(m_game->offset().file.over_game_bin.levelCapEC, slti_v0_v1);
		over_game_bin->write(m_game->offset().file.over_game_bin.levelCapEC + 0xC, li_v1);

		const auto shift{ m_game->isVersion(Version::NtscJ1) ? 0x238 : 0x250 };

		over_game_bin->write(m_game->offset().file.over_game_bin.levelCapEC - shift, slti_v0_v1);
		over_game_bin->write(m_game->offset().file.over_game_bin.levelCapEC - shift + 0xC, li_v1);
	}
}

void Randomizer::miscPalToNtsc() const
{
	if (!m_game->isNtsc())
	{
		auto executable{ m_game->executable() };

		static constexpr auto move_a0_zero{ Mips_t(0x00002021) };

		executable.write(m_game->offset().file.executable.setVideoModeArgument, move_a0_zero);
		executable.write(m_game->offset().file.executable.stretchingY, move_a0_zero);
	}
}