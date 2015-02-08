# ZWO-ASI-ReverseEngineering

This repository contains efforts to reverse-engineer the protocol of the ZWO ASI120MM-S camera.

## List of files

filename                                   | description
-------------------------------------------|--------------------------------------------------------------------------------------------------------------
README.md                                  | Description of the library
MD5SUM                                     | contains MD5 checksums of the original library and include files (v0.0.918).
ASICamera2.h.orig                          | The original header file
libASICamera2.a.orig                       | The original static library
libASICamera2.so.0.0.0918.orig             | The original dynamic library
ISSUES.txt                                 | A text file describing several issues with the current include file and library
ASICamera2.h                               | A hand-edited version of the library include file to fix some problems (see ISSUES.txt).
patch-library.py                           | A script to make patched versions of the library where all references to "libusb" are replaced by "libUSB".
libASICamera2.a                            | The patched static library; uses "libUSB_<xxx>" rather than "libusb_<xxx>" calls.
libASICamera2.so.0.0.0918                  | The patched dynamic library; uses "libUSB_<xxx>" rather than "libusb_<xxx>" calls.
asi-test.cc                                | A test program that controls the camera and captures a bunch of images.
libUSB.c                                   | A set of functions that mimic libusb behavior, printing invocations and results along the way.
Makefile                                   | Makefile for asi-test, using the libUSB.c functions.

## Methodology of reverse-engineering the library

The proprietary ASICamera2 library uses libusb 1.0 (the modern version of the user-space USB library) to
communicate with devices.

To understand the way in which the library uses libusb, we generated a patched version of the ASICamera2 library
where all references to "libusb" are replaced by "libUSB".

Next, we implement a "libUSB.c" that re-implements 15 of the "libusb" functions, but renames them with the prefix "libUSB".
These 15 functions print their arguments, call the actual libusb functions, and then print the return value of the libusb functions.

This effectively means that all usage of libusb by the ASICamera2 is now logged.

## What we learned so far

The "ASICamera2" API currently consists of 21 function calls.
We describe them below and discuss below what they do in terms of USB bus activity.

All functions use the 'default' context of libusb, meaning that they pass a NULL pointer as the 'context' argument
wherever it is needed.

##### int ASIGetNumOfConnectedCameras()

The ASIGetNumOfConnectedCameras() call is intended to be the first function called in a program that
uses the ASICamera2 library.


It starts by executing a libusb_init() call. Next it executes libusb_get_device_list() to obtain a list of
all USB devices available in the system.

Next, it traverses the list of all devices. For each device, three libusb functions are called:

- libusb_get_device_descriptor()
- libusb_get_device_address()
- libusb_get_bus_number()

This allows the ASICamera2 library to find all ZWO ASI devices, and build an internal data structure to describe
where they are.

Finally, the device list is freed by executing libusb_free_device_list(), and libusb_exit() is called to detach from
libusb.

Note that this is the only function in the API that does not return an ASI_ERROR_CODE. Instead, it returns the
number of ASI camera devices found.

##### ASI_ERROR_CODE ASIGetCameraProperty(ASI_CAMERA_INFO *pASICameraInfo, int iCameraIndex)

This function fills a user-supplied ASI_CAMERA_INFO structure with information about an ASI camera device.

This function only works if ASIGetNumOfConnectedCameras() was executed beforehand. If it is called before, an
ASI_ERROR_INVALID_ID is returned, presumably because the internal list of devices maintained by ASICamera2
contains zero devices.

This function does not cause any traffic on the USB bus. This suggests that all information is contained
as a lookup table in the library.

The ASI_CAMERA_INFO contains the all-important 'CameraID' field. This is an integer value that is used to
identify the camera in all API calls below.

##### ASI_ERROR_CODE ASIOpenCamera(int iCameraID)

Almost all functions in the API require that the camera be opened using this call.

Note that on my machine, this call fails with error ASI_ERROR_CAMERA_REMOVED if the camera is connected in
an actual USB3 port.

This function causes a flurry of libusb activity.

- libusb_init()
- libusb_open_device_with_vid_pid(vendor_id = 963, product_id = 4621)
- libusb_set_configuration(configuration = 1)
- libusb_claim_interface(interface_number = 0)
- libusb_control_transfer calls (129x !!!)

The 129 control calls are where much of the action is happening
This will be rather hard to reverse engineer.

##### ASI_ERROR_CODE ASICloseCamera(int iCameraID)

This function closes the camera device that was previously opened by ASIOpenCamera().

It causes two calls to libusb_close(); one with the USB device handle, and another one with NULL.
The latter invocation may be a bug.

##### ASI_ERROR_CODE ASIIsUSB3Host(int iCameraID, ASI_BOOL *bSet);

