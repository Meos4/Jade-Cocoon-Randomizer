#pragma once

#include "Backend/Model.hpp"
#include "Common/Buffer.hpp"
#include "Common/Types.hpp"

#include <array>
#include <vector>

namespace Merge
{
	struct ModelInterp
	{
		const Buffer& raw;
		s16 interpolation;
	};

	struct PartInterp
	{
		const Buffer* raw;
		s16 interpolation;
	};

	inline constexpr s16 maxInterpolation{ 4096 }; // 100%

	void Model(Buffer* mainModel,
		const Model::Minion::Animation::UnpackedOffsetSize& animations,
		const std::vector<ModelInterp>& models,
		bool skipYCoord);

	void ModelPerPart(Buffer* mainModel,
		const Model::Minion::Animation::UnpackedOffsetSize& animations,
		const std::array<PartInterp, Model::Minion::nbParts>& parts);
};