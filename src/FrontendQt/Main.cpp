#include "MainWindow.hpp"

#include <QApplication>

int main(int argc, char** argv)
{
	const QApplication app{ argc, argv };
	MainWindow window;
	window.show();
	const auto exec{ app.exec() };
	window.saveSettings();
	return exec;
}