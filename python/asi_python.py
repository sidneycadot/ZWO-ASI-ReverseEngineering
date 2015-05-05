#! /usr/bin/env python3

import ctypes
import h5py
import numpy as np

#typedef struct _ASI_CAMERA_INFO
#{
#    char Name[64]; //the name of the camera, you can display this to the UI
#    int CameraID; //this is used to control everything of the camera in other functions
#    long MaxHeight; //the max height of the camera
#    long MaxWidth;  //the max width of the camera
#
#    ASI_BOOL IsColorCam;
#    ASI_BAYER_PATTERN BayerPattern;
#
#    int SupportedBins[16]; //1 means bin1 which is supported by every camera, 2 means bin 2 etc.. 0 is the end of supported binning method
#    ASI_IMG_TYPE SupportedVideoFormat[8]; //this array will content with the support output format type.IMG_END is the end of supported video format
#
#    double PixelSize; //the pixel size of the camera, unit is um. such like 5.6um
#    ASI_BOOL MechanicalShutter;
#    ASI_BOOL ST4Port;
#    ASI_BOOL IsCoolerCam;
#    ASI_BOOL IsUSB3Host;
#    char Unused[32];
#} ASI_CAMERA_INFO;

class ASI_CAMERA_INFO(ctypes.Structure):
    _fields_ = [
        ("Name"                 , ctypes.c_char * 64),
        ("CameraID"             , ctypes.c_int      ),
        ("MaxHeight"            , ctypes.c_long     ),
        ("MaxWidth"             , ctypes.c_long     ),
        ("IsColorCam"           , ctypes.c_int      ),
        ("BayerPattern"         , ctypes.c_int      ),
        ("SupportedBins"        , ctypes.c_int * 16 ),
        ("SupportedVideoFormat" , ctypes.c_int * 8  ),
        ("PixelSize"            , ctypes.c_double   ),
        ("MechanicalShutter"    , ctypes.c_int      ),
        ("ST4Port"              , ctypes.c_int      ),
        ("IsCoolerCam"          , ctypes.c_int      ),
        ("IsUSB3Host"           , ctypes.c_int      ),
        ("Unused"               , ctypes.c_char * 32)
    ]

class ASI_CONTROL_CAPS(ctypes.Structure):
    _fields_ = [
        ("Name"            , ctypes.c_char * 64 ),
        ("Description"     , ctypes.c_char * 128),
        ("MaxValue"        , ctypes.c_long      ),
        ("MinValue"        , ctypes.c_long      ),
        ("DefaultValue"    , ctypes.c_long      ),
        ("IsAutoSupported" , ctypes.c_int       ),
        ("IsWritable"      , ctypes.c_int       ),
        ("ControlType"     , ctypes.c_int       ),
        ("Unused"          , ctypes.c_char * 32 )
    ]

libasi = ctypes.cdll.LoadLibrary("../libASICamera2.so.0.1.0320")

numberOfCameras = libasi.ASIGetNumOfConnectedCameras()

for cameraIndex in range(numberOfCameras):

    cameraInfo = ASI_CAMERA_INFO()

    errorcode = libasi.ASIGetCameraProperty(ctypes.byref(cameraInfo), cameraIndex)
    assert errorcode == 0

    errorcode = libasi.ASIOpenCamera(cameraInfo.CameraID)
    assert errorcode == 0

    numberOfControls = ctypes.c_int()
    errorcode = libasi.ASIGetNumOfControls(cameraInfo.CameraID, ctypes.byref(numberOfControls))
    assert errorcode == 0

    print("number of controls:", numberOfControls.value)

    controlInfo = ASI_CONTROL_CAPS()

    for controlIndex in range(numberOfControls.value):
        print(controlIndex)
        errorcode = libasi.ASIGetControlCaps(cameraInfo.CameraID, controlIndex, ctypes.byref(controlInfo))
        assert errorcode == 0

        print(controlInfo.Name, controlInfo.Description)

    errorcode = libasi.ASISetControlValue(cameraInfo.CameraID, 1, 1000, False);


    errorcode = libasi.ASISetROIFormat(cameraInfo.CameraID, 1280, 960, 1, 2)
    assert errorcode == 0

    print("starting...")

    errorcode = libasi.ASIStartVideoCapture(cameraInfo.CameraID)
    assert errorcode == 0

    fb = np.zeros((960, 1280), dtype = '<u2')

    f = h5py.File("images.h5", "w")
    images = f.create_dataset("images", shape = (0, 960, 1280), maxshape = (None, 960, 1280), dtype = 'u2')

    for frame in range(1000):
        print("capturing frame", frame)
        errorcode = libasi.ASIGetVideoData(cameraInfo.CameraID, fb.ctypes, 960 * 1280 * 2, 500)
        print(errorcode)
        assert errorcode == 0
        images.resize(frame + 1, axis = 0)
        images[frame] = (fb + frame)
        print(images.shape)

    f.close()

    errorcode = libasi.ASIStopVideoCapture(cameraInfo.CameraID)
    assert errorcode == 0

    errorcode = libasi.ASICloseCamera(cameraInfo.CameraID)
    assert errorcode == 0
