#pragma once

#include "ui_WelcomeDialog.h"

#include <QDialog>

class QShowEvent;

class WelcomeDialog final : public QDialog
{
	Q_OBJECT
public:
	WelcomeDialog();
protected:
	void showEvent(QShowEvent* event) override;
private:
	void applyFixedSize();
	void centerOnScreen();

	Ui::WelcomeDialog m_ui;
};