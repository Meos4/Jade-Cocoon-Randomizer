#pragma once

#include "ui_DefaultDialog.h"

#include "Common/Json.hpp"
#include "FrontendQt/RandomizerWidgetSettings.hpp"

#include <QDialog>

#include <vector>

class HelpConsoleWidget;

class DefaultDialog final : public QDialog
{
	Q_OBJECT
public:
	DefaultDialog(QWidget* parent = nullptr);

	static constexpr const char* nameKey{ "Default" };

	const char* name() const;
	void loadPresets(const Json::Read& json);
	void savePresets(Json::Write* json) const;

	const Ui::DefaultDialog& Ui() const;
private:
	Ui::DefaultDialog m_ui;
	HelpConsoleWidget* m_helpConsole{};
	std::vector<RandomizerQCheckBox> m_qCheckBox;
};