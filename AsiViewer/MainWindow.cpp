
////////////////////
// MainWindow.cpp //
////////////////////

#include <QDebug>
#include "MainWindow.h"
#include "CentralWidget.h"

MainWindow::MainWindow()
{
    qDebug() << __PRETTY_FUNCTION__;
    QWidget * centralWidget = new CentralWidget();
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    qDebug() << __PRETTY_FUNCTION__;
}
