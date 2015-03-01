
///////////////////
// AsiCamera.cpp //
///////////////////

#include "AsiCamera.h"
#include "AsiUtils.h"

AsiCamera::AsiCamera(int cameraIndex) : cameraIndex(cameraIndex)
{
    // This is done without opening the camera.
    // The ASIGetCameraProperty() call must be preceded by ASIGetNumOfConnectedCameras().

    ASI_ERROR_CODE errorcode = ASIGetCameraProperty(&cameraInfo, cameraIndex);
    check_asi_errorcode(errorcode);
}

AsiCamera::~AsiCamera()
{
}

const ASI_CAMERA_INFO & AsiCamera::getCameraInfo() const
{
    return cameraInfo;
}

void AsiCamera::open()
{
    ASI_ERROR_CODE errorcode = ASIOpenCamera(cameraInfo.CameraID);
    check_asi_errorcode(errorcode);
}

void AsiCamera::close()
{
    ASI_ERROR_CODE errorcode = ASICloseCamera(cameraInfo.CameraID);
    check_asi_errorcode(errorcode);
}

void AsiCamera::startVideoCapture()
{
    ASI_ERROR_CODE errorcode = ASIStartVideoCapture(cameraInfo.CameraID);
    check_asi_errorcode(errorcode);
}

void AsiCamera::stopVideoCapture()
{
    ASI_ERROR_CODE errorcode = ASIStopVideoCapture(cameraInfo.CameraID);
    check_asi_errorcode(errorcode);
}
