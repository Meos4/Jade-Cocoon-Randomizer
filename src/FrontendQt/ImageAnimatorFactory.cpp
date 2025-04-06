#include "ImageAnimatorFactory.hpp"

#include "Common/Types.hpp"
#include "FrontendQt/ImageAnimator.hpp"

#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <QVector>

#include <array>
#include <format>

namespace ImageAnimatorFactory
{
	static constexpr auto iconsPath{ "icons/" };

	ImageAnimator* create(ImageAnimatorFactory::Loading loading, QLabel* label, QObject* parent)
	{
		struct LoadingInfo
		{
			const char* filename;
			s32 frames;
			ImageAnimator::Mode mode;
			s32 msec;
		};

		static constexpr std::array<LoadingInfo, static_cast<std::size_t>(ImageAnimatorFactory::Loading::Count)> infos
		{
			"LoadingJ", 8, ImageAnimator::Mode::Revert, 60,
			"LoadingU", 7, ImageAnimator::Mode::Revert, 60,
			"LoadingE", 8, ImageAnimator::Mode::Revert, 60
		};

		const auto& [filename, frames, mode, msec]{ infos[static_cast<std::size_t>(loading)] };
		QVector<QPixmap> images(frames);

		for (s32 i{}; i < frames; ++i)
		{
			images[i] = QPixmap{ QString::fromStdString(std::format(":{}{}{}.png", iconsPath, filename, i + 1)) };
		}

		return new ImageAnimator{ std::move(images), mode, msec, label, parent };
	}
}