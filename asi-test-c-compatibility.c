
////////////////////////////////
// asi-test-c-compatibility.c //
////////////////////////////////

// NOTE: since libASICamera2 uses C++ internally, we must use a C++-aware linker to link,
//       even though this program is pure C.

#include <stdio.h>
#include <assert.h>
#include "ASICamera2.h"

int main()
{
    printf("query number of connected cameras...\n");
    int numberOfConnectedCameras = ASIGetNumOfConnectedCameras();
    printf("number of connected cameras: %d\n", numberOfConnectedCameras);

    for (int i = 0; i < numberOfConnectedCameras; ++i)
    {
        ASI_ERROR_CODE errorcode;

        ASI_CAMERA_INFO info;

        printf("getting camera info ...\n");
        errorcode = ASIGetCameraProperty(&info, i);
        assert(errorcode = ASI_SUCCESS);
        printf("done getting camera info.\n");

        printf("opening camera ...\n");
        errorcode = ASIOpenCamera(info.CameraID);
        assert(errorcode = ASI_SUCCESS);
        printf("open done.\n");

        printf("closing camera ...\n");
        errorcode = ASICloseCamera(info.CameraID);
        assert(errorcode = ASI_SUCCESS);
        printf("close done.\n");
    }

    return 0;
}
