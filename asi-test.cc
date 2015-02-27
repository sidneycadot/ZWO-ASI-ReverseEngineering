
/////////////////
// asi-test.cc //
/////////////////

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

// TODO: report spelling of ASI_ERROR_INVALID_SENQUENCE
// TODO: report ASIGetNumOfConnectedCameras() return value weirdness
// TODO: report strange charcters in header file.

// Functions to obtain string representation of the enum constants defined in the API.

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

const char * toString(const ASI_GUIDE_DIRECTION direction)
{
    switch (direction)
    {
        case ASI_GUIDE_NORTH : return "ASI_GUIDE_NORTH";
        case ASI_GUIDE_SOUTH : return "ASI_GUIDE_SOUTH";
        case ASI_GUIDE_EAST  : return "ASI_GUIDE_EAST";
        case ASI_GUIDE_WEST  : return "ASI_GUIDE_WEST";
    }
    return "unknown";
}

const char * toString(const ASI_FLIP_STATUS flipStatus)
{
    switch (flipStatus)
    {
        case ASI_FLIP_NONE  : return "ASI_FLIP_NONE";
        case ASI_FLIP_HORIZ : return "ASI_FLIP_HORIZ";
        case ASI_FLIP_VERT  : return "ASI_FLIP_VERT";
        case ASI_FLIP_BOTH  : return "ASI_FLIP_BOTH";
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
        case ASI_ERROR_INVALID_CONTROL_TYPE : return "ASI_ERROR_INVALID_CONTROL_TYPE";
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
        case ASI_ERROR_GENERAL_ERROR        : return "ASI_ERROR_GENERAL_ERROR";
        case ASI_ERROR_END                  : return "ASI_ERROR_END";
    }
    return "unknown";
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

const char * toString(const ASI_CONTROL_TYPE controlType)
{
    switch (controlType)
    {
        case ASI_GAIN                : return "ASI_GAIN";
        case ASI_EXPOSURE            : return "ASI_EXPOSURE";
        case ASI_GAMMA               : return "ASI_GAMMA";
        case ASI_WB_R                : return "ASI_WB_R";
        case ASI_WB_B                : return "ASI_WB_B";
        case ASI_BRIGHTNESS          : return "ASI_BRIGHTNESS";
        case ASI_BANDWIDTHOVERLOAD   : return "ASI_BANDWIDTHOVERLOAD";
        case ASI_OVERCLOCK           : return "ASI_OVERCLOCK";
        case ASI_TEMPERATURE         : return "ASI_TEMPERATURE";
        case ASI_FLIP                : return "ASI_FLIP";
        case ASI_AUTO_MAX_GAIN       : return "ASI_AUTO_MAX_GAIN";
        case ASI_AUTO_MAX_EXP        : return "ASI_AUTO_MAX_EXP";
        case ASI_AUTO_MAX_BRIGHTNESS : return "ASI_AUTO_MAX_BRIGHTNESS";
        case ASI_HARDWARE_BIN        : return "ASI_HARDWARE_BIN";
        case ASI_HIGH_SPEED_MODE     : return "ASI_HIGH_SPEED_MODE";
        case ASI_COOLER_POWER_PERC   : return "ASI_COOLER_POWER_PERC";
        case ASI_TARGET_TEMP         : return "ASI_TARGET_TEMP";
    }
    return "unknown";
}

const char * toString(const ASI_EXPOSURE_STATUS exposureStatus)
{
    switch (exposureStatus)
    {
        case ASI_EXP_IDLE             : return "ASI_EXP_IDLE";
        case ASI_EXP_WORKING          : return "ASI_EXP_WORKING";
        case ASI_EXP_SUCCESS          : return "ASI_EXP_SUCCESS";
        case ASI_EXP_FAILED           : return "ASI_EXP_FAILED";
    }
    return "unknown";
}

// Check the error code as returned by an ASI function.
// Throw exception if the call was unsuccesful.

void check_errorcode(ASI_ERROR_CODE errorcode)
{
    if (errorcode != ASI_SUCCESS)
    {
        const string message = string("check_errorcode failed; errorcode = ") + to_string(errorcode) + " (" + toString(errorcode) + ")";
        throw runtime_error(message);
    }
}

// Show camera info

void show_AsiCameraInfo(const ASI_CAMERA_INFO & info)
{
    cout << "=== start of ASI_CAMERA_INFO" << endl;
    cout << endl;
    cout << "    Name ...................... : " << info.Name         << endl;
    cout << "    CameraID .................. : " << info.CameraID     << endl;
    cout << "    MaxHeight ................. : " << info.MaxHeight    << " [px]" << endl;
    cout << "    MaxWidth .................. : " << info.MaxWidth     << " [px]" << endl;
    cout << "    IsColorCam ................ : " << info.IsColorCam << " (" << toString(info.IsColorCam) << ")" << endl;
    cout << "    BayerPattern .............. : " << info.BayerPattern << " (" << toString(info.BayerPattern) << ")" << endl;

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

    cout << "    PixelSize ................. : " << info.PixelSize    << " [um]" << endl;
    cout << "    MechanicalShutter ......... : " << info.MechanicalShutter << " (" << toString(info.MechanicalShutter) << ")" << endl;
    cout << "    ST4Port ................... : " << info.ST4Port << " (" << toString(info.ST4Port) << ")" << endl;
    cout << "    IsCoolerCam ............... : " << info.IsCoolerCam << " (" << toString(info.IsCoolerCam) << ")" << endl;
    cout << "    IsUSB3Host ................ : " << info.IsUSB3Host << " (" << toString(info.IsUSB3Host) << ")" << endl;
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

// Tests

void test_GetCameraControlDescriptions(int CameraID)
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
        cout << "    MaxValue ............. : " << controlInfo.MaxValue << endl;
        cout << "    MinValue ............. : " << controlInfo.MinValue << endl;
        cout << "    DefaultValue ......... : " << controlInfo.DefaultValue << endl;
        cout << "    IsAutoSupported ...... : " << controlInfo.IsAutoSupported << endl;
        cout << "    IsWriteable .......... : " << controlInfo.IsWritable << endl;
        cout << "    ControlType .......... : " << controlInfo.ControlType << " (" << toString(controlInfo.ControlType) << ")" << endl;
        cout << endl;
    }
}

