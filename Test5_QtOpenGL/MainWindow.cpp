#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	this->renWin = new RenderWindow();

	QWidget* w = QWidget::createWindowContainer(this->renWin, this);

	this->setCentralWidget(w);
}
