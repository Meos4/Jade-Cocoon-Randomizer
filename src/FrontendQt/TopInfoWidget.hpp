#pragma once

#include "ui_TopInfoWidget.h"

#include "Common/Types.hpp"

#include <QWidget>

class QString;

class TopInfoWidget final : public QWidget
{
	Q_OBJECT
public:
	TopInfoWidget(QWidget* parent = nullptr);

	void enableUI(const QString& version);
	void disableUI();

	u64 seed() const;
	bool isSeedEnabled() const;

	void setVersion(const QString& version);
	void setSeed(u64 value);
Q_SIGNALS:
	void buttonLoadPresetsClicked();
	void buttonSavePresetsClicked();
private Q_SLOTS:
	void setSeedLineEditText(const QString& text);
private:
	Ui::TopInfoWidget m_ui;
};