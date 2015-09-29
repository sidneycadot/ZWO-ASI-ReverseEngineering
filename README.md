# ZWO-ASI-ReverseEngineering

This repository contains efforts to reverse-engineer the protocol of the ZWO ASI120MM-S camera.

This work is currently based on version 0.1.0803 of the library (29 sept 2015).

## List of files

filename                           | description
-----------------------------------|--------------------------------------------------------------------------------------------------------------
README.md                          | Description of the library
MD5SUM                             | contains MD5 checksums of the original library and include files (v0.1.0320).
ASICamera2.h                       | The original header file (v0.1.0320)
libASICamera2.a                    | The original static library
libASICamera2.so.0.1.0320          | The original dynamic library
patch-library.py                   | A script to make patched versions of the library where all references to "libusb" are replaced by "libUSB".
libASICamera2_patched.a            | The patched static library; uses "libUSB_<xxx>" rather than "libusb_<xxx>" calls.
libASICamera2_patched.so.0.1.0320  | The patched dynamic library; uses "libUSB_<xxx>" rather than "libusb_<xxx>" calls.
ISSUES.txt                         | A text file describing several issues with the current include file and library
asi-test.cc                        | A test program that controls the camera and captures a bunch of images.
libUSB.h                           | A set of functions that mimic libusb behavior (header file)
libUSB.c                           | A set of functions that mimic libusb behavior, printing invocations and results along the way.
Makefile                           | Makefile for several program.
asi-test-c-compatibility.c         | A C program that just opens/closes a camera, to test library compatibility with C rather than C++.
libASICamera2_ReverseEngineered.c  | A first stab at a reverse-engineered version of libASICamera2, having the same API.

## ASI SDK download

The driver can be downloaded from this page:

http://astronomy-imaging-camera.com/software/

Version 0.1.0803 can be downloaded from this link:

http://astronomy-imaging-camera.com/software/ASI_linux_mac_SDK_V0.1.0803.tar

Its MD5SUM is e61ce4022ebab5b0068095a96ddc7f40.

Untarring yields the file ASI_linux_mac_SDK.tar.bz2.

Untarring this file yields 4 directories: "include", "libusb", "demo", and "lib".

The interesting parts (for amd64 / Linux development) are:

a6f1a4b69aefa318488ef2893e5f3dc0  include/ASICamera2.h
1714b27830e44788c209cba28b7f97b1  lib/c64/libASICamera2.a

## External info

A partial datasheet for the MT9M034 sensor can be found by googling, e.g. here:

    http://ghgtechn.com/admin/upload/634912822373126250MT9M034_DS_B.pdf.

Note that this datasheet is marked "Aptina Confidential and Proprietary".
Furthermore, the datasheet sadly doesn't include the register file description.

Some more info is available here:

    https://github.com/Aptina/BeagleBoard-xM/tree/master/MT9M034

This has a V4L2 driver for the MT9M034 sensor that is partially done, it seems.
At the very least we see some names for the register file in the code there.

## Register file

This taken from the BeagleBoard-xM driver.

