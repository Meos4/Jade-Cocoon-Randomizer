#include "TopInfoWidget.hpp"

#include "FrontendQt/GuiSettings.hpp"
#include "FrontendQt/MainWindow.hpp"
#include "FrontendQt/QtUtility.hpp"

#include <QString>

TopInfoWidget::TopInfoWidget(QWidget* parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);

	connect(m_ui.loadPresets, &QAbstractButton::clicked, this, &TopInfoWidget::buttonLoadPresetsClicked);
	connect(m_ui.savePresets, &QAbstractButton::clicked, this, &TopInfoWidget::buttonSavePresetsClicked);
	connect(m_ui.seedCheckBox, &QAbstractButton::toggled, m_ui.seedLineEdit, &QWidget::setEnabled);
	connect(m_ui.seedLineEdit, &QLineEdit::textChanged, this, &TopInfoWidget::setSeedLineEditText);
}

void TopInfoWidget::enableUI(const QString& version)
{
	setVersion(version);

	m_ui.loadPresets->setEnabled(true);
	m_ui.savePresets->setEnabled(true);
	m_ui.seedCheckBox->setEnabled(true);
	if (m_ui.seedCheckBox->isChecked())
	{
		m_ui.seedLineEdit->setEnabled(true);
	}
}

void TopInfoWidget::disableUI()
{
	setVersion("");

	m_ui.loadPresets->setEnabled(false);
	m_ui.savePresets->setEnabled(false);
	m_ui.seedCheckBox->setEnabled(false);
	m_ui.seedLineEdit->setEnabled(false);
}

u64 TopInfoWidget::seed() const
{
	return m_ui.seedLineEdit->text().toULongLong();
}

bool TopInfoWidget::isSeedEnabled() const
{
	return m_ui.seedCheckBox->isChecked();
}

void TopInfoWidget::setVersion(const QString& version)
{
	m_ui.versionLabel2->setText(version);
}

void TopInfoWidget::setSeed(u64 value)
{
	m_ui.seedLineEdit->setText(QString::number(value));
}

void TopInfoWidget::setSeedLineEditText(const QString& text)
{
	auto textQStr{ text };
	QtUtility::qStrU64Filter(&textQStr);
	m_ui.seedLineEdit->setText(textQStr);
}