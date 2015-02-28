
/////////////////
// AsiThread.h //
/////////////////

#ifndef AsiThread_h
#define AsiThread_h

#include <QThread>

class AsiThread : public QThread
{
    Q_OBJECT

    signals:

        void receivedImage(const QByteArray & imageBytes);

    public:

        AsiThread();
        virtual ~AsiThread();
        virtual void run();

        void requestQuit();

    private:

        void getImage(const int CameraID);
        bool quitFlag;
};

#endif // AsiThread_h
