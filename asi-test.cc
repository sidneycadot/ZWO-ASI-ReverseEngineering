#include "ASICamera2.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <unistd.h>
#include <stdexcept>

#include <pthread.h>

using namespace std;

/////////////////

//ASICAMERA_API int ASIGetNumOfConnectedCameras(); 
//ASICAMERA_API ASI_ERROR_CODE ASIGetCameraProperty(ASI_CAMERA_INFO *pASICameraInfo, int iCameraIndex);
//ASICAMERA_API ASI_ERROR_CODE ASIOpenCamera(int iCameraID);
//ASICAMERA_API ASI_ERROR_CODE ASICloseCamera(int iCameraID);
//ASICAMERA_API ASI_ERROR_CODE ASIGetNumOfControls(int iCameraID, int * piNumberOfControls);
//ASICAMERA_API ASI_ERROR_CODE ASIGetControlCaps(int iCameraID, int iControlIndex, ASI_CONTROL_CAPS * pControlCaps);

//ASICAMERA_API ASI_ERROR_CODE ASIGetControlValue(int  iCameraID, int  iControlID, long *plValue, ASI_BOOL *pbAuto);
//ASICAMERA_API ASI_ERROR_CODE ASISetControlValue(int  iCameraID, int  iControlID, long lValue, ASI_BOOL bAuto);

//ASICAMERA_API  ASI_ERROR_CODE ASISetROIFormat(int iCameraID, int iWidth, int iHeight,  int iBin, ASI_IMG_TYPE Img_type); 
//ASICAMERA_API  ASI_ERROR_CODE ASIGetROIFormat(int iCameraID, int *piWidth, int *piHeight,  int *piBin, ASI_IMG_TYPE *pImg_type); 

//ASICAMERA_API  ASI_ERROR_CODE ASISetStartPos(int iCameraID, int iStartX, int iStartY); 
//ASICAMERA_API  ASI_ERROR_CODE ASIGetStartPos(int iCameraID, int *piStartX, int *piStartY); 

//ASICAMERA_API  ASI_ERROR_CODE ASIGetDroppedFrames(int iCameraID,int *piDropFrames); 

//ASICAMERA_API ASI_ERROR_CODE ASIEnableDarkSubtract(int iCameraID, char *pcBMPPath, bool *bIsSubDarkWorking);
//ASICAMERA_API ASI_ERROR_CODE ASIDisableDarkSubtract(int iCameraID);

//ASICAMERA_API  ASI_ERROR_CODE ASIStartVideoCapture(int iCameraID);
//ASICAMERA_API  ASI_ERROR_CODE ASIStopVideoCapture(int iCameraID);

//ASICAMERA_API  ASI_ERROR_CODE ASIGetVideoData(int iCameraID, unsigned char* pBuffer, long lBuffSize, int iWaitms);

//ASICAMERA_API ASI_ERROR_CODE ASIPulseGuideOn(int iCameraID, ASI_GUIDE_DIRECTION direction);
//ASICAMERA_API ASI_ERROR_CODE ASIPulseGuideOff(int iCameraID, ASI_GUIDE_DIRECTION direction);

//ASICAMERA_API ASI_ERROR_CODE ASIIsUSB3Host(int iCameraID, ASI_BOOL *bSet);

/////////////////

const char * toString(const ASI_IMG_TYPE imageType)
{
    switch (imageType)
    {
        case ASI_IMG_RAW8  : return "ASI_IMG_RAW8";
        case ASI_IMG_RGB24 : return "ASI_IMG_RGB24";
        case ASI_IMG_RAW16 : return "ASI_IMG_RAW16";
        case ASI_IMG_Y8    : return "ASI_IMG_Y8";
        case ASI_IMG_END   : return "ASI_IMG_END";
    }
    return "unknown";
}

