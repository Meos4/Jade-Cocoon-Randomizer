#include "Backend/Randomizer.hpp"

#include "Backend/File.hpp"
#include "Backend/Mips.hpp"
#include "Backend/MipsFn.hpp"
#include "Backend/Version.hpp"
#include "Common/TemplateTypes.hpp"

#include <array>
#include <utility>

void Randomizer::defaultX2Framerate() const
{
	const auto
		li32_controllerTemp{ Mips::li32(Mips::Register::t0, m_game->offset().game.controllerTemp) },
		li32_frameLimiter{ Mips::li32(Mips::Register::t3, m_game->offset().game.frameLimiter) };

	const MipsFn::ToggleX2Framerate toggleX2FramerateFn
	{
		li32_controllerTemp[0], // lui t0, 0xXXXX
		li32_controllerTemp[1], // ori t0, 0xXXXX
		0x95090000, // lhu t1, 0(t0)
		0x00000000, // nop
		0x31290100, // andi t1, 0x0100
		0x1120000C, // beqz t1, 0xC
		0x95090014, // lhu t1, 0x14(t0)
		0x00000000, // nop
		0x15200009, // bnez t1, 9
		0x240A0001, // li t2, 1
		li32_frameLimiter[0], // lui t3, 0xXXXX
		li32_frameLimiter[1], // ori t3, 0xXXXX
		0x91690000, // lbu t1, 0(t3)
		0x00000000, // nop
		0x152A0002, // bne t1, t2, +2
		0xA10A0014, // sb t2, 0x14(t0)
		0x254A0001, // addiu t2, 1
		0xA16A0000, // sb t2, 0(t3)
		Mips::j(m_game->offset().game.frameLimiter + 0x14),
		0x00000000  // nop
	};

	auto executable{ m_game->executable() };

	const auto toggleX2FramerateOffset{ m_game->customCodeOffset(sizeof(MipsFn::ToggleX2Framerate)) };

	executable.write(toggleX2FramerateOffset.file, toggleX2FramerateFn);
	executable.write(m_game->offset().file.executable.afterFramerateLimiter, Mips::j(toggleX2FramerateOffset.game));
}

void Randomizer::defaultSkipOpeningLogos() const
{
	if (m_game->isNtscJ())
	{
		m_game->executable().write(m_game->offset().file.executable.openingBehaviorFn + 8, Mips_t(0x00000000));
	}
	else
	{
		m_game->executable().write(m_game->offset().file.executable.mainLoopFn + 0x5C, Mips_t(0x10000019)); // b +0x19
	}
}

