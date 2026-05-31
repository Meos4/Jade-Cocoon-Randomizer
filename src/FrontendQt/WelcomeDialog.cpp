#include "WelcomeDialog.hpp"

#include "Common/Software.hpp"

#include <QGuiApplication>
#include <QIcon>
#include <QPushButton>
#include <QScreen>
#include <QShowEvent>
#include <QSize>

#include <format>

WelcomeDialog::WelcomeDialog()
	: QDialog(nullptr)
{
	setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
#ifdef Q_OS_WIN
	setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
#endif

	m_ui.setupUi(this);

	setWindowIcon(QIcon{ ":/icons/SwIcon.png" });
	setWindowModality(Qt::ApplicationModal);
	setModal(true);
	setSizeGripEnabled(false);

	m_ui.titleLabel->setText(QString::fromStdString(std::format(
		"<html><head/><body><p align=\"center\">"
		"<span style=\" font-size:18pt; font-weight:600;\">Welcome to Jade Cocoon Randomizer v{}</span>"
		"</p></body></html>",
		Software::version)));

	m_ui.logoLayout->setStretch(0, 1);
	m_ui.logoLayout->setStretch(2, 1);
	setWindowTitle(QString::fromStdString(std::format("Welcome - Jade Cocoon Randomizer v{}", Software::version)));

	connect(m_ui.okButton, &QPushButton::clicked, this, &QDialog::accept);

	applyFixedSize();
}

void WelcomeDialog::applyFixedSize()
{
	const QSize welcomeDialogSize{ 720, 460 };

	setMinimumSize(welcomeDialogSize);
	setMaximumSize(welcomeDialogSize);
	resize(welcomeDialogSize);
	setFixedSize(welcomeDialogSize);
}

void WelcomeDialog::centerOnScreen()
{
	const QScreen* screen{ QGuiApplication::primaryScreen() };

	if (!screen)
	{
		return;
	}

	const QRect availableGeometry{ screen->availableGeometry() };
	move(availableGeometry.center() - rect().center());
}

void WelcomeDialog::showEvent(QShowEvent* event)
{
	QDialog::showEvent(event);
	applyFixedSize();
	centerOnScreen();
}