const char * toString(const ASI_ERROR_CODE errorcode)
{
    switch (errorcode)
    {
        case ASI_SUCCESS                    : return "ASI_SUCCESS";
        case ASI_ERROR_INVALID_INDEX        : return "ASI_ERROR_INVALID_INDEX";
        case ASI_ERROR_INVALID_ID           : return "ASI_ERROR_INVALID_ID";
        case ASI_ERROR_INVALID_CONTROL_ID   : return "ASI_ERROR_INVALID_CONTROL_ID";
        case ASI_ERROR_CAMERA_CLOSED        : return "ASI_ERROR_CAMERA_CLOSED";
        case ASI_ERROR_CAMERA_REMOVED       : return "ASI_ERROR_CAMERA_REMOVED";
        case ASI_ERROR_INVALID_PATH         : return "ASI_ERROR_INVALID_PATH";
        case ASI_ERROR_INVALID_FILEFORMAT   : return "ASI_ERROR_INVALID_FILEFORMAT";
        case ASI_ERROR_INVALID_SIZE         : return "ASI_ERROR_INVALID_SIZE";
        case ASI_ERROR_INVALID_IMGTYPE      : return "ASI_ERROR_INVALID_IMGTYPE";
        case ASI_ERROR_OUTOF_BOUNDARY       : return "ASI_ERROR_OUTOF_BOUNDARY";
        case ASI_ERROR_TIMEOUT              : return "ASI_ERROR_TIMEOUT";
        case ASI_ERROR_INVALID_SENQUENCE    : return "ASI_ERROR_INVALID_SENQUENCE";
        case ASI_ERROR_BUFFER_TOO_SMALL     : return "ASI_ERROR_BUFFER_TOO_SMALL";
        case ASI_ERROR_VIDEO_MODE_ACTIVE    : return "ASI_ERROR_VIDEO_MODE_ACTIVE";
        case ASI_ERROR_EXPOSURE_IN_PROGRESS : return "ASI_ERROR_EXPOSURE_IN_PROGRESS";
        case ASI_ERROR_END                  : return "ASI_ERROR_END";
    }
    return "unknown";
}

const char * toString(const ASI_Control_TYPE controltype)
{
    switch (controltype)
    {
        case ASI_GAIN                 : return "ASI_GAIN";
        case ASI_EXPOSURE             : return "ASI_EXPOSURE";
        case ASI_GAMMA                : return "ASI_GAMMA";
        case ASI_WB_R                 : return "ASI_WB_R";
        case ASI_WB_B                 : return "ASI_WB_B";
        case ASI_BRIGHTNESS           : return "ASI_BRIGHTNESS";
        case ASI_BANDWIDTHOVERLOAD    : return "ASI_BANDWIDTHOVERLOAD";
        case ASI_OVERCLOCK            : return "ASI_OVERCLOCK";
        case ASI_TEMPERATURE          : return "ASI_TEMPERATURE";
        case ASI_FLIP                 : return "ASI_FLIP";
        case ASI_AutoExpMaxGain       : return "ASI_AutoExpMaxGain";
        case ASI_AutoExpMaxExp        : return "ASI_AutoExpMaxExp";
        case ASI_AutoExpMaxBrightness : return "ASI_AutoExpMaxBrightness";
    }
    return "unknown";
}

const char * toString(const ASI_BAYER_PATTERN bayerPattern)
{
    switch (bayerPattern)
    {
        case ASI_BAYER_RG : return "ASI_BAYER_RG";
        case ASI_BAYER_BG : return "ASI_BAYER_BG";
        case ASI_BAYER_GR : return "ASI_BAYER_GR";
        case ASI_BAYER_GB : return "ASI_BAYER_GB";
    }
    return "unknown";
}

void check_errorcode(ASI_ERROR_CODE errorcode)
{
    if (errorcode == ASI_SUCCESS)
    {
        return;
    }

    cout << "runtime error " << errorcode << " (" << toString(errorcode) << ")." << endl;
    throw runtime_error("yikes");
}

const char * toString(const ASI_BOOL boolean)
{
    switch (boolean)
    {
        case ASI_FALSE : return "ASI_FALSE";
        case ASI_TRUE  : return "ASI_TRUE";
    }
    return "unknown";
}

