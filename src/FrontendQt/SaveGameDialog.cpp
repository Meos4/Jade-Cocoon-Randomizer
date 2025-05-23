#include "SaveGameDialog.hpp"

#include "Common/Random.hpp"
#include "FrontendQt/ImageAnimator.hpp"
#include "FrontendQt/ImageAnimatorFactory.hpp"

SaveGameDialog::SaveGameDialog(Random* random, QWidget* parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	const auto loading{ static_cast<ImageAnimatorFactory::Loading>(random->generate(static_cast<s32>(ImageAnimatorFactory::Loading::Count) - 1)) };
	m_animator = ImageAnimatorFactory::create(loading, m_ui.loadingLabel, this);

	setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

	connect(this, &SaveGameDialog::progressBarChanged, this, &SaveGameDialog::setProgressBarValue);
	connect(this, &SaveGameDialog::onStateChanged, this, &SaveGameDialog::setStateLabel);
	connect(this, &SaveGameDialog::onStateError, this, &SaveGameDialog::setStateLabelError);
	connect(this, &SaveGameDialog::taskCompleted, this, &SaveGameDialog::enableOkButton);
	connect(m_ui.okButton, &QPushButton::clicked, this, &QWidget::close);
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
	m_ui.okButton->setEnabled(true);
}