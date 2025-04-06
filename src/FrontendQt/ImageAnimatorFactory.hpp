#pragma once

class ImageAnimator;
class QLabel;
class QObject;

namespace ImageAnimatorFactory
{
	enum class Loading
	{
		NtscJ,
		NtscU,
		Pal,
		Count
	};

	ImageAnimator* create(ImageAnimatorFactory::Loading loading, QLabel* label, QObject* parent = nullptr);
}