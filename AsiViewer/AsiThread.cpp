
///////////////////
// AsiThread.cpp //
///////////////////

#include <QDebug>

#include <string>
#include <stdexcept>

#include "ASICamera2.h"

#include "AsiThread.h"
#include "AsiUtils.h"
#include "AsiCameraManager.h"
#include "AsiCamera.h"
#include "Application.h"

AsiThread::AsiThread(QObject * parent) : QThread(parent)
{
    qDebug() << __PRETTY_FUNCTION__;
}

AsiThread::~AsiThread()
{
    qDebug() << __PRETTY_FUNCTION__;
    quitGetImageLoop = true; // request that the image acquisition loop is stopped.
    wait();
}

void AsiThread::run()
{
    const int PIX_WIDTH  = 1280;
    const int PIX_HEIGHT =  960;
    const int BINNING    =    1;
    const ASI_IMG_TYPE IMAGE_TYPE = ASI_IMG_RAW16;

    qDebug() << __PRETTY_FUNCTION__;

    AsiCameraManager * cameraManager = application()->getAsiCameraManager();
    AsiCamera * camera = cameraManager->getCamera(0);

    camera->open();

    int CameraID = camera->getCameraID();

    camera->getCameraControlByControlType(ASI_EXPOSURE)->setControlValue(1, false);

    camera->setROIFormat(PIX_WIDTH, PIX_HEIGHT, BINNING, IMAGE_TYPE);

    camera->startVideoCapture();

    quitGetImageLoop = false;
    while (!quitGetImageLoop)
    {
        getImage(CameraID);
    }

    camera->stopVideoCapture();
    camera->close();
}

void AsiThread::getImage(const int CameraID)
{
    const int PIX_WIDTH  = 1280;
    const int PIX_HEIGHT =  960;
    const ASI_IMG_TYPE IMAGE_TYPE = ASI_IMG_RAW16;
    const int BYTES_PER_PIXEL = bytes_per_pixel(IMAGE_TYPE);

    qDebug() << __PRETTY_FUNCTION__;

    QByteArray byteArray(PIX_HEIGHT * PIX_WIDTH * BYTES_PER_PIXEL, 0);
    ASI_ERROR_CODE errorcode = ASIGetVideoData(CameraID, reinterpret_cast<unsigned char *>(byteArray.data()), byteArray.size(), -1);
    check_asi_errorcode(errorcode);

    emit receivedImage(byteArray);
}
