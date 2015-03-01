
////////////////////////
// AsiCameraManager.h //
////////////////////////

#ifndef AsiCameraManager_h
#define AsiCameraManager_h

#include <vector>
#include "AsiCamera.h"

// This class implements the AsiCameraManager.
// It is independent of Qt.
// Its methods may be blocking. If you need non-blocking behavior,
//   instantiate this class in a separate thread and call it from there.

class AsiCameraManager
{
    public:
        AsiCameraManager();
        ~AsiCameraManager();
        int getNumberOfCameras() const;
        AsiCamera * getCamera(const int cameraIndex) const;
    private:
        std::vector<AsiCamera *> cameras;
};

#endif // AsiCameraManager_h
