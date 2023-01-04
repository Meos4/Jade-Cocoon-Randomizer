#include "Forest.hpp"

#include "Backend/File.hpp"
#include "Backend/JCUtility.hpp"
#include "Backend/Mips.hpp"
#include "Backend/Ost.hpp"
#include "Backend/Random.hpp"

#include <array>
#include <set>
#include <unordered_set>
#include <utility>

struct MapsShift
{
	u32 nbMaps;
	u32 shift;
};

static constexpr std::array<MapsShift, Forest::nbForest> ostMapsShift
{
	13, 0xE88, // F1
	21, 0xF68, // F2
	49, 0x10A8, // F3
	24, 0x13C8, // F4
	11, 0x1558, // F1_2
	18, 0x1618, // F2_2
	50, 0x1728, // F3_2
	75, 0xE8 // EC
};

static constexpr Forest::OstArray vanillaOstId
{
	17, 32, 26, 31, 41, 42, 39, 143
};

Forest::Forest(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
	: m_game(std::move(game)), m_sharedData(std::move(sharedData))
{
}

Forest::OstArray Forest::ost() const
{
	const auto over_game_bin{ m_game->file(File::OVER_GAME_BIN) };
	Forest::OstArray ostsId;

	for (u32 i{}; i < Forest::nbForest; ++i)
	{
		over_game_bin->read(m_game->offset().file.over_game_bin.tableOfMapsBehavior + ostMapsShift[i].shift, &ostsId[i]);
	}

	return ostsId;
}

void Forest::setPaletteColor() const
{
	auto rotate = [](RawFile* file, s32 rotation, auto... offsets)
	{
		auto mono = [&file, &rotation](auto offset)
		{
			const auto nbClut{ file->read<u16>(offset + 0x12) };

			for (u16 i{}; i < nbClut; ++i)
			{
				static constexpr auto clutSize{ 0x100u }, sizeTimHeader{ 0x14u };
				const u32 clutOffset{ offset + sizeTimHeader + clutSize * sizeof(u16) * i };

				auto clut{ file->read<std::array<u16, clutSize>>(clutOffset) };
				JCUtility::rotateCLUT(clut, rotation);
				file->write(clutOffset, clut);
			}
		};

		(mono(offsets), ...);
	};

	auto rotateBPP16 = [](RawFile* file, s32 rotation, auto... offsets)
	{
		auto mono = [&file, &rotation](auto offset)
		{
			static constexpr auto sizeTimHeader{ 0x14u };
			const u32 clutOffset{ offset + sizeTimHeader };

			const auto clutSize{ file->read<u32>(offset + 8) - 0xC };
			std::vector<u16> clut(clutSize / sizeof(u16));
			auto* clutPtr{ clut.data() };

			file->read(clutOffset, clutPtr, clutSize);
			JCUtility::rotateCLUT(clut, rotation);
			file->write(clutOffset, *clutPtr, clutSize);
		};

		(mono(offsets), ...);
	};

	auto rng{ Random::get().generate(JCUtility::clutRotationLimit) };

	// Gate
	const auto 
		scene_field1_gate_sce00_sce{ m_game->file(File::SCENE_FIELD1_GATE_SCE00_SCE) },
		scene_field1_gate_sce06_sce{ m_game->file(File::SCENE_FIELD1_GATE_SCE06_SCE) };

	// Preview
	const auto 
		whole_parell_reduct2_tiy{ m_game->file(File::WHOLE_PARELL_REDUCT2_TIY) },
		whole_parell_reduct3_tiy{ m_game->file(File::WHOLE_PARELL_REDUCT3_TIY) },
		whole_parell_reduct4_tiy{ m_game->file(File::WHOLE_PARELL_REDUCT4_TIY) };

	// Forest 1
	rotate(m_game->file(File::SCENE_FIELD1_FOREST1_SCE00_SCE).get(), rng, 0x130, 0x3191C, 0x4D124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST1_SCE01_SCE).get(), rng, 0x12C, 0x2F924, 0x59924, 0x7D928, 0x9D920);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST1_SCE02_SCE).get(), rng, 0x12C, 0x2A128, 0x4C928, 0x6F11C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST1_SCE03_SCE).get(), rng, 0x128, 0x20920, 0x3C920, 0x5A124, 0x7791C, 0x8D124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST1_SCE04_SCE).get(), rng, 0x12C, 0x2F130, 0x67124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST1_SCE05_SCE).get(), rng, 0x128, 0x2612C, 0x55128);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST1_SCE06_SCE).get(), rng, 0x124, 0x26120, 0x49128, 0x6911C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST1_SCE07_SCE).get(), rng, 0x134, 0x39120, 0x56124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST1_SCE08_SCE).get(), rng, 0x124, 0x19124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST1_SCE09_SCE).get(), rng, 0x124, 0x24128, 0x56130);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST1_SCE10_SCE).get(), rng, 0x12C, 0x2680C);

	rotate(m_game->file(File::EFFECT_BTFIELD_EFD).get(), rng, 0x4054, 0x8274, 0xA494, 0xB8B4, 0xE2D4, 0x10CF4, 0x12F14, 0x13D34, 0x15F54);
	rotate(m_game->file(File::EFFECT_BTFIELD2_EFD).get(), rng, 0x3928, 0x7B48, 0xCD68, 0xEF88, 0x111A8, 0x12BC8);
	rotate(m_game->file(File::EFFECT_BTFLD2A_EFD).get(), rng, 0x3C9C, 0x90DC, 0xB0FC, 0xD11C, 0xE53C, 0x1075C, 0x11B7C, 0x13D9C, 0x15FBC);
	rotate(m_game->file(File::EFFECT_BTFLD2B_EFD).get(), rng, 0x3F74, 0x8194, 0x93B4, 0xB3D4, 0xD3F4, 0xE814, 0x10A34, 0x11E54, 0x14074, 0x16294);

	rotate(scene_field1_gate_sce00_sce.get(), rng, 0x7C11C);
	rotate(scene_field1_gate_sce06_sce.get(), rng, 0x7C11C);

	rotate(m_game->file(File::SCENE_OTHER_DREAM_SCE00_SCE).get(), rng, 0x11C, 0x1D11C, 0x3A11C, 0x5711C, 0x7411C, 0x9111C);
	rotate(m_game->file(File::SCENE_OTHER_DREAM_SCE01_SCE).get(), rng, 0x11C, 0x1D11C, 0x3A11C);

	rotateBPP16(m_game->file(File::WHOLE_PARELL_TAMAMUSI_TIY).get(), rng, 8);
	rotateBPP16(m_game->file(File::WHOLE_PARELL_REDUCT1_TIY).get(), rng, 0x1858);
	rotateBPP16(whole_parell_reduct2_tiy.get(), rng, 0x3008);
	rotateBPP16(whole_parell_reduct3_tiy.get(), rng, 0x5FF4);
	rotateBPP16(whole_parell_reduct4_tiy.get(), rng, 0xEFA8);

	// Forest 2
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE00_SCE).get(), rng, 0x120, 0x1D124, 0x40924);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE01_SCE).get(), rng, 0x12C, 0x2991C, 0x44124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE02_SCE).get(), rng, 0x128, 0x2B120);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE03_SCE).get(), rng, 0x124, 0x2492C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE04_SCE).get(), rng, 0x124, 0x26928, 0x4F128);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE05_SCE).get(), rng, 0x124, 0x20928, 0x4A120, 0x67120, 0x84920, 0xA2120, 0xBF924);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE06_SCE).get(), rng, 0x128, 0x1F418, 0x25920, 0x41570, 0x47920, 0x69DB0);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE07_SCE).get(), rng, 0x11C, 0x19924, 0x3891C, 0x4D91C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE08_SCE).get(), rng, 0x11C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE09_SCE).get(), rng, 0x124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE10_SCE).get(), rng, 0x12C, 0x35928, 0x68128, 0x9B12C, 0xD0130);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE11_SCE).get(), rng, 0x11C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE12_SCE).get(), rng, 0x120, 0x24924, 0x3E124, 0x57924);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE13_SCE).get(), rng, 0x128, 0x20920, 0x4292C, 0x71920);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE14_SCE).get(), rng, 0x134);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE15_SCE).get(), rng, 0x120, 0x1D124, 0x40924);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE16_SCE).get(), rng, 0x120, 0x1D124, 0x40924);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST2_SCE17_SCE).get(), rng, 0x130);

	rotate(m_game->file(File::EFFECT_BTFIELD3_EFD).get(), rng, 0x3E7C, 0x809C, 0xC31C, 0xD73C, 0xEB5C, 0x10B7C, 0x1359C, 0x15FBC);
	rotate(m_game->file(File::EFFECT_BTFIELD4_EFD).get(), rng, 0x40FC, 0x831C, 0xC59C, 0xEFBC, 0x10FDC, 0x125FC, 0x13A1C, 0x15A3C, 0x17A5C);
	rotate(m_game->file(File::EFFECT_BTFIELD5_EFD).get(), rng, 0x39E0, 0x7C00, 0xBE80, 0xE4A0, 0xFFC0, 0x121E0, 0x14400, 0x16420, 0x17240);
	rotate(m_game->file(File::EFFECT_BTFIELD6_EFD).get(), rng, 0x3D6C, 0x7F8C, 0xC1AC, 0xE7CC, 0x111EC, 0x1340C, 0x14F2C, 0x15D4C, 0x17F6C);

	rotate(scene_field1_gate_sce00_sce.get(), rng, 0xB011C);
	rotate(scene_field1_gate_sce06_sce.get(), rng, 0xB011C);

	rotateBPP16(m_game->file(File::WHOLE_PARELL_TOMBO_TIY).get(), rng, 8);
	rotateBPP16(whole_parell_reduct2_tiy.get(), rng, 0x24);
	rotateBPP16(whole_parell_reduct3_tiy.get(), rng, 0x2C);
	rotateBPP16(whole_parell_reduct4_tiy.get(), rng, 0x5FFC);

	// Forest 3
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE00_SCE).get(), rng, 0x12C, 0x27928);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE01_SCE).get(), rng, 0x120, 0x1D924, 0x35928);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE02_SCE).get(), rng, 0x124, 0x19124, 0x43920);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE03_SCE).get(), rng, 0x128, 0x3392C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE04_SCE).get(), rng, 0x128, 0x2D128);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE05_SCE).get(), rng, 0x124, 0x1F920);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE06_SCE).get(), rng, 0x12C, 0x28120, 0x4A12C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE07_SCE).get(), rng, 0x124, 0x2A924);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE08_SCE).get(), rng, 0x134);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE09_SCE).get(), rng, 0x124, 0x24124, 0x48128);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE10_SCE).get(), rng, 0x11C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE11_SCE).get(), rng, 0x130, 0x32124, 0x5D920);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE12_SCE).get(), rng, 0x124, 0x2A124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE13_SCE).get(), rng, 0x120, 0x2A120);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE14_SCE).get(), rng, 0x130, 0x31120);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE15_SCE).get(), rng, 0x130, 0x37130);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE16_SCE).get(), rng, 0x120, 0x1C934, 0x55920);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE17_SCE).get(), rng, 0x120, 0x1D920, 0x3B928, 0x64120);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE18_SCE).get(), rng, 0x11C, 0x1B92C, 0x44920);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE19_SCE).get(), rng, 0x128, 0x39130, 0x70928);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE20_SCE).get(), rng, 0x120, 0x15918, 0x2711C, 0x40120, 0x4F918, 0x6111C, 0x7A120, 0x89920, 0x99120, 0xAE918, 0xC0120);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE21_SCE).get(), rng, 0x12C, 0x28128);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE22_SCE).get(), rng, 0x120, 0x1D124, 0x35128);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE23_SCE).get(), rng, 0x124, 0x19924, 0x44920);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE24_SCE).get(), rng, 0x12C, 0x3612C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE25_SCE).get(), rng, 0x12C, 0x2E128);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE26_SCE).get(), rng, 0x124, 0x1F924);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE27_SCE).get(), rng, 0x12C, 0x28920, 0x4B12C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE28_SCE).get(), rng, 0x124, 0x2A924);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE29_SCE).get(), rng, 0x134);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE30_SCE).get(), rng, 0x124, 0x24124, 0x48928);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE31_SCE).get(), rng, 0x11C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE32_SCE).get(), rng, 0x128, 0x34124, 0x5F92C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE33_SCE).get(), rng, 0x124, 0x2A124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE34_SCE).get(), rng, 0x12C, 0x2F124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE35_SCE).get(), rng, 0x130, 0x31120);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE36_SCE).get(), rng, 0x130, 0x37930);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE37_SCE).get(), rng, 0x124, 0x18128, 0x4A11C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE38_SCE).get(), rng, 0x120, 0x1D920, 0x3B928, 0x64120);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE39_SCE).get(), rng, 0x120, 0x1C12C, 0x45120);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE40_SCE).get(), rng, 0x128, 0x39130, 0x70928);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE41_SCE).get(), rng, 0x124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE42_SCE).get(), rng, 0x128);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST3_SCE43_SCE).get(), rng, 0x128);

	rotate(m_game->file(File::EFFECT_BTFIELD7_EFD).get(), rng, 0x3CBC, 0x7EDC, 0xC15C, 0xEB7C, 0x1161C, 0x137BC, 0x15CDC, 0x1757C);
	rotate(m_game->file(File::EFFECT_BTFIELD8_EFD).get(), rng, 0x403C, 0x825C, 0xAC7C, 0xD71C, 0xF8BC, 0x11DDC, 0x1367C, 0x14C1C, 0x1723C);
	rotate(m_game->file(File::EFFECT_BTFLD09_EFD).get(), rng, 0x3CC0, 0x7EE0, 0xC160, 0xEB80, 0x11620, 0x137C0, 0x15CE0, 0x17580);
	rotate(m_game->file(File::EFFECT_BTFLD10_EFD).get(), rng, 0x4A40, 0x8C60, 0xCEE0, 0xF900, 0x123A0, 0x14540, 0x16A60, 0x18300);
	rotate(m_game->file(File::EFFECT_BTFLD07X_EFD).get(), rng, 0x3E4C, 0x806C, 0xC2EC, 0xED0C, 0x117AC, 0x1394C, 0x15E6C, 0x1770C);
	rotate(m_game->file(File::EFFECT_BTFLD08X_EFD).get(), rng, 0x403C, 0x825C, 0xAC7C, 0xD71C, 0xF8BC, 0x11DDC, 0x1367C, 0x14C1C, 0x1723C);
	rotate(m_game->file(File::EFFECT_BTFLD09X_EFD).get(), rng, 0x3CBC, 0x7EDC, 0xC15C, 0xEB7C, 0x1161C, 0x137BC, 0x15CDC, 0x1757C);
	rotate(m_game->file(File::EFFECT_BTFLD10X_EFD).get(), rng, 0x4A40, 0x8C60, 0xCEE0, 0xF900, 0x123A0, 0x14540, 0x16A60, 0x18300);

	rotate(scene_field1_gate_sce00_sce.get(), rng, 0x9591C);
	rotate(scene_field1_gate_sce06_sce.get(), rng, 0x9591C);

	rotateBPP16(m_game->file(File::WHOLE_PARELL_KUMO_TIY).get(), rng, 8);
	rotateBPP16(whole_parell_reduct3_tiy.get(), rng, 0x3010);
	rotateBPP16(whole_parell_reduct4_tiy.get(), rng, 0x8FE0);

	// Forest 4
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE00_SCE).get(), rng, 0x128, 0x2791C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE01_SCE).get(), rng, 0x128, 0x22920, 0x46124, 0x60928);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE02_SCE).get(), rng, 0x130, 0x37920);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE03_SCE).get(), rng, 0x12C, 0x2A124, 0x4B124, 0x65920);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE04_SCE).get(), rng, 0x11C, 0x1B124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE05_SCE).get(), rng, 0x12C);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE06_SCE).get(), rng, 0x12C, 0x35924);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE07_SCE).get(), rng, 0x130, 0x30F30, 0x36920, 0x51F5C, 0x57924, 0x7BA44);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE08_SCE).get(), rng, 0x134, 0x38928, 0x6C130);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE09_SCE).get(), rng, 0x11C, 0x1B124, 0x32920, 0x5612C, 0x8B128, 0xB9120, 0xDB128, 0xFC118);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE10_SCE).get(), rng, 0x124, 0x1E11C, 0x38920, 0x55124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE11_SCE).get(), rng, 0x124, 0x2B124);
	rotate(m_game->file(File::SCENE_FIELD1_FOREST4_SCE12_SCE).get(), rng, 0x11C, 0x1491C, 0x2811C, 0x35120, 0x4491C, 0x5911C, 0x6111C, 0x7591C, 0x89120, 0x98918, 0x9F11C, 0xA6918, 0xB291C, 0xBF920);

	rotate(m_game->file(File::EFFECT_BTFLD20_EFD).get(), rng, 0x3EBC, 0x98DC, 0xDAFC, 0xF51C, 0x10F3C, 0x1395C, 0x15F7C);
	rotate(m_game->file(File::EFFECT_BTFLD21_EFD).get(), rng, 0x3D6C, 0x7F8C, 0xC1AC, 0xD3CC, 0xEDEC, 0x1080C, 0x13A2C, 0x1604C);
	rotate(m_game->file(File::EFFECT_BTFLD22_EFD).get(), rng, 0x3D1C, 0x973C, 0xD95C, 0xF37C, 0x10D9C, 0x137BC, 0x15DDC);

	rotate(scene_field1_gate_sce00_sce.get(), rng, 0xC911C);
	rotate(scene_field1_gate_sce06_sce.get(), rng, 0xC911C);

	rotateBPP16(m_game->file(File::WHOLE_PARELL_GA_TIY).get(), rng, 8);
	rotateBPP16(whole_parell_reduct4_tiy.get(), rng, 0xBFC4);

	// Forest 1-2
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	rotate(m_game->file(File::SCENE_FIELD2_FOREST1_SCE00_SCE).get(), rng, 0x130, 0x3111C, 0x4C124);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST1_SCE01_SCE).get(), rng, 0x12C, 0x1CFCC, 0x24924, 0x4E924, 0x72928, 0x92920);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST1_SCE02_SCE).get(), rng, 0x12C, 0x2A128, 0x4C928, 0x6F11C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST1_SCE03_SCE).get(), rng, 0x128, 0x20920, 0x3C920, 0x5A924, 0x7811C, 0x8D924);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST1_SCE04_SCE).get(), rng, 0x12C, 0x2F130, 0x67124);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST1_SCE05_SCE).get(), rng, 0x128, 0x2612C, 0x55128);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST1_SCE06_SCE).get(), rng, 0x124, 0x26120, 0x49928, 0x6A91C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST1_SCE07_SCE).get(), rng, 0x134, 0x39120, 0x56124);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST1_SCE08_SCE).get(), rng, 0x124, 0x18924);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST1_SCE09_SCE).get(), rng, 0x124, 0x2412C, 0x52930);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST1_SCE10_SCE).get(), rng, 0x128);

	rotate(m_game->file(File::EFFECT_BTFIELDZ_EFD).get(), rng, 0x4054, 0x8274, 0xA494, 0xB8B4, 0xE2D4, 0x10CF4, 0x12F14, 0x13D34, 0x15F54);
	rotate(m_game->file(File::EFFECT_BTFLD2AZ_EFD).get(), rng, 0x3C9C, 0x7EBC, 0x90DC, 0xB0FC, 0xD11C, 0xE53C, 0x1075C, 0x11B7C, 0x13D9C, 0x15FBC);
	rotate(m_game->file(File::EFFECT_BTFLD2BZ_EFD).get(), rng, 0x3F74, 0x8194, 0x93B4, 0xB3D4, 0xD3F4, 0xE814, 0x10A34, 0x11E54, 0x14074, 0x16294);
	rotate(m_game->file(File::EFFECT_BTFLD15_EFD).get(), rng, 0x415C, 0x837C, 0x101A4, 0x115C4, 0x129E4, 0x13804, 0x16224);

	// Forest 2-2
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE00_SCE).get(), rng, 0x120, 0x1D924, 0x41924);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE01_SCE).get(), rng, 0x12C, 0x2891C, 0x42924);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE02_SCE).get(), rng, 0x128, 0x2B120);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE03_SCE).get(), rng, 0x124, 0x2492C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE04_SCE).get(), rng, 0x124, 0x25928, 0x4D128);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE05_SCE).get(), rng, 0x124, 0x20928, 0x4A120, 0x67120, 0x84920, 0xA2120, 0xBF924);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE06_SCE).get(), rng, 0x128, 0x22120, 0x40120);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE07_SCE).get(), rng, 0x11C, 0x19124, 0x3791C, 0x4C11C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE08_SCE).get(), rng, 0x11C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE09_SCE).get(), rng, 0x124);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE10_SCE).get(), rng, 0x12C, 0x34930, 0x6592C, 0x95930, 0xCA630, 0xCD128);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE11_SCE).get(), rng, 0x11C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE12_SCE).get(), rng, 0x120, 0x24924, 0x3E124, 0x57924);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE13_SCE).get(), rng, 0x128, 0x21920, 0x3E12C, 0x6E120);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE14_SCE).get(), rng, 0x12C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE15_SCE).get(), rng, 0x120, 0x1D124, 0x40924);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE16_SCE).get(), rng, 0x120, 0x1D124, 0x40924);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST2_SCE17_SCE).get(), rng, 0x130);

	rotate(m_game->file(File::EFFECT_BTFLD03Z_EFD).get(), rng, 0x3E7C, 0x809C, 0xC31C, 0xD73C, 0xEB5C, 0x10B7C, 0x1359C, 0x15FBC);
	rotate(m_game->file(File::EFFECT_BTFLD04Z_EFD).get(), rng, 0x40FC, 0x831C, 0xC59C, 0xEFBC, 0x10FDC, 0x125FC, 0x13A1C, 0x15A3C, 0x17A5C);
	rotate(m_game->file(File::EFFECT_BTFLD05Z_EFD).get(), rng, 0x39E4, 0x7C04, 0xBE84, 0xE4A4, 0xFFC4, 0x121E4, 0x14404, 0x16424, 0x17244);
	rotate(m_game->file(File::EFFECT_BTFLD06Z_EFD).get(), rng, 0x3D70, 0x7F90, 0xC1B0, 0xE7D0, 0x111F0, 0x13410, 0x14F30, 0x15D50, 0x17F70);
	rotate(m_game->file(File::EFFECT_BTFLD16_EFD).get(), rng, 0x3D6C, 0x7F8C, 0xC1AC, 0xE3CC, 0x105EC, 0x1480C);

	// Forest 3-2
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE00_SCE).get(), rng, 0x12C, 0x27924);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE01_SCE).get(), rng, 0x120, 0x1D124, 0x35128);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE02_SCE).get(), rng, 0x124, 0x19124, 0x43920);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE03_SCE).get(), rng, 0x128, 0x3392C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE04_SCE).get(), rng, 0x128, 0x2D128);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE05_SCE).get(), rng, 0x124, 0x1F920);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE06_SCE).get(), rng, 0x12C, 0x28920, 0x4B120);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE07_SCE).get(), rng, 0x124, 0x2B124);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE08_SCE).get(), rng, 0x134);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE09_SCE).get(), rng, 0x124, 0x24124, 0x49128);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE10_SCE).get(), rng, 0x11C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE11_SCE).get(), rng, 0x12C, 0x34924, 0x5F92C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE12_SCE).get(), rng, 0x124, 0x2A124);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE13_SCE).get(), rng, 0x12C, 0x2F124);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE14_SCE).get(), rng, 0x130, 0x31920);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE15_SCE).get(), rng, 0x130, 0x37128);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE16_SCE).get(), rng, 0x124, 0x18928, 0x4C11C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE17_SCE).get(), rng, 0x120, 0x1D920, 0x3B928, 0x64120);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE18_SCE).get(), rng, 0x11C, 0x1B92C, 0x44920);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE19_SCE).get(), rng, 0x128, 0x39130, 0x70928);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE20_SCE).get(), rng, 0x11C, 0xD91C, 0x1B11C, 0x2891C, 0x36120, 0x4611C, 0x5391C, 0x6111C, 0x6E91C, 0x7C11C, 0x8991C);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE21_SCE).get(), rng, 0x128);
	rotate(m_game->file(File::SCENE_FIELD2_FOREST3_SCE22_SCE).get(), rng, 0x11C, 0xD11C, 0x1A11C, 0x27120, 0x3691C, 0x4391C, 0x5091C, 0x5D91C, 0x6A91C, 0x7791C, 0x8491C, 0x9191C);

	rotate(m_game->file(File::EFFECT_BTFLD07Z_EFD).get(), rng, 0x3CBC, 0x7EDC, 0xC15C, 0xEB7C, 0x1161C, 0x137BC, 0x15CDC, 0x1757C);
	rotate(m_game->file(File::EFFECT_BTFLD08Z_EFD).get(), rng, 0x403C, 0x825C, 0xAC7C, 0xD71C, 0xF8BC, 0x11DDC, 0x1367C, 0x14C1C, 0x1723C);
	rotate(m_game->file(File::EFFECT_BTFLD09Z_EFD).get(), rng, 0x3CBC, 0x7EDC, 0xC15C, 0xEB7C, 0x1161C, 0x137BC, 0x15CDC, 0x1757C);
	rotate(m_game->file(File::EFFECT_BTFLD10Z_EFD).get(), rng, 0x4A40, 0x8C60, 0xCEE0, 0xF900, 0x123A0, 0x14540, 0x16A60, 0x18300);
	rotate(m_game->file(File::EFFECT_BTFLD17_EFD).get(), rng, 0x3F64, 0x8184, 0xA6A4, 0xC5C4, 0x107E4, 0x13204, 0x14C24);
	rotate(m_game->file(File::EFFECT_BTFLD18_EFD).get(), rng, 0x5288, 0x94A8, 0x116C8, 0x158E8);
	rotate(m_game->file(File::EFFECT_BTFLD19_EFD).get(), rng, 0x46F0, 0x8910, 0x94F4, 0xD714, 0x101B4, 0x12354, 0x14974, 0x16394);

	// Eternal Corridor
	struct FileOffsets
	{
		s32 file;
		std::unordered_set<u32> offsets;
	};

	static constexpr auto nbElementsEC{ 5u }; // None - Fire - Air - Earth - Water
	const std::array<FileOffsets, nbElementsEC> fileBattleOffsets
	{
		File::EFFECT_BTLFD27_EFD, { 0x3E44, 0x8064, 0xC2E4, 0xE504, 0x10724, 0x12944, 0x14B64, 0x16D84 },
		File::EFFECT_BTFLD23_EFD, { 0x3E44, 0x8064, 0xC2E4, 0xE904, 0x10F24, 0x13144, 0x15364, 0x17584 },
		File::EFFECT_BTFLD24_EFD, { 0x3D84, 0x7FA4, 0xC224, 0xE044, 0xFE64, 0x13084, 0x162A4 },
		File::EFFECT_BTFLD25_EFD, { 0x3E44, 0x8064, 0xC2E4, 0xE504, 0x10724, 0x13144, 0x15364, 0x17584 },
		File::EFFECT_BTFLD26_EFD, { 0x3E44, 0x8064, 0xC2E4, 0xE504, 0x10724, 0x12944, 0x14B64, 0x16D84 }
	};

	for (u32 i{}; i < nbElementsEC; ++i)
	{
		static constexpr auto nbMapsElement{ 15u };
		const auto elementFileId{ i * nbMapsElement };

		rng = Random::get().generate(JCUtility::clutRotationLimit);

		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE00_SCE + elementFileId).get(), rng, 0x124);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE01_SCE + elementFileId).get(), rng, 0x11C, 0x1491C);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE02_SCE + elementFileId).get(), rng, 0x11C, 0x1491C, 0x29120, 0x3E11C);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE03_SCE + elementFileId).get(), rng, 0x124, 0x1E91C);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE04_SCE + elementFileId).get(), rng, 0x124);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE05_SCE + elementFileId).get(), rng, 0x124);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE06_SCE + elementFileId).get(), rng, 0x120, 0x1B11C);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE07_SCE + elementFileId).get(), rng, 0x128, 0x1F120, 0x3A920, 0x5611C);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE08_SCE + elementFileId).get(), rng, 0x124, 0x1E924);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE09_SCE + elementFileId).get(), rng, 0x12C);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE10_SCE + elementFileId).get(), rng, 0x11C);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE11_SCE + elementFileId).get(), rng, 0x128, 0x1F920);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE12_SCE + elementFileId).get(), rng, 0x120, 0x1B91C, 0x30120, 0x45924);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE13_SCE + elementFileId).get(), rng, 0x128, 0x2011C);
		rotate(m_game->file(File::SCENE_OTHER_HUNTING_SCE14_SCE + elementFileId).get(), rng, 0x124);

		for (const auto& offset : fileBattleOffsets[i].offsets)
		{
			rotate(m_game->file(fileBattleOffsets[i].file).get(), rng, offset);
		}
	}
}