void Randomizer::defaultShowHiddenStats() const
{
	std::array<u8, 10> bipedal, winged, critical;

	switch (m_game->version())
	{
	case Version::NtscJ1:
	case Version::NtscJ2:
		bipedal = { 0x22, 1, 0x45, 0x3E, 0x3D, 0x3A, 2 }; // Bipedal
		winged = { 0x36, 1, 0x43, 0x3F, 0x3E, 0x3D }; // Winged
		critical = { 0x23, 6, 1, 7, 0xFB }; // Crit.
		break;
	case Version::NtscU:
	case Version::PalEn:
		bipedal = { 0x32, 0x79, 0x80, 0x75, 0x74, 0x71, 0x7C }; // Bipedal
		winged = { 0x63, 0x79, 0x7E, 0x77, 0x75, 0x74 }; // Winged
		critical = { 0x33, 0x82, 0x79, 0x84, 0x79, 0x73, 0x71, 0x7C }; // Critical
		break;
	case Version::PalFr:
		bipedal = { 0x32, 0xB1, 0x90, 0x0D, 0x84, 0x85 }; // Bipede - e1 grave
		winged = { 0x31, 0xB1, 0x8C, 0x0C, 0x85 }; // Ailee - e1 aigu
		critical = { 0x33, 0x92, 0xB1, 0x94, 0xB1, 0x91, 0x95, 0x85 }; // Critique
		break;
	case Version::PalDe:
		bipedal = { 0x44, 0x87, 0x75, 0x79, 0x72, 0x75, 0x79, 0x7E, 0x75, 0x82 }; // Zweibeiner
		winged = { 0x37, 0x75, 0x76, 0x7C, 0x18, 0x77, 0x75, 0x7C, 0x84 }; // Geflugelt - u trema
		critical = { 0x3A, 0x82, 0x79, 0x84, 0x79, 0x83, 0x73, 0x78 }; // Kritisch
		break;
	case Version::PalEs:
		bipedal = { 0x32, 0xB1, 0x90, 0x85, 0x84, 0x81 }; // Bipeda
		winged = { 0x31, 0x8C, 0x81, 0x84, 0x81 }; // Alada
		critical = { 0x33, 0x92, 0xB1, 0x94, 0xB1, 0x83, 0x81 }; // Critica
		break;
	case Version::PalIt:
		bipedal = { 0x32, 0xB1, 0x90, 0x85, 0x84, 0x85 }; // Bipede
		winged = { 0x31, 0x8C, 0x81, 0x94, 0x81 }; // Alata
		critical = { 0x33, 0x92, 0xB1, 0x94, 0xB1, 0x83, 0x81 }; // Critica
		break;
	}

	struct
	{
		u16 x, y;
	} criticalAxis, bodyAxis;

	switch (m_game->version())
	{
	case Version::NtscJ1:
	case Version::NtscJ2:
		criticalAxis = { 78, 101 };
		bodyAxis = { 87, 17 };
		break;
	case Version::NtscU:
		criticalAxis = { 78, 104 };
		bodyAxis = { 102, 15 };
		break;
	case Version::PalEn:
		criticalAxis = { 78, 102 };
		bodyAxis = { 102, 15 };
		break;
	case Version::PalFr:
		criticalAxis = { 78, 102 };
		bodyAxis = { 107, 14 };
		break;
	default: // DE - ES - IT
		criticalAxis = { 78, 102 };
		bodyAxis = { 98, 13 };
		break;
	}

	auto arrayTextToInteger = []<Integral T>(u8* ch) -> T
	{
		T val{};
		for (std::size_t i{}; i < sizeof(T); ++i)
		{
			val += (ch[i] << (i * 8));
		}
		return val;
	};

	const auto
		li32_criticalAxis{ Mips::li32(Mips::Register::v1, (criticalAxis.y << 16) + criticalAxis.x) },
		li32_bodyAxis{ Mips::li32(Mips::Register::v1, (bodyAxis.y << 16) + bodyAxis.x) },
		li32_criticalText0{ Mips::li32(Mips::Register::v1, arrayTextToInteger.operator()<u32>(critical.data())) },
		li32_criticalText1{ Mips::li32(Mips::Register::v1, arrayTextToInteger.operator()<u32>(critical.data() + 4)) },
		li32_bipedalText0{ Mips::li32(Mips::Register::v1, arrayTextToInteger.operator()<u32>(bipedal.data())) },
		li32_bipedalText1{ Mips::li32(Mips::Register::v1, arrayTextToInteger.operator()<u32>(bipedal.data() + 4)) },
		li32_wingedText0{ Mips::li32(Mips::Register::v1, arrayTextToInteger.operator()<u32>(winged.data())) },
		li32_wingedText1{ Mips::li32(Mips::Register::v1, arrayTextToInteger.operator()<u32>(winged.data() + 4)) };

	const auto
		li_criticalText2{ Mips::li(Mips::Register::v1, arrayTextToInteger.operator()<u16>(critical.data() + 8)) },
		li_bipedalText2{ Mips::li(Mips::Register::v1, arrayTextToInteger.operator()<u16>(bipedal.data() + 8)) },
		li_wingedText2{ Mips::li(Mips::Register::v1, arrayTextToInteger.operator()<u16>(winged.data() + 8)) };

	const bool isNtscJ{ m_game->isNtscJ()};

	const MipsFn::DrawHiddenStats drawCriticalAndBodyTextFn
	{
		0x27BDFFE0, // addiu sp, -0x20
		0xAFBF0010, // sw ra, 0x10(sp)

		// Critical
		0x8E680000, // lw t0, 0(s3) // XY Coordinates
		li32_criticalAxis[0], // lui v1, 0xXXXX
		li32_criticalAxis[1], // ori v1, 0xXXXX
		0x00681821, // addu v1, t0
		0xAFA30000, // sw v1, 0(sp)
		li32_criticalText0[0], // lui v1, 0xXXXX
		li32_criticalText0[1], // ori v1, 0xXXXX
		0xAFA30004, // sw v1, 4(sp)
		li32_criticalText1[0], // lui v1, 0xXXXX
		li32_criticalText1[1], // ori v1, 0xXXXX
		0xAFA30008, // sw v1, 8(sp)
		li_criticalText2, // li v1, 0xXXXX
		0xAFA3000C, // sw v1, 0xC(sp)
		0x03A02821, // move a1, sp
		0x03A03021, // move a2, sp
		0x02C03821, // move a3, s6
		Mips::jal(m_game->offset().game.drawLargeTextOnScreenFn),
		0x24C60004, // addiu a2, 4	

		// Entity Enhancement Read
		0x12800001, // beqz s4, 1
		0x02802021, // move a0, s4 
		0x02402021, // move a0, s2 
		isNtscJ ? Mips_t(0x9084FF96) : Mips_t(0x9084FF7E), // lbu a0, -0x6A/-0x82(a0)
		0x00000000, // nop
		Mips::jal(m_game->offset().game.getEntityEnhancement),
		0x00000000, // nop

		// None
		0x1040001C, // beqz v0, 0x1C

		// Bipedal
		0x24050001, // li a1, 1
		0x14450008, // bne v0, a1, 8
		li32_bipedalText0[0], // lui v1, 0xXXXX
		li32_bipedalText0[1], // ori v1, 0xXXXX
		0xAFA30004, // sw v1, 4(sp)
		li32_bipedalText1[0], // lui v1, 0xXXXX
		li32_bipedalText1[1], // ori v1, 0xXXXX
		0xAFA30008, // sw v1, 8(sp)
		0x10000008, // b, 8
		li_bipedalText2, // li v1, 0xXXXX

		// Winged
		li32_wingedText0[0], // lui v1, 0xXXXX
		li32_wingedText0[1], // ori v1, 0xXXXX
		0xAFA30004, // sw v1, 4(sp)
		li32_wingedText1[0], // lui v1, 0xXXXX
		li32_wingedText1[1], // ori v1, 0xXXXX
		0xAFA30008, // sw v1, 8(sp)
		li_wingedText2, // li v1, 0xXXXX
		0xAFA3000C, // sw v1, 0xC(sp)

		// Draw Body
		0x8E680000, // lw t0, 0(s3) // XY Coordinates
		li32_bodyAxis[0], // lui v1, 0xXXXX
		li32_bodyAxis[1], // ori v1, 0xXXXX
		0x00681821, // addu v1, t0
		0x03A02821, // move a1, sp
		0x03A03021, // move a2, sp
		0x02C03821, // move a3, s6
		0x24C60004, // addiu a2, 4	
		Mips::jal(m_game->offset().game.drawLargeTextOnScreenFn),
		0xAFA30000, // sw v1, 0(sp)

		0x00002021, // move a0, zero
		0x8FBF0010, // lw ra, 0x10(sp)
		0x27BD0020, // addiu sp, 0x20
		isNtscJ ? Mips_t(0x96620000) : Mips_t(0x26460010), // lhu v0, 0(s3) / addiu a2, s2, 0x10
		0x03E00008, // jr ra
		0x27A50020, // addiu a1, sp, 0x20
	};

	static constexpr auto li32_multDiv10{ Mips::li32(Mips::Register::t0, 0xCCCCCCCD) };
	
	const MipsFn::CriticalRateFromStats criticalRateFromStatsFn
	{
		0x27BDFFF0, // addiu sp, -0x10
		0xAFBF0000, // sw ra, 0(sp)
		li32_multDiv10[0], // lui t0, 0xCCCC
		li32_multDiv10[1], // ori t0, 0xCCCD
		0x00880019, // multu a0, t0
		0x00004010, // mfhi t0
		0x000820C2, // srl a0, t0, 3
		Mips::jal(m_game->offset().game.criticalRateTransformFn),
		0x308400FF, // andi a0, 0xFF
		0x8FBF0000, // lw ra, 0(sp)
		0x00000000, // nop
		0x03E00008, // jr ra
		0x27BD0010, // addiu sp, 0x10
	};

	const auto criticalRateFromStatsOffset{ m_game->customCodeOffset(sizeof(MipsFn::CriticalRateFromStats)) };

	const MipsFn::DrawCriticalRate drawCriticalRateFn
	{
		0x03A31021, // addu v0, sp, v1
		0x27BDFFF0, // addiu sp, -0x10
		0xAFBF0000, // sw ra, 0(sp)
		0xAFA40004, // sw a0, 4(sp)
		0x24090005, // li t1, 5
		0x16090004, // bne s0, t1, 4
		0x00404821, // move t1, v0
		Mips::jal(criticalRateFromStatsOffset.game),
		isNtscJ ? Mips_t(0x92440033) : Mips_t(0x92440037), // lbu a0, 0x33/0x37(s2)
		0xA1220040, // sb v0, 0x40(t1)
		0x91250040, // lbu a1, 0x40(t1)
		0x8FBF0000, // lw ra, 0(sp)
		0x8FA40004, // lw a0, 4(sp)
		0x03E00008, // jr ra
		0x27BD0010, // addiu sp, 0x10
	};

	const MipsFn::DrawCriticalRateMerge drawCriticalRateMergeFn
	{
		0x27BDFFF0, // addiu sp, -0x10
		0xAFBF0000, // sw ra, 0(sp)
		Mips::jal(criticalRateFromStatsOffset.game),
		isNtscJ ? Mips_t(0x92440033) : Mips_t(0x92440037), // lbu a0, 0x33/0x37(s2)
		0xA2020005, // sb v0, 5(s0)
		Mips::jal(criticalRateFromStatsOffset.game),
		isNtscJ ? Mips_t(0x92840033) : Mips_t(0x92840037), // lbu a0, 0x33/0x37(s4)
		0x03A04021, // move t0, sp
		0xA102006D, // sb v0, 0x6D(t0)
		0x8FBF0000, // lw ra, 0(sp)
		0x00009021, // move s2, zero
		0x03E00008, // jr ra
		0x27BD0010, // addiu sp, 0x10
	};

	auto executable{ m_game->executable() };

	const auto
		drawHiddenStatsOffset{ m_game->customCodeOffset(sizeof(MipsFn::DrawHiddenStats)) },
		drawCriticalRateOffset{ m_game->customCodeOffset(sizeof(MipsFn::DrawCriticalRate)) },
		drawCriticalRateMergeOffset{ m_game->customCodeOffset(sizeof(MipsFn::DrawCriticalRateMerge)) };

	const std::array<Mips_t, 2>
		jal_drawCriticalAndBodyText_nop{ Mips::jal(drawHiddenStatsOffset.game), 0x00000000 },
		jal_drawCriticalRate_nop{ Mips::jal(drawCriticalRateOffset.game), 0x00000000 };

	executable.write(m_game->offset().file.executable.drawMinionStatsFrameFn + 0x60, jal_drawCriticalAndBodyText_nop);

	static constexpr auto slti_v0_v0_6{ Mips_t(0x28420006) };

	const auto shift{ isNtscJ ? 0x318 : 0x334 };
	executable.write(m_game->offset().file.executable.drawMinionStatsFrameFn + shift, Mips::jal(drawCriticalRateMergeOffset.game));
	executable.write(m_game->offset().file.executable.drawMinionStatsFrameFn + shift + 0xE0, slti_v0_v0_6);
	executable.write(m_game->offset().file.executable.drawMinionStatsFrameFn + shift + 0x130, jal_drawCriticalRate_nop);
	executable.write(m_game->offset().file.executable.drawMinionStatsFrameFn + shift + 0x150, slti_v0_v0_6);

	executable.write(drawHiddenStatsOffset.file, drawCriticalAndBodyTextFn);
	executable.write(criticalRateFromStatsOffset.file, criticalRateFromStatsFn);
	executable.write(drawCriticalRateOffset.file, drawCriticalRateFn);
	executable.write(drawCriticalRateMergeOffset.file, drawCriticalRateMergeFn);

	static constexpr std::array<u8, 6> statsY{ 32, 46, 60, 74, 88, 102 };
	executable.write(m_game->offset().file.executable.tableOfYPositionMinionStatsFrame, statsY);

	if (isNtscJ)
	{
		// Remove growth rate tim text
		executable.write(m_game->offset().file.executable.drawMinionTIMStatsFrameFn + 0x11C, Mips_t(0));
	}
}

