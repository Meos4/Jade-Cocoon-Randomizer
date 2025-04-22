#include "Backend/Randomizer.hpp"

void Randomizer::challengeNuzlocke(Randomizer::ChallengeNuzlocke_t state) const
{
	auto executable{ m_game->executable() };
	const auto over_battle_bin{ m_game->file(File::OVER_BATTLE_BIN) };

	if (state & Randomizer::CHALLENGE_NUZLOCKE_DEFINITIVE_DEATH)
	{
		const auto li32_minion1State{ Mips::li32(Mips::Register::t0, m_game->offset().game.minion1State) };
		const bool isNtscJ{ m_game->isNtscJ() };

		const MipsFn::NuzlockeDefinitiveDeath nuzlockeDefinitiveDeathFn
		{
			// If Levant animation is "Unsummon"
			0x2408001C, // li t0, 0x1C
			0x14880013, // bne a0, t0, 0x13
			m_game->isVersion(Version::NtscJ1, Version::NtscU) ? Mips_t(0x8E270038) : Mips_t(0x8E470038), // lw a3, 0x38(s1/s2)

			// If a minion has state Death (0x400), change the slot to empty
			li32_minion1State[0], // lui t0, 0xXXXX
			li32_minion1State[1], // ori t0, 0xXXXX
			0x00005021, // move t2, zero
			0x240B0003, // li t3, 3
			0x95090000, // lhu t1, 0(t0)
			0x00000000, // nop
			0x31290400, // andi t1, 0x0400
			0x15200005, // bnez t1, 5
			0x254A0001, // addiu t2, 1
			0x154BFFFA, // bne t2, t3, -5
			isNtscJ ? Mips_t(0x250800DC) : Mips_t(0x250800F8), // addiu t0, 0xDC/0xF8
			0x10000006, // b, +6
			0x00000000, // nop
			isNtscJ ? Mips_t(0x2508FF94) : Mips_t(0x2508FF7C), // addiu t0, -0x6C/-0x84
			0x00094A02, // srl t1, 8
			0xA5090000, // sh t1, 0(t0)
			0x1000FFF8, // b, -8
			isNtscJ ? Mips_t(0x2508006C) : Mips_t(0x25080084), // addiu t0, 0x6C/0x84

			Mips::j(m_game->offset().game.levantAnimArgs + 8),
			0x00000000  // nop
		};

		const auto nuzlockeDefinitiveDeathOffset{ m_game->customCodeOffset(sizeof(MipsFn::NuzlockeDefinitiveDeath)) };

		executable.write(nuzlockeDefinitiveDeathOffset.file, nuzlockeDefinitiveDeathFn);
		m_game->file(File::OVER_GAME_BIN)->write(m_game->offset().file.over_game_bin.levantAnimArgs, Mips::j(nuzlockeDefinitiveDeathOffset.game));
	}

	if (state & Randomizer::CHALLENGE_NUZLOCKE_ONE_CAPTURE)
	{
		static constexpr MipsFn::WriteUsedMapEOBData nuzlockeMaps
		{
			0x00, 0x03, // Eternal Corridor
			0x04, 0x00, // Koris Minion
			0x04, 0x01, // Beetle Forest
			0x04, 0x02, // Dragonfly Forest
			0x04, 0x03, // Spider Forest
			0x04, 0x04, // Moth Forest
			0x05, 0x01, // Netherworld Beetle Forest
			0x05, 0x02, // Netherworld Dragonfly Forest
			0x05, 0x03, // Netherworld Spider Forest
			0x00, 0x00  // 4 Bytes padding
		};
		
		const auto nuzlockeValidMapsOffset{ m_game->customCodeOffset(sizeof(MipsFn::WriteUsedMapEOBData)) };

		const auto
			li32_idOpponent1{ Mips::li32(Mips::Register::t0, m_game->offset().game.idOpponent1) },
			li32_mapId{ Mips::li32(Mips::Register::t0, m_game->offset().game.mapId) },
			li32_validMaps{ Mips::li32(Mips::Register::t0, nuzlockeValidMapsOffset.game) },
			li32_levantStats{ Mips::li32(Mips::Register::t0, m_game->offset().game.levantStats) },
			li32_captureText{ Mips::li32(Mips::Register::t0, m_game->offset().game.captureText) },
			li32_fireflyCounter{ Mips::li32(Mips::Register::t0, m_game->offset().game.levantFireflyCounter) };

		const MipsFn::WriteUsedMapEOB writeUsedMapEOBFn
		{
			0x27BDFFF0, // addiu sp, -0x10
			0xAFA20000, // sw v0, 0(sp)
			0xAFA30004, // sw v1, 4(sp)

			// If opponent 1 is not a boss 
			li32_idOpponent1[0], // lui t0, 0xXXXX
			li32_idOpponent1[1], // ori t0, 0xXXXX
			0x8D020000, // lw v0, 0(t0)
			0x00000000, // nop
			0x2C430029, // sltiu v1, v0, 0x29
			0x1C600004, // bgtz v1, 4
			0x2C430039, // sltiu v1, v0, 0x39
			0x1C60001D, // bgtz v1, 0x1D
			0x2C4300C9, // sltiu v1, v0, 0xC9
			0x0460001B, // bltz v1, 0x1B

			// Find capture id of this map
			0x2409FFFF, // li t1, -1
			li32_mapId[0], // lui t0, 0xXXXX
			li32_mapId[1], // ori t0, 0xXXXX
			0x950A0000, // lhu t2, 0(t0)
			0x00000000, // nop
			0x950B0004, // lhu t3, 4(t0)
			0x00000000, // nop
			li32_validMaps[0], // lui t0, 0xXXXX
			li32_validMaps[1], // ori t0, 0xXXXX
			0x240C0008, // li t4, 8
			0x91020000, // lbu v0, 0(t0)
			0x00000000, // nop
			0x91030001, // lbu v1, 1(t0)
			0x00000000, // nop
			0x112C000C, // beq t1, t4, 0xC
			0x25290001, // addiu t1, 1
			0x144AFFF9, // bne v0, t2, -6
			0x25080002, // addiu t0, 2
			0x146BFFF7, // bne v1, t3, -8
			0x00000000, // nop

			// Add actual capture id on the Levant Max MP
			li32_levantStats[0], // lui t0, 0xXXXX
			li32_levantStats[1], // ori t0, 0xXXXX
			0x9502000E, // lhu v0, 0xE(t0)
			0x24030001, // li v1, 1
			0x01231804, // sllv v1, t1
			0x00431025, // or v0, v1
			0xA502000E, // sh v0, 0xE(t0)

			0x8FA30004, // lw v1, 4(sp)
			0x8FA20000, // lw v0, 0(sp)
			0x03E00008, // jr ra
			0X27BD0010  // addiu sp, -0x10
		};

		const MipsFn::WriteUsedMapCapture writeUsedMapCaptureFn
		{
			0x27BDFFF0, // addiu sp, -0x10
			0xAFBF0000, // sw ra, 0(sp)
			Mips::jal(m_game->offset().game.writeNewFirefly),
			0x00000000, // nop
			0x8FBF0000, // lw ra, 0(sp)
			0xAFA20000, // sw v0, 0(sp)
			Mips::j(nuzlockeValidMapsOffset.game + 0x48),
			0xAFA30004, // sw v1, 4(sp)
		};

		const MipsFn::ReadUsedMap readUsedMapFn
		{
			0x27BDFFF0, // addiu sp, -0x10
			0xAFA30000, // sw v1, 0(sp)
			li32_idOpponent1[0], // lui t0, 0xXXXX
			li32_idOpponent1[1], // ori t0, 0xXXXX
			0x8D020000, // lw v0, 0(t0)
			0x00000000, // nop
			0x2C430029, // sltiu v1 v0, 0x29
			0x1C600004, // bgtz v1, 4
			0x2C430039, // sltiu v1 v0, 0x39
			0x1C600024, // bgtz v1, 0x24
			0x2C4300C9, // sltiu v1 v0, 0xC9
			0x04600022, // bltz v1, 0x22
			0x2409FFFF, // addiu t1, -1
			li32_mapId[0], // lui t0, 0xXXXX
			li32_mapId[1], // ori t0, 0xXXXX
			0x950A0000, // lhu t2, 0(t0)
			0x00000000, // nop
			0x950B0004, // lhu t3, 4(t0)
			0x00000000, // nop
			li32_validMaps[0], // lui t0, 0xXXXX
			li32_validMaps[1], // ori t0, 0xXXXX
			0x240C0008, // li t4, 8
			0x91020000, // lbu v0, 0(t0)
			0x00000000, // nop
			0x91030001, // lbu v1, 1(t0) 
			0x00000000, // nop
			0x112C0013, // beq t1, t4, 0x13
			0x25290001, // addiu t1, 1
			0x144AFFF9, // bne t0, t2, -6
			0x25080002, // addiu t0, 2
			0x146BFFF7, // bne v1, t3, -8
			0x00000000, // nop
			li32_levantStats[0], // lui t0, 0xXXXX
			li32_levantStats[1], // ori t0, 0xXXXX
			0x9502000E, // lhu v0, 0xE
			0x24030001, // li v1, 1
			0x01231804, // sllv v1, t1
			0x00431024, // and v0, v1
			0x10400007, // beqz v0, 7
			li32_captureText[0], // lui t0, 0xXXXX
			li32_captureText[1], // ori t0, 0xXXXX
			0x01004821, // move t1, t0
			0x2529FFE0, // addiu t1, -0x20
			0xAD090000, // sw t1, 0(t0)
			0x2402000A, // li v0, 0xA
			0xA50200E4, // sh v0, 0xE4(t0)
			li32_fireflyCounter[0], // lui t0, 0xXXXX
			li32_fireflyCounter[1], // ori t0, 0xXXXX
			0x8D020000, // lw v0, 0(t0)
			0x8FA30000, // lw v1, 0(sp)
			0x03E00008, // jr ra
			0x27BD0010  // addiu sp, 10
		};

		const auto
			writeUsedMapEOBOffset{ m_game->customCodeOffset(sizeof(MipsFn::WriteUsedMapEOB)) },
			writeUsedMapCaptureOffset{ m_game->customCodeOffset(sizeof(MipsFn::WriteUsedMapCapture)) },
			readUsedMap{ m_game->customCodeOffset(sizeof(MipsFn::ReadUsedMap)) };

		const auto
			jal_writeUsedMapCapture{ Mips::jal(writeUsedMapCaptureOffset.game) },
			jal_readUsedMap{ Mips::jal(readUsedMap.game) };

		over_battle_bin->write(m_game->offset().file.over_battle_bin.endOfBattle, Mips::j(writeUsedMapEOBOffset.game));

		if (m_game->isVersion(Version::NtscJ1))
		{
			m_game->file(File::OVER_BTLEXEC_BIN)->write(m_game->offset().file.over_btlexec_bin.captureSuccess, jal_writeUsedMapCapture);
			m_game->file(File::OVER_PLCOM_BIN)->write(m_game->offset().file.over_plcom_bin.getFireflyCounterBattle, jal_readUsedMap);
		}
		else
		{
			over_battle_bin->write(m_game->offset().file.over_battle_bin.captureSuccess, jal_writeUsedMapCapture);
			over_battle_bin->write(m_game->offset().file.over_battle_bin.getFireflyCounterBattle, jal_readUsedMap);
		}

		executable.write(nuzlockeValidMapsOffset.file, nuzlockeMaps);
		executable.write(writeUsedMapEOBOffset.file, writeUsedMapEOBFn);
		executable.write(writeUsedMapCaptureOffset.file, writeUsedMapCaptureFn);
		executable.write(readUsedMap.file, readUsedMapFn);
	}

	if (state & Randomizer::CHALLENGE_NUZLOCKE_DEFINITIVE_LEVANT_DEATH)
	{
		const auto jal_setDefeatScene{ over_battle_bin->read<Mips_t>(m_game->offset().file.over_battle_bin.endOfBattleSetSceneFn + 0xE4) };
		const auto li32_idOpponent1{ Mips::li32(Mips::Register::t0, m_game->offset().game.idOpponent1) };

		const MipsFn::ResetFromNowhere resetFromNowhereFn
		{
			0x27BDFFF0, // addiu sp, -0x10

			// If opponent 1 is not "Dream Man"
			li32_idOpponent1[0],
			li32_idOpponent1[1],
			0x8D080000, // lw t0, 0(t0)
			Mips::li(Mips::Register::t1, ID_DREAM_MAN),
			0x11090004, // beq t0, t1, 4
			0xAFBF0000, // sw ra, 0(sp)

			// Reset the game
			0x3C1D801F, // lui sp, 0x801F
			Mips::j(m_game->isVersion(Version::NtscJ2) ? 0x800180C0 : 0x800100C0),
			0x37BDFFE0, // ori sp, 0xFFE0

			// Else set the defeat scene
			jal_setDefeatScene,
			0x00000000, // nop
			0x8FBF0000, // lw ra, 0(sp)
			0x00000000, // nop
			0x03E00008, // jr ra
			0x27BD0010  // addiu sp, 0x10
		};

		const auto resetFromNowhereOffset{ m_game->customCodeOffset(sizeof(MipsFn::ResetFromNowhere)) };

		executable.write(resetFromNowhereOffset.file, resetFromNowhereFn);
		over_battle_bin->write(m_game->offset().file.over_battle_bin.endOfBattleSetSceneFn + 0xE4, Mips::jal(resetFromNowhereOffset.game));
	}
}

