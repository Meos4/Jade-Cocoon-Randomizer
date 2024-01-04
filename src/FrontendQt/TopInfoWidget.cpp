#include "TopInfoWidget.hpp"

#include "FrontendQt/MainWindow.hpp"
#include "FrontendQt/QtUtility.hpp"

#include <QString>

TopInfoWidget::TopInfoWidget(QWidget* parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);

	disableUI();

	connect(m_ui.loadSettings, &QAbstractButton::clicked, this, &TopInfoWidget::buttonLoadSettingsClicked);
	connect(m_ui.saveSettings, &QAbstractButton::clicked, this, &TopInfoWidget::buttonSaveSettingsClicked);
	connect(m_ui.seedCheckBox, &QAbstractButton::toggled, m_ui.seedLineEdit, &QWidget::setEnabled);
	connect(m_ui.seedLineEdit, &QLineEdit::textChanged, this, &TopInfoWidget::setSeedLineEditText);
}

void TopInfoWidget::enableUI(const QString& version, const QString& filename, TopInfoWidget::State state)
{
	setVersion(version);
	setFilename(filename);
	setState(state);

	m_ui.loadSettings->setEnabled(true);
	m_ui.saveSettings->setEnabled(true);
	m_ui.seedCheckBox->setEnabled(true);
	if (m_ui.seedCheckBox->isChecked())
	{
		m_ui.seedLineEdit->setEnabled(true);
	}
}

void TopInfoWidget::disableUI()
{
	setVersion("");
	setFilename("");
	setState(TopInfoWidget::State::Disabled);

	m_ui.loadSettings->setEnabled(false);
	m_ui.saveSettings->setEnabled(false);
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

void TopInfoWidget::setFilename(const QString& filename)
{
	static constexpr auto maxSize{ 50 };
	if (filename.size() > maxSize)
	{
		auto filenameReduced{ filename };
		filenameReduced.resize(maxSize);
		for (s32 i{}; i < 3; ++i)
		{
			filenameReduced[maxSize - 1 - i] = '.';
		}
		m_ui.fileLabel2->setText(filenameReduced);
	}
	else
	{
		m_ui.fileLabel2->setText(filename);
	}
}

void TopInfoWidget::setState(TopInfoWidget::State state)
{
	if (state == TopInfoWidget::State::Vanilla)
	{
		m_ui.stateLabel2->setText("Vanilla");
		m_ui.stateLabel2->setStyleSheet("");
		m_ui.stateLabel2->setToolTip("");
	}
	else if (state == TopInfoWidget::State::Randomized)
	{
		m_ui.stateLabel2->setText("Randomized");
		m_ui.stateLabel2->setStyleSheet("QLabel{color: rgb(200, 0, 0);}");
		m_ui.stateLabel2->setToolTip("Randomized iso can't be re-randomized.");
	}
	else
	{
		m_ui.stateLabel2->setText("");
		m_ui.stateLabel2->setToolTip("");
	}
}

void TopInfoWidget::setSeed(u64 value)
{
	m_ui.seedLineEdit->setText(QString::number(value));
}

void TopInfoWidget::setSeedLineEditText(const QString& text)
{
	auto textQStr{ text };
	QtUtility::qStrToQStrU64(&textQStr);
	m_ui.seedLineEdit->setText(textQStr);
}