#include "AboutDialog.hpp"

#include "Common/Software.hpp"

#include <QPixmap>

#include <format>

AboutDialog::AboutDialog(QWidget* parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	m_ui.nameLabel->setText(QString::fromStdString(
		std::format("<b><font size=\"+2\">Jade Cocoon Randomizer v{} by Meos</b></font>", Software::version)));

	m_ui.creditsLabel->setText
	(
		"<b><p>Credits:</b></p>"
		"<p>Levant and Boss Icon made by Monsterinabox.INC</p>"
	);

	m_ui.linkLabel->setOpenExternalLinks(true);
	m_ui.linkLabel->setText
	(
		"<a href=\"https://github.com/Meos4/Jade-Cocoon-Randomizer\">GitHub</a> | "
		"<a href=\"https://discord.com/invite/ErwKG2v\">Discord</a> | "
		"<a href=\"https://thejadecocoonproject.fandom.com/wiki/Jade_Cocoon_Wiki\">Wiki</a>"
	);
}