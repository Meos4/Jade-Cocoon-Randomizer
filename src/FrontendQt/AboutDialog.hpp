#pragma once

#include "ui_AboutDialog.h"

#include <QDialog>

class AboutDialog final : public QDialog
{
	Q_OBJECT
public:
	AboutDialog(QWidget* parent = nullptr);
private:
	Ui::AboutDialog m_ui;
};