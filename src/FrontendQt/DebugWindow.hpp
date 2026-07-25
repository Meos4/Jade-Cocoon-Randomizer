#pragma once

#ifdef JCR_DEBUG

#include "ui_DebugWindow.h"

#include <QWidget>

class Randomizer;

class DebugWindow final : public QWidget
{
	Q_OBJECT
public:
	DebugWindow();

	void enableUI();
	void disableUI();
	void apply(Randomizer* randomizer) const;
private:
	Ui::DebugWindow m_ui;
};

#endif