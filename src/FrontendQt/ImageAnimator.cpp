#include "ImageAnimator.hpp"

#include <QLabel>
#include <QTimer>

#include <type_traits>

ImageAnimator::ImageAnimator(QVector<QPixmap>&& images, ImageAnimator::Mode mode, s32 msec, QLabel* label, QObject* parent)
	: QObject(parent), m_frames(std::move(images)), m_label(label)
{
	m_timer = new QTimer(this);
	m_timer->start(msec);
	const auto callback{ mode == ImageAnimator::Mode::Normal ? &ImageAnimator::nextFrameNormal : &ImageAnimator::nextFrameRevert };
	connect(m_timer, &QTimer::timeout, this, callback);
}

void ImageAnimator::nextFrameNormal()
{
	m_label->setPixmap(m_frames[m_currentFrame]);
	m_currentFrame = (m_currentFrame + 1) % m_frames.size();
}

void ImageAnimator::nextFrameRevert()
{
	static constexpr auto revertBit{ 1 << 31 };
	m_label->setPixmap(m_frames[m_currentFrame & ~revertBit]);
	(m_currentFrame & revertBit) ? --m_currentFrame : ++m_currentFrame;
	const auto frame{ m_currentFrame & ~revertBit };

	if (frame == 0 || frame + 1 == m_frames.size())
	{
		m_currentFrame ^= revertBit;
	}
}