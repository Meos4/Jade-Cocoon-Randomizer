#pragma once

#include "ui_ExtractGameDialog.h"

#include <QDialog>

class QString;

class ExtractGameDialog final : public QDialog
{
	Q_OBJECT
public:
	ExtractGameDialog(QWidget* parent = nullptr);
private Q_SLOTS:
	void setStateLabel(const QString& text);
	void setStateLabelError(const QString& text);
	void enableOkButton();
	void setOkButtonVisibility(bool enable);
Q_SIGNALS:
	void onStateChanged(const QString&);
	void onStateError(const QString&);
	void taskCompleted();
	void shouldClose();
	void onOkButtonVisibilityChanged(bool);
private:
	Ui::ExtractGameDialog m_ui;
};