#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{

	QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QApplication a(argc, argv);

	MainWindow w;
	w.show();


	return a.exec();
}
