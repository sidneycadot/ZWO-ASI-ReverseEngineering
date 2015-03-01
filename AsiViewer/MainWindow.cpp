
////////////////////
// MainWindow.cpp //
////////////////////

#include <QDebug>
#include <QMenuBar>
#include <QStatusBar>

#include "MainWindow.h"
#include "CentralWidget.h"

MainWindow::MainWindow()
{
    qDebug() << __PRETTY_FUNCTION__;
    QWidget * centralWidget = new CentralWidget();
    setCentralWidget(centralWidget); // QMainWindow takes responsibility for deletion.

    QMenu * fileMenu = menuBar()->addMenu("&File");

    statusBar()->showMessage("hello");
}

MainWindow::~MainWindow()
{
    qDebug() << __PRETTY_FUNCTION__;
}
