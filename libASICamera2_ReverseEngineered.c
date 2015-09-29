
///////////////////////////////////////
// libASICamera2_ReverseEngineered.c //
///////////////////////////////////////

// NOTE: The implementation of this library is based on the externally observed
//       behavior of the proprietary ASICamera2 library (i.e., its calls to libusb).
//       That behavior is normative for now; we try to mimic it as closely as possible,
//       even though some improvements could be made.

#include "ASICamera2.h"
#include "libUSB.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: The comment on the return value of ASIGetNumOfConnectedCameras() is wrong in ASICamera2.h.
//       Report this.

struct CameraInfo {
    uint8_t bus_number;
    uint8_t device_address;
};

static struct CameraInfo * caminfo = NULL;
static int numberOfConnectedCameras = 0;

static bool cleanup_atexit_registered = false;

static void cleanup_atexit(void)
{
    free(caminfo); // no-op if caminfo is NULL
    numberOfConnectedCameras = 0;
}

static void show_caminfo(void)
{
    printf("ASI USB cameras detected:\n");
    for (int i = 0; i < numberOfConnectedCameras; ++i)
    {
        printf("    [%3d] bus = %3d device_address = %3d\n", i, caminfo[i].bus_number, caminfo[i].device_address);
    }
    printf("(end of list)\n");
}

int ASIGetNumOfConnectedCameras()
{
    // Register cleanup function to deallocate caminfo.

    if (!cleanup_atexit_registered)
    {
        int retval = atexit(cleanup_atexit);
        if (retval == 0)
        {
            // succesfully registered to cleanup function.
            // Note that if it fails, we will try again next time.
            cleanup_atexit_registered = true;
        }
    }

    const unsigned ASI_VENDOR_ID  = 0x03c3;
    const unsigned ASI_PRODUCT_ID = 0x120d;

    // Remove existing caminfo.
    free(caminfo); // no-op if caminfo is NULL
    caminfo = NULL;
    numberOfConnectedCameras = 0;

    // initialize USB library using the default context.

    int retval = libUSB_init(NULL);
    if (retval == LIBUSB_SUCCESS)
    {
        libusb_device ** devices;

        ssize_t device_count = libUSB_get_device_list(NULL, &devices);
        if (device_count >= 0)
        {
            for (int i = 0; i < device_count; ++i)
            {
                libusb_device * device = devices[i];

                // Obtain the device's descriptor.
                // If we fail to obtain the device descriptor, silently skip this device.
                struct libusb_device_descriptor descriptor;
                enum libusb_error retval = libusb_get_device_descriptor(device, &descriptor);

                if (retval == LIBUSB_SUCCESS)
                {
                    if (descriptor.idVendor == ASI_VENDOR_ID && descriptor.idProduct == ASI_PRODUCT_ID)
                    {
                        // Make room for this camera.
                        // If the realloc fails, we will silently skip this camera.
                        struct CameraInfo * resized_caminfo = realloc(caminfo, sizeof(struct CameraInfo) * (numberOfConnectedCameras + 1));
                        if (resized_caminfo != NULL)
                        {
                            // realloc succeeded.
                            caminfo = resized_caminfo;
                            caminfo[numberOfConnectedCameras].bus_number     = libUSB_get_bus_number(device);
                            caminfo[numberOfConnectedCameras].device_address = libUSB_get_device_address(device);
                            numberOfConnectedCameras += 1;
                        }
                    }
                }
            }

            // Free the device list.
            libUSB_free_device_list(devices, 1);
        }

        // De-initialize USB library.
        libUSB_exit(NULL);
    }

    show_caminfo();
    return numberOfConnectedCameras;
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

ASI_ERROR_CODE ASIEnableDarkSubtract(int iCameraID, char * pcBMPPath, ASI_BOOL * bIsSubDarkWorking)
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

ASI_ERROR_CODE ASIStartExposure(int iCameraID, ASI_BOOL bIsDark)
{
    (void)iCameraID; // unused
    (void)bIsDark; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIStopExposure(int iCameraID)
{
    (void)iCameraID; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIGetExpStatus(int iCameraID, ASI_EXPOSURE_STATUS * pExpStatus)
{
    (void)iCameraID; // unused
    (void)pExpStatus; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIGetDataAfterExp(int iCameraID, unsigned char * pBuffer, long lBuffSize)
{
    (void)iCameraID; // unused
    (void)pBuffer; // unused
    (void)lBuffSize; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASIGetID(int iCameraID, ASI_ID * pID)
{
    (void)iCameraID; // unused
    (void)pID; // unused

    return ASI_SUCCESS;
}

ASI_ERROR_CODE ASISetID(int iCameraID, ASI_ID ID)
{
    (void)iCameraID; // unused
    (void)ID; // unused

    return ASI_SUCCESS;
}
