#include "DebugWindow.hpp"

#ifdef JCR_DEBUG

#include "Backend/Randomizer.hpp"

DebugWindow::DebugWindow()
	: QWidget(nullptr, Qt::Window)
{
	m_ui.setupUi(this);

	setAttribute(Qt::WA_ShowWithoutActivating);
	setAttribute(Qt::WA_QuitOnClose, false);

	disableUI();
}

void DebugWindow::enableUI()
{
	m_ui.stageSelect->setEnabled(true);
	m_ui.titleDebug->setEnabled(true);
}

void DebugWindow::disableUI()
{
	m_ui.stageSelect->setEnabled(false);
	m_ui.titleDebug->setEnabled(false);
}

void DebugWindow::apply(Randomizer* randomizer) const
{
	if (m_ui.stageSelect->isChecked())
	{
		randomizer->debugStageSelect();
	}

	if (m_ui.titleDebug->isChecked())
	{
		randomizer->debugTitleDebug();
	}
}

#endif