int bytes_per_pixel(const ASI_IMG_TYPE img_type)
{
    switch (img_type)
    {
        case ASI_IMG_RAW8  : return 1;
        case ASI_IMG_RGB24 : return 3;
        case ASI_IMG_RAW16 : return 2;
        case ASI_IMG_Y8    : return 1;
        default :
        {
            const string message = string("bytes_per_pixel failed; img_type = ") + to_string(img_type) + " (" + toString(img_type) + ")";
            throw runtime_error(message);
        }
    }
}

void test_GetCameraImages(const int CameraID, const unsigned count, bool writeToFile)
{
    const int PIX_WIDTH  = 1280;
    const int PIX_HEIGHT =  960;
    const int BINNING    =    1;
    const ASI_IMG_TYPE IMAGE_TYPE = ASI_IMG_RAW16;
    const int BYTES_PER_PIXEL = bytes_per_pixel(IMAGE_TYPE);

    ASI_ERROR_CODE errorcode;

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

    ofstream f;
    if (writeToFile)
    {
        f.open("images.raw", ios::binary);
    }

    for (unsigned frame = 0; frame < count; ++frame)
    {
        cout << "capturing image " << frame << endl;
        chrono::time_point<chrono::high_resolution_clock> t1 = chrono::high_resolution_clock::now();
        errorcode = ASIGetVideoData(CameraID, imageBuffer.data(), imageBuffer.size(), -1);
        chrono::time_point<chrono::high_resolution_clock> t2 = chrono::high_resolution_clock::now();
        check_errorcode(errorcode);

        const double duration = chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();
        const double bandwidth = imageBuffer.size() * 8 / duration / 1048576;

        cout << "received frame (" << duration << " s, " << bandwidth << " MBit/sec)" << endl;

        if (writeToFile)
        {
            f.write(reinterpret_cast<const char *>(imageBuffer.data()), imageBuffer.size());
        }
    }

    if (writeToFile)
    {
        f.close();
    }

    cout << "stopping video capture ..." << endl;
    errorcode = ASIStopVideoCapture(CameraID);
    check_errorcode(errorcode);
    cout << "done." << endl;
}

void test_GetControlValue(const int CameraID)
{
    const int num_controls = 17;

    const ASI_CONTROL_TYPE controls[num_controls] = {
        ASI_GAIN,
        ASI_EXPOSURE,
        ASI_GAMMA,
        ASI_WB_R,
        ASI_WB_B,
        ASI_BRIGHTNESS,
        ASI_BANDWIDTHOVERLOAD,  
        ASI_OVERCLOCK,
        ASI_TEMPERATURE,
        ASI_FLIP,
        ASI_AUTO_MAX_GAIN,
        ASI_AUTO_MAX_EXP,
        ASI_AUTO_MAX_BRIGHTNESS,
        ASI_HARDWARE_BIN,
        ASI_HIGH_SPEED_MODE,
        ASI_COOLER_POWER_PERC,
        ASI_TARGET_TEMP
    };

    for (int controlIndex = 0; controlIndex < num_controls; ++controlIndex)
    {
        const ASI_CONTROL_TYPE control = controls[controlIndex];

        for (int rep = 0; rep < 10; ++rep)
        {
            printf("ASIGetControlValue control %s\n", toString(control));
            long value;
            ASI_BOOL isAuto;
            ASI_ERROR_CODE errorcode = ASIGetControlValue(CameraID, control, &value, &isAuto);
            check_errorcode(errorcode);
            printf("ASIGetControlValue done: control %s value %ld auto %d\n", toString(control), value, isAuto);
        }
    }
}

