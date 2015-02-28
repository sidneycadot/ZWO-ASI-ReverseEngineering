
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

        AsiThread(QObject * parent = nullptr);
        virtual ~AsiThread();
        virtual void run();

    private:

        void getImage(const int CameraID);
        bool quitGetImageLoop;
};

#endif // AsiThread_h
