#include "Backend/Randomizer.hpp"

#ifdef JCR_DEBUG

#include "Backend/File.hpp"
#include "Backend/Mips.hpp"
#include "Backend/Version.hpp"

#include <array>

void Randomizer::debugStageSelect() const
{
	using StageSelectWarp = std::array<Mips_t, 75>;
	using StageSelectInstallHook = std::array<Mips_t, 10>;
	using StageSelectConfirmHook = std::array<Mips_t, 6>;

	struct StageSelectOffset
	{
		u32 mapFileStateInstall;
		u32 mapFileInstallerCall;
		u32 mapFileConfirm;
		u32 stageSelectState;
		u32 stageSelectBody;
		u32 confirmConvergence;
		u32 sceneHierarchy;
		u32 warpDescriptor;
		u32 warpRequestFlag;
		u32 mapWarpFlag;
		u32 sceneTransitionFn;
		u32 screenFadeFn;
		u32 warpCommitFn;
		u32 mapTeardownFn;
		u32 fieldReturnState;
	};

	constexpr std::array<StageSelectOffset, static_cast<std::size_t>(Version::Count)> stageSelectOffsets
	{{
		{ // NtscJ1
			0x00001104, 0x00001CB0, 0x00002100,
			0x800B5990, 0x800B59D0, 0x800B5EA8, 0x800ADCA4, 0x80089530,
			0x80094124, 0x80089228, 0x800A6488, 0x800266FC, 0x800A43CC, 0x800B43A0, 0x800A4C94
		},
		{ // NtscJ2
			0x000011A8, 0x00001D2C, 0x0000217C,
			0x800BD5BC, 0x800BD5FC, 0x800BDAD4, 0x800B5898, 0x80090BE0,
			0x8009B7D4, 0x800908D8, 0x800ADF8C, 0x8002E938, 0x800ABD80, 0x800BBF68, 0x800AC648
		},
		{ // NtscU
			0x000013A0, 0x00001F34, 0x00002384,
			0x800B85CC, 0x800B860C, 0x800B8AE4, 0x800B0608, 0x8008B850,
			0x80091C94, 0x8008B540, 0x800A8DEC, 0x8001C19C, 0x800A6D30, 0x800B6F60, 0x800A75F8
		},
		{ // PalEn
			0x00001380, 0x00001F14, 0x00002364,
			0x800B877C, 0x800B87BC, 0x800B8C94, 0x800B07D8, 0x8008B7E0,
			0x80091C24, 0x8008B4D0, 0x800A8ECC, 0x8001C1A8, 0x800A6CC0, 0x800B7118, 0x800A7588
		},
		{ // PalFr
			0x000013F8, 0x00001F84, 0x000023D4,
			0x800B8A7C, 0x800B8ABC, 0x800B8F94, 0x800B0A68, 0x8008BA70,
			0x80091EB4, 0x8008B760, 0x800A915C, 0x8001C1A8, 0x800A6F50, 0x800B7420, 0x800A7818
		},
		{ // PalDe
			0x00001394, 0x00001F28, 0x00002378,
			0x800B88D8, 0x800B8918, 0x800B8DF0, 0x800B0920, 0x8008B928,
			0x80091D6C, 0x8008B618, 0x800A9014, 0x8001C1A8, 0x800A6E08, 0x800B7274, 0x800A76D0
		},
		{ // PalEs
			0x00001440, 0x00001FCC, 0x0000241C,
			0x800B8BE4, 0x800B8C24, 0x800B90FC, 0x800B0B88, 0x8008BB90,
			0x80091FD4, 0x8008B880, 0x800A927C, 0x8001C1A8, 0x800A7070, 0x800B7588, 0x800A7938
		},
		{ // PalIt
			0x00001440, 0x00001FCC, 0x0000241C,
			0x800B8BC4, 0x800B8C04, 0x800B90DC, 0x800B0B70, 0x8008BB78,
			0x80091FBC, 0x8008B868, 0x800A9264, 0x8001C1A8, 0x800A7058, 0x800B7568, 0x800A7920
		}
	}};

	const auto& offset{ stageSelectOffsets[static_cast<std::size_t>(m_game->version())] };

	auto hi{ [](u32 addr) { return static_cast<u16>((addr + 0x8000) >> 16); } };
	auto lo{ [](u32 addr) { return static_cast<u16>(addr); } };

	const auto codeOffset
	{
		m_game->customCodeOffset(sizeof(StageSelectWarp) + sizeof(StageSelectInstallHook) + sizeof(u32))
	};

	const u32 taskSlot{ codeOffset.game + sizeof(StageSelectWarp) + sizeof(StageSelectInstallHook) };

	const StageSelectWarp stageSelectWarpFn
	{
		0x27BDFFE0, // addiu sp, sp, -0x20
		0xAFBF0018, // sw ra, 0x18(sp)
		0x94880004, // lhu t0, 4(a0)    map
		0x94890008, // lhu t1, 8(a0)    area
		0x948A000C, // lhu t2, 0xC(a0)  record
		Mips::lui(Mips::Register::a1, hi(offset.sceneHierarchy)),
		static_cast<Mips_t>(0x24A50000 | lo(offset.sceneHierarchy)), // addiu a1, a1, sceneHierarchy
		0x01003021, // addu a2, t0, zero
		0x8CA20000, // lw v0, 0(a1)
		0x00000000, // nop
		0x1040003C, // beqz v0, fail
		0x00000000, // nop
		0x10C00004, // beqz a2, level1Done
		0x00000000, // nop
		0x24A50004, // addiu a1, a1, 4
		0x1000FFF8, // b -8
		0x24C6FFFF, // addiu a2, a2, -1
		0x00402821, // addu a1, v0, zero
		0x01203021, // addu a2, t1, zero
		0x8CA20000, // lw v0, 0(a1)
		0x00000000, // nop
		0x10400031, // beqz v0, fail
		0x00000000, // nop
		0x10C00004, // beqz a2, level2Done
		0x00000000, // nop
		0x24A50004, // addiu a1, a1, 4
		0x1000FFF8, // b -8
		0x24C6FFFF, // addiu a2, a2, -1
		0x00402821, // addu a1, v0, zero
		0x01403021, // addu a2, t2, zero
		0x2407FFFF, // li a3, -1
		0x84A20000, // lh v0, 0(a1)
		0x00000000, // nop
		0x10470025, // beq v0, a3, fail
		0x00000000, // nop
		0x10C00004, // beqz a2, level3Done
		0x00000000, // nop
		0x24A50010, // addiu a1, a1, 0x10
		0x1000FFF8, // b -8
		0x24C6FFFF, // addiu a2, a2, -1
		Mips::lui(Mips::Register::a1, hi(offset.warpDescriptor)),
		static_cast<Mips_t>(0x24A50000 | lo(offset.warpDescriptor)), // addiu a1, a1, warpDescriptor
		0x2402FFFF, // li v0, -1
		0xA4A20006, // sh v0, 6(a1)
		0xA4A2000A, // sh v0, 0xA(a1)
		0xA4A2000E, // sh v0, 0xE(a1)
		0xA4A80004, // sh t0, 4(a1)
		0xA4A90008, // sh t1, 8(a1)
		0xA4AA000C, // sh t2, 0xC(a1)
		Mips::jal(offset.sceneTransitionFn),
		0x00000000, // nop
		0x00002021, // move a0, zero
		Mips::jal(offset.screenFadeFn),
		0x24050020, // li a1, 0x20
		Mips::jal(offset.warpCommitFn),
		0x24040001, // li a0, 1
		0x24030001, // li v1, 1
		Mips::lui(Mips::Register::v0, hi(offset.warpRequestFlag)),
		static_cast<Mips_t>(0xAC430000 | lo(offset.warpRequestFlag)), // sw v1, warpRequestFlag(v0)
		Mips::lui(Mips::Register::v0, hi(offset.mapWarpFlag)),
		static_cast<Mips_t>(0xAC430000 | lo(offset.mapWarpFlag)), // sw v1, mapWarpFlag(v0)
		Mips::lui(Mips::Register::v0, hi(taskSlot)),
		static_cast<Mips_t>(0x8C440000 | lo(taskSlot)), // lw a0, taskSlot(v0)
		0x00000000, // nop
		Mips::jal(offset.mapTeardownFn),
		0x00000000, // nop
		Mips::lui(Mips::Register::v0, hi(taskSlot)),
		static_cast<Mips_t>(0x8C430000 | lo(taskSlot)), // lw v1, taskSlot(v0)
		Mips::lui(Mips::Register::v0, hi(offset.fieldReturnState)),
		static_cast<Mips_t>(0x24420000 | lo(offset.fieldReturnState)), // addiu v0, v0, fieldReturnState
		0xAC620008, // sw v0, 8(v1)
		0x8FBF0018, // fail: lw ra, 0x18(sp)
		0x00000000, // nop
		0x03E00008, // jr ra
		0x27BD0020  // addiu sp, sp, 0x20
	};

	const StageSelectInstallHook installHookFn
	{
		0x27BDFFE8, // addiu sp, sp, -0x18
		0xAFBF0010, // sw ra, 0x10(sp)
		Mips::lui(Mips::Register::v1, hi(taskSlot)),
		static_cast<Mips_t>(0xAC640000 | lo(taskSlot)), // sw a0, taskSlot(v1)
		Mips::jal(offset.stageSelectBody),
		0x00000000, // nop
		0x8FBF0010, // lw ra, 0x10(sp)
		0x00000000, // nop
		0x03E00008, // jr ra
		0x27BD0018  // addiu sp, sp, 0x18
	};

	const StageSelectConfirmHook confirmHookFn
	{
		0x00000000, // nop
		0x02602021, // move a0, s3
		Mips::jal(codeOffset.game),
		0x00000000, // nop
		Mips::j(offset.confirmConvergence),
		0x00000000  // nop
	};

	auto executable{ m_game->executable() };
	executable.write(codeOffset.file, stageSelectWarpFn);
	executable.write(codeOffset.file + sizeof(StageSelectWarp), installHookFn);
	executable.write(codeOffset.file + sizeof(StageSelectWarp) + sizeof(StageSelectInstallHook), u32{});

	const auto over_map_bin{ m_game->file(File::OVER_MAP_BIN) };
	over_map_bin->write(offset.mapFileStateInstall, Mips::li32(Mips::Register::v0, offset.stageSelectState));
	over_map_bin->write(offset.mapFileInstallerCall, Mips::jal(codeOffset.game + sizeof(StageSelectWarp)));
	over_map_bin->write(offset.mapFileConfirm, confirmHookFn);
}

void Randomizer::debugTitleDebug() const
{
	static constexpr std::array<u32, static_cast<std::size_t>(Version::Count)> titleFileMenuOffsets
	{
		0x00000810,
		0x000009D0,
		0x00000A38,
		0x00000A38,
		0x00000A90,
		0x00000A3C,
		0x000009A4,
		0x000009A4
	};

	const auto offset{ titleFileMenuOffsets[static_cast<std::size_t>(m_game->version())] };
	m_game->file(File::OVER_TITLE_BIN)->write(offset, Mips::li(Mips::Register::v0, 8));
}

#endif