name                               | value
-----------------------------------|-------
MT9M034_CHIP_ID_REG                | 0x3000
MT9M034_CHIP_ID                    | 0x2400
MT9M034_RESET_REG                  | 0x301A
MT9M034_SEQ_CTRL_PORT              | 0x3088
MT9M034_SEQ_DATA_PORT              | 0x3086
MT9M034_ANALOG_REG                 | 0x3ED6
MT9M034_TEST_RAW_MODE              | 0x307A
MT9M034_DARK_CTRL                  | 0x3044
MT9M034_DATA_PEDESTAL              | 0x301E
MT9M034_COLUMN_CORRECTION          | 0x30D4
MT9M034_VT_SYS_CLK_DIV             | 0x302A
MT9M034_VT_PIX_CLK_DIV             | 0x302C
MT9M034_PRE_PLL_CLK_DIV            | 0x302E
MT9M034_PLL_MULTIPLIER             | 0x3030
MT9M034_DIGITAL_TEST               | 0x30B0
MT9M034_Y_ADDR_START               | 0x3002
MT9M034_X_ADDR_START               | 0x3004
MT9M034_Y_ADDR_END                 | 0x3006
MT9M034_X_ADDR_END                 | 0x3008
MT9M034_FRAME_LENGTH_LINES         | 0x300A
MT9M034_LINE_LENGTH_PCK            | 0x300C
MT9M034_COARSE_INT_TIME            | 0x3012
MT9M034_FINE_INT_TIME              | 0x3014
MT9M034_COARSE_INT_TIME_CB         | 0x3016
MT9M034_FINE_INT_TIME_CB           | 0x3018
MT9M034_X_ODD_INC                  | 0x30A2
MT9M034_Y_ODD_INC                  | 0x30A6
MT9M034_READ_MODE                  | 0x3040
MT9M034_TEST_PATTERN               | 0x3070
MT9M034_LLP_RECOMMENDED            |   1650
MT9M034_DIGITAL_BINNING            | 0x3032
MT9M034_HOR_AND_VER_BIN            | 0x0022
MT9M034_HOR_BIN                    | 0x0011
MT9M034_DISABLE_BINNING            | 0x0000
MT9M034_AE_CTRL_REG                | 0x3100
MT9M034_GREEN1_GAIN                | 0x3056
MT9M034_BLUE_GAIN                  | 0x3058
MT9M034_RED_GAIN                   | 0x305A
MT9M034_GREEN2_GAIN                | 0x305C
MT9M034_GLOBAL_GAIN                | 0x305E
MT9M034_GREEN1_GAIN_CB             | 0x30BC
MT9M034_BLUE_GAIN_CB               | 0x30BE
MT9M034_RED_GAIN_CB                | 0x30C0
MT9M034_GREEN2_GAIN_CB             | 0x30C2
MT9M034_GLOBAL_GAIN_CB             | 0x30C4
MT9M034_RESET_REGISTER             | 0x301A
MT9M034_RESET                      | 0x00D9
MT9M034_STREAM_OFF                 | 0x00D8
MT9M034_STREAM_ON                  | 0x00DC
MT9M034_ERS_PROG_START_ADDR        | 0x309E
MT9M034_MODE_CTRL                  | 0x3082
MT9M034_DAC_LD_14_15               | 0x3EDA
MT9M034_DAC_LD_18_19               | 0x3EDE
MT9M034_DAC_LD_12_13               | 0x3ED8
MT9M034_DAC_LD_22_23               | 0x3EE2
MT9M034_DAC_LD_20_21               | 0x3EE0
MT9M034_DAC_LD_16_17               | 0x3EDC
MT9M034_DARK_CONTROL               | 0x3044
MT9M034_DAC_LD_26_27               | 0x3EE6
MT9M034_DAC_LD_24_25               | 0x3EE4
MT9M034_DAC_LD_10_11               | 0x3ED6
MT9M034_ADC_BITS_6_7               | 0x30E4
MT9M034_ADC_BITS_4_5               | 0x30E2
MT9M034_ADC_BITS_2_3               | 0x30E0
MT9M034_ADC_CONFIG1                | 0x30E6
MT9M034_ADC_CONFIG2                | 0x30E8
MT9M034_DIGITAL_CTRL               | 0x30BA
MT9M034_COARSE_INTEGRATION_TIME    | 0x3012
MT9M034_HDR_COMP                   | 0x31D0
MT9M034_AE_DCG_EXPOSURE_HIGH_REG   | 0x3112
MT9M034_AE_DCG_EXPOSURE_LOW_REG    | 0x3114
MT9M034_AE_DCG_GAIN_FACTOR_REG     | 0x3116
MT9M034_AE_DCG_GAIN_FACTOR_INV_REG | 0x3118
MT9M034_AE_LUMA_TARGET_REG         | 0x3102
MT9M034_AE_HIST_TARGET_REG         | 0x3104
MT9M034_AE_ALPHA_V1_REG            | 0x3126
MT9M034_AE_MAX_EXPOSURE_REG        | 0x311C
MT9M034_AE_MIN_EXPOSURE_REG        | 0x311E
MT9M034_EMBEDDED_DATA_CTRL         | 0x3064