void test_SetControlValue(const int CameraID)
{
    const int num_controls = 17;

    const ASI_CONTROL_TYPE controls[num_controls] = {
        ASI_GAIN,
        ASI_EXPOSURE,
        ASI_GAMMA,
        ASI_WB_R,
        ASI_WB_B,
        ASI_BRIGHTNESS,
        ASI_BANDWIDTHOVERLOAD,
        ASI_OVERCLOCK,
        ASI_TEMPERATURE,
        ASI_FLIP,
        ASI_AUTO_MAX_GAIN,
        ASI_AUTO_MAX_EXP,
        ASI_AUTO_MAX_BRIGHTNESS,
        ASI_HARDWARE_BIN,
        ASI_HIGH_SPEED_MODE,
        ASI_COOLER_POWER_PERC,
        ASI_TARGET_TEMP
    };

    for (int controlIndex = 0; controlIndex < num_controls; ++controlIndex)
    {
        const ASI_CONTROL_TYPE control = controls[controlIndex];

        for (int isAutoIndex = 0; isAutoIndex < 2; ++isAutoIndex)
        {
            for (long value = 20; value < 30; ++value)
            {
                ASI_BOOL isAuto = (isAutoIndex != 0) ? ASI_TRUE : ASI_FALSE;
                printf("ASISetControlValue control %s value %ld auto %d\n", toString(control), value, isAuto);
                ASI_ERROR_CODE errorcode = ASISetControlValue(CameraID, control, value, isAuto);
                check_errorcode(errorcode);
                printf("ASISetControlValue done.\n");
            }
        }
    }
}

void test_PulseGuide(const int CameraID)
{
    const int num_directions = 4;

    const ASI_GUIDE_DIRECTION directions[num_directions] = {ASI_GUIDE_NORTH, ASI_GUIDE_SOUTH, ASI_GUIDE_EAST, ASI_GUIDE_WEST};

    for (int rep = 0; rep < 10; ++rep)
    {
        for (int direction_index = 0; direction_index < num_directions; ++direction_index)
        {
            ASI_GUIDE_DIRECTION direction = directions[direction_index];

            cout << "ASIPulseGuideOn: " << toString(direction) << endl;
            ASI_ERROR_CODE errorcode = ASIPulseGuideOn(CameraID, direction);
            check_errorcode(errorcode);
            cout << "ASIPulseGuideOn done. " << endl;
        }

        for (int direction_index = 0; direction_index < num_directions; ++direction_index)
        {
            ASI_GUIDE_DIRECTION direction = directions[direction_index];
            cout << "ASIPulseGuideOff: " << toString(direction) << endl;
            ASI_ERROR_CODE errorcode = ASIPulseGuideOff(CameraID, direction);
            check_errorcode(errorcode);
            cout << "ASIPulseGuideOff done. " << endl;
        }
    }
}

void test_RoiFormat(const int CameraID)
{
    const int num_image_types = 4;
    const ASI_IMG_TYPE imageTypes[num_image_types] = {ASI_IMG_RAW8, ASI_IMG_RGB24, ASI_IMG_RAW16, ASI_IMG_Y8};

    ASI_ERROR_CODE errorcode;

    errorcode = ASISetStartPos(CameraID, 0, 0);
    check_errorcode(errorcode);

    // Setting the ROI appears to center the view (i.e., also change the StartPos).

    for (int setWidth = 128; setWidth <= 128; setWidth += 1)
    {
        for (int setHeight = 64; setHeight <= 64; setHeight += 1)
        {
            for (int setBin = 1; setBin <= 2; setBin += 1)
            {
                for (int imageTypeIndex = 0; imageTypeIndex < num_image_types; ++imageTypeIndex)
                {
                    const ASI_IMG_TYPE setImageType = imageTypes[imageTypeIndex];

                    cout << "ASISetROIFormat: width " << setWidth << " height " << setHeight << " bin " << setBin << " imageType " << setImageType << " (" << toString(setImageType) << ")" << endl;
                    errorcode = ASISetROIFormat(CameraID, setWidth, setHeight, setBin, setImageType);
                    if (errorcode == ASI_ERROR_INVALID_SIZE)
                    {
                        cout << "ASISetROIFormat done. Invalid size: " << setWidth << " " << setHeight << " " << setBin << " " << toString(setImageType) << endl;
                        continue;
                    }
                    check_errorcode(errorcode);
                    cout << "ASISetROIFormat done." << endl;

                    int getWidth, getHeight, getBin;
                    ASI_IMG_TYPE getImageType;

                    errorcode = ASIGetROIFormat(CameraID, &getWidth, &getHeight, &getBin, &getImageType);
                    check_errorcode(errorcode);

                    if (!(setWidth == getWidth && setHeight == getHeight && setBin == getBin && setImageType == getImageType))
                    {
                        cout << "****** MISMATCH ******" << endl;
                        cout << "ASIGetROIFormat reports: width " << getWidth << " height " << getHeight << " bin " << getBin << " imageType " << getImageType << " (" << toString(getImageType) << ")" << endl;
                        assert(0);
                    }
                }
            }
        }
    }
}

