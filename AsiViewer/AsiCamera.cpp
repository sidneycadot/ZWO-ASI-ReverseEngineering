
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
    for (AsiCameraControl * cameraControl : cameraControls)
    {
        delete cameraControl;
    }
    cameraControls.clear();
}

int AsiCamera::getCameraIndex() const
{
    return cameraIndex;
}

const char * AsiCamera::getName() const
{
    return cameraInfo.Name;
}

int AsiCamera::getCameraID() const
{
    return cameraInfo.CameraID;
}

long AsiCamera::getMaxWidth() const
{
    return cameraInfo.MaxWidth;
}

long AsiCamera::getMaxHeight() const
{
    return cameraInfo.MaxHeight;
}

bool AsiCamera::getIsColorCam() const
{
    return cameraInfo.IsColorCam;
}

ASI_BAYER_PATTERN AsiCamera::getBayerPattern() const
{
    return cameraInfo.BayerPattern;
}

std::vector<int> AsiCamera::getSupportedBins() const
{
    std::vector<int> bins;

    for (int i = 0; i < 16; ++i)
    {
        if (cameraInfo.SupportedBins[i] == 0)
        {
            break;
        }
        bins.push_back(cameraInfo.SupportedBins[i]);
    }
    return bins;
}

std::vector<ASI_IMG_TYPE> AsiCamera::getSupportedVideoFormats() const
{
    std::vector<ASI_IMG_TYPE> supportedVideoFormats;

    for (int i = 0; i < 8; ++i)
    {
        if (cameraInfo.SupportedVideoFormat[i] == ASI_IMG_END)
        {
            break;
        }
        supportedVideoFormats.push_back(cameraInfo.SupportedVideoFormat[i]);
    }

    return supportedVideoFormats;
}

double AsiCamera::getPixelSize() const
{
    return cameraInfo.PixelSize;
}

bool AsiCamera::getMechanicalShutter() const
{
    return cameraInfo.MechanicalShutter;
}

bool AsiCamera::getST4Port() const
{
    return cameraInfo.ST4Port;
}

bool AsiCamera::getIsCoolerCam() const
{
    return cameraInfo.IsCoolerCam;
}

bool AsiCamera::getIsUSB3Host() const
{
    return cameraInfo.IsUSB3Host;
}

void AsiCamera::open()
{
    ASI_ERROR_CODE errorcode;

    errorcode = ASIOpenCamera(cameraInfo.CameraID);
    check_asi_errorcode(errorcode);

    // The camera is now opened. Query the set off controls.

    int numberOfControls;
    errorcode = ASIGetNumOfControls(cameraInfo.CameraID, &numberOfControls);
    check_asi_errorcode(errorcode);

    for (int controlIndex = 0; controlIndex < numberOfControls; ++controlIndex)
    {
        AsiCameraControl * cameraControl = new AsiCameraControl(cameraInfo.CameraID, controlIndex);
        cameraControls.push_back(cameraControl);
    }
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

void AsiCamera::setROIFormat(int width, int height, int bin, ASI_IMG_TYPE img_type)
{
    ASI_ERROR_CODE errorcode = ASISetROIFormat(cameraInfo.CameraID, width, height, bin, img_type);
    check_asi_errorcode(errorcode);
}

void AsiCamera::getROIFormat(int * p_width, int * p_height, int * p_bin, ASI_IMG_TYPE * p_img_type)
{
    int width, height, bin;
    ASI_IMG_TYPE img_type;

    ASI_ERROR_CODE errorcode = ASIGetROIFormat(cameraInfo.CameraID, &width, &height, &bin, &img_type);
    check_asi_errorcode(errorcode);

    if (p_width != nullptr)
    {
        *p_width = width;
    }

    if (p_height != nullptr)
    {
        *p_height = height;
    }

    if (p_bin != nullptr)
    {
        *p_bin = bin;
    }

    if (p_img_type != nullptr)
    {
        *p_img_type = img_type;
    }
}

int AsiCamera::getNumberOfCameraControls() const
{
    return cameraControls.size();
}

AsiCameraControl * AsiCamera::getCameraControlByIndex(int controlIndex) const
{
    return cameraControls.at(controlIndex);
}

AsiCameraControl * AsiCamera::getCameraControlByControlType(ASI_CONTROL_TYPE controlType) const
{
    for (AsiCameraControl * cameraControl : cameraControls)
    {
        if (cameraControl->getControlType() == controlType)
        {
            return cameraControl;
        }
    }

    return nullptr;
}
