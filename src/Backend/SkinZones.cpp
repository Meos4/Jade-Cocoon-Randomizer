#include "SkinZones.hpp"

#include "SkinZonesMask.hpp"

#include <algorithm>
#include <array>
#include <span>
#include <vector>

namespace SkinZones
{
	struct Texture
	{
		u32 pixelsOffset;
		u16 width;
		u16 height;
	};

	struct ClutZone
	{
		Model_t model;
		u32 clutOffset;
		std::span<const Texture> textures;
	};

	static constexpr std::array<Texture, 2> bodyTextures
	{{
		{ 0x2454u, 128, 192 },
		{ 0x8474u, 52, 64 }
	}};

	static constexpr std::array<Texture, 1> portraitTextures
	{{
		{ 0x1234u, 64, 64 }
	}};

	// KIKI shares the same layout shifted by +0x800, so it needs its own offsets
	static constexpr std::array<Texture, 2> kikiBodyTextures
	{{
		{ 0x2C54u, 128, 192 },
		{ 0x8C74u, 52, 64 }
	}};

	static constexpr std::array<Texture, 1> kikiPortraitTextures
	{{
		{ 0x1A34u, 64, 64 }
	}};

	static constexpr std::array<ClutZone, 10> clutZones
	{{
		{ MODEL_LUI_A, 0x2248u, bodyTextures },
		{ MODEL_BSGE, 0x2248u, bodyTextures },
		{ MODEL_BSFE, 0x2248u, bodyTextures },
		{ MODEL_BSWE, 0x2248u, bodyTextures },
		{ MODEL_YUME, 0x2248u, bodyTextures },
		{ MODEL_NAGK, 0x2248u, bodyTextures },
		{ MODEL_KIKI, 0x2A48u, kikiBodyTextures },
		{ MODEL_LUI_A, 0x1028u, portraitTextures },
		{ MODEL_NAGK, 0x1028u, portraitTextures },
		{ MODEL_KIKI, 0x1828u, kikiPortraitTextures }
	}};

	static constexpr u16 transparentClr{ 0x0000 };

	static bool maskBit(const u8* mask, std::size_t i)
	{
		return mask && (mask[i >> 3] & (1u << (i & 7)));
	}

	static u32 colorDist2(u16 a, u16 b)
	{
		const s32
			dr{ (a & 0x1F) - (b & 0x1F) },
			dg{ ((a >> 5) & 0x1F) - ((b >> 5) & 0x1F) },
			db{ ((a >> 10) & 0x1F) - ((b >> 10) & 0x1F) };

		return static_cast<u32>(dr * dr + dg * dg + db * db);
	}

	static u32 quantizeSide(const std::array<u32, TimPalette::clutSize>& pxCount,
		const std::array<u16, TimPalette::clutSize>& original,
		u32 budget, u32 startSlot,
		std::array<u16, TimPalette::clutSize>& newPalette,
		std::array<u8, TimPalette::clutSize>& remap)
	{
		struct ColorWeight
		{
			u16 color;
			u32 weight;
		};

		std::vector<ColorWeight> values;

		for (u32 idx{}; idx < TimPalette::clutSize; ++idx)
		{
			if (!pxCount[idx] || original[idx] == transparentClr)
			{
				continue;
			}

			const auto it{ std::ranges::find(values, original[idx], &ColorWeight::color) };

			if (it == values.end())
			{
				values.push_back({ original[idx], pxCount[idx] });
			}
			else
			{
				it->weight += pxCount[idx];
			}
		}

		std::ranges::stable_sort(values, [](const ColorWeight& a, const ColorWeight& b)
		{
			return a.weight != b.weight ? a.weight > b.weight : a.color < b.color;
		});

		const auto survivors{ std::min<u32>(budget, static_cast<u32>(values.size())) };

		for (u32 j{}; j < survivors; ++j)
		{
			newPalette[startSlot + j] = values[j].color;
		}

		for (u32 idx{}; idx < TimPalette::clutSize; ++idx)
		{
			if (!pxCount[idx] || original[idx] == transparentClr)
			{
				continue;
			}

			const auto color{ original[idx] };
			auto bestSlot{ startSlot };
			auto bestDist{ colorDist2(color, newPalette[startSlot]) };

			for (u32 j{}; j < survivors; ++j)
			{
				if (newPalette[startSlot + j] == color)
				{
					bestSlot = startSlot + j;
					break;
				}

				const auto dist{ colorDist2(color, newPalette[startSlot + j]) };

				if (dist < bestDist)
				{
					bestDist = dist;
					bestSlot = startSlot + j;
				}
			}

			remap[idx] = static_cast<u8>(bestSlot);
		}

		return survivors;
	}

	Rearrangement rearrangeCLUT(RawFile* file, Model_t model, u32 clutOffset)
	{
		Rearrangement result;
		const auto original{ file->read<std::array<u16, TimPalette::clutSize>>(clutOffset) };
		result.palette = original;

		const ClutZone* zone{};

		for (const auto& candidate : clutZones)
		{
			if (candidate.model == model && candidate.clutOffset == clutOffset)
			{
				zone = &candidate;
				break;
			}
		}

		if (!zone)
		{
			return result; // No mask: palette untouched, everything rotates
		}

		std::array<u32, TimPalette::clutSize> protPx{}, rotPx{};
		std::vector<std::vector<u8>> texPixels(zone->textures.size());
		std::vector<const u8*> texMasks(zone->textures.size());
		bool hasTransparent{};

		for (std::size_t t{}; t < zone->textures.size(); ++t)
		{
			const auto& texture{ zone->textures[t] };
			auto& pixels{ texPixels[t] };
			pixels.resize(static_cast<std::size_t>(texture.width) * texture.height);
			file->read(texture.pixelsOffset, pixels.data(), pixels.size());
			texMasks[t] = SkinZonesMask::protectMask(model, clutOffset, static_cast<u32>(t));

			for (std::size_t i{}; i < pixels.size(); ++i)
			{
				const auto idx{ pixels[i] };

				if (original[idx] == transparentClr)
				{
					hasTransparent = true;
				}

				(maskBit(texMasks[t], i) ? protPx : rotPx)[idx]++;
			}
		}

		result.palette.fill(transparentClr);

		const u32 totalBudget{ TimPalette::clutSize - (hasTransparent ? 1u : 0u) };

		std::array<u8, TimPalette::clutSize> remapProt{}, remapRot{};
		const auto usedP{ quantizeSide(protPx, original, totalBudget - 1, 0, result.palette, remapProt) };
		const auto usedR{ quantizeSide(rotPx, original, totalBudget - usedP, usedP, result.palette, remapRot) };

		for (u32 slot{}; slot < usedP; ++slot)
		{
			result.protectedSlots.set(slot);
		}

		if (hasTransparent)
		{
			const auto transSlot{ usedP + usedR };
			result.palette[transSlot] = transparentClr;
			result.protectedSlots.set(transSlot);

			for (u32 idx{}; idx < TimPalette::clutSize; ++idx)
			{
				if (original[idx] == transparentClr)
				{
					remapProt[idx] = remapRot[idx] = static_cast<u8>(transSlot);
				}
			}
		}

		for (std::size_t t{}; t < zone->textures.size(); ++t)
		{
			auto& pixels{ texPixels[t] };

			for (std::size_t i{}; i < pixels.size(); ++i)
			{
				pixels[i] = maskBit(texMasks[t], i) ? remapProt[pixels[i]] : remapRot[pixels[i]];
			}

			file->write(zone->textures[t].pixelsOffset, *pixels.data(), pixels.size());
		}

		return result;
	}
}