void Randomizer::defaultTurboModeInDialogues() const
{
	const auto callerGame{ m_game->offset().game.dialogueInputWrapperCallerFn };

	if (callerGame == 0)
	{
		return;
	}

	auto executable{ m_game->executable() };

	const auto shift{ m_game->gameToFileTextSectionShift() };
	const auto callerFile{ callerGame - 0x80000000 - shift };
	const auto callerInsn{ executable.read<Mips_t>(callerFile) };

	if (((callerInsn >> 26) & 0x3F) != 3)
	{
		return;
	}

	const auto wrapperGame{ ((callerInsn & 0x03FFFFFF) << 2) | 0x80000000 };
	const auto wrapperFile{ wrapperGame - 0x80000000 - shift };
	const auto wrapperBlock{ executable.read<std::array<Mips_t, 8>>(wrapperFile) };

	const auto readerGame{ ((wrapperBlock[2] & 0x03FFFFFF) << 2) | 0x80000000 };
	const auto readerContinueGame{ readerGame + 0xC };
	const auto readerFile{ readerGame - 0x80000000 - shift };
	const auto readerEdgeLoad{ executable.read<std::array<Mips_t, 2>>(readerFile + 4) };
	const auto heldPadHi{ static_cast<u16>(readerEdgeLoad[0]) };
	const auto edgePadLo{ static_cast<u16>(readerEdgeLoad[1]) };
	const auto heldPadLo{ static_cast<u16>(static_cast<s16>(edgePadLo) - 0x7C) };

	const auto turboDialogueOffset{ m_game->customCodeOffset(sizeof(MipsFn::TurboDialogueInput)) };

	MipsFn::TurboDialogueInput turboDialogueFn
	{
		wrapperBlock[0],
		wrapperBlock[1],
		Mips::lui(Mips::Register::v0, heldPadHi),
		Mips::addiu(Mips::Register::v0, heldPadLo),
		Mips::j(readerContinueGame),
		wrapperBlock[3],
		wrapperBlock[4],
		wrapperBlock[5],
		wrapperBlock[6],
		wrapperBlock[7],
	};

	executable.write(turboDialogueOffset.file, turboDialogueFn);
	executable.write(callerFile, Mips::jal(turboDialogueOffset.game));
}

