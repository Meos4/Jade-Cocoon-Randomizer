#include "Backend/Randomizer/Helpers.hpp"

#include "Backend/Game.hpp"
#include "Backend/Merge.hpp"
#include "Backend/Model.hpp"
#include "Backend/SharedData.hpp"
#include "Backend/TimPalette.hpp"
#include "Common/RawFile.hpp"

#include <vector>

namespace Helpers
{
	void randomizeModelInterp(RawFile* file, Model_t model, const SharedData& sharedData, Game* game, RawFile& executable)
	{
		std::vector<Merge::ModelInterp> modelsInterp;

		static constexpr s16 maxInterpolation{ 3072 }, iterationRate{ 512 };

		const auto rate{ game->random()->generate(0.2f, 1.f) * maxInterpolation };
		const auto iteration{ maxInterpolation / iterationRate + 1 - static_cast<s32>(rate / iterationRate) };
		modelsInterp.reserve(iteration);

		for (s32 i{}; i < iteration; ++i)
		{
			const auto rngModel{ game->random()->generate(Model::Minion::models.size() - 1) };

			modelsInterp.emplace_back(
				sharedData.model(Model::Minion::models[rngModel]),
				game->random()->generate(static_cast<s16>(rate / 1.5f), static_cast<s16>(rate))
			);
		}

		auto mainModel{ sharedData.model(model) };
		const auto firstAnimPtr{ executable.read<u32>(game->offset().file.executable.tableOfModelAnimationsPtr) };
		const auto animPtr{ executable.read<u32>(game->offset().file.executable.tableOfModelAnimationsPtr + sizeof(u32) * model) };
		const auto offset{ (animPtr - firstAnimPtr + game->offset().file.executable.tableOfModelAnimations) & ~0x80000000 };

		Model::Minion::Animation::UnpackedOffsetSize unpackedOffsetSize
		{
			executable.read<std::array<std::pair<u16, u16>, Model::Minion::Animation::nbUnpacked>>(offset + 0x3E)
		};

		Merge::Model(&mainModel, unpackedOffsetSize, modelsInterp);
		file->write(0, *mainModel.data(), mainModel.size());
	}

	void randomizeCLUT(RawFile* file, Random* random)
	{
		auto clut{ file->read<std::array<u16, Model::Minion::Texture::clutSize>>(Model::Minion::Texture::clutBegin) };
		TimPalette::rotateCLUT(clut, random->generate(TimPalette::clutRotationLimit));
		file->write(Model::Minion::Texture::clutBegin, clut);
	}
}
