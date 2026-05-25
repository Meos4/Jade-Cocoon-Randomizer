#include "Backend/Randomizer/Helpers.hpp"

#include "Backend/Game.hpp"
#include "Backend/Merge.hpp"
#include "Backend/Model.hpp"
#include "Backend/SharedData.hpp"
#include "Backend/TimPalette.hpp"
#include "Common/RawFile.hpp"

namespace Helpers
{
	void randomizeModelInterp(RawFile* file, Model_t model, const SharedData& sharedData, Game* game, RawFile& executable, s32 craziness)
	{
		static constexpr s16 maxInterpolation{ 3072 };

		const auto rate{ game->random()->generate(0.2f, 1.f) * maxInterpolation };
		const s16 globalRate{ game->random()->generate(static_cast<s16>(rate / 1.5f), static_cast<s16>(rate)) };
		const Buffer& globalModel{ sharedData.model(Model::Minion::models[game->random()->generate(Model::Minion::models.size() - 1)]) };

		auto mainModel{ sharedData.model(model) };
		const auto firstAnimPtr{ executable.read<u32>(game->offset().file.executable.tableOfModelAnimationsPtr) };
		const auto animPtr{ executable.read<u32>(game->offset().file.executable.tableOfModelAnimationsPtr + sizeof(u32) * model) };
		const auto offset{ (animPtr - firstAnimPtr + game->offset().file.executable.tableOfModelAnimations) & ~0x80000000 };

		Model::Minion::Animation::UnpackedOffsetSize unpackedOffsetSize
		{
			executable.read<std::array<std::pair<u16, u16>, Model::Minion::Animation::nbUnpacked>>(offset + 0x3E)
		};

		Merge::Model(&mainModel, unpackedOffsetSize, { { globalModel, globalRate } }, craziness > 0);

		if (craziness > 0)
		{
			std::array<Merge::PartInterp, Model::Minion::nbParts> partsInterp{};
			for (auto& part : partsInterp)
			{
				if (game->random()->generateProba(static_cast<float>(craziness)))
				{
					part =
					{
						&sharedData.model(Model::Minion::models[game->random()->generate(Model::Minion::models.size() - 1)]),
						game->random()->generate(static_cast<s16>(rate / 1.5f), static_cast<s16>(rate))
					};
				}
				else
				{
					part = { &mainModel, s16{} };
				}
			}
			Merge::ModelPerPart(&mainModel, unpackedOffsetSize, partsInterp);
		}

		file->write(0, *mainModel.data(), mainModel.size());
	}

	void randomizeCLUT(RawFile* file, Random* random)
	{
		auto clut{ file->read<std::array<u16, Model::Minion::Texture::clutSize>>(Model::Minion::Texture::clutBegin) };
		TimPalette::rotateCLUT(clut, random->generate(TimPalette::clutRotationLimit));
		file->write(Model::Minion::Texture::clutBegin, clut);
	}
}