## Methodology of reverse-engineering the library

The proprietary ASICamera2 library uses libusb 1.0 (the modern version of the user-space USB library) to communicate with devices.

To understand the way in which the library uses libusb, we generated a patched version of the ASICamera2 library where all references to "libusb" are replaced by "libUSB".

Next, we implement a "libUSB.c" that re-implements 15 of the "libusb" functions, but renames them with the prefix "libUSB". These 15 functions print their arguments, call the actual libusb functions, and then print the return value of the libusb functions.

This effectively means that all usage of libusb by the ASICamera2 is now logged.

## What we learned so far

The "ASICamera2" API currently consists of 26 function calls (v0.1.0320).

We describe them below and discuss below what they do in terms of USB bus activity.

All functions use the 'default' context of libusb, meaning that they pass a NULL pointer as the 'context' argument wherever it is needed.

##### int ASIGetNumOfConnectedCameras()

The ASIGetNumOfConnectedCameras() call is intended to be the first function called in a program that uses the ASICamera2 library.


It starts by executing a libusb_init() call. Next it executes libusb_get_device_list() to obtain a list of all USB devices available in the system.

Next, it traverses the list of all devices. For each device, three libusb functions are called:

- libusb_get_device_descriptor()
- libusb_get_device_address()
- libusb_get_bus_number()

This allows the ASICamera2 library to find all ZWO ASI devices, and build an internal data structure to describe
where they are.

Finally, the device list is freed by executing libusb_free_device_list(), and libusb_exit() is called to detach from
libusb.

Note that this is the only function in the API that does not return an ASI_ERROR_CODE.
(The comments is "ASICamera.h" claims it does, but it cannot be true, since the error codes are positive numbers,
which cannot be distinguished from an actual number of cameras.)
Instead, it returns the number of ASI camera devices found as an integer, or zero in case of problems.

##### ASI_ERROR_CODE ASIGetCameraProperty(ASI_CAMERA_INFO *pASICameraInfo, int iCameraIndex)

This function fills a user-supplied ASI_CAMERA_INFO structure with information about an ASI camera device.

This function only works if ASIGetNumOfConnectedCameras() was executed beforehand. If it is called before a call to ASIGetNumOfConnectedCameras was made, an ASI_ERROR_INVALID_ID is returned, presumably because the internal list of devices maintained by ASICamera2 contains zero devices.

Since version v0.1.0214, this function causes a lot of traffix on the USB bus, comparable to the traffic of the ASIOpenCamera()
call:

- libusb_init()
- libusb_open_device_with_vid_pid(vendor_id = 0x03c3, product_id = 0x120d)
- libusb_set_configuration(configuration = 1)
- libusb_claim_interface(interface_number = 0)
- libusb_control_transfer calls (134 calls in v0.1.0214)
- libusb_close()

One difference is that libusb_control_transfer() is called one more time.

The ASI_CAMERA_INFO contains the all-important 'CameraID' field. This is an integer value that is used to identify the camera in all API calls below.

##### ASI_ERROR_CODE ASIOpenCamera(int iCameraID)

Almost all functions in the API require that the camera be opened using this call.

Note that on my machine, this call fails with error ASI_ERROR_CAMERA_REMOVED if the camera is connected to an actual USB3 port, and the process is run as a normal user. When running as root, everything works fine. This indicates an udev configuration issue, where USB2 and USB3 ports are not handled identically.

This function causes a flurry of libusb activity:

- libusb_init()
- libusb_open_device_with_vid_pid(vendor_id = 0x03c3, product_id = 0x120d)
- libusb_set_configuration(configuration = 1)
- libusb_claim_interface(interface_number = 0)
- libusb_control_transfer calls (133 calls in v0.1.0214)

The 134 control_transfer calls are where much of the action is happening.
This will be rather hard to reverse engineer.

