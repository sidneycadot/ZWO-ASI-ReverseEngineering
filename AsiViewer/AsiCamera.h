
/////////////////
// AsiCamera.h //
/////////////////

#ifndef AsiCamera_h
#define AsiCamera_h

// Include the proprietary C interface
#include "ASICamera2.h"

// The AsiCamera class is independent of Qt.
// Its methods may be blocking. If you need non-blocking behavior,
//   instantiate this class in a separate thread and call it from there.

class AsiCamera
{
    public:
        AsiCamera(int cameraIndex);
        ~AsiCamera();

        const ASI_CAMERA_INFO & getCameraInfo() const;

        void open();
        void close();

        void startVideoCapture();
        void stopVideoCapture();

    private:

        int cameraIndex;
        ASI_CAMERA_INFO cameraInfo;
};

#endif // AsiCameraManager_h
