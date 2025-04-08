#include "HelpConsoleWidget.hpp"

HelpConsoleWidget::HelpConsoleWidget(QWidget* parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);

	m_ui.textEdit->setText(m_default);
}

bool HelpConsoleWidget::eventFilter(QObject* object, QEvent* event)
{
	const auto type{ event->type() };

	if (type == QEvent::Enter)
	{
		const auto it{ m_objectHelp.find(object) };

		if (it != m_objectHelp.end())
		{
			m_ui.textEdit->setText(it.value());
		}
	}
	else if (type == QEvent::Leave)
	{
		m_ui.textEdit->setText(m_default);
	}

	return QWidget::eventFilter(object, event);
}

void HelpConsoleWidget::addFeature(QObject* object, const QString& title, const QString& name, const QString& description)
{
	addInternal(object, std::move(formatFeature(title, name, description)));
}

QString HelpConsoleWidget::formatFeature(const QString& title, const QString& name, const QString& description) const
{
	QString descriptionRichText{ description };
	descriptionRichText.replace("\n", "<br>");

	return { "<b>" + title + "</b> -> [" + name + "]<hr>" + descriptionRichText };
}

void HelpConsoleWidget::addInternal(QObject* object, QString&& formatted)
{
	m_objectHelp[object] = std::move(formatted);
	object->installEventFilter(this);
}