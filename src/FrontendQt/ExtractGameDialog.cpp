#include "ExtractGameDialog.hpp"

#include <QString>

ExtractGameDialog::ExtractGameDialog(QWidget* parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	setOkButtonVisibility(false);

	setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

	connect(this, &ExtractGameDialog::onStateChanged, this, &ExtractGameDialog::setStateLabel);
	connect(this, &ExtractGameDialog::onStateError, this, &ExtractGameDialog::setStateLabelError);
	connect(this, &ExtractGameDialog::taskCompleted, this, &ExtractGameDialog::enableOkButton);
	connect(this, &ExtractGameDialog::onOkButtonVisibilityChanged, this, &ExtractGameDialog::setOkButtonVisibility);
	connect(m_ui.okButton, &QPushButton::clicked, this, &QWidget::close);
	connect(this, &ExtractGameDialog::shouldClose, this, &QWidget::close);
}

void ExtractGameDialog::setStateLabel(const QString& text)
{
	m_ui.stateLabel->setText(text);
}

void ExtractGameDialog::setStateLabelError(const QString& text)
{
	setStateLabel(text);
	m_ui.stateLabel->setStyleSheet("QLabel {color : red;}");
}

void ExtractGameDialog::enableOkButton()
{
	m_ui.okButton->setEnabled(true);
}

void ExtractGameDialog::setOkButtonVisibility(bool enable)
{
	m_ui.okButton->setVisible(enable);
}