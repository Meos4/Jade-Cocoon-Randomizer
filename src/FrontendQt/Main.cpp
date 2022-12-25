#include "MainWindow.hpp"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char** argv)
{
	const QApplication app{ argc, argv };
	qApp->setStyle(QStyleFactory::create("Fusion"));

	MainWindow window;
	window.show();

	return app.exec();
}