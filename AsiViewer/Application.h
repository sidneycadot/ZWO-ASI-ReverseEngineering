
///////////////////
// Application.h //
///////////////////

#ifndef Application_h
#define Application_h

#include <QApplication>
#include "MainWindow.h"
#include "AsiCameraManager.h"

class Application : public QApplication
{
    public:
        Application(int & argc, char ** argv);
        ~Application();

        AsiCameraManager * getAsiCameraManager() const;

    private:

        MainWindow * mainWindow;
        AsiCameraManager * asiCameraManager;
};

Application * application(); // return the Application singleton

#endif // Application_h