void Randomizer::challengeDifficulty(Randomizer::ChallengeDifficulty state) const
{
	const auto li32_idOpponent1{ Mips::li32(Mips::Register::t0, m_game->offset().game.idOpponent1) };
	const bool isHard{ state == Randomizer::ChallengeDifficulty::Hard };

	const MipsFn::DifficultyModeStats difficultyModeStatsFn
	{
		0x27BDFFF0, // addiu sp -0x10
		0xAFA30000, // sw v1, 0(sp)
		0xAFA80004, // sw t0, 4(sp)
		li32_idOpponent1[0], // lui t0, 0xXXXX
		li32_idOpponent1[1], // ori t0, 0xXXXX
		0x91030000, // lbu v1, 0(t0)
		0x25080088, // addiu t0, 0x88
		0x00A8502B, // sltu t2, a1, t0
		0x11400015, // beqz t2, 0x15
		0x2C680029, // sltiu t0, v1, 0x29
		0x15000012, // bnez t0, 0x12
		0x2C680039, // sltiu t0, v1, 0x39
		0x15000002, // bnez t0, 2
		0x2C6800D1, // sltiu t0, v1, 0xD1
		0x1500000E, // bnez t0, 0xE
		0x2CE80004, // sltiu t0, a3, 4
		0x15000002, // bnez t0, 2
		isHard ? Mips_t(0x24030003) /* 33.33% */ : Mips_t(0x24030002) /* 50% */, // li v1, 3/2 // Attack, Defense, Magic Attack, Magic Defense
		isHard ? Mips_t(0x24030005) /* 20% */ : Mips_t(0x24030003) /* 33.33% */, // li v1, 5/3 // Speed, Critical
		0x00005010, // mfhi t2
		0x0043001A, // div v0, v1
		0x00004012, // mflo t0
		0x00481021, // move v0, t0
		m_game->isNtscJ() ? Mips_t(0xA0A20028) : Mips_t(0xA0A2002C), // sb v0, 0x28/0x2C
		0x000A20C2, // srl a0, t2, 3
		0x00801821, // move v1, a0
		0x8FA80004, // lw t0, 4(sp)
		Mips::j(m_game->offset().game.afterGetStats + 0x34),
		0x27BD0010, // addiu sp, 0x10
		0x8FA30000, // lw v1, 0(sp)
		0x8FA80004, // lw t0, 4(sp)
		Mips::j(m_game->offset().game.afterGetStats),
		0x27BD0010  // addiu sp, 0x10
	};

	auto executable{ m_game->executable() };

	const auto difficultyModeStatsOffset{ m_game->customCodeOffset(sizeof(MipsFn::DifficultyModeStats)) };

	executable.write(m_game->offset().file.executable.afterGetStatsJump, Mips::j(difficultyModeStatsOffset.game));
	executable.write(difficultyModeStatsOffset.file, difficultyModeStatsFn);
}