
//////////////////////
// asi-open-test.cc //
//////////////////////

#include <iostream>
#include "ASICamera2.h"

using namespace std;

int main()
{
    int numberOfConnectedCameras = ASIGetNumOfConnectedCameras();
    cout << "ASIGetNumOfConnectedCameras result: " << numberOfConnectedCameras << endl;

    for (int i = 0; i < numberOfConnectedCameras; ++i)
    {
        ASI_ERROR_CODE errorcode;

        ASI_CAMERA_INFO info;

        // This is done without opening the camera.
        // The ASIGetCameraProperty() call must be preceded by ASIGetNumOfConnectedCameras().

        cout << "executing ASIGetCameraProperty ..." << endl;
        errorcode = ASIGetCameraProperty(&info, i);
        if (errorcode != ASI_SUCCESS)
        {
            cout << "*** GET CAMERA INFO FAILED ***" << endl;
            continue; // skip close
        }
        cout << "ASIGetCameraProperty result: " << errorcode << endl;

        cout << endl << "camera found: \"" << info.Name << "\"\n" << endl;

        cout << "executing ASIOpenCamera ..." << endl;
        errorcode = ASIOpenCamera(info.CameraID);
        cout << "ASIOpenCamera result: " << errorcode << endl;
        if (errorcode != ASI_SUCCESS)
        {
            cout << "*** OPEN FAILED ***" << endl;
            continue; // skip close
        }

        cout << "executing ASICloseCamera ..." << endl;
        errorcode = ASICloseCamera(info.CameraID);
        cout << "ASIOpenCamera result: " << errorcode << endl;
        if (errorcode != ASI_SUCCESS)
        {
            cout << "*** CLOSE FAILED ***" << endl;
        }
    }

    return 0;
}