void test_StartPos(const int CameraID)
{
    ASI_ERROR_CODE errorcode;

    errorcode = ASISetROIFormat(CameraID, 400, 200, 2, ASI_IMG_RAW16);
    check_errorcode(errorcode);

    for (int setX = 50; setX <= 160; setX += 2)
    {
        for (int setY = 50; setY <= 160; setY += 2)
        {
            cout << "ASISetStartPos: x " << setX << " y " << setY << endl;
            errorcode = ASISetStartPos(CameraID, setX, setY);
            check_errorcode(errorcode);
            cout << "ASISetStartPos done." << endl;

            int getX, getY;

            errorcode = ASIGetStartPos(CameraID, &getX, &getY);
            check_errorcode(errorcode);

            if (!(setX == getX && setY == getY))
            {
                cout << "****** MISMATCH ******" << endl;
                cout << "ASIGetStartPos reports: x " << getX << " y " << getY << endl;
                assert(0);
            }
        }
    }
}

int rgen(int lo, int hi)
{
    assert(lo <= hi);
    return lo + rand() % (hi - lo + 1);
}

void test_StartPosAndROI(const int CameraID, const unsigned repetitions)
{
    cout << "START OF TEST" << endl;
    for (unsigned rep = 1; rep <= repetitions; ++rep)
    {
        cout << "START CYCLE " << rep << endl;
        if (rgen(0, 1) != 0)
        {
            int x = rgen(0 - 50, 1280 + 50);
            int y = rgen(0 - 50,  960 + 50);

            cout << "ASISetStartPos " <<  x << " " << y << endl;
            ASI_ERROR_CODE errorcode = ASISetStartPos(CameraID, x, y);
            cout << "ASISetStartPos exitcode " << errorcode << endl;
        }
        else
        {
            int w = rgen(0 - 50, 1280 + 50);
            int h = rgen(0 - 50,  960 + 50);
            int b = rgen(1 -  5,    2 +  5);
            ASI_IMG_TYPE i = static_cast<ASI_IMG_TYPE>(rgen(0 -  5,    3 +  5));

            cout << "ASISetROIFormat " <<  w << " " << h << " " << b << " " << i << endl;
            ASI_ERROR_CODE errorcode = ASISetROIFormat(CameraID, w, h, b, i);
            cout << "ASISetROIFormat exitcode " << errorcode << endl;
        }

        // report status
        {
            int w, h, b;
            ASI_IMG_TYPE i;
            ASI_ERROR_CODE errorcode = ASIGetROIFormat(CameraID, &w, &h, &b, &i);
            check_errorcode(errorcode);
            cout << "ASIGetROIFormat " <<  w << " " << h << " " << b << " " << i << endl;
        }

        {
            int x, y;
            ASI_ERROR_CODE errorcode = ASIGetStartPos(CameraID, &x, &y);
            check_errorcode(errorcode);
            cout << "ASIGetStartPos " <<  x << " " << y << endl;
        }
        cout << "END CYCLE " << rep << endl;
        cout << endl;
    }
    cout << "END OF TEST" << endl;
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

        cout << "getting camera info ..." << endl;
        errorcode = ASIGetCameraProperty(&info, i);
        check_errorcode(errorcode);
        cout << "done getting camera info." << endl;

        show_AsiCameraInfo(info);

        if (1)
        {
            cout << "opening camera ..." << endl;
            errorcode = ASIOpenCamera(info.CameraID);
            check_errorcode(errorcode);
            cout << "open done." << endl;

            // test_GetCameraControlDescriptions(info.CameraID);

            // test_RoiFormat(info.CameraID);
            // test_StartPos(info.CameraID);

            // test_GetControlValue(info.CameraID);

            // test_PulseGuide(info.CameraID);

            // test_StartPosAndROI(info.CameraID, 1000000);

            test_GetCameraImages(info.CameraID, 100, true);

            cout << "closing camera ..." << endl;
            errorcode = ASICloseCamera(info.CameraID);
            check_errorcode(errorcode);
            cout << "close done." << endl;
        }
    }

    return 0;
}
