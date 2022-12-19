#pragma once

#include "ui_SpecialMagicDialog.h"

#include "Backend/Entity.hpp"

#include <QDialog>

#include <unordered_map>
#include <unordered_set>

class SpecialMagicDialog final : public QDialog
{
	Q_OBJECT
public:
	SpecialMagicDialog(QWidget* parent = nullptr);

	std::unordered_set<Special_t> specials() const;
	Magic_t magics() const;
	void setSpecials(const std::unordered_set<Special_t>& specials);
	void setMagics(Magic_t magics);
private:
	Ui::SpecialMagicDialog m_ui;

	std::unordered_map<Special_t, QCheckBox*> m_specialsBox;
	std::unordered_map<Magic_t, QCheckBox*> m_magicsBox;
};