void Randomizer::defaultAnalogMode() const
{
	auto executable{ m_game->executable() };
	const auto gameBin{ m_game->file(File::OVER_GAME_BIN) };

	if (m_game->isVersion(Version::NtscU))
	{
		static constexpr MipsFn::AnalogMode movementFn
		{
			0x3C028009, // lui v0, 0x8009
			0x244718E8, // addiu a3, v0, 0x18E8
			0x90E20001, // lbu v0, 1(a3)
			0x24030007, // addiu v1, zero, 7
			0x00021102, // srl v0, v0, 4
			0x14430051, // bne v0, v1, padCheckFail
			0x3C028009, // lui v0, 0x8009
			0x3C038009, // lui v1, 0x8009
			0x246318D8, // addiu v1, v1, 0x18D8
			0x80620002, // lb v0, 2(v1)
			0x80680003, // lb t0, 3(v1)
			0x00420018, // mult v0, v0
			0x00002012, // mflo a0
			0x01080018, // mult t0, t0
			0x00001812, // mflo v1
			0x00839021, // addu s2, a0, v1
			0x2A422AC9, // slti v0, s2, 0x2AC9
			0x1440000C, // bnez v0, cardinalPath
			0x2A420AB3, // slti v0, s2, 0xAB3
			0x0C02A459, // jal 0x800A9164
			0x27A40010, // addiu a0, sp, 0x10
			0x24030001, // addiu v1, zero, 1
			0x14430013, // bne v0, v1, applySpeed
			0x24100001, // addiu s0, zero, 1
			0x8FA20010, // lw v0, 0x10(sp)
			0x00000000, // nop
			0x0440000E, // bltz v0, fastForward
			0x3C106000, // lui s0, 0x6000
			0x1000000E, // b applySpeed
			0x2A420AB3, // slti v0, s2, 0xAB3
			0x1440000B, // bnez v0, applySpeed
			0x00008021, // move s0, zero
			0x0C02A459, // jal 0x800A9164
			0x27A40010, // addiu a0, sp, 0x10
			0x24030001, // addiu v1, zero, 1
			0x14430006, // bne v0, v1, applySpeed
			0x24100004, // addiu s0, zero, 4
			0x8FA20010, // lw v0, 0x10(sp)
			0x00000000, // nop
			0x04410002, // bgez v0, applySpeed
			0x3C106000, // lui s0, 0x6000
			0x3C105000, // lui s0, 0x5000
			0x2A420AB3, // slti v0, s2, 0xAB3
			0x1440002A, // bnez v0, deadzone
			0x3C028009, // lui v0, 0x8009
			0x244218D8, // addiu v0, v0, 0x18D8
			0x80440002, // lb a0, 2(v0)
			0x80450003, // lb a1, 3(v0)
			0x0C00ED52, // jal 0x8003B548
			0x00000000, // nop
			0x3C048009, // lui a0, 0x8009
			0x2484D504, // addiu a0, a0, -0x2AFC
			0x2485000C, // addiu a1, a0, 0xC
			0x00026400, // sll t4, v0, 0x10
			0x0C00EE78, // jal 0x8003B9E0
			0x000C6403, // sra t4, t4, 0x10
			0x00021400, // sll v0, v0, 0x10
			0x00021403, // sra v0, v0, 0x10
			0x3C038009, // lui v1, 0x8009
			0x8463BD26, // lh v1, -0x42DA(v1)
			0x01826021, // addu t4, t4, v0
			0x01836023, // subu t4, t4, v1
			0x05810002, // bgez t4, anglePositive
			0x01801021, // move v0, t4
			0x25820FFF, // addiu v0, t4, -1
			0x00021B03, // sra v1, v0, 0xC
			0x00031300, // sll v0, v1, 0xC
			0x01821823, // subu v1, t4, v0
			0x2862F800, // slti v0, v1, -0x800
			0x10400003, // beqz v0, clampDone
			0x28620801, // slti v0, v1, 0x801
			0x24631000, // addiu v1, v1, 0x1000
			0x28620801, // slti v0, v1, 0x801
			0x14400002, // bnez v0, setDirectionBits
			0x00000000, // nop
			0x2463F000, // addiu v1, v1, -0x1000
			0x3C018000, // lui at, 0x8000
			0x02018025, // or s0, s0, at
			0x0060082A, // slt at, v1, zero
			0x14200002, // bnez at, setBit2000
			0x3C022000, // lui v0, 0x2000
			0x0802A0D0, // j 0x800A8340
			0x00000000, // nop
			0x3C024000, // lui v0, 0x4000
			0x0802A0D0, // j 0x800A8340
			0x00000000, // nop
			0x0802A0DA, // j 0x800A8368
			0x0802A083, // j 0x800A820C
			0x00000000, // nop
		};

		static constexpr MipsFn::JoystickRotation rotationFn
		{
			0x8E020004, // lw v0, 4(s0)
			0x3C018000, // lui at, 0x8000
			0x00411824, // and v1, v0, at
			0x10600034, // beqz v1, vanillaContinue
			0x3C028009, // lui v0, 0x8009
			0x244218D8, // addiu v0, v0, 0x18D8
			0x80440002, // lb a0, 2(v0)
			0x80450003, // lb a1, 3(v0)
			0x0C00ED52, // jal 0x8003B548
			0x00000000, // nop
			0x3C048009, // lui a0, 0x8009
			0x2484D504, // addiu a0, a0, -0x2AFC
			0x2485000C, // addiu a1, a0, 0xC
			0x00024400, // sll t0, v0, 0x10
			0x0C00EE78, // jal 0x8003B9E0
			0x00084403, // sra t0, t0, 0x10
			0x00021400, // sll v0, v0, 0x10
			0x00021403, // sra v0, v0, 0x10
			0x3C038009, // lui v1, 0x8009
			0x2465B570, // addiu a1, v1, -0x4A90
			0x01024021, // addu t0, t0, v0
			0x84A307B6, // lh v1, 0x7B6(a1)
			0x94A407B6, // lhu a0, 0x7B6(a1)
			0x01034023, // subu t0, t0, v1
			0x05010002, // bgez t0, rotAnglePositive
			0x01001021, // move v0, t0
			0x25020FFF, // addiu v0, t0, -1
			0x00021B03, // sra v1, v0, 0xC
			0x00031300, // sll v0, v1, 0xC
			0x01021823, // subu v1, t0, v0
			0x2862F800, // slti v0, v1, -0x800
			0x10400003, // beqz v0, rotClampDone
			0x28620801, // slti v0, v1, 0x801
			0x24631000, // addiu v1, v1, 0x1000
			0x28620801, // slti v0, v1, 0x801
			0x14400002, // bnez v0, rotWriteAngle
			0x00000000, // nop
			0x2463F000, // addiu v1, v1, -0x1000
			0x04610006, // bgez v1, rotWriteAngle
			0x28620100, // slti v0, v1, 0x100
			0x2862FF01, // slti v0, v1, -0xFF
			0x14400006, // bnez v0, rotWriteAngle
			0x2482FF00, // addiu v0, a0, -0x100
			0x10000004, // b rotWriteAngle
			0x00831021, // addu v0, a0, v1
			0x14400002, // bnez v0, rotWriteAngle
			0x00831021, // addu v0, a0, v1
			0x24820100, // addiu v0, a0, 0x100
			0xA4A207B6, // sh v0, 0x7B6(a1)
			0x3C039FFF, // lui v1, 0x9FFF
			0x8E020004, // lw v0, 4(s0)
			0x00000000, // nop
			0x3463FFFF, // ori v1, v1, 0xFFFF
			0x00431024, // and v0, v0, v1
			0x0802A6A3, // j 0x800A9A8C
			0xAE020004, // sw v0, 4(s0)
			0x0802A67C, // j 0x800A99F0
			0x00000000, // nop
		};

		static constexpr MipsFn::AnalogToDpad analogToDpadFn
		{
			0x3C088009, // lui t0, 0x8009
			0x250818D8, // addiu t0, t0, 0x18D8
			0x00A84823, // subu t1, a1, t0
			0x00094843, // sra t1, t1, 1
			0x2508FFF8, // addiu t0, t0, -8
			0x01094021, // addu t0, t0, t1
			0x950A0000, // lhu t2, 0(t0)
			0x80A60002, // lb a2, 2(a1)
			0x80A70003, // lb a3, 3(a1)
			0x28C10061, // slti at, a2, 0x61
			0x14200002, // bnez at, +2
			0x00000000, // nop
			0x354A2000, // ori t2, t2, 0x2000          - right
			0x28C1FFA0, // slti at, a2, -0x60
			0x10200002, // beqz at, +2
			0x00000000, // nop
			0x354A8000, // ori t2, t2, 0x8000          - left
			0x28E10061, // slti at, a3, 0x61
			0x14200002, // bnez at, +2
			0x00000000, // nop
			0x354A1000, // ori t2, t2, 0x1000          - up
			0x28E1FFA0, // slti at, a3, -0x60
			0x10200002, // beqz at, +2
			0x00000000, // nop
			0x354A4000, // ori t2, t2, 0x4000          - down
			0xA50A0000, // sh t2, 0(t0)
			0x03E00008, // jr ra
			0x00000000, // nop
		};

		const auto movementOffset{ m_game->customCodeOffset(sizeof(MipsFn::AnalogMode)) };
		const auto rotationOffset{ m_game->customCodeOffset(sizeof(MipsFn::JoystickRotation)) };
		const auto analogToDpadOffset{ m_game->customCodeOffset(sizeof(MipsFn::AnalogToDpad)) };

		executable.write(movementOffset.file, movementFn);
		gameBin->write(0x00007C68, Mips::j(movementOffset.game));

		executable.write(rotationOffset.file, rotationFn);
		gameBin->write(0x0000944C, Mips::j(rotationOffset.game));

		executable.write(analogToDpadOffset.file, analogToDpadFn);
		executable.write(0x0000D4FC, Mips::j(analogToDpadOffset.game));
	}
	else if (m_game->isVersion(Version::NtscJ1))
	{
		static constexpr MipsFn::AnalogModeNtscJ1 movementFn
		{
			0x3C028009, // lui v0, 0x8009
			0x24473D78, // addiu a3, v0, 0x3D78
			0x90E20001, // lbu v0, 1(a3)
			0x24030007, // addiu v1, zero, 7
			0x00021102, // srl v0, v0, 4
			0x14430057, // bne v0, v1, padCheckFail
			0x3C038009, // lui v1, 0x8009
			0x24633D7C, // addiu v1, v1, 0x3D7C
			0x90620002, // lbu v0, 2(v1)
			0x90680003, // lbu t0, 3(v1)
			0x24090080, // addiu t1, zero, 0x80
			0x01221023, // subu v0, t1, v0
			0x00420018, // mult v0, v0
			0x00002012, // mflo a0
			0x01091023, // subu v0, t0, t1
			0x00420018, // mult v0, v0
			0x00001812, // mflo v1
			0x00839021, // addu s2, a0, v1
			0x2A422AC9, // slti v0, s2, 0x2AC9
			0x1440000C, // bnez v0, cardinalPath
			0x2A420AB3, // slti v0, s2, 0xAB3
			0x0C029A00, // jal 0x800A6800
			0x27A40010, // addiu a0, sp, 0x10
			0x24030001, // addiu v1, zero, 1
			0x14430013, // bne v0, v1, applySpeed
			0x24100001, // addiu s0, zero, 1
			0x8FA20010, // lw v0, 0x10(sp)
			0x00000000, // nop
			0x0440000E, // bltz v0, fastForward
			0x3C106000, // lui s0, 0x6000
			0x1000000E, // b applySpeed
			0x2A420AB3, // slti v0, s2, 0xAB3
			0x1440000B, // bnez v0, applySpeed
			0x00008021, // move s0, zero
			0x0C029A00, // jal 0x800A6800
			0x27A40010, // addiu a0, sp, 0x10
			0x24030001, // addiu v1, zero, 1
			0x14430006, // bne v0, v1, applySpeed
			0x24100004, // addiu s0, zero, 4
			0x8FA20010, // lw v0, 0x10(sp)
			0x00000000, // nop
			0x04410002, // bgez v0, applySpeed
			0x3C106000, // lui s0, 0x6000
			0x3C105000, // lui s0, 0x5000
			0x2A420AB3, // slti v0, s2, 0xAB3
			0x1440002D, // bnez v0, deadzone
			0x3C028009, // lui v0, 0x8009
			0x24423D7C, // addiu v0, v0, 0x3D7C
			0x90440002, // lbu a0, 2(v0)
			0x90480003, // lbu t0, 3(v0)
			0x24090080, // addiu t1, zero, 0x80
			0x00892023, // subu a0, a0, t1
			0x01282823, // subu a1, t1, t0
			0x0C011445, // jal 0x80045114
			0x00000000, // nop
			0x3C048009, // lui a0, 0x8009
			0x2484AFD4, // addiu a0, a0, -0x502C
			0x2485000C, // addiu a1, a0, 0xC
			0x00026400, // sll t4, v0, 0x10
			0x0C01156B, // jal 0x800455AC
			0x000C6403, // sra t4, t4, 0x10
			0x00021400, // sll v0, v0, 0x10
			0x00021403, // sra v0, v0, 0x10
			0x3C038009, // lui v1, 0x8009
			0x84639A06, // lh v1, -0x65FA(v1)
			0x01826021, // addu t4, t4, v0
			0x01836023, // subu t4, t4, v1
			0x05810002, // bgez t4, anglePositive
			0x01801021, // move v0, t4
			0x25820FFF, // addiu v0, t4, -1
			0x00021B03, // sra v1, v0, 0xC
			0x00031300, // sll v0, v1, 0xC
			0x01821823, // subu v1, t4, v0
			0x2862F800, // slti v0, v1, -0x800
			0x10400003, // beqz v0, clampDone
			0x28620801, // slti v0, v1, 0x801
			0x24631000, // addiu v1, v1, 0x1000
			0x28620801, // slti v0, v1, 0x801
			0x14400002, // bnez v0, setDirectionBits
			0x00000000, // nop
			0x2463F000, // addiu v1, v1, -0x1000
			0x3C018000, // lui at, 0x8000
			0x02018025, // or s0, s0, at
			0x0060082A, // slt at, v1, zero
			0x14200002, // bnez at, setBit2000
			0x3C022000, // lui v0, 0x2000
			0x08029677, // j 0x800A59DC
			0x00000000, // nop
			0x3C024000, // lui v0, 0x4000
			0x08029677, // j 0x800A59DC
			0x00000000, // nop
			0x00008021, // move s0, zero
			0x08029677, // j 0x800A59DC
			0x0802962A, // j 0x800A58A8
		};

		static constexpr MipsFn::JoystickRotationNtscJ1 rotationFn
		{
			0x8E020004, // lw v0, 4(s0)
			0x3C018000, // lui at, 0x8000
			0x00411824, // and v1, v0, at
			0x10600037, // beqz v1, vanillaContinue
			0x3C028009, // lui v0, 0x8009
			0x24423D7C, // addiu v0, v0, 0x3D7C
			0x90440002, // lbu a0, 2(v0)
			0x90480003, // lbu t0, 3(v0)
			0x24090080, // addiu t1, zero, 0x80
			0x00892023, // subu a0, a0, t1
			0x01282823, // subu a1, t1, t0
			0x0C011445, // jal 0x80045114
			0x00000000, // nop
			0x3C048009, // lui a0, 0x8009
			0x2484AFD4, // addiu a0, a0, -0x502C
			0x2485000C, // addiu a1, a0, 0xC
			0x00024400, // sll t0, v0, 0x10
			0x0C01156B, // jal 0x800455AC
			0x00084403, // sra t0, t0, 0x10
			0x00021400, // sll v0, v0, 0x10
			0x00021403, // sra v0, v0, 0x10
			0x3C038009, // lui v1, 0x8009
			0x24659250, // addiu a1, v1, -0x6DB0
			0x01024021, // addu t0, t0, v0
			0x84A307B6, // lh v1, 0x7B6(a1)
			0x94A407B6, // lhu a0, 0x7B6(a1)
			0x01034023, // subu t0, t0, v1
			0x05010002, // bgez t0, rotAnglePositive
			0x01001021, // move v0, t0
			0x25020FFF, // addiu v0, t0, -1
			0x00021B03, // sra v1, v0, 0xC
			0x00031300, // sll v0, v1, 0xC
			0x01021823, // subu v1, t0, v0
			0x2862F800, // slti v0, v1, -0x800
			0x10400003, // beqz v0, rotClampDone
			0x28620801, // slti v0, v1, 0x801
			0x24631000, // addiu v1, v1, 0x1000
			0x28620801, // slti v0, v1, 0x801
			0x14400002, // bnez v0, rotWriteAngle
			0x00000000, // nop
			0x2463F000, // addiu v1, v1, -0x1000
			0x04610006, // bgez v1, rotWriteAngle
			0x28620100, // slti v0, v1, 0x100
			0x2862FF01, // slti v0, v1, -0xFF
			0x14400006, // bnez v0, rotWriteAngle
			0x2482FF00, // addiu v0, a0, -0x100
			0x10000004, // b rotWriteAngle
			0x00831021, // addu v0, a0, v1
			0x14400002, // bnez v0, rotWriteAngle
			0x00831021, // addu v0, a0, v1
			0x24820100, // addiu v0, a0, 0x100
			0xA4A207B6, // sh v0, 0x7B6(a1)
			0x3C039FFF, // lui v1, 0x9FFF
			0x8E020004, // lw v0, 4(s0)
			0x00000000, // nop
			0x3463FFFF, // ori v1, v1, 0xFFFF
			0x00431024, // and v0, v0, v1
			0x08029C4A, // j 0x800A7128
			0xAE020004, // sw v0, 4(s0)
			0x08029C23, // j 0x800A708C
			0x00000000, // nop
		};

		static constexpr MipsFn::AnalogToDpadNtscJ1 analogToDpadFn
		{
			0x90C80002, // lbu t0, 2(a2)
			0x90C90003, // lbu t1, 3(a2)
			0x00084200, // sll t0, t0, 8
			0x01094027, // nor t0, t0, t1
			0xA4680000, // sh t0, 0(v1)
			0x90C80001, // lbu t0, 1(a2)
			0x24090007, // addiu t1, zero, 7
			0x00084102, // srl t0, t0, 4
			0x15090018, // bne t0, t1, done
			0x00000000, // nop
			0x90C80006, // lbu t0, 6(a2)
			0x90C90007, // lbu t1, 7(a2)
			0x240A0080, // addiu t2, zero, 0x80
			0x010A4023, // subu t0, t0, t2
			0x01494823, // subu t1, t2, t1
			0x946B0000, // lhu t3, 0(v1)
			0x29010061, // slti at, t0, 0x61
			0x14200002, // bnez at, +2
			0x00000000, // nop
			0x356B2000, // ori t3, t3, 0x2000         - right
			0x2901FFA0, // slti at, t0, -0x60
			0x10200002, // beqz at, +2
			0x00000000, // nop
			0x356B8000, // ori t3, t3, 0x8000         - left
			0x29210061, // slti at, t1, 0x61
			0x14200002, // bnez at, +2
			0x00000000, // nop
			0x356B1000, // ori t3, t3, 0x1000         - up
			0x2921FFA0, // slti at, t1, -0x60
			0x10200002, // beqz at, +2
			0x00000000, // nop
			0x356B4000, // ori t3, t3, 0x4000         - down
			0xA46B0000, // sh t3, 0(v1)
			0x08009BAC, // j 0x80026EB0               - back to delay-slot instruction
			0x00000000, // nop
		};

		const auto movementOffset{ m_game->customCodeOffset(sizeof(MipsFn::AnalogModeNtscJ1)) };
		const auto rotationOffset{ m_game->customCodeOffset(sizeof(MipsFn::JoystickRotationNtscJ1)) };
		const auto analogToDpadOffset{ m_game->customCodeOffset(sizeof(MipsFn::AnalogToDpadNtscJ1)) };

		executable.write(movementOffset.file, movementFn);
		gameBin->write(0x00007AB4, Mips::j(movementOffset.game));

		executable.write(rotationOffset.file, rotationFn);
		gameBin->write(0x00009298, Mips::j(rotationOffset.game));

		executable.write(analogToDpadOffset.file, analogToDpadFn);
		executable.write(0x000176AC, Mips::j(analogToDpadOffset.game));
	}
}

