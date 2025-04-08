#pragma once

#include "ui_HelpConsoleWidget.h"

#include <QMap>
#include <QObject>
#include <QString>

#include <type_traits>

template <typename T>
concept QObjectWithText = requires(T t) { { t.text() } -> std::convertible_to<QString>; } && std::is_base_of_v<QObject, T>;

class HelpConsoleWidget final : public QWidget
{
	Q_OBJECT
public:
	HelpConsoleWidget(QWidget* parent = nullptr);

	bool eventFilter(QObject* object, QEvent* event) override;

	void addFeature(QObject* object, const QString& title, const QString& name, const QString& description);

	template <QObjectWithText T>
	void addFeature(T* object, const QString& title, const QString& description)
	{
		addInternal(object, std::move(formatFeature(title, object->text(), description)));
	}
private:
	QString formatFeature(const QString& title, const QString& name, const QString& description) const;
	void addInternal(QObject* object, QString&& formatted);

	Ui::HelpConsoleWidget m_ui;

	const QString m_default{ "&nbsp;<hr>" };
	QMap<QObject*, QString> m_objectHelp;
};