void Forest::setBattleMaps() const
{
	static constexpr u8 nbBattleMaps{ 39 };

	const auto over_game_bin{ m_game->file(File::OVER_GAME_BIN) };
	auto mapsIdRaw{ over_game_bin->read<std::array<u16, 250>>(m_game->offset().file.over_game_bin.battleMaps) };

	std::unordered_set<u16> mapsId;
	std::array<std::set<u16>, nbBattleMaps> battleMaps;

	for (const auto& map : mapsIdRaw)
	{
		mapsId.insert(map);
	}

	mapsId.insert(1); // Dream 2
	mapsId.insert(0x0065); // Koris Minion Map
	mapsId.insert(0x0071); // Masked Boy Map
	mapsId.erase(0xFFFF); // Delimitator
	mapsId.erase(0x00D9); // Tuturis
	mapsId.erase(0x00E4); // Seterian
	mapsId.erase(0x0104); // Delfanel
	battleMaps[15].insert(0x00D9);
	battleMaps[16].insert(0x00E4);
	battleMaps[17].insert(0x0104);

	for (const auto& mapId : mapsId)
	{
		u8 rngBattleMap;
		do
		{
			rngBattleMap = Random::get().generate<u8>(nbBattleMaps - 1);
		} while (rngBattleMap >= 15 && rngBattleMap <= 17); // Elemental map causes a deva infinite loop

		battleMaps.at(rngBattleMap).insert(mapId);
	}

	std::array<u32, nbBattleMaps> battleMapsPtr{};
	over_game_bin->read(m_game->offset().file.over_game_bin.battleMaps + sizeof(mapsIdRaw), &battleMapsPtr[0]);
	mapsIdRaw = {};

	u32 pos{};
	for (u8 i{}; i < nbBattleMaps; ++i)
	{
		for (const auto& battleMap : battleMaps[i])
		{
			mapsIdRaw.at(pos) = battleMap;
			++pos;
		}

		mapsIdRaw.at(pos) = 0xFFFF;
		++pos;

		if (i != nbBattleMaps - 1) // No new pointer needed since it's the last map
		{
			battleMapsPtr[i + 1] = battleMapsPtr[0] + pos * 2;
		}
	}

	over_game_bin->write(m_game->offset().file.over_game_bin.battleMaps, mapsIdRaw);
	over_game_bin->write(m_game->offset().file.over_game_bin.battleMaps + sizeof(mapsIdRaw), battleMapsPtr);
}

