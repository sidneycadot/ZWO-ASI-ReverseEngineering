
///////////////////////////////////////
// libASICamera2_ReverseEngineered.c //
///////////////////////////////////////

// NOTE: The implementation of this library is based on the externally observed
//       behavior of the proprietary ASICamera2 library (i.e., its calls to libusb).
//       That behavior is normative for now; we try to mimic it as closely as possible,
//       even though some improvements could be made.

#include "ASICamera2_patched.h"
#include "libUSB.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

// TODO: The comment on the return value of ASIGetNumOfConnectedCameras() is wrong in ASICamera2.h.
//       Report this.

int ASIGetNumOfConnectedCameras()
{
    const unsigned ASI_VENDOR_ID  = 0x03c3;
    const unsigned ASI_PRODUCT_ID = 0x120d;

    (void)ASI_VENDOR_ID; // unused
    (void)ASI_PRODUCT_ID; // unused

    int numberOfCameras = 0;
    bool libusb_initialized = false;

    // Open the USB library

    int retval = libUSB_init(NULL);
    if (retval == LIBUSB_SUCCESS)
    {
        libusb_initialized = true;

        libusb_device ** devices;
        bool have_valid_device_list = false;

        ssize_t numberOfDevices = libUSB_get_device_list(NULL, &devices);
        if (numberOfDevices >= 0)
        {
            have_valid_device_list = true;
        }

        for (int i = 0; i < numberOfDevices; ++i)
        {
            libusb_device * device = devices[i];

            struct libusb_device_descriptor descriptor;

            enum libusb_error retval = libusb_get_device_descriptor(device, &descriptor);
            assert(retval == LIBUSB_SUCCESS);

            const uint8_t device_address = libUSB_get_device_address(device);
            const uint8_t bus_number = libUSB_get_bus_number(device);

            (void)device_address; // unused
            (void)bus_number; // unused

            //printf("==> device %2d Bus %03d Device %03d: ID %04x:%04x\n",
            //        i, bus_number, device_address, descriptor.idVendor, descriptor.idProduct);

            if (descriptor.idVendor == ASI_VENDOR_ID && descriptor.idProduct == ASI_PRODUCT_ID)
            {
                //TODO: maintain a list of cameras found.
                ++numberOfCameras;
            }
        }

        // free the device list
        if (have_valid_device_list)
        {
            libUSB_free_device_list(devices, 1);
        }
    }

    if (libusb_initialized)
    {
        libUSB_exit(NULL);
    }

    return numberOfCameras; // number of cameras
}

ASI_ERROR_CODE ASIGetCameraProperty(ASI_CAMERA_INFO * pASICameraInfo, int iCameraIndex)
{
    (void)pASICameraInfo; // unused
    (void)iCameraIndex; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIOpenCamera(int iCameraID)
{
    (void)iCameraID; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASICloseCamera(int iCameraID)
{
    (void)iCameraID; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIGetNumOfControls(int iCameraID, int * piNumberOfControls)
{
    (void)iCameraID; // unused
    (void)piNumberOfControls; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIGetControlCaps(int iCameraID, int iControlIndex, ASI_CONTROL_CAPS * pControlCaps)
{
    (void)iCameraID; // unused
    (void)iControlIndex; // unused
    (void)pControlCaps; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIGetControlValue(int iCameraID, int iControlID, long * plValue, ASI_BOOL * pbAuto)
{
    (void)iCameraID; // unused
    (void)iControlID; // unused
    (void)plValue; // unused
    (void)pbAuto; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASISetControlValue(int iCameraID, int iControlID, long lValue, ASI_BOOL bAuto)
{
    (void)iCameraID; // unused
    (void)iControlID; // unused
    (void)lValue; // unused
    (void)bAuto; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASISetROIFormat(int iCameraID, int iWidth, int iHeight, int iBin, ASI_IMG_TYPE Img_type)
{
    (void)iCameraID; // unused
    (void)iWidth; // unused
    (void)iHeight; // unused
    (void)iBin; // unused
    (void)Img_type; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIGetROIFormat(int iCameraID, int * piWidth, int * piHeight, int * piBin, ASI_IMG_TYPE * pImg_type)
{
    (void)iCameraID; // unused
    (void)piWidth; // unused
    (void)piHeight; // unused
    (void)piBin; // unused
    (void)pImg_type; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASISetStartPos(int iCameraID, int iStartX, int iStartY)
{
    (void)iCameraID; // unused
    (void)iStartX;   // unused
    (void)iStartY;   // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIGetStartPos(int iCameraID, int * piStartX, int * piStartY)
{
    (void)iCameraID; // unused
    (void)piStartX;  // unused
    (void)piStartY;  // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIGetDroppedFrames(int iCameraID, int * piDropFrames)
{
    (void)iCameraID; // unused
    (void)piDropFrames; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIEnableDarkSubtract(int iCameraID, char * pcBMPPath, bool * bIsSubDarkWorking)
{
    (void)iCameraID; // unused
    (void)pcBMPPath; // unused
    (void)bIsSubDarkWorking; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIDisableDarkSubtract(int iCameraID)
{
    (void)iCameraID; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIStartVideoCapture(int iCameraID)
{
    (void)iCameraID; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIStopVideoCapture(int iCameraID)
{
    (void)iCameraID; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIGetVideoData(int iCameraID, unsigned char * pBuffer, long lBuffSize, int iWaitms)
{
    (void)iCameraID; // unused
    (void)pBuffer; // unused
    (void)lBuffSize; // unused
    (void)iWaitms; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIPulseGuideOn(int iCameraID, ASI_GUIDE_DIRECTION direction)
{
    (void)iCameraID; // unused
    (void)direction; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIPulseGuideOff(int iCameraID, ASI_GUIDE_DIRECTION direction)
{
    (void)iCameraID; // unused
    (void)direction; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIIsUSB3Host(int iCameraID, ASI_BOOL * bSet)
{
    (void)iCameraID; // unused
    (void)bSet; // unused

    return ASI_SUCCESS;
}