void Randomizer::defaultBugFixesHpMpBars() const
{
	auto executable{ m_game->executable() };

	const auto shift{ m_game->gameToFileTextSectionShift() };
	const auto hpMpBarsFile{ m_game->offset().file.executable.hpMpBars };
	const u32 hpMpBarsGame{ hpMpBarsFile + shift + 0x80000000 };
	const auto drawBlock{ executable.read<std::array<Mips_t, 15>>(hpMpBarsFile + 0x34C) };
	const auto hpMpBarsFixOffset{ m_game->customCodeOffset(sizeof(MipsFn::HpMpBarsFix)) };

	MipsFn::HpMpBarsFix hpMpBarsFixFn{};

	hpMpBarsFixFn[0] = 0x00108242;                      // srl   s0, s0, 9
	hpMpBarsFixFn[1] = 0x24080100;                      // addiu t0, zero
	hpMpBarsFixFn[2] = 0xAFA80010;                      // sw    t0, 0x10(sp)

	for (std::size_t i{ 0 }; i < drawBlock.size(); ++i)
	{
		hpMpBarsFixFn[3 + i] = (i == 10) ? Mips_t{ 0 } : drawBlock[i]; // nop the 'sw s0,0x10(sp)'
	}

	hpMpBarsFixFn[18] = 0x86220000;                     // lh    v0, (s1)
	hpMpBarsFixFn[19] = 0x00000000;                     // nop
	hpMpBarsFixFn[20] = 0x24420001;                     // addiu v0, v0, 1
	hpMpBarsFixFn[21] = 0xA6220000;                     // sh    v0, (s1)
	hpMpBarsFixFn[22] = 0x2610FFFF;                     // addiu s0, s0, -1
	hpMpBarsFixFn[23] = 0x1E00FFEB;                     // bgtz  s0, loop
	hpMpBarsFixFn[24] = 0x00000000;                     // nop
	hpMpBarsFixFn[25] = Mips::j(hpMpBarsGame + 0x388);  // j
	hpMpBarsFixFn[26] = 0x00000000;                     // nop

	executable.write(hpMpBarsFixOffset.file, hpMpBarsFixFn);
	executable.write(hpMpBarsFile + 0x34C, Mips::j(hpMpBarsFixOffset.game));
}

