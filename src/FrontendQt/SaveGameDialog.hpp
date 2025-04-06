#pragma once

#include "ui_SaveGameDialog.h"

#include "Common/Types.hpp"

#include <QDialog>

class ImageAnimator;
class QString;
class Random;

class SaveGameDialog final : public QDialog
{
	Q_OBJECT
public:
	SaveGameDialog(Random* random, QWidget* parent = nullptr);
private Q_SLOTS:
	void setProgressBarValue(s32 value);
	void setStateLabel(const QString& text);
	void setStateLabelError(const QString& text);
	void enableOkButton();
Q_SIGNALS:
	void progressBarChanged(s32);
	void onStateChanged(const QString&);
	void onStateError(const QString&);
	void taskCompleted();
private:
	Ui::SaveGameDialog m_ui;
	ImageAnimator* m_animator;
};