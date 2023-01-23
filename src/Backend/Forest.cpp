#include "Forest.hpp"

#include "Backend/File.hpp"
#include "Backend/JCUtility.hpp"
#include "Backend/Mips.hpp"
#include "Backend/Ost.hpp"
#include "Backend/Random.hpp"
#include "Backend/Tim.hpp"
#include "Backend/Utility.hpp"
#include "Common/TemplateTypes.hpp"

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
	auto rotate255 = [](RawFile* file, s32 rotation, u32 offset)
	{
		auto rgb{ file->read<std::array<u8, 3>>(offset) };
		auto timClutRGB
		{
			static_cast<u16>
			(
				rgb[0] / 8 + ((rgb[1] / 8) << 5) + ((rgb[2] / 8) << 10)
			)
		};

		JCUtility::rotateCLUT({ &timClutRGB, 1 }, rotation);

		rgb =
		{
			static_cast<u8>(timClutRGB * 8),
			static_cast<u8>((timClutRGB >> 5) * 8),
			static_cast<u8>((timClutRGB >> 10) * 8)
		};

		file->write(offset, rgb);
	};

	auto rotate4096 = [](RawFile* file, s32 rotation, u32 offset)
	{
		auto rgb{ file->read<std::array<u16, 3>>(offset) };

		if (!Utility::areSame<u16>(rgb))
		{
			auto timClutRGB
			{
				static_cast<u16>
				(
					(255 * rgb[0] / 32768) + ((255 * rgb[1] / 32768) << 5) + ((255 * rgb[2] / 32768) << 10)
				)
			};

			JCUtility::rotateCLUT({ &timClutRGB, 1 }, rotation);

			rgb =
			{
				static_cast<u16>((timClutRGB & 0x1F) * 32768 / 255),
				static_cast<u16>(((timClutRGB >> 5) & 0x1F) * 32768 / 255),
				static_cast<u16>(((timClutRGB >> 10) & 0x1F) * 32768 / 255)
			};

			file->write(offset, rgb);
		}
	};

	auto rotateLightSpot = [&rotate4096]<s32 One = -1, s32 Two = -1, s32 Three = -1, s32 Four = -1, Integral... Args>
		(RawFile* file, s32 rotation, Args... offsets)
	{
		auto mono = [&]<s32 ShiftRotation, u32 ShiftOffset>()
		{
			if constexpr (ShiftRotation != -1)
			{
				const auto lightSpotRotation{ (rotation + ShiftRotation) % JCUtility::clutRotationLimit };
				(rotate4096(file, lightSpotRotation, static_cast<u32>(offsets + ShiftOffset)), ...);
			}
		};

		mono.operator()<One, 0>();
		mono.operator()<Two, 0x18>();
		mono.operator()<Three, 0x30>();
		mono.operator()<Four, 0x48>();
	};

	auto rotateLight = [&rotate4096, &rotateLightSpot]
		<s32 Spot1 = -1, s32 Spot2 = -1, s32 Spot3 = -1, s32 Spot4 = -1, Integral... Args>
		(RawFile* file, s32 rotation, Args... offsets)
	{
		(rotate4096(file, rotation, static_cast<u32>(offsets)), ...);

		if constexpr (Spot1 != -1 || Spot2 != -1 || Spot3 != -1 || Spot4 != -1)
		{
			((offsets += 0x14), ...);
			rotateLightSpot.operator()<Spot1, Spot2, Spot3, Spot4>(file, rotation, offsets...);
		}	
	};

	auto rotateShade = [&rotate255, &rotateLight, &rotateLightSpot]
		<s32 Light = -1, s32 Spot1 = -1, s32 Spot2 = -1, s32 Spot3 = -1, s32 Spot4 = -1, Integral... Args>
		(RawFile* file, s32 rotation, Args... offsets)
	{
		(rotate255(file, rotation, static_cast<u32>(offsets)), ...);

		if constexpr (Light != -1)
		{
			((offsets += 0xC), ...);
			const auto lightRotation{ (rotation + Light) % JCUtility::clutRotationLimit };
			rotateLight.operator()<Spot1, Spot2, Spot3, Spot4>(file, lightRotation, offsets...);
		}
		else if constexpr (Spot1 != -1 || Spot2 != -1 || Spot3 != -1 || Spot4 != -1)
		{
			((offsets += 0x20), ...);
			rotateLightSpot.operator()<Spot1, Spot2, Spot3, Spot4>(file, rotation, offsets...);
		}
	};

	auto addRGB = []<Integral T, Integral... Args>(RawFile* file, const std::array<T, 3>& rgb, Args... offsets)
	{
		auto mono = [&file, &rgb](u32 offset)
		{
			auto rgbFile{ file->read<std::array<T, 3>>(offset) };

			for (std::size_t i{}; i < rgb.size(); ++i)
			{
				rgbFile[i] += rgb[i];
			}

			file->write(offset, rgbFile);
		};

		(mono(static_cast<u32>(offsets)), ...);
	};

	auto addRGBShadeAndLight = [&addRGB]<Integral... Args>
		(RawFile* file, const std::array<u8, 3>& rgbShade, const std::array<u16, 3>& rgbLight, Args... offsets)
	{
		addRGB(file, rgbShade, offsets...);
		((offsets += 0xC), ...);
		addRGB(file, rgbLight, offsets...);
	};

	auto rng{ Random::get().generate(JCUtility::clutRotationLimit) };

	static constexpr auto sizeLimitBPP16{ 0x25800u };

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
	const auto 
		scene_field1_forest1_sce02_sce{ m_game->file(File::SCENE_FIELD1_FOREST1_SCE02_SCE) },
		scene_field1_forest1_sce04_sce{ m_game->file(File::SCENE_FIELD1_FOREST1_SCE04_SCE) };

	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST1_SCE00_SCE).get(), rng, 0x130, 0x3191C, 0x4D124);
	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST1_SCE01_SCE).get(), rng, 0x12C, 0x2F924, 0x59924, 0x7D928, 0x9D920);
	Tim::rotateBPP8(scene_field1_forest1_sce02_sce.get(), rng, 0x12C, 0x2A128, 0x4C928, 0x6F11C);
	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST1_SCE03_SCE).get(), rng, 0x128, 0x20920, 0x3C920, 0x5A124, 0x7791C, 0x8D124);
	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST1_SCE04_SCE).get(), rng, 0x12C, 0x2F130, 0x67124);
	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST1_SCE05_SCE).get(), rng, 0x128, 0x2612C, 0x55128);
	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST1_SCE06_SCE).get(), rng, 0x124, 0x26120, 0x49128, 0x6911C);
	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST1_SCE07_SCE).get(), rng, 0x134, 0x39120, 0x56124);
	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST1_SCE08_SCE).get(), rng, 0x124, 0x19124);
	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST1_SCE09_SCE).get(), rng, 0x124, 0x24128, 0x56130);
	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST1_SCE10_SCE).get(), rng, 0x12C, 0x2680C);

	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFIELD_EFD).get(), rng, 0x4054, 0x8274, 0xA494, 0xB8B4, 0xE2D4, 0x10CF4, 0x12F14, 0x13D34, 0x15F54);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFIELD2_EFD).get(), rng, 0x3928, 0x7B48, 0xCD68, 0xEF88, 0x111A8, 0x12BC8);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD2A_EFD).get(), rng, 0x3C9C, 0x90DC, 0xB0FC, 0xD11C, 0xE53C, 0x1075C, 0x11B7C, 0x13D9C, 0x15FBC);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD2B_EFD).get(), rng, 0x3F74, 0x8194, 0x93B4, 0xB3D4, 0xD3F4, 0xE814, 0x10A34, 0x11E54, 0x14074, 0x16294);

	Tim::rotateBPP8(scene_field1_gate_sce00_sce.get(), rng, 0x7C11C);
	Tim::rotateBPP8(scene_field1_gate_sce06_sce.get(), rng, 0x7C11C);

	Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_DREAM_SCE00_SCE).get(), rng, 0x11C, 0x1D11C, 0x3A11C, 0x5711C, 0x7411C, 0x9111C);
	Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_DREAM_SCE01_SCE).get(), rng, 0x11C, 0x1D11C, 0x3A11C);

	Tim::rotateBPP16(m_game->file(File::WHOLE_PARELL_TAMAMUSI_TIY).get(), rng, 8, sizeLimitBPP16);
	Tim::rotateBPP16(m_game->file(File::WHOLE_PARELL_REDUCT1_TIY).get(), rng, 0x1858, sizeLimitBPP16);
	Tim::rotateBPP16(whole_parell_reduct2_tiy.get(), rng, 0x3008, sizeLimitBPP16);
	Tim::rotateBPP16(whole_parell_reduct3_tiy.get(), rng, 0x5FF4, sizeLimitBPP16);
	Tim::rotateBPP16(whole_parell_reduct4_tiy.get(), rng, 0xEFA8, sizeLimitBPP16);

	rotateShade.operator()<0>(scene_field1_forest1_sce02_sce.get(), (rng + 180) % JCUtility::clutRotationLimit, 0x30, 0x2A030, 0x4C830, 0x6F030);
	rotateLightSpot.operator()<0>(scene_field1_forest1_sce04_sce.get(), rng, 0x50);

	// Forest 2
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	const auto
		scene_field1_forest2_sce00_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE00_SCE) },
		scene_field1_forest2_sce01_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE01_SCE) },
		scene_field1_forest2_sce02_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE02_SCE) },
		scene_field1_forest2_sce03_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE03_SCE) },
		scene_field1_forest2_sce04_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE04_SCE) },
		scene_field1_forest2_sce05_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE05_SCE) },
		scene_field1_forest2_sce06_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE06_SCE) },
		scene_field1_forest2_sce07_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE07_SCE) },
		scene_field1_forest2_sce08_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE08_SCE) },
		scene_field1_forest2_sce09_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE09_SCE) },
		scene_field1_forest2_sce10_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE10_SCE) },
		scene_field1_forest2_sce11_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE11_SCE) },
		scene_field1_forest2_sce12_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE12_SCE) },
		scene_field1_forest2_sce13_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE13_SCE) },
		scene_field1_forest2_sce14_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE14_SCE) },
		scene_field1_forest2_sce17_sce{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE17_SCE) };

	Tim::rotateBPP8(scene_field1_forest2_sce00_sce.get(), rng, 0x120, 0x1D124, 0x40924);
	Tim::rotateBPP8(scene_field1_forest2_sce01_sce.get(), rng, 0x12C, 0x2991C, 0x44124);
	Tim::rotateBPP8(scene_field1_forest2_sce02_sce.get(), rng, 0x128, 0x2B120);
	Tim::rotateBPP8(scene_field1_forest2_sce03_sce.get(), rng, 0x124, 0x2492C);
	Tim::rotateBPP8(scene_field1_forest2_sce04_sce.get(), rng, 0x124, 0x26928, 0x4F128);
	Tim::rotateBPP8(scene_field1_forest2_sce05_sce.get(), rng, 0x124, 0x20928, 0x4A120, 0x67120, 0x84920, 0xA2120, 0xBF924);
	Tim::rotateBPP8(scene_field1_forest2_sce06_sce.get(), rng, 0x128, 0x1F418, 0x25920, 0x41570, 0x47920, 0x69DB0);
	Tim::rotateBPP8(scene_field1_forest2_sce07_sce.get(), rng, 0x11C, 0x19924, 0x3891C, 0x4D91C);
	Tim::rotateBPP8(scene_field1_forest2_sce08_sce.get(), rng, 0x11C);
	Tim::rotateBPP8(scene_field1_forest2_sce09_sce.get(), rng, 0x124);
	Tim::rotateBPP8(scene_field1_forest2_sce10_sce.get(), rng, 0x12C, 0x35928, 0x68128, 0x9B12C, 0xD0130);
	Tim::rotateBPP8(scene_field1_forest2_sce11_sce.get(), rng, 0x11C);
	Tim::rotateBPP8(scene_field1_forest2_sce12_sce.get(), rng, 0x120, 0x24924, 0x3E124, 0x57924);
	Tim::rotateBPP8(scene_field1_forest2_sce13_sce.get(), rng, 0x128, 0x20920, 0x4292C, 0x71920);
	Tim::rotateBPP8(scene_field1_forest2_sce14_sce.get(), rng, 0x134);
	Tim::rotateBPP8(scene_field1_forest2_sce17_sce.get(), rng, 0x130);

	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFIELD3_EFD).get(), rng, 0x3E7C, 0x809C, 0xC31C, 0xD73C, 0xEB5C, 0x10B7C, 0x1359C, 0x15FBC);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFIELD4_EFD).get(), rng, 0x40FC, 0x831C, 0xC59C, 0xEFBC, 0x10FDC, 0x125FC, 0x13A1C, 0x15A3C, 0x17A5C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFIELD5_EFD).get(), rng, 0x39E0, 0x7C00, 0xBE80, 0xE4A0, 0xFFC0, 0x121E0, 0x14400, 0x16420, 0x17240);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFIELD6_EFD).get(), rng, 0x3D6C, 0x7F8C, 0xC1AC, 0xE7CC, 0x111EC, 0x1340C, 0x14F2C, 0x15D4C, 0x17F6C);

	Tim::rotateBPP8(scene_field1_gate_sce00_sce.get(), rng, 0xB011C);
	Tim::rotateBPP8(scene_field1_gate_sce06_sce.get(), rng, 0xB011C);

	Tim::rotateBPP16(m_game->file(File::WHOLE_PARELL_TOMBO_TIY).get(), rng, 8, sizeLimitBPP16);
	Tim::rotateBPP16(whole_parell_reduct2_tiy.get(), rng, 0x24, sizeLimitBPP16);
	Tim::rotateBPP16(whole_parell_reduct3_tiy.get(), rng, 0x2C, sizeLimitBPP16);
	Tim::rotateBPP16(whole_parell_reduct4_tiy.get(), rng, 0x5FFC, sizeLimitBPP16);

	const auto rotationShadeForest2{ (rng + 300) % JCUtility::clutRotationLimit };

	rotateShade(scene_field1_forest2_sce00_sce.get(), rotationShadeForest2, 0x30, 0x1D030, 0x40830);
	rotateShade(scene_field1_forest2_sce01_sce.get(), rotationShadeForest2, 0x30, 0x29830, 0x44030);
	rotateShade(scene_field1_forest2_sce02_sce.get(), rotationShadeForest2, 0x30, 0x2B030);
	rotateShade(scene_field1_forest2_sce03_sce.get(), rotationShadeForest2, 0x30, 0x24830);
	rotateShade(scene_field1_forest2_sce04_sce.get(), rotationShadeForest2, 0x30, 0x26830, 0x4F030);
	rotateShade(scene_field1_forest2_sce05_sce.get(), rotationShadeForest2, 0x30, 0x20830, 0x4A030, 0x67030, 0x84830, 0xA2030, 0xBF830);
	rotateShade(scene_field1_forest2_sce06_sce.get(), rotationShadeForest2, 0x30, 0x25830, 0x47830);
	rotateShade.operator()<0>(scene_field1_forest2_sce07_sce.get(), rotationShadeForest2, 0x30, 0x19830, 0x38830, 0x4D830);
	rotateShade(scene_field1_forest2_sce08_sce.get(), rotationShadeForest2, 0x30);
	rotateShade.operator()<-1, 140>(scene_field1_forest2_sce09_sce.get(), rotationShadeForest2, 0x30);
	rotateShade(scene_field1_forest2_sce10_sce.get(), rotationShadeForest2, 0x30, 0x35830, 0x68030, 0x9B030, 0xD0030);
	rotateShade(scene_field1_forest2_sce11_sce.get(), rotationShadeForest2, 0x30);
	rotateShade(scene_field1_forest2_sce12_sce.get(), rotationShadeForest2, 0x30, 0x24830, 0x3E030, 0x57830);
	rotateShade(scene_field1_forest2_sce13_sce.get(), rotationShadeForest2, 0x30, 0x20830, 0x42830, 0x71830);
	rotateShade(scene_field1_forest2_sce14_sce.get(), rotationShadeForest2, 0x30);
	rotateShade.operator()<-1, 90>(scene_field1_forest2_sce17_sce.get(), rotationShadeForest2, 0x30);

	rotateShade(scene_field1_gate_sce00_sce.get(), rotationShadeForest2, 0xB0030);
	rotateShade(scene_field1_gate_sce06_sce.get(), rotationShadeForest2, 0xB0030);

	// Forest 3
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	const auto
		scene_field1_forest3_sce00_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE00_SCE) },
		scene_field1_forest3_sce01_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE01_SCE) },
		scene_field1_forest3_sce02_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE02_SCE) },
		scene_field1_forest3_sce03_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE03_SCE) },
		scene_field1_forest3_sce04_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE04_SCE) },
		scene_field1_forest3_sce05_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE05_SCE) },
		scene_field1_forest3_sce06_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE06_SCE) },
		scene_field1_forest3_sce07_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE07_SCE) },
		scene_field1_forest3_sce08_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE08_SCE) },
		scene_field1_forest3_sce09_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE09_SCE) },
		scene_field1_forest3_sce10_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE10_SCE) },
		scene_field1_forest3_sce11_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE11_SCE) },
		scene_field1_forest3_sce12_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE12_SCE) },
		scene_field1_forest3_sce13_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE13_SCE) },
		scene_field1_forest3_sce14_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE14_SCE) },
		scene_field1_forest3_sce15_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE15_SCE) },
		scene_field1_forest3_sce16_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE16_SCE) },
		scene_field1_forest3_sce17_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE17_SCE) },
		scene_field1_forest3_sce18_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE18_SCE) },
		scene_field1_forest3_sce19_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE19_SCE) },
		scene_field1_forest3_sce20_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE20_SCE) },
		scene_field1_forest3_sce21_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE21_SCE) },
		scene_field1_forest3_sce22_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE22_SCE) },
		scene_field1_forest3_sce23_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE23_SCE) },
		scene_field1_forest3_sce24_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE24_SCE) },
		scene_field1_forest3_sce25_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE25_SCE) },
		scene_field1_forest3_sce26_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE26_SCE) },
		scene_field1_forest3_sce27_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE27_SCE) },
		scene_field1_forest3_sce28_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE28_SCE) },
		scene_field1_forest3_sce29_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE29_SCE) },
		scene_field1_forest3_sce30_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE30_SCE) },
		scene_field1_forest3_sce31_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE31_SCE) },
		scene_field1_forest3_sce32_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE32_SCE) },
		scene_field1_forest3_sce33_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE33_SCE) },
		scene_field1_forest3_sce34_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE34_SCE) },
		scene_field1_forest3_sce35_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE35_SCE) },
		scene_field1_forest3_sce36_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE36_SCE) },
		scene_field1_forest3_sce37_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE37_SCE) },
		scene_field1_forest3_sce38_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE38_SCE) },
		scene_field1_forest3_sce39_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE39_SCE) },
		scene_field1_forest3_sce40_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE40_SCE) },
		scene_field1_forest3_sce41_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE41_SCE) },
		scene_field1_forest3_sce42_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE42_SCE) },
		scene_field1_forest3_sce43_sce{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE43_SCE) };

	Tim::rotateBPP8(scene_field1_forest3_sce00_sce.get(), rng, 0x12C, 0x27928);
	Tim::rotateBPP8(scene_field1_forest3_sce01_sce.get(), rng, 0x120, 0x1D924, 0x35928);
	Tim::rotateBPP8(scene_field1_forest3_sce02_sce.get(), rng, 0x124, 0x19124, 0x43920);
	Tim::rotateBPP8(scene_field1_forest3_sce03_sce.get(), rng, 0x128, 0x3392C);
	Tim::rotateBPP8(scene_field1_forest3_sce04_sce.get(), rng, 0x128, 0x2D128);
	Tim::rotateBPP8(scene_field1_forest3_sce05_sce.get(), rng, 0x124, 0x1F920);
	Tim::rotateBPP8(scene_field1_forest3_sce06_sce.get(), rng, 0x12C, 0x28120, 0x4A12C);
	Tim::rotateBPP8(scene_field1_forest3_sce07_sce.get(), rng, 0x124, 0x2A924);
	Tim::rotateBPP8(scene_field1_forest3_sce08_sce.get(), rng, 0x134);
	Tim::rotateBPP8(scene_field1_forest3_sce09_sce.get(), rng, 0x124, 0x24124, 0x48128);
	Tim::rotateBPP8(scene_field1_forest3_sce10_sce.get(), rng, 0x11C);
	Tim::rotateBPP8(scene_field1_forest3_sce11_sce.get(), rng, 0x130, 0x32124, 0x5D920);
	Tim::rotateBPP8(scene_field1_forest3_sce12_sce.get(), rng, 0x124, 0x2A124);
	Tim::rotateBPP8(scene_field1_forest3_sce13_sce.get(), rng, 0x120, 0x2A120);
	Tim::rotateBPP8(scene_field1_forest3_sce14_sce.get(), rng, 0x130, 0x31120);
	Tim::rotateBPP8(scene_field1_forest3_sce15_sce.get(), rng, 0x130, 0x37130);
	Tim::rotateBPP8(scene_field1_forest3_sce16_sce.get(), rng, 0x120, 0x1C934, 0x55920);
	Tim::rotateBPP8(scene_field1_forest3_sce17_sce.get(), rng, 0x120, 0x1D920, 0x3B928, 0x64120);
	Tim::rotateBPP8(scene_field1_forest3_sce18_sce.get(), rng, 0x11C, 0x1B92C, 0x44920);
	Tim::rotateBPP8(scene_field1_forest3_sce19_sce.get(), rng, 0x128, 0x39130, 0x70928);
	Tim::rotateBPP8(scene_field1_forest3_sce20_sce.get(), rng, 0x120, 0x15918, 0x2711C, 0x40120, 0x4F918, 0x6111C, 0x7A120, 0x89920, 0x99120, 0xAE918, 0xC0120);
	Tim::rotateBPP8(scene_field1_forest3_sce21_sce.get(), rng, 0x12C, 0x28128);
	Tim::rotateBPP8(scene_field1_forest3_sce22_sce.get(), rng, 0x120, 0x1D124, 0x35128);
	Tim::rotateBPP8(scene_field1_forest3_sce23_sce.get(), rng, 0x124, 0x19924, 0x44920);
	Tim::rotateBPP8(scene_field1_forest3_sce24_sce.get(), rng, 0x12C, 0x3612C);
	Tim::rotateBPP8(scene_field1_forest3_sce25_sce.get(), rng, 0x12C, 0x2E128);
	Tim::rotateBPP8(scene_field1_forest3_sce26_sce.get(), rng, 0x124, 0x1F924);
	Tim::rotateBPP8(scene_field1_forest3_sce27_sce.get(), rng, 0x12C, 0x28920, 0x4B12C);
	Tim::rotateBPP8(scene_field1_forest3_sce28_sce.get(), rng, 0x124, 0x2A924);
	Tim::rotateBPP8(scene_field1_forest3_sce29_sce.get(), rng, 0x134);
	Tim::rotateBPP8(scene_field1_forest3_sce30_sce.get(), rng, 0x124, 0x24124, 0x48928);
	Tim::rotateBPP8(scene_field1_forest3_sce31_sce.get(), rng, 0x11C);
	Tim::rotateBPP8(scene_field1_forest3_sce32_sce.get(), rng, 0x128, 0x34124, 0x5F92C);
	Tim::rotateBPP8(scene_field1_forest3_sce33_sce.get(), rng, 0x124, 0x2A124);
	Tim::rotateBPP8(scene_field1_forest3_sce34_sce.get(), rng, 0x12C, 0x2F124);
	Tim::rotateBPP8(scene_field1_forest3_sce35_sce.get(), rng, 0x130, 0x31120);
	Tim::rotateBPP8(scene_field1_forest3_sce36_sce.get(), rng, 0x130, 0x37930);
	Tim::rotateBPP8(scene_field1_forest3_sce37_sce.get(), rng, 0x124, 0x18128, 0x4A11C);
	Tim::rotateBPP8(scene_field1_forest3_sce38_sce.get(), rng, 0x120, 0x1D920, 0x3B928, 0x64120);
	Tim::rotateBPP8(scene_field1_forest3_sce39_sce.get(), rng, 0x120, 0x1C12C, 0x45120);
	Tim::rotateBPP8(scene_field1_forest3_sce40_sce.get(), rng, 0x128, 0x39130, 0x70928);
	Tim::rotateBPP8(scene_field1_forest3_sce41_sce.get(), rng, 0x124);
	Tim::rotateBPP8(scene_field1_forest3_sce42_sce.get(), rng, 0x128);
	Tim::rotateBPP8(scene_field1_forest3_sce43_sce.get(), rng, 0x128);

	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFIELD7_EFD).get(), rng, 0x3CBC, 0x7EDC, 0xC15C, 0xEB7C, 0x1161C, 0x137BC, 0x15CDC, 0x1757C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFIELD8_EFD).get(), rng, 0x403C, 0x825C, 0xAC7C, 0xD71C, 0xF8BC, 0x11DDC, 0x1367C, 0x14C1C, 0x1723C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD09_EFD).get(), rng, 0x3CC0, 0x7EE0, 0xC160, 0xEB80, 0x11620, 0x137C0, 0x15CE0, 0x17580);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD10_EFD).get(), rng, 0x4A40, 0x8C60, 0xCEE0, 0xF900, 0x123A0, 0x14540, 0x16A60, 0x18300);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD07X_EFD).get(), rng, 0x3E4C, 0x806C, 0xC2EC, 0xED0C, 0x117AC, 0x1394C, 0x15E6C, 0x1770C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD08X_EFD).get(), rng, 0x403C, 0x825C, 0xAC7C, 0xD71C, 0xF8BC, 0x11DDC, 0x1367C, 0x14C1C, 0x1723C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD09X_EFD).get(), rng, 0x3CBC, 0x7EDC, 0xC15C, 0xEB7C, 0x1161C, 0x137BC, 0x15CDC, 0x1757C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD10X_EFD).get(), rng, 0x4A40, 0x8C60, 0xCEE0, 0xF900, 0x123A0, 0x14540, 0x16A60, 0x18300);

	Tim::rotateBPP8(scene_field1_gate_sce00_sce.get(), rng, 0x9591C);
	Tim::rotateBPP8(scene_field1_gate_sce06_sce.get(), rng, 0x9591C);

	Tim::rotateBPP16(m_game->file(File::WHOLE_PARELL_KUMO_TIY).get(), rng, 8, sizeLimitBPP16);
	Tim::rotateBPP16(whole_parell_reduct3_tiy.get(), rng, 0x3010, sizeLimitBPP16);
	Tim::rotateBPP16(whole_parell_reduct4_tiy.get(), rng, 0x8FE0, sizeLimitBPP16);

	static constexpr std::array<u8, 3> rgbShadeAddForest3_2{ 45, 0, 0 };
	static constexpr std::array<u16, 3> rgbLightAddForest3_2{ 0, 0, 1146 };

	addRGBShadeAndLight(scene_field1_gate_sce06_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x95830);
	addRGBShadeAndLight(scene_field1_forest3_sce21_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x28030);
	addRGBShadeAndLight(scene_field1_forest3_sce22_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x1D030, 0x35030);
	addRGBShadeAndLight(scene_field1_forest3_sce23_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x19830, 0x44830);
	addRGBShadeAndLight(scene_field1_forest3_sce24_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x36030);
	addRGBShadeAndLight(scene_field1_forest3_sce25_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x2E030);
	addRGBShadeAndLight(scene_field1_forest3_sce26_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x1F830);
	addRGBShadeAndLight(scene_field1_forest3_sce27_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x28830, 0x4B030);
	addRGBShadeAndLight(scene_field1_forest3_sce28_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x2A830);
	addRGBShadeAndLight(scene_field1_forest3_sce29_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30);
	addRGBShadeAndLight(scene_field1_forest3_sce30_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x24030, 0x48830);
	addRGBShadeAndLight(scene_field1_forest3_sce31_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30);
	addRGBShadeAndLight(scene_field1_forest3_sce32_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x34030, 0x5F830);
	addRGBShadeAndLight(scene_field1_forest3_sce34_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x2F030);
	addRGBShadeAndLight(scene_field1_forest3_sce35_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x31030);
	addRGBShadeAndLight(scene_field1_forest3_sce36_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x37830);
	addRGBShadeAndLight(scene_field1_forest3_sce37_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x18030, 0x4A030);
	addRGBShadeAndLight(scene_field1_forest3_sce38_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x1D830, 0x3B830, 0x64030);
	addRGBShadeAndLight(scene_field1_forest3_sce39_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x1C030, 0x45030);
	addRGBShadeAndLight(scene_field1_forest3_sce40_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30, 0x39030, 0x70830);
	addRGBShadeAndLight(scene_field1_forest3_sce43_sce.get(), rgbShadeAddForest3_2, rgbLightAddForest3_2, 0x30);

	const auto
		rotationShadeForest3{ (rng + 330) % JCUtility::clutRotationLimit },
		rotationLightForest3_2{ (rng + 345) % JCUtility::clutRotationLimit };

	rotateShade.operator()<50, 325>(scene_field1_forest3_sce00_sce.get(), rotationShadeForest3, 0x30, 0x27830);
	rotateShade.operator()<50, 325, 325>(scene_field1_forest3_sce01_sce.get(), rotationShadeForest3, 0x30, 0x1D830, 0x35830);
	rotateShade.operator()<50, 325>(scene_field1_forest3_sce02_sce.get(), rotationShadeForest3, 0x30, 0x19030, 0x43830);
	rotateShade.operator()<50, 325>(scene_field1_forest3_sce03_sce.get(), rotationShadeForest3, 0x30, 0x33830);
	rotateShade.operator()<50, 325, 325>(scene_field1_forest3_sce04_sce.get(), rotationShadeForest3, 0x30, 0x2D030);
	rotateShade.operator()<50, 325>(scene_field1_forest3_sce05_sce.get(), rotationShadeForest3, 0x30, 0x1F830);
	rotateShade.operator()<50, 325, 325>(scene_field1_forest3_sce06_sce.get(), rotationShadeForest3, 0x30, 0x28030, 0x4A030);
	rotateShade.operator()<50, 325>(scene_field1_forest3_sce07_sce.get(), rotationShadeForest3, 0x30, 0x2A830);
	rotateShade.operator()<50, 325>(scene_field1_forest3_sce08_sce.get(), rotationShadeForest3, 0x30);
	rotateShade.operator()<50, 325>(scene_field1_forest3_sce09_sce.get(), rotationShadeForest3, 0x30, 0x24030, 0x48030);
	rotateShade.operator()<50, 325>(scene_field1_forest3_sce10_sce.get(), rotationShadeForest3, 0x30);
	rotateShade.operator()<210>(scene_field1_forest3_sce11_sce.get(), (rng + 180) % JCUtility::clutRotationLimit, 0x30, 0x32030, 0x5D830);
	rotateLightSpot.operator()<15>(scene_field1_forest3_sce11_sce.get(), rotationShadeForest3, 0x50);
	rotateLightSpot.operator()<325>(scene_field1_forest3_sce11_sce.get(), rotationShadeForest3, 0x32050, 0x5D850);
	rotateShade.operator()<50, 325, 325>(scene_field1_forest3_sce12_sce.get(), rotationShadeForest3, 0x30, 0x2A030);
	rotateShade.operator()<190, 325>(scene_field1_forest3_sce13_sce.get(), (rng + 190) % JCUtility::clutRotationLimit, 0x30, 0x2A030);
	rotateShade.operator()<190, 325>(scene_field1_forest3_sce14_sce.get(), (rng + 190) % JCUtility::clutRotationLimit, 0x30, 0x31030);
	rotateShade.operator()<-1, 40>(scene_field1_forest3_sce15_sce.get(), rotationShadeForest3, 0x30);
	rotateShade.operator()<-1, 60>(scene_field1_forest3_sce15_sce.get(), (rng + 120) % JCUtility::clutRotationLimit, 0x37030);
	rotateShade.operator()<210, 0, 0>(scene_field1_forest3_sce16_sce.get(), rotationShadeForest3, 0x30, 0x1C830, 0x55830);
	rotateShade.operator()<210, 30, 30>(scene_field1_forest3_sce17_sce.get(), rotationShadeForest3, 0x30, 0x1D830, 0x3B830, 0x64030);
	rotateShade.operator()<80, 85>(scene_field1_forest3_sce18_sce.get(), (rng + 160) % JCUtility::clutRotationLimit, 0x30, 0x1B830, 0x44830);
	rotateShade.operator()<0, 160>(scene_field1_forest3_sce19_sce.get(), (rng + 160) % JCUtility::clutRotationLimit, 0x30, 0x39030, 0x70830);
	rotateShade.operator()<350, 30>(scene_field1_forest3_sce20_sce.get(), (rng + 60) % JCUtility::clutRotationLimit, 0x30, 0x15830, 0x27030, 0x40030, 0x4F830, 0x61030, 0x7A030);
	rotateShade.operator()<260, 0>(scene_field1_forest3_sce20_sce.get(), rotationShadeForest3, 0x89830, 0x99030, 0xAE830, 0xC0030);
	rotateLight.operator()<310>(scene_field1_forest3_sce21_sce.get(), rotationLightForest3_2, 0x3C, 0x2803C);
	rotateLight.operator()<60, 60>(scene_field1_forest3_sce22_sce.get(), rotationLightForest3_2, 0x3C, 0x1D03C, 0x3503C);
	rotateLight.operator()<60>(scene_field1_forest3_sce23_sce.get(), rotationLightForest3_2, 0x3C, 0x4483C);
	rotateLight.operator()<310>(scene_field1_forest3_sce23_sce.get(), (rng + 90) % JCUtility::clutRotationLimit, 0x1983C);
	rotateLight.operator()<30>(scene_field1_forest3_sce24_sce.get(), rotationLightForest3_2, 0x3C, 0x3603C);
	rotateLight.operator()<60, 60>(scene_field1_forest3_sce25_sce.get(), rotationLightForest3_2, 0x3C, 0x2E03C);
	rotateLight.operator()<0>(scene_field1_forest3_sce26_sce.get(), rotationLightForest3_2, 0x3C, 0x1F83C);
	rotateLight.operator()<30, 30>(scene_field1_forest3_sce27_sce.get(), rotationLightForest3_2, 0x3C, 0x2883C, 0x4B03C);
	rotateLight.operator()<60>(scene_field1_forest3_sce28_sce.get(), rotationLightForest3_2, 0x3C, 0x2A83C);
	rotateLight.operator()<60>(scene_field1_forest3_sce29_sce.get(), rotationLightForest3_2, 0x3C);
	rotateLight.operator()<40>(scene_field1_forest3_sce30_sce.get(), rotationLightForest3_2, 0x3C, 0x2403C, 0x4883C);
	rotateLight.operator()<70>(scene_field1_forest3_sce31_sce.get(), rotationLightForest3_2, 0x3C);
	rotateLight.operator()<0>(scene_field1_forest3_sce32_sce.get(), rotationLightForest3_2, 0x3C, 0x3403C, 0x5F83C);
	rotateShade.operator()<40, 290, 290>(scene_field1_forest3_sce33_sce.get(), (rng + 60) % JCUtility::clutRotationLimit, 0x30, 0x2A030);
	rotateLight.operator()<60>(scene_field1_forest3_sce34_sce.get(), rotationLightForest3_2, 0x3C, 0x2F03C);
	rotateLight.operator()<60>(scene_field1_forest3_sce35_sce.get(), rotationLightForest3_2, 0x3C, 0x3103C);
	rotateLight.operator()<60>(scene_field1_forest3_sce36_sce.get(), rotationLightForest3_2, 0x3C, 0x3783C);
	rotateLight.operator()<20, 20>(scene_field1_forest3_sce37_sce.get(), rotationLightForest3_2, 0x3C, 0x1803C, 0x4A03C);
	rotateLight.operator()<60>(scene_field1_forest3_sce38_sce.get(), rotationLightForest3_2, 0x3C, 0x1D83C, 0x3B83C, 0x6403C);
	rotateLight.operator()<50>(scene_field1_forest3_sce39_sce.get(), rotationLightForest3_2, 0x3C, 0x1C03C, 0x4503C);
	rotateLight.operator()<60>(scene_field1_forest3_sce40_sce.get(), rotationLightForest3_2, 0x3C, 0x3903C, 0x7083C);
	rotateShade.operator()<340, 30>(scene_field1_forest3_sce41_sce.get(), (rng + 260) % JCUtility::clutRotationLimit, 0x30);
	rotateShade.operator()<50, 210>(scene_field1_forest3_sce42_sce.get(), rotationShadeForest3, 0x30);
	rotateLight(scene_field1_forest3_sce43_sce.get(), rotationLightForest3_2, 0x3C);

	rotateShade.operator()<50>(scene_field1_gate_sce00_sce.get(), rotationShadeForest3, 0x95830);
	rotateLight(scene_field1_gate_sce06_sce.get(), rotationLightForest3_2, 0x9583C);

	// Forest 4
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	const auto
		scene_field1_forest4_sce00_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE00_SCE) },
		scene_field1_forest4_sce01_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE01_SCE) },
		scene_field1_forest4_sce02_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE02_SCE) },
		scene_field1_forest4_sce03_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE03_SCE) },
		scene_field1_forest4_sce04_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE04_SCE) },
		scene_field1_forest4_sce05_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE05_SCE) },
		scene_field1_forest4_sce06_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE06_SCE) },
		scene_field1_forest4_sce07_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE07_SCE) },
		scene_field1_forest4_sce08_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE08_SCE) },
		scene_field1_forest4_sce09_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE09_SCE) },
		scene_field1_forest4_sce10_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE10_SCE) },
		scene_field1_forest4_sce11_sce{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE11_SCE) };

	Tim::rotateBPP8(scene_field1_forest4_sce00_sce.get(), rng, 0x128, 0x2791C);
	Tim::rotateBPP8(scene_field1_forest4_sce01_sce.get(), rng, 0x128, 0x22920, 0x46124, 0x60928);
	Tim::rotateBPP8(scene_field1_forest4_sce02_sce.get(), rng, 0x130, 0x37920);
	Tim::rotateBPP8(scene_field1_forest4_sce03_sce.get(), rng, 0x12C, 0x2A124, 0x4B124, 0x65920);
	Tim::rotateBPP8(scene_field1_forest4_sce04_sce.get(), rng, 0x11C, 0x1B124);
	Tim::rotateBPP8(scene_field1_forest4_sce05_sce.get(), rng, 0x12C);
	Tim::rotateBPP8(scene_field1_forest4_sce06_sce.get(), rng, 0x12C, 0x35924);
	Tim::rotateBPP8(scene_field1_forest4_sce07_sce.get(), rng, 0x130, 0x30F30, 0x36920, 0x51F5C, 0x57924, 0x7BA44);
	Tim::rotateBPP8(scene_field1_forest4_sce08_sce.get(), rng, 0x134, 0x38928, 0x6C130);
	Tim::rotateBPP8(scene_field1_forest4_sce09_sce.get(), rng, 0x11C, 0x1B124, 0x32920, 0x5612C, 0x8B128, 0xB9120, 0xDB128, 0xFC118);
	Tim::rotateBPP8(scene_field1_forest4_sce10_sce.get(), rng, 0x124, 0x1E11C, 0x38920, 0x55124);
	Tim::rotateBPP8(scene_field1_forest4_sce11_sce.get(), rng, 0x124, 0x2B124);
	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST4_SCE12_SCE).get(), rng, 0x11C, 0x1491C, 0x2811C, 0x35120, 0x4491C, 0x5911C, 0x6111C, 0x7591C, 0x89120, 0x98918, 0x9F11C, 0xA6918, 0xB291C, 0xBF920);

	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD20_EFD).get(), rng, 0x3EBC, 0x98DC, 0xDAFC, 0xF51C, 0x10F3C, 0x1395C, 0x15F7C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD21_EFD).get(), rng, 0x3D6C, 0x7F8C, 0xC1AC, 0xD3CC, 0xEDEC, 0x1080C, 0x13A2C, 0x1604C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD22_EFD).get(), rng, 0x3D1C, 0x973C, 0xD95C, 0xF37C, 0x10D9C, 0x137BC, 0x15DDC);

	Tim::rotateBPP8(scene_field1_gate_sce00_sce.get(), rng, 0xC911C);
	Tim::rotateBPP8(scene_field1_gate_sce06_sce.get(), rng, 0xC911C);

	const auto ktGateOffset{ m_game->offset().file.scene_field1_forest4_sce12_sbh.kemuelTempleGate };

	Tim::rotateBPP8(m_game->file(File::SCENE_FIELD1_FOREST4_SCE12_SBH).get(), rng,
		ktGateOffset,
		ktGateOffset + 0x5C20,
		ktGateOffset + 0x7E40,
		ktGateOffset + 0xAC60,
		ktGateOffset + 0xB280);

	Tim::rotateBPP16(m_game->file(File::WHOLE_PARELL_GA_TIY).get(), rng, 8, sizeLimitBPP16);
	Tim::rotateBPP16(whole_parell_reduct4_tiy.get(), rng, 0xBFC4, sizeLimitBPP16);

	rotateLight(scene_field1_forest4_sce00_sce.get(), rng, 0x3C, 0x2783C);
	rotateLight.operator()<0, 0, 0>(scene_field1_forest4_sce01_sce.get(), rng, 0x3C, 0x2283C, 0x4603C, 0x6083C);
	rotateLight(scene_field1_forest4_sce02_sce.get(), rng, 0x3C, 0x3783C);
	rotateLight.operator()<0>(scene_field1_forest4_sce03_sce.get(), rng, 0x3C, 0x2A03C, 0x4B03C, 0x6583C);
	rotateLight.operator()<0>(scene_field1_forest4_sce04_sce.get(), rng, 0x3C, 0x1B03C);
	rotateLight(scene_field1_forest4_sce05_sce.get(), rng, 0x3C);
	rotateLight.operator()<330>(scene_field1_forest4_sce06_sce.get(), rng, 0x3C, 0x3583C);
	rotateLight(scene_field1_forest4_sce07_sce.get(), rng, 0x3C, 0x3683C, 0x5783C);
	rotateLight(scene_field1_forest4_sce08_sce.get(), rng, 0x3C, 0x3883C, 0x6C03C);
	rotateLight.operator()<180>(scene_field1_forest4_sce09_sce.get(), rng, 0x3C, 0x1B03C, 0x3283C, 0x5603C, 0x8B03C, 0xB903C, 0xDB03C, 0xFC03C);
	rotateLight(scene_field1_forest4_sce10_sce.get(), rng, 0x3C, 0x1E03C, 0x3883C, 0x5503C);
	rotateLight(scene_field1_forest4_sce11_sce.get(), rng, 0x3C, 0x2B03C);

	rotateLight(scene_field1_gate_sce00_sce.get(), rng, 0xC903C);
	rotateLight(scene_field1_gate_sce06_sce.get(), rng, 0xC903C);

	// Forest 1-2
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	const auto
		scene_field2_forest1_sce00_sce{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE00_SCE) },
		scene_field2_forest1_sce01_sce{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE01_SCE) },
		scene_field2_forest1_sce02_sce{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE02_SCE) },
		scene_field2_forest1_sce03_sce{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE03_SCE) },
		scene_field2_forest1_sce04_sce{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE04_SCE) },
		scene_field2_forest1_sce05_sce{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE05_SCE) },
		scene_field2_forest1_sce06_sce{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE06_SCE) },
		scene_field2_forest1_sce07_sce{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE07_SCE) },
		scene_field2_forest1_sce08_sce{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE08_SCE) },
		scene_field2_forest1_sce09_sce{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE09_SCE) },
		scene_field2_forest1_sce10_sce{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE10_SCE) };

	Tim::rotateBPP8(scene_field2_forest1_sce00_sce.get(), rng, 0x130, 0x3111C, 0x4C124);
	Tim::rotateBPP8(scene_field2_forest1_sce01_sce.get(), rng, 0x12C, 0x1CFCC, 0x24924, 0x4E924, 0x72928, 0x92920);
	Tim::rotateBPP8(scene_field2_forest1_sce02_sce.get(), rng, 0x12C, 0x2A128, 0x4C928, 0x6F11C);
	Tim::rotateBPP8(scene_field2_forest1_sce03_sce.get(), rng, 0x128, 0x20920, 0x3C920, 0x5A924, 0x7811C, 0x8D924);
	Tim::rotateBPP8(scene_field2_forest1_sce04_sce.get(), rng, 0x12C, 0x2F130, 0x67124);
	Tim::rotateBPP8(scene_field2_forest1_sce05_sce.get(), rng, 0x128, 0x2612C, 0x55128);
	Tim::rotateBPP8(scene_field2_forest1_sce06_sce.get(), rng, 0x124, 0x26120, 0x49928, 0x6A91C);
	Tim::rotateBPP8(scene_field2_forest1_sce07_sce.get(), rng, 0x134, 0x39120, 0x56124);
	Tim::rotateBPP8(scene_field2_forest1_sce08_sce.get(), rng, 0x124, 0x18924);
	Tim::rotateBPP8(scene_field2_forest1_sce09_sce.get(), rng, 0x124, 0x2412C, 0x52930);
	Tim::rotateBPP8(scene_field2_forest1_sce10_sce.get(), rng, 0x128);

	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFIELDZ_EFD).get(), rng, 0x4054, 0x8274, 0xA494, 0xB8B4, 0xE2D4, 0x10CF4, 0x12F14, 0x13D34, 0x15F54);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD2AZ_EFD).get(), rng, 0x3C9C, 0x7EBC, 0x90DC, 0xB0FC, 0xD11C, 0xE53C, 0x1075C, 0x11B7C, 0x13D9C, 0x15FBC);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD2BZ_EFD).get(), rng, 0x3F74, 0x8194, 0x93B4, 0xB3D4, 0xD3F4, 0xE814, 0x10A34, 0x11E54, 0x14074, 0x16294);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD15_EFD).get(), rng, 0x415C, 0x837C, 0x101A4, 0x115C4, 0x129E4, 0x13804, 0x16224);

	rotateLight(scene_field2_forest1_sce00_sce.get(), rng, 0x3C, 0x3103C, 0x4C03C);
	rotateLight(scene_field2_forest1_sce01_sce.get(), rng, 0x3C, 0x2483C, 0x4E83C, 0x7283C, 0x9283C);
	rotateLight(scene_field2_forest1_sce02_sce.get(), rng, 0x3C, 0x2A03C, 0x4C83C, 0x6F03C);
	rotateLight(scene_field2_forest1_sce03_sce.get(), rng, 0x3C, 0x2083C, 0x3C83C, 0x5A83C, 0x7803C, 0x8D83C);
	rotateLight.operator()<0>(scene_field2_forest1_sce04_sce.get(), rng, 0x3C, 0x2F03C, 0x6703C);
	rotateLight(scene_field2_forest1_sce05_sce.get(), rng, 0x3C, 0x2603C, 0x5503C);
	rotateLight(scene_field2_forest1_sce06_sce.get(), rng, 0x3C, 0x2603C, 0x4983C, 0x6A83C);
	rotateLight(scene_field2_forest1_sce07_sce.get(), rng, 0x3C, 0x3903C, 0x5603C);
	rotateLight(scene_field2_forest1_sce08_sce.get(), rng, 0x3C, 0x1883C);
	rotateLight(scene_field2_forest1_sce09_sce.get(), rng, 0x3C, 0x2403C, 0x5283C);
	rotateLight(scene_field2_forest1_sce10_sce.get(), rng, 0x3C);

	// Forest 2-2
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	const auto
		scene_field2_forest2_sce00_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE00_SCE) },
		scene_field2_forest2_sce01_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE01_SCE) },
		scene_field2_forest2_sce02_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE02_SCE) },
		scene_field2_forest2_sce03_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE03_SCE) },
		scene_field2_forest2_sce04_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE04_SCE) },
		scene_field2_forest2_sce05_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE05_SCE) },
		scene_field2_forest2_sce06_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE06_SCE) },
		scene_field2_forest2_sce07_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE07_SCE) },
		scene_field2_forest2_sce08_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE08_SCE) },
		scene_field2_forest2_sce09_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE09_SCE) },
		scene_field2_forest2_sce10_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE10_SCE) },
		scene_field2_forest2_sce11_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE11_SCE) },
		scene_field2_forest2_sce12_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE12_SCE) },
		scene_field2_forest2_sce13_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE13_SCE) },
		scene_field2_forest2_sce14_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE14_SCE) },
		scene_field2_forest2_sce17_sce{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE17_SCE) };

	Tim::rotateBPP8(scene_field2_forest2_sce00_sce.get(), rng, 0x120, 0x1D924, 0x41924);
	Tim::rotateBPP8(scene_field2_forest2_sce01_sce.get(), rng, 0x12C, 0x2891C, 0x42924);
	Tim::rotateBPP8(scene_field2_forest2_sce02_sce.get(), rng, 0x128, 0x2B120);
	Tim::rotateBPP8(scene_field2_forest2_sce03_sce.get(), rng, 0x124, 0x2492C);
	Tim::rotateBPP8(scene_field2_forest2_sce04_sce.get(), rng, 0x124, 0x25928, 0x4D128);
	Tim::rotateBPP8(scene_field2_forest2_sce05_sce.get(), rng, 0x124, 0x20928, 0x4A120, 0x67120, 0x84920, 0xA2120, 0xBF924);
	Tim::rotateBPP8(scene_field2_forest2_sce06_sce.get(), rng, 0x128, 0x22120, 0x40120);
	Tim::rotateBPP8(scene_field2_forest2_sce07_sce.get(), rng, 0x11C, 0x19124, 0x3791C, 0x4C11C);
	Tim::rotateBPP8(scene_field2_forest2_sce08_sce.get(), rng, 0x11C);
	Tim::rotateBPP8(scene_field2_forest2_sce09_sce.get(), rng, 0x124);
	Tim::rotateBPP8(scene_field2_forest2_sce10_sce.get(), rng, 0x12C, 0x34930, 0x6592C, 0x95930, 0xCA630, 0xCD128);
	Tim::rotateBPP8(scene_field2_forest2_sce11_sce.get(), rng, 0x11C);
	Tim::rotateBPP8(scene_field2_forest2_sce12_sce.get(), rng, 0x120, 0x24924, 0x3E124, 0x57924);
	Tim::rotateBPP8(scene_field2_forest2_sce13_sce.get(), rng, 0x128, 0x21920, 0x3E12C, 0x6E120);
	Tim::rotateBPP8(scene_field2_forest2_sce14_sce.get(), rng, 0x12C);
	Tim::rotateBPP8(scene_field2_forest2_sce17_sce.get(), rng, 0x130);

	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD03Z_EFD).get(), rng, 0x3E7C, 0x809C, 0xC31C, 0xD73C, 0xEB5C, 0x10B7C, 0x1359C, 0x15FBC);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD04Z_EFD).get(), rng, 0x40FC, 0x831C, 0xC59C, 0xEFBC, 0x10FDC, 0x125FC, 0x13A1C, 0x15A3C, 0x17A5C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD05Z_EFD).get(), rng, 0x39E4, 0x7C04, 0xBE84, 0xE4A4, 0xFFC4, 0x121E4, 0x14404, 0x16424, 0x17244);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD06Z_EFD).get(), rng, 0x3D70, 0x7F90, 0xC1B0, 0xE7D0, 0x111F0, 0x13410, 0x14F30, 0x15D50, 0x17F70);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD16_EFD).get(), rng, 0x3D6C, 0x7F8C, 0xC1AC, 0xE3CC, 0x105EC, 0x1480C);

	static constexpr std::array<u8, 3> rgbShadeAddNForest2{ 0, 17, 0 };
	static constexpr std::array<u16, 3> rgbLightAddNForest2{ 0, 0, 870 };

	addRGBShadeAndLight(scene_field2_forest2_sce00_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30, 0x1D830, 0x41830);
	addRGBShadeAndLight(scene_field2_forest2_sce01_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30, 0x28830, 0x42830);
	addRGBShadeAndLight(scene_field2_forest2_sce02_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30, 0x2B030);
	addRGBShadeAndLight(scene_field2_forest2_sce03_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30, 0x24830);
	addRGB(scene_field2_forest2_sce04_sce.get(), rgbShadeAddNForest2, 0x30, 0x25830, 0x4D030);
	addRGBShadeAndLight(scene_field2_forest2_sce05_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30, 0x20830, 0x4A030, 0x67030, 0x84830, 0xA2030, 0xBF830);
	addRGBShadeAndLight(scene_field2_forest2_sce06_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30, 0x22030, 0x40030);
	addRGB(scene_field2_forest2_sce07_sce.get(), std::array<u8, 3>{ 0, 10, 0 }, 0x30, 0x19030, 0x37830, 0x4C030);
	addRGBShadeAndLight(scene_field2_forest2_sce08_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30);
	addRGBShadeAndLight(scene_field2_forest2_sce09_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30);
	addRGBShadeAndLight(scene_field2_forest2_sce10_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30, 0x34830, 0x65830, 0x95830, 0xCD030);
	addRGBShadeAndLight(scene_field2_forest2_sce11_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30);
	addRGBShadeAndLight(scene_field2_forest2_sce12_sce.get(), rgbShadeAddNForest2, std::array<u16, 3>{ 0, 0, 409 }, 0x30);
	addRGBShadeAndLight(scene_field2_forest2_sce12_sce.get(), rgbShadeAddNForest2, std::array<u16, 3>{ 0, 0, 819 }, 0x24830);
	addRGBShadeAndLight(scene_field2_forest2_sce12_sce.get(), rgbShadeAddNForest2, std::array<u16, 3>{ 0, 0, 512 }, 0x3E030);
	addRGBShadeAndLight(scene_field2_forest2_sce12_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x57830);
	addRGBShadeAndLight(scene_field2_forest2_sce13_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30, 0x21830, 0x3E030, 0x6E030);
	addRGBShadeAndLight(scene_field2_forest2_sce14_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30);
	addRGBShadeAndLight(scene_field2_forest2_sce17_sce.get(), rgbShadeAddNForest2, rgbLightAddNForest2, 0x30);

	rotateLight(scene_field2_forest2_sce00_sce.get(), rng, 0x3C, 0x1D83C, 0x4183C);
	rotateLight(scene_field2_forest2_sce01_sce.get(), rng, 0x3C, 0x2883C, 0x4283C);
	rotateLight(scene_field2_forest2_sce02_sce.get(), rng, 0x3C, 0x2B03C);
	rotateLight(scene_field2_forest2_sce03_sce.get(), rng, 0x3C, 0x2483C);
	rotateLight(scene_field2_forest2_sce04_sce.get(), rng, 0x3C, 0x2583C, 0x4D03C);
	rotateLight(scene_field2_forest2_sce05_sce.get(), rng, 0x3C, 0x2083C, 0x4A03C, 0x6703C, 0x8483C, 0xA203C, 0xBF83C);
	rotateLight(scene_field2_forest2_sce06_sce.get(), rng, 0x3C, 0x2203C, 0x4003C);
	rotateLight(scene_field2_forest2_sce07_sce.get(), rng, 0x3C, 0x1903C, 0x3783C, 0x4C03C);
	rotateLight(scene_field2_forest2_sce08_sce.get(), rng, 0x3C);
	rotateLight.operator()<0>(scene_field2_forest2_sce09_sce.get(), rng, 0x3C);
	rotateLight(scene_field2_forest2_sce10_sce.get(), rng, 0x3C, 0x3483C, 0x6583C, 0x9583C, 0xCD03C);
	rotateLight(scene_field2_forest2_sce11_sce.get(), rng, 0x3C);
	rotateLight.operator()<0, 0>(scene_field2_forest2_sce12_sce.get(), rng, 0x3C, 0x2483C, 0x3E03C, 0x5783C);
	rotateLight(scene_field2_forest2_sce13_sce.get(), rng, 0x3C, 0x2183C, 0x3E03C, 0x6E03C);
	rotateLight(scene_field2_forest2_sce14_sce.get(), rng, 0x3C);
	rotateLight.operator()<0>(scene_field2_forest2_sce17_sce.get(), rng, 0x3C);

	// Forest 3-2
	rng = Random::get().generate(JCUtility::clutRotationLimit);

	const auto
		scene_field2_forest3_sce00_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE00_SCE) },
		scene_field2_forest3_sce01_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE01_SCE) },
		scene_field2_forest3_sce02_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE02_SCE) },
		scene_field2_forest3_sce03_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE03_SCE) },
		scene_field2_forest3_sce04_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE04_SCE) },
		scene_field2_forest3_sce05_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE05_SCE) },
		scene_field2_forest3_sce06_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE06_SCE) },
		scene_field2_forest3_sce07_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE07_SCE) },
		scene_field2_forest3_sce08_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE08_SCE) },
		scene_field2_forest3_sce09_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE09_SCE) },
		scene_field2_forest3_sce10_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE10_SCE) },
		scene_field2_forest3_sce11_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE11_SCE) },
		scene_field2_forest3_sce12_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE12_SCE) },
		scene_field2_forest3_sce13_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE13_SCE) },
		scene_field2_forest3_sce14_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE14_SCE) },
		scene_field2_forest3_sce15_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE15_SCE) },
		scene_field2_forest3_sce16_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE16_SCE) },
		scene_field2_forest3_sce17_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE17_SCE) },
		scene_field2_forest3_sce18_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE18_SCE) },
		scene_field2_forest3_sce19_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE19_SCE) },
		scene_field2_forest3_sce20_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE20_SCE) },
		scene_field2_forest3_sce21_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE21_SCE) },
		scene_field2_forest3_sce22_sce{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE22_SCE) };

	Tim::rotateBPP8(scene_field2_forest3_sce00_sce.get(), rng, 0x12C, 0x27924);
	Tim::rotateBPP8(scene_field2_forest3_sce01_sce.get(), rng, 0x120, 0x1D124, 0x35128);
	Tim::rotateBPP8(scene_field2_forest3_sce02_sce.get(), rng, 0x124, 0x19124, 0x43920);
	Tim::rotateBPP8(scene_field2_forest3_sce03_sce.get(), rng, 0x128, 0x3392C);
	Tim::rotateBPP8(scene_field2_forest3_sce04_sce.get(), rng, 0x128, 0x2D128);
	Tim::rotateBPP8(scene_field2_forest3_sce05_sce.get(), rng, 0x124, 0x1F920);
	Tim::rotateBPP8(scene_field2_forest3_sce06_sce.get(), rng, 0x12C, 0x28920, 0x4B120);
	Tim::rotateBPP8(scene_field2_forest3_sce07_sce.get(), rng, 0x124, 0x2B124);
	Tim::rotateBPP8(scene_field2_forest3_sce08_sce.get(), rng, 0x134);
	Tim::rotateBPP8(scene_field2_forest3_sce09_sce.get(), rng, 0x124, 0x24124, 0x49128);
	Tim::rotateBPP8(scene_field2_forest3_sce10_sce.get(), rng, 0x11C);
	Tim::rotateBPP8(scene_field2_forest3_sce11_sce.get(), rng, 0x12C, 0x34924, 0x5F92C);
	Tim::rotateBPP8(scene_field2_forest3_sce12_sce.get(), rng, 0x124, 0x2A124);
	Tim::rotateBPP8(scene_field2_forest3_sce13_sce.get(), rng, 0x12C, 0x2F124);
	Tim::rotateBPP8(scene_field2_forest3_sce14_sce.get(), rng, 0x130, 0x31920);
	Tim::rotateBPP8(scene_field2_forest3_sce15_sce.get(), rng, 0x130, 0x37128);
	Tim::rotateBPP8(scene_field2_forest3_sce16_sce.get(), rng, 0x124, 0x18928, 0x4C11C);
	Tim::rotateBPP8(scene_field2_forest3_sce17_sce.get(), rng, 0x120, 0x1D920, 0x3B928, 0x64120);
	Tim::rotateBPP8(scene_field2_forest3_sce18_sce.get(), rng, 0x11C, 0x1B92C, 0x44920);
	Tim::rotateBPP8(scene_field2_forest3_sce19_sce.get(), rng, 0x128, 0x39130, 0x70928);
	Tim::rotateBPP8(scene_field2_forest3_sce20_sce.get(), rng, 0x11C, 0xD91C, 0x1B11C, 0x2891C, 0x36120, 0x4611C, 0x5391C, 0x6111C, 0x6E91C, 0x7C11C, 0x8991C);
	Tim::rotateBPP8(scene_field2_forest3_sce21_sce.get(), rng, 0x128);
	Tim::rotateBPP8(scene_field2_forest3_sce22_sce.get(), rng, 0x11C, 0xD11C, 0x1A11C, 0x27120, 0x3691C, 0x4391C, 0x5091C, 0x5D91C, 0x6A91C, 0x7791C, 0x8491C, 0x9191C);

	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD07Z_EFD).get(), rng, 0x3CBC, 0x7EDC, 0xC15C, 0xEB7C, 0x1161C, 0x137BC, 0x15CDC, 0x1757C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD08Z_EFD).get(), rng, 0x403C, 0x825C, 0xAC7C, 0xD71C, 0xF8BC, 0x11DDC, 0x1367C, 0x14C1C, 0x1723C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD09Z_EFD).get(), rng, 0x3CBC, 0x7EDC, 0xC15C, 0xEB7C, 0x1161C, 0x137BC, 0x15CDC, 0x1757C);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD10Z_EFD).get(), rng, 0x4A40, 0x8C60, 0xCEE0, 0xF900, 0x123A0, 0x14540, 0x16A60, 0x18300);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD17_EFD).get(), rng, 0x3F64, 0x8184, 0xA6A4, 0xC5C4, 0x107E4, 0x13204, 0x14C24);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD18_EFD).get(), rng, 0x5288, 0x94A8, 0x116C8, 0x158E8);
	Tim::rotateBPP8(m_game->file(File::EFFECT_BTFLD19_EFD).get(), rng, 0x46F0, 0x8910, 0x94F4, 0xD714, 0x101B4, 0x12354, 0x14974, 0x16394);

	const auto rotationLightNForest3{ (rng + 120) % JCUtility::clutRotationLimit };

	rotateLight(scene_field2_forest3_sce00_sce.get(), rotationLightNForest3, 0x3C, 0x2783C);
	rotateLight(scene_field2_forest3_sce01_sce.get(), rotationLightNForest3, 0x3C, 0x1D03C, 0x3503C);
	rotateLight(scene_field2_forest3_sce02_sce.get(), rotationLightNForest3, 0x3C, 0x1903C, 0x4383C);
	rotateLight(scene_field2_forest3_sce03_sce.get(), rotationLightNForest3, 0x3C, 0x3383C);
	rotateLight(scene_field2_forest3_sce04_sce.get(), rotationLightNForest3, 0x3C, 0x2D03C);
	rotateLight(scene_field2_forest3_sce05_sce.get(), rotationLightNForest3, 0x3C, 0x1F83C);
	rotateLight(scene_field2_forest3_sce06_sce.get(), rotationLightNForest3, 0x3C, 0x2883C, 0x4B03C);
	rotateLight(scene_field2_forest3_sce07_sce.get(), rotationLightNForest3, 0x3C, 0x2B03C);
	rotateLight(scene_field2_forest3_sce08_sce.get(), rotationLightNForest3, 0x3C);
	rotateLight(scene_field2_forest3_sce09_sce.get(), rotationLightNForest3, 0x3C, 0x2403C, 0x4903C);
	rotateLight(scene_field2_forest3_sce10_sce.get(), rotationLightNForest3, 0x3C);
	rotateLight(scene_field2_forest3_sce11_sce.get(), rotationLightNForest3, 0x3C, 0x3483C, 0x5F83C);
	rotateLight(scene_field2_forest3_sce12_sce.get(), rotationLightNForest3, 0x3C, 0x2A03C);
	rotateLight(scene_field2_forest3_sce13_sce.get(), rotationLightNForest3, 0x3C, 0x2F03C);
	rotateLight(scene_field2_forest3_sce14_sce.get(), rotationLightNForest3, 0x3C, 0x3183C);
	rotateLight(scene_field2_forest3_sce15_sce.get(), rotationLightNForest3, 0x3C, 0x3703C);
	rotateLight(scene_field2_forest3_sce16_sce.get(), rotationLightNForest3, 0x3C, 0x1883C, 0x4C03C);
	rotateLight(scene_field2_forest3_sce17_sce.get(), rotationLightNForest3, 0x3C, 0x1D83C, 0x3B83C, 0x6403C);
	rotateLight(scene_field2_forest3_sce18_sce.get(), rotationLightNForest3, 0x3C, 0x1B83C, 0x4483C);
	rotateLight(scene_field2_forest3_sce19_sce.get(), rotationLightNForest3, 0x3C, 0x3903C, 0x7083C);
	rotateLight(scene_field2_forest3_sce20_sce.get(), rotationLightNForest3, 0x3C, 0xD83C, 0x1B03C, 0x2883C, 0x3603C, 0x4603C, 0x5383C, 0x6103C, 0x6E83C, 0x7C03C, 0x8983C);
	rotateLight(scene_field2_forest3_sce21_sce.get(), rotationLightNForest3, 0x3C);
	rotateLight(scene_field2_forest3_sce22_sce.get(), rotationLightNForest3, 0x3C, 0xD03C, 0x1A03C, 0x2703C, 0x3683C, 0x4383C, 0x5083C, 0x5D83C, 0x6A83C, 0x7783C, 0x8483C, 0x9183C);

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

		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE00_SCE + elementFileId).get(), rng, 0x124);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE01_SCE + elementFileId).get(), rng, 0x11C, 0x1491C);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE02_SCE + elementFileId).get(), rng, 0x11C, 0x1491C, 0x29120, 0x3E11C);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE03_SCE + elementFileId).get(), rng, 0x124, 0x1E91C);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE04_SCE + elementFileId).get(), rng, 0x124);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE05_SCE + elementFileId).get(), rng, 0x124);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE06_SCE + elementFileId).get(), rng, 0x120, 0x1B11C);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE07_SCE + elementFileId).get(), rng, 0x128, 0x1F120, 0x3A920, 0x5611C);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE08_SCE + elementFileId).get(), rng, 0x124, 0x1E924);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE09_SCE + elementFileId).get(), rng, 0x12C);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE10_SCE + elementFileId).get(), rng, 0x11C);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE11_SCE + elementFileId).get(), rng, 0x128, 0x1F920);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE12_SCE + elementFileId).get(), rng, 0x120, 0x1B91C, 0x30120, 0x45924);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE13_SCE + elementFileId).get(), rng, 0x128, 0x2011C);
		Tim::rotateBPP8(m_game->file(File::SCENE_OTHER_HUNTING_SCE14_SCE + elementFileId).get(), rng, 0x124);

		for (const auto& offset : fileBattleOffsets[i].offsets)
		{
			Tim::rotateBPP8(m_game->file(fileBattleOffsets[i].file).get(), rng, offset);
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