void Randomizer::defaultBugFixesBodyEnhancement() const
{
	const auto
		li32_minionStats{ Mips::li32(Mips::Register::v0, m_game->offset().game.minionStats) },
		li32_bossStatsEC{ Mips::li32(Mips::Register::v0, m_game->offset().game.bossStatsEC) };

	const auto fixEntityEnhancementOffset{ m_game->customCodeOffset(sizeof(MipsFn::FixEntityEnhancement)) };

	using EntityEnhancement = std::array<Mips_t, 7>;

	const EntityEnhancement entityEnhancementFn
	{
		0x27BDFFF0, // addiu sp, -0x10
		0xAFBF0000, // sw ra, 0(sp)
		Mips::j(fixEntityEnhancementOffset.game),
		0x00000000, // nop
		0x8FBF0000, // lw ra, 0(sp)
		0x03E00008, // jr ra
		0x27BD0010  // addiu sp, 0x10
	};

	const MipsFn::FixEntityEnhancement fixEntityEnhancementFn
	{
		li32_minionStats[0], // lui v0, 0xXXXX
		li32_minionStats[1], // ori v0, 0xXXXX
		0x2C880039, // sltiu t0, a0, 0x39
		0x11000003, // beqz t0, 3
		0x2C8800C9, // sltiu t0, a0, 0xC9
		0x1000000A, // b, +0xA
		0x00000000, // nop
		0x11000005, // beqz t0, 5
		0x00000000, // nop
		0x2484FFC7, // addiu a0 -0x39
		0x00042082, // srl a0, 2
		0x10000004, // b, 4
		0x00000000, // nop
		li32_bossStatsEC[0], // lui v0, 0xXXXX
		li32_bossStatsEC[1], // ori v0, 0xXXXX
		0x2484FF37, // addiu a0, -0xC9 
		0x000420C0, // sll a0, 3
		0x00822021, // move a0, v0
		Mips::j(m_game->offset().game.getEntityEnhancement + 0x10),
		0x90820007 // lbu v0, 7(a0)
	};

	auto executable{ m_game->executable() };

	executable.write(m_game->offset().file.executable.bodyEnhancementFn, entityEnhancementFn);
	executable.write(fixEntityEnhancementOffset.file, fixEntityEnhancementFn);
}

