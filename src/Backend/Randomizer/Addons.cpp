#include "Backend/Randomizer.hpp"

#include "Backend/File.hpp"
#include "Backend/Mips.hpp"
#include "Backend/MipsFn.hpp"
#include "Backend/Version.hpp"
#include "Common/TemplateTypes.hpp"

#include <utility>

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

void Randomizer::addonsSkipTutorial(bool skipKoris) const
{
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
		executable.write(afterTutorialStateOffset.file + 0xCE, u8(0x0B));
		executable.write(afterTutorialStateOffset.file + 0x126, u8(0x64));
		executable.write(afterTutorialStateOffset.file + 0x12E, u8(0x6A));
		executable.write(afterTutorialStateOffset.file + 0x12F, u8(0));
	}

	const auto over_title_bin{ m_game->file(File::OVER_TITLE_BIN) };

	const auto
		sb_setLevantGarb{ m_game->isNtscJ() ? Mips_t(0xA04007DD) : Mips_t(0xA04007E5)}; // sb zero, 0x7DD/0x7E5(v0)

	over_title_bin->write(m_game->offset().file.over_title_bin.initMapNewGame, Mips::jal(writeAfterTutorialStateOffset.game));
	over_title_bin->write(m_game->offset().file.over_title_bin.initMapNewGame - 8, Mips_t(0));
	over_title_bin->write(m_game->offset().file.over_title_bin.setLevantGarb, sb_setLevantGarb);
}

void Randomizer::addonsX2Framerate() const
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

void Randomizer::addonsItemQuantityLimit(u8 limit) const
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

void Randomizer::addonsLevelCapEC(u8 levelCap) const
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

void Randomizer::addonsShowHiddenStats() const
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

void Randomizer::addonsPalToNtsc() const
{
	if (!m_game->isNtsc())
	{
		auto executable{ m_game->executable() };

		static constexpr auto move_a0_zero{ Mips_t(0x00002021) };

		executable.write(m_game->offset().file.executable.setVideoModeArgument, move_a0_zero);
		executable.write(m_game->offset().file.executable.stretchingY, move_a0_zero);
	}
}