##### ASI_ERROR_CODE ASICloseCamera(int iCameraID)

This function closes the camera device that was previously opened by ASIOpenCamera().

It causes two calls to libusb_close(); one with the USB device handle, and another one with NULL.
The latter invocation may be a bug.

##### ASI_ERROR_CODE ASIGetNumOfControls(int iCameraID, int * piNumberOfControls)

This function queries the number of controls.
A "control" is a camera parameter that can be set and queried.
The camera device must be opened for this to work.

This function causes no USB activity.

##### ASI_ERROR_CODE ASIGetControlCaps(int iCameraID, int iControlIndex, ASI_CONTROL_CAPS * pControlCaps)

This function gives information about a specific control.
A "control" is a camera parameter that can be set and queried.
The camera device must be opened for this to work.

The ControlID and ControlType fields in ASI_CONTROL_CAPS appear to be identical.

This function causes no USB activity.

##### ASI_ERROR_CODE ASIGetControlValue(int iCameraID, int iControlID, long *plValue, ASI_BOOL *pbAuto)

Get the current value of a certain control.

Note that only a query of the ASI_TEMPERATURE control causes bus activity.
An ASI_TEMPERATURE request looks like this:

libusb_control_transfer(dev, bmRequestType = 192, bRequest = 0xa7, wValue = 0x30b2, wIndex = 0, data = ..., wLength = 2, timeout = 500)

All other values are apparently local, requiring no device interaction.

##### ASI_ERROR_CODE ASISetControlValue(int iCameraID, int iControlID, long lValue, ASI_BOOL bAuto)

Setting values will not fail on an open camera. No range checking is performed.

TODO: update for v0.1.0214.

control                      | number of libusb_control_transfer() calls
-----------------------------|------------------------------------------
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

##### ASI_ERROR_CODE ASISetROIFormat(int iCameraID, int   iWidth, int   iHeight,  int   iBin, ASI_IMG_TYPE   Img_type)

```
libusb_control_transfer(dev = 0x8b37f0, bmRequestType =  64, bRequest = 0xa6, wValue = 0x3002, wIndex =   450, data =          (nil), wLength =    0, timeout = 500)
libusb_control_transfer(dev = 0x8b37f0, bmRequestType =  64, bRequest = 0xa6, wValue = 0x3004, wIndex =   576, data =          (nil), wLength =    0, timeout = 500)
libusb_control_transfer(dev = 0x8b37f0, bmRequestType =  64, bRequest = 0xa6, wValue = 0x3006, wIndex =   513, data =          (nil), wLength =    0, timeout = 500)
libusb_control_transfer(dev = 0x8b37f0, bmRequestType =  64, bRequest = 0xa6, wValue = 0x3008, wIndex =   703, data =          (nil), wLength =    0, timeout = 500)
libusb_control_transfer(dev = 0x8b37f0, bmRequestType =  64, bRequest = 0xac, wValue = 0x0000, wIndex =     0, data =          (nil), wLength =    0, timeout = 200)
libusb_clear_halt()
libusb_control_transfer(dev = 0x8b37f0, bmRequestType =  64, bRequest = 0xb5, wValue = 0x0000, wIndex =     1, data =          (nil), wLength =    0, timeout = 500)
libusb_control_transfer(dev = 0x8b37f0, bmRequestType =  64, bRequest = 0xa6, wValue = 0x300c, wIndex =  1390, data =          (nil), wLength =    0, timeout = 500)
libusb_control_transfer(dev = 0x8b37f0, bmRequestType =  64, bRequest = 0xa6, wValue = 0x3012, wIndex =   835, data =          (nil), wLength =    0, timeout = 500)
libusb_control_transfer(dev = 0x8b37f0, bmRequestType =  64, bRequest = 0xa6, wValue = 0x300a, wIndex =    90, data =          (nil), wLength =    0, timeout = 500)
```

##### ASI_ERROR_CODE ASIGetROIFormat(int iCameraID, int *piWidth, int *piHeight,  int *piBin, ASI_IMG_TYPE *pImg_type)