void show_camera_info(const ASI_CAMERA_INFO & info)
{
    cout << "=== start of ASI_CAMERA_INFO" << endl;
    cout << endl;
    cout << "    Name ...................... : " << info.Name         << endl;
    cout << "    CameraID .................. : " << info.CameraID     << endl;
    cout << "    MaxHeight ................. : " << info.MaxHeight    << " [px]" << endl;
    cout << "    MaxWidth .................. : " << info.MaxWidth     << " [px]" << endl;
    cout << "    IsColorCam ................ : " << info.IsColorCam << " (" << toString(info.IsColorCam) << ")" << endl;
    cout << "    BayerPattern .............. : " << info.BayerPattern << " (" << toString(info.BayerPattern) << ")" << endl;
    cout << "    PixelSize ................. : " << info.PixelSize    << " [um]" << endl;

    cout << "    SupportedBins ............. : {";
    for (int i = 0; i < 16; ++i)
    {
        if (info.SupportedBins[i] == 0)
        {
            break; // 0 denotes end-of-list
        }
        if (i != 0)
        {
            cout << ", ";
        }
        cout << info.SupportedBins[i];
    }
    cout << "}" << endl;

    cout << "    SupportedVideoFormat ...... : {";
    for (int i = 0; i < 8; ++i)
    {
        if (info.SupportedVideoFormat[i] == ASI_IMG_END) // ASI_IMG_END denotes end-of-list
        {
            break; // 0 denotes end of list
        }
        if (i != 0)
        {
            cout << ", ";
        }
        cout << info.SupportedVideoFormat[i] << " (" << toString(info.SupportedVideoFormat[i]) << ")";
    }
    cout << "}" << endl;

    cout << "    MechanicalShutter ......... : " << info.MechanicalShutter << " (" << toString(info.MechanicalShutter) << ")" << endl;
    cout << "    ST4Port ................... : " << info.ST4Port << " (" << toString(info.ST4Port) << ")" << endl;
    cout << endl;
    cout << "=== end of ASI_CAMERA_INFO" << endl;
    cout << endl;
}

void show_camera_roi_format(int CameraID)
{
    int width, height, bin;
    ASI_IMG_TYPE imageType;

    ASI_ERROR_CODE errorcode = ASIGetROIFormat(CameraID, &width, &height, &bin, &imageType);
    check_errorcode(errorcode);

    cout << "=== start of ROI format parameters" << endl;
    cout << endl;
    cout << "    width .......... : " << width     << endl;
    cout << "    height ......... : " << height    << endl;
    cout << "    bin ............ : " << bin       << endl;
    cout << "    imageType ...... : " << imageType << " (" << toString(imageType) << ")" << endl;
    cout << endl;
    cout << "=== end of ASI_CAMERA_INFO" << endl;
    cout << endl;
}

void show_camera_controls(int CameraID)
{
    // This only works on an open camera.

    cout << "querying number of controls ..." << endl;
    int numberOfControls;
    ASI_ERROR_CODE errorcode = ASIGetNumOfControls(CameraID, &numberOfControls);
    check_errorcode(errorcode);

    cout << "done. Number of controls found: " << numberOfControls << endl;

    for (int i = 0; i < numberOfControls; ++i)
    {
        ASI_CONTROL_CAPS controlInfo;

        ASI_ERROR_CODE errorcode = ASIGetControlCaps(CameraID, i, &controlInfo);
        check_errorcode(errorcode);

        cout << "=== control " << i << ":" << endl;
        cout << endl;
        cout << "    Name ................. : " << controlInfo.Name << endl;
        cout << "    Description .......... : " << controlInfo.Description << endl;
        cout << "    ControlID ............ : " << controlInfo.ControlID << endl;
        cout << "    MaxValue ............. : " << controlInfo.MaxValue << endl;
        cout << "    MinValue ............. : " << controlInfo.MinValue << endl;
        cout << "    DefaultValue ......... : " << controlInfo.DefaultValue << endl;
        cout << "    IsAutoSupported ...... : " << controlInfo.IsAutoSupported << endl;
        cout << "    IsWriteable .......... : " << controlInfo.IsWritable << endl;
        cout << "    ControlType .......... : " << controlInfo.ControlType << " (" << toString(controlInfo.ControlType) << ")" << endl;
        cout << endl;
    }
}

