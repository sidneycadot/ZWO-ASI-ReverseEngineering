
/////////////////////
// Application.cpp //
/////////////////////

#include <QDebug>

#include "Application.h"

Application::Application(int & argc, char ** argv) : QApplication(argc, argv)
{
    qDebug() << __PRETTY_FUNCTION__;

    asiCameraManager = new AsiCameraManager();

    mainWindow = new MainWindow();
    mainWindow->show();
}

Application::~Application()
{
    qDebug() << __PRETTY_FUNCTION__;
    delete mainWindow;
    delete asiCameraManager;
}

AsiCameraManager * Application::getAsiCameraManager() const
{
    return asiCameraManager;
}

Application * application()
{
    return reinterpret_cast<Application *>(qApp);
}