void Forest::setOst(Forest::Ost state) const
{
	Forest::OstArray ostsId;

	if (state == Forest::Ost::Random)
	{
		for (auto& ostId : ostsId)
		{
			ostId = -1;
		}
	}
	else // Shuffle
	{
		std::vector<s16> ostsList{ vanillaOstId.begin(), vanillaOstId.end() };

		for (auto& ostId : ostsId)
		{
			const auto rngOst{ Random::get().generate(ostsList.size() - 1) };
			ostId = ostsList[rngOst];
			ostsList.erase(ostsList.begin() + rngOst);
		}
	}

	setOst(ostsId);
}

void Forest::setOst(const Forest::OstArray& ostsId) const
{
	Forest::OstArray forestOsts;
	auto availableOsts{ ::Ost::id() };

	static constexpr std::array<s16, 3> duplicateOsts{ 30, 126, 144 }; // Legend of Arcana ~ Clandestine Meeting duplicate

	availableOsts.erase(std::remove_if(availableOsts.begin(), availableOsts.end(),
		[](const auto& ost)
		{
			for (const auto& duplicateOst : duplicateOsts)
				if (ost == duplicateOst) return true;
			return false;
		}), availableOsts.end());

	for (u32 i{}; i < Forest::nbForest; ++i)
	{
		if (ostsId[i] == -1)
		{
			forestOsts[i] = availableOsts[Random::get().generate(availableOsts.size() - 1)];
		}
		else
		{
			forestOsts[i] = ostsId[i];
		}
	}

	const auto over_game_bin{ m_game->file(File::OVER_GAME_BIN) };
	const auto offset{ m_game->offset().file.over_game_bin.tableOfMapsBehavior };

	for (u32 i{}; i < Forest::nbForest; ++i)
	{
		const auto firstOst{ over_game_bin->read<s16>(offset + ostMapsShift[i].shift)};

		for (u32 j{}; j < ostMapsShift[i].nbMaps; ++j)
		{
			static constexpr auto mapBehaviorSize{ 0x10u };
			const auto ostOffset{ offset + ostMapsShift[i].shift + j * mapBehaviorSize };

			if (over_game_bin->read<s16>(ostOffset) == firstOst)
			{
				over_game_bin->write(ostOffset, forestOsts[i]);
			}
		}
	}

	// Gate
	const std::array<FileOffset, 5> gateFilesOffset
	{{
		{ File::SCENE_FIELD1_GATE_SCE00_SBH, m_game->offset().file.scene_field1_gate_sce00_sbh.forestOstId },
		{ File::SCENE_FIELD1_GATE_SCE00A_SBH, m_game->offset().file.scene_field1_gate_sce00a_sbh.forestOstId },
		{ File::SCENE_FIELD1_GATE_SCE00B_SBH, m_game->offset().file.scene_field1_gate_sce00b_sbh.forestOstId },
		{ File::SCENE_FIELD1_GATE_SCE00C_SBH, m_game->offset().file.scene_field1_gate_sce00c_sbh.forestOstId },
		{ File::SCENE_FIELD1_GATE_SCE06_SBH, m_game->offset().file.scene_field1_gate_sce06_sbh.forestOstId }
	}};

	for (std::size_t i{}; i < gateFilesOffset.size(); ++i)
	{
		const auto file{ m_game->file(gateFilesOffset[i].first) };

		for (s32 j{}; j < 4; ++j)
		{
			if (gateFilesOffset[i].first == File::SCENE_FIELD1_GATE_SCE00A_SBH && j == 0) // Koris Ost
			{
				continue;
			}
			file->write(gateFilesOffset[i].second + j * 0x34, forestOsts[j]);
		}
	}

	// Gate behind
	const std::array<FileOffset, 5> gateFilesOffsetFromBehind
	{{
		{ File::SCENE_FIELD1_GATE_SCE00_SBH, m_game->offset().file.scene_field1_gate_sce00_sbh.forestOstIdFromBehind },
		{ File::SCENE_FIELD1_GATE_SCE00A_SBH, m_game->offset().file.scene_field1_gate_sce00a_sbh.forestOstIdFromBehind },
		{ File::SCENE_FIELD1_GATE_SCE00B_SBH, m_game->offset().file.scene_field1_gate_sce00b_sbh.forestOstIdFromBehind },
		{ File::SCENE_FIELD1_GATE_SCE00C_SBH, m_game->offset().file.scene_field1_gate_sce00c_sbh.forestOstIdFromBehind },
		{ File::SCENE_FIELD1_GATE_SCE06_SBH, m_game->offset().file.scene_field1_gate_sce06_sbh.forestOstIdFromBehind }
	}};

	const std::array<s16, 4> ostFromBehind{ forestOsts[0], forestOsts[2], forestOsts[1], forestOsts[3] };

	for (std::size_t i{}; i < gateFilesOffsetFromBehind.size(); ++i)
	{
		const auto file{ m_game->file(gateFilesOffsetFromBehind[i].first) };

		for (std::size_t j{}; j < ostFromBehind.size(); ++j)
		{
			file->write(gateFilesOffsetFromBehind[i].second + j * 4, ostFromBehind[j]);
		}
	}

	auto executable{ m_game->executable() };

	const auto
		scene_field1_forest1_sce01_sbh{ m_game->file(File::SCENE_FIELD1_FOREST1_SCE01_SBH) },
		scene_field1_forest2_sce06_sbh{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE06_SBH) },
		scene_field1_forest2_sce10_sbh{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE10_SBH) },
		scene_field1_forest2_sce14a_sbh{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE14A_SBH) },
		scene_field1_forest4_sce07_sbh{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE07_SBH) };

	// Boss ost reload model
	executable.write(m_game->offset().file.executable.actorBehaviorFn + 0x43C, Mips::li(Mips::Register::v0, forestOsts[4])); // F1_2
	executable.write(m_game->offset().file.executable.actorBehaviorFn + 0x460, Mips::li(Mips::Register::v0, forestOsts[5])); // F2_2

	// Sound effect
	if (forestOsts[0] != vanillaOstId[0])
	{
		scene_field1_forest1_sce01_sbh->write(m_game->offset().file.scene_field1_forest1_sce01_sbh.treeBranchBehaviorFn + 0xA8, Mips_t(0));
	}

	if (forestOsts[1] != vanillaOstId[1])
	{
		scene_field1_forest2_sce06_sbh->write(m_game->offset().file.scene_field1_forest2_sce06_sbh.larvaKeyBehaviorFn + 0xA8, Mips_t(0));
		scene_field1_forest2_sce06_sbh->write(m_game->offset().file.scene_field1_forest2_sce06_sbh.doorBehaviorFn + 0x138, Mips_t(0));
		scene_field1_forest2_sce10_sbh->write(m_game->offset().file.scene_field1_forest2_sce10_sbh.bridgeBehaviorFn + 0xA8, Mips_t(0));
		scene_field1_forest2_sce10_sbh->write(m_game->offset().file.scene_field1_forest2_sce10_sbh.bridgeBehaviorFn + 0x104, Mips_t(0));

		// Dragonfly Kikinak return
		scene_field1_forest2_sce14a_sbh->write(m_game->offset().file.scene_field1_forest2_sce14a_sbh.kikinakReturnOstId, forestOsts[1]);
	}

	if (forestOsts[3] != vanillaOstId[3])
	{
		scene_field1_forest4_sce07_sbh->write(m_game->offset().file.scene_field1_forest4_sce07_sbh.pupaKeyBehaviorFn + 0xA8, Mips_t(0));
		scene_field1_forest4_sce07_sbh->write(m_game->offset().file.scene_field1_forest4_sce07_sbh.doorBehaviorFn + 0x138, Mips_t(0));
	}
}