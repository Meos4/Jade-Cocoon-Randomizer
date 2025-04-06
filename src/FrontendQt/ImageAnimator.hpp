#pragma once

#include "Common/Types.hpp"

#include <QObject>
#include <QPixmap>
#include <QVector>

class QLabel;
class QTimer;

class ImageAnimator final : public QObject
{
	Q_OBJECT
public:
	enum class Mode
	{
		Normal,
		Revert
	};

	ImageAnimator(QVector<QPixmap>&& images, ImageAnimator::Mode mode, s32 msec, QLabel* label, QObject* parent = nullptr);
private Q_SLOTS:
	void nextFrameNormal();
	void nextFrameRevert();
private:
	QTimer* m_timer;
	QLabel* m_label;
	QVector<QPixmap> m_frames;
	s32 m_currentFrame{ 1 };
};