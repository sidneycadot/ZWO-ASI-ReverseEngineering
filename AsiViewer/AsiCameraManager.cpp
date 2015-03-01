
//////////////////////////
// AsiCameraManager.cpp //
//////////////////////////

#include "AsiCameraManager.h"

// Include the proprietary C interface
#include "ASICamera2.h"

#include "AsiUtils.h"

AsiCameraManager::AsiCameraManager()
{
    int numberOfConnectedCameras = ASIGetNumOfConnectedCameras();

    for (int cameraIndex = 0; cameraIndex < numberOfConnectedCameras; ++cameraIndex)
    {
        AsiCamera * camera = new AsiCamera(cameraIndex);
        cameras.push_back(camera);
    }
}

AsiCameraManager::~AsiCameraManager()
{
    for (AsiCamera * camera : cameras)
    {
        delete camera;
    }
    cameras.clear();
}

int AsiCameraManager::getNumberOfCameras() const
{
    return cameras.size();
}

AsiCamera * AsiCameraManager::getCamera(const int cameraIndex) const
{
    return cameras.at(cameraIndex);
}
