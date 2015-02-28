
/////////////////////
// CentralWidget.h //
/////////////////////

#ifndef CentralWidget_h
#define CentralWidget_h

#include <QWidget>
#include <QLabel>

#include "AsiThread.h"

class CentralWidget : public QWidget
{
    Q_OBJECT

    public:

        CentralWidget();
        virtual ~CentralWidget();

    private slots:

        void updateImage(const QByteArray &);

    private:

        AsiThread * asiThread;
        QLabel * label;
};

#endif // CentralWidget_h