void Randomizer::defaultBugFixesAutumnMoonEffect() const
{
	Randomizer::setDamageEffectFromWeaponIdFn(*m_game, true);
}

void Randomizer::defaultBugFixesSpecialAttackModifiersDisplay() const
{
	if (m_game->isVersion(Version::PalFr))
	{
		static constexpr std::array<u8, 19> puissance
		{
			0x7D, 0x00, 0x75, 0x00, 0x52, 0x00, 0x15, 0x00, 0x2A, 0x00, 0x2A, 0x00, 0x4A, 0x00, 0x4F, 0x00,
			0x25, 0x00, 0x26
		};

		static constexpr std::array<u8, 23> precision
		{
			0x7D, 0x00, 0x75, 0x00, 0x29, 0x00, 0x39, 0x00, 0x25, 0x00, 0x15, 0x00, 0x2A, 0x00, 0x15, 0x00,
			0x28, 0x00, 0x4F, 0x00, 0x00, 0x00, 0x00
		};

		static constexpr std::array<u8, 21> critique
		{
			0x7D, 0x00, 0x6E, 0x00, 0x29, 0x00, 0x15, 0x00, 0x17, 0x00, 0x15, 0x00, 0x51, 0x00, 0x52, 0x00,
			0x26, 0x00, 0x00, 0x00, 0x00
		};

		auto executable{ m_game->executable() };

		executable.write(0x00073F20, u32(0x800746D2));
		executable.write(0x00064ED2, puissance);
		executable.write(0x00064EB8, precision);
		executable.write(0x00064EA0, critique);
	}
}