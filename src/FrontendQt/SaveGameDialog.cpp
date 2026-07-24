#include "SaveGameDialog.hpp"

#include "Common/Random.hpp"
#include "FrontendQt/ImageAnimator.hpp"
#include "FrontendQt/ImageAnimatorFactory.hpp"

#include <QKeyEvent>
#include <QString>

SaveGameDialog::SaveGameDialog(Random* random, QWidget* parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	const auto loading{ static_cast<ImageAnimatorFactory::Loading>(random->generate(static_cast<s32>(ImageAnimatorFactory::Loading::Count) - 1)) };
	m_animator = ImageAnimatorFactory::create(loading, m_ui.loadingLabel, this);

	setRetryButtonVisibility(false);

	setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

	connect(this, &SaveGameDialog::progressBarChanged, this, &SaveGameDialog::setProgressBarValue);
	connect(this, &SaveGameDialog::onStateChanged, this, &SaveGameDialog::setStateLabel);
	connect(this, &SaveGameDialog::onStateError, this, &SaveGameDialog::setStateLabelError);
	connect(this, &SaveGameDialog::taskCompleted, this, &SaveGameDialog::enableOkButton);
	connect(this, &SaveGameDialog::onRetryButtonVisibilityChanged, this, &SaveGameDialog::setRetryButtonVisibility);
	connect(m_ui.okButton, &QPushButton::clicked, this, &QWidget::close);
	connect(m_ui.retryButton, &QPushButton::clicked, this, &SaveGameDialog::onRetryButtonClicked);
}

void SaveGameDialog::setProgressBarValue(s32 value)
{
	m_ui.progressBar->setValue(value > 100 ? 100 : value);
}

void SaveGameDialog::setStateLabel(const QString& text)
{
	m_ui.stateLabel2->setText(text);
}

void SaveGameDialog::setStateLabelError(const QString& text)
{
	setStateLabel(text);
	m_ui.stateLabel2->setStyleSheet("QLabel {color : red;}");
}

void SaveGameDialog::enableOkButton()
{
	m_taskCompleted = true;
	m_ui.okButton->setEnabled(true);
}

void SaveGameDialog::setRetryButtonVisibility(bool enable)
{
	static constexpr auto buttonsSpacing{ 6 };

	const auto okButtonWidth{ m_ui.okButton->width() };
	const auto buttonsWidth{ enable ? okButtonWidth + buttonsSpacing + m_ui.retryButton->width() : okButtonWidth };
	const auto okButtonX{ (width() - buttonsWidth) / 2 };

	m_ui.okButton->move(okButtonX, m_ui.okButton->y());
	m_ui.retryButton->move(okButtonX + okButtonWidth + buttonsSpacing, m_ui.retryButton->y());
	m_ui.retryButton->setVisible(enable);
}

void SaveGameDialog::onRetryButtonClicked()
{
	setRetryButtonVisibility(false);
	m_ui.stateLabel2->setStyleSheet(QString{});
	m_taskCompleted = false;
	m_ui.okButton->setEnabled(false);
	emit retryRequested();
}

void SaveGameDialog::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape && !m_taskCompleted)
	{
		event->accept();
		return;
	}

	QDialog::keyPressEvent(event);
}