No traffic.

##### ASI_ERROR_CODE ASISetStartPos(int iCameraID, int iStartX, int iStartY)

```
libusb_control_transfer(dev, bmRequestType = 0x40, bRequest = 0xa6, wValue = 0x3002, wIndex = y0 + 2                    , data = NULL, wLength = 0, timeout = 500)
libusb_control_transfer(dev, bmRequestType = 0x40, bRequest = 0xa6, wValue = 0x3004, wIndex = x0                        , data = NULL, wLength = 0, timeout = 500)
libusb_control_transfer(dev, bmRequestType = 0x40, bRequest = 0xa6, wValue = 0x3006, wIndex = y0 + BinSize * yHeight + 1, data = NULL, wLength = 0, timeout = 500)
libusb_control_transfer(dev, bmRequestType = 0x40, bRequest = 0xa6, wValue = 0x3008, wIndex = x0 + BinSize * xWidth  - 1, data = NULL, wLength = 0, timeout = 500)
```

##### ASI_ERROR_CODE ASIGetStartPos(int iCameraID, int *piStartX, int *piStartY)

No traffic.

##### ASI_ERROR_CODE ASIEnableDarkSubtract(int iCameraID, char *pcBMPPath, bool *bIsSubDarkWorking)
##### ASI_ERROR_CODE ASIDisableDarkSubtract(int iCameraID)

##### ASI_ERROR_CODE ASIStartVideoCapture(int iCameraID)
##### ASI_ERROR_CODE ASIStopVideoCapture(int iCameraID)

##### ASI_ERROR_CODE ASIGetDroppedFrames(int iCameraID,int *piDropFrames)

##### ASI_ERROR_CODE ASIGetVideoData(int iCameraID, unsigned char* pBuffer, long lBuffSize, int iWaitms)

##### ASI_ERROR_CODE ASIPulseGuideOn(int iCameraID, ASI_GUIDE_DIRECTION direction)

Enable the pulse guide for a given direction.

libusb_control_transfer(dev, bmRequestType = 64, bRequest = 0xb0, wValue = direction, wIndex = 0, data = NULL, wLength = 0, timeout = 500)

- bRequest = 0xb0

- ASI_GUIDE_NORTH = 0
- ASI_GUIDE_SOUTH = 1
- ASI_GUIDE_EAST  = 2
- ASI_GUIDE_WEST  = 3

This function is completely understood.

##### ASI_ERROR_CODE ASIPulseGuideOff(int iCameraID, ASI_GUIDE_DIRECTION direction)

Disable the pulse guide for a given direction.

libusb_control_transfer(dev, bmRequestType = 64, bRequest = 0xb1, wValue = direction, wIndex = 0, data = NULL, wLength = 0, timeout = 500)

- bRequest = 0xb1

- ASI_GUIDE_NORTH = 0
- ASI_GUIDE_SOUTH = 1
- ASI_GUIDE_EAST  = 2
- ASI_GUIDE_WEST  = 3

This function is completely understood.

##### ASI_ERROR_CODE ASIStartExposure(int iCameraID, long lTimems, ASI_BOOL bIsDark)

New in API v0.1.0214; not investigated yet.

##### ASI_ERROR_CODE ASIStopExposure(int iCameraID)

New in API v0.1.0214; not investigated yet.

##### ASI_ERROR_CODE ASIGetExpStatus(int iCameraID, ASI_EXPOSURE_STATUS *pExpStatus)

New in API v0.1.0214; not investigated yet.

##### ASI_ERROR_CODE ASIGetDataAfterExp(int iCameraID, unsigned char* pBuffer, long lBuffSize)

New in API v0.1.0214; not investigated yet.

##### ASI_ERROR_CODE ASIGetID(int iCameraID, ASI_ID* pID)

New in API v0.1.0320; not investigated yet.

##### ASI_ERROR_CODE ASISetID(int iCameraID, ASI_ID pID)

New in API v0.1.0320; not investigated yet.
