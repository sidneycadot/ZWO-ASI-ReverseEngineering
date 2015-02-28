
///////////////////
// Application.h //
///////////////////

#ifndef Application_h
#define Application_h

#include <QApplication>
#include "MainWindow.h"

class Application : public QApplication
{
    public:
        Application(int & argc, char ** argv);
        ~Application();
    private:
        MainWindow * mainWindow;
};

#endif // Application_h