This function returns whether the camera is a USB3 host.
The camera device must be opened for this to work.

With my ASI120/MM-S, this function returns 0 if it is plugged in an USB2 port.
As indicated above, my camera doesn't currently work on an USB3 port under Linux.

This function causes no USB activity.

##### ASI_ERROR_CODE ASIGetNumOfControls(int iCameraID, int * piNumberOfControls)

This function queries the number of controls.
A "control" is a camera parameter that can be set.
The camera device must be opened for this to work.

This function causes no USB activity.

##### ASI_ERROR_CODE ASIGetControlCaps(int iCameraID, int iControlIndex, ASI_CONTROL_CAPS * pControlCaps)

This function gives information about a specific control.
A "control" is a camera parameter that can be set.
The camera device must be opened for this to work.

The ControlID and ControlType fields in ASI_CONTROL_CAPS appear to be identical.

However, the "AutoExpMaxBrightness", with ControlID 12, control gives ControlType 11 (ASI_AutoExpMaxExp),
which should probably be 12 (ASI_AutoExpMaxBrightness).

This function causes no USB activity.

##### ASI_ERROR_CODE ASIGetControlValue(int iCameraID, int iControlID, long *plValue, ASI_BOOL *pbAuto)

Get the current value of a certain control.

Note that only a query of the ASI_TEMPERATURE control causes bus activity.
All other values are (apparently) cached.

##### ASI_ERROR_CODE ASISetControlValue(int iCameraID, int iControlID, long lValue, ASI_BOOL bAuto)

Setting values will not fail on an open camera. No range checking is performed.

control                      | effect
-----------------------------|----------------
ASI_GAIN                     | 2
ASI_EXPOSURE                 | 1
ASI_GAMMA                    | 0 (local)
ASI_WB_R                     | 0 (local)
ASI_WB_B                     | 0 (local)
ASI_BRIGHTNESS               | 1
ASI_BANDWIDTHOVERLOAD        | 2
ASI_OVERCLOCK                | 7
ASI_TEMPERATURE              | 0 (local)
ASI_FLIP                     | 0 (local)
ASI_AutoExpMaxGain           | 0 (local)
ASI_AutoExpMaxExp            | 0 (local)
ASI_AutoExpMaxBrightness     | 0 (local)

##### ASI_ERROR_CODE ASISetROIFormat(int iCameraID, int iWidth, int iHeight,  int iBin, ASI_IMG_TYPE Img_type);
##### ASI_ERROR_CODE ASIGetROIFormat(int iCameraID, int *piWidth, int *piHeight,  int *piBin, ASI_IMG_TYPE *pImg_type)

##### ASI_ERROR_CODE ASISetStartPos(int iCameraID, int iStartX, int iStartY)
##### ASI_ERROR_CODE ASIGetStartPos(int iCameraID, int *piStartX, int *piStartY)

##### ASI_ERROR_CODE ASIEnableDarkSubtract(int iCameraID, char *pcBMPPath, bool *bIsSubDarkWorking)
##### ASI_ERROR_CODE ASIDisableDarkSubtract(int iCameraID)

##### ASI_ERROR_CODE ASIStartVideoCapture(int iCameraID)
##### ASI_ERROR_CODE ASIStopVideoCapture(int iCameraID)

##### ASI_ERROR_CODE ASIGetDroppedFrames(int iCameraID,int *piDropFrames)

##### ASI_ERROR_CODE ASIGetVideoData(int iCameraID, unsigned char* pBuffer, long lBuffSize, int iWaitms)

##### ASI_ERROR_CODE ASIPulseGuideOn(int iCameraID, ASI_GUIDE_DIRECTION direction)

Enable the pulse guide for a given direction.

libUSB_control_transfer(dev, bmRequestType = 64, bRequest = 0xb0, wValue = direction, wIndex = 0, data = NULL, wLength = 0, timeout = 500)

- bRequest = 0xb0

- ASI_GUIDE_NORTH = 0
- ASI_GUIDE_SOUTH = 1
- ASI_GUIDE_EAST  = 2
- ASI_GUIDE_WEST  = 3

##### ASI_ERROR_CODE ASIPulseGuideOff(int iCameraID, ASI_GUIDE_DIRECTION direction)

Disable the pulse guide for a given direction.

libUSB_control_transfer(dev, bmRequestType = 64, bRequest = 0xb1, wValue = direction, wIndex = 0, data = NULL, wLength = 0, timeout = 500)

- bRequest = 0xb1

- ASI_GUIDE_NORTH = 0
- ASI_GUIDE_SOUTH = 1
- ASI_GUIDE_EAST  = 2
- ASI_GUIDE_WEST  = 3