void get_camera_images(int CameraID, unsigned count)
{
    const int PIX_WIDTH = 1280;
    const int PIX_HEIGHT = 960;
    const int BINNING  = 1;
    const ASI_IMG_TYPE IMAGE_TYPE = ASI_IMG_RAW16;
    const int BYTES_PER_PIXEL = 2;

    ASI_ERROR_CODE errorcode;

    // Limit USB usage
    //rc = ASISetControlValue(CameraID, 6, 10, ASI_FALSE);
    //assert(rc == ASI_SUCCESS);

    show_camera_roi_format(CameraID);

    cout << "set ROI format ..." << endl;
    errorcode = ASISetROIFormat(CameraID, PIX_WIDTH, PIX_HEIGHT, BINNING, IMAGE_TYPE);
    check_errorcode(errorcode);
    cout << "done set ROI format." << endl;

    show_camera_roi_format(CameraID);

    vector<unsigned char> imageBuffer(PIX_HEIGHT * PIX_WIDTH * BYTES_PER_PIXEL);

    cout << "starting video capture ..." << endl;
    errorcode = ASIStartVideoCapture(CameraID);
    check_errorcode(errorcode);
    cout << "done." << endl;

    for (unsigned frame = 0; frame < count; ++frame)
    {
        cout << "capturing image " << frame << endl;
        chrono::time_point<chrono::high_resolution_clock> t1 = chrono::high_resolution_clock::now();
        errorcode = ASIGetVideoData(CameraID, imageBuffer.data(), imageBuffer.size(), -1);
        chrono::time_point<chrono::high_resolution_clock> t2 = chrono::high_resolution_clock::now();
        check_errorcode(errorcode);

        double duration = chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();
        double bw = imageBuffer.size() * 8 / duration / 1048576;

        cout << "received frame (" << duration << " s, " << bw << " MBit/sec)" << endl;

        //ostringstream out;
        //out << "image_" << setw(6) << setfill('0') << frame << ".raw";

        //ofstream f(out.str());
        //f.write(reinterpret_cast<const char *>(imageBuffer.data()), imageBuffer.size());
        //f.close();
    }

    cout << "stopping video capture ..." << endl;
    errorcode = ASIStopVideoCapture(CameraID);
    check_errorcode(errorcode);
    cout << "done." << endl;
}

int main()
{
    cout << "main thread: " << pthread_self() << endl;

    cout << "query number of connected cameras..." << endl;
    int numberOfConnectedCameras = ASIGetNumOfConnectedCameras();
    cout << "number of connected cameras: " << numberOfConnectedCameras << endl;

    for (int i = 0; i < numberOfConnectedCameras; ++i)
    {
        ASI_ERROR_CODE errorcode;

        ASI_CAMERA_INFO info;

        // This is done without opening the camera.
        // The ASIGetCameraProperty() call must be preceded by ASIGetNumOfConnectedCameras().
        errorcode = ASIGetCameraProperty(&info, i);
        check_errorcode(errorcode);

        show_camera_info(info);

        if (1)
        {
            cout << "opening camera ..." << endl;
            errorcode = ASIOpenCamera(info.CameraID);
            check_errorcode(errorcode);
            cout << "open done." << endl;

            show_camera_controls(info.CameraID);

            get_camera_images(info.CameraID, 3);

            cout << "closing camera ..." << endl;
            errorcode = ASICloseCamera(info.CameraID);
            check_errorcode(errorcode);
            cout << "close done." << endl;
        }
    }

    return 0;
}
