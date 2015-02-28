
/////////////////////
// Application.cpp //
/////////////////////

#include <QDebug>

#include "Application.h"

Application::Application(int & argc, char ** argv) : QApplication(argc, argv)
{
    qDebug() << __PRETTY_FUNCTION__;

    mainWindow = new MainWindow();
    mainWindow->show();
}

Application::~Application()
{
    qDebug() << __PRETTY_FUNCTION__;
    delete mainWindow;
}
