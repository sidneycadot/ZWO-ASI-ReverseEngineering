
/////////////////
// AsiCamera.h //
/////////////////

#ifndef AsiCamera_h
#define AsiCamera_h

#include <vector>

// Include the proprietary C interface
#include "ASICamera2.h"

#include "AsiCameraControl.h"

// The AsiCamera class is independent of Qt.
// Its methods may be blocking. If you need non-blocking behavior,
//   instantiate this class in a separate thread and call it from there.

class AsiCamera
{
    public:
        AsiCamera(int cameraIndex);
        ~AsiCamera();

        int getCameraIndex() const;

        // inquire fields of cameraInfo

        const char * getName() const;
        int getCameraID() const;
        long getMaxWidth() const;
        long getMaxHeight() const;
        bool getIsColorCam() const;
        ASI_BAYER_PATTERN getBayerPattern() const;
        std::vector<int> getSupportedBins() const;
        std::vector<ASI_IMG_TYPE> getSupportedVideoFormats() const;
        double getPixelSize() const;
        bool getMechanicalShutter() const;
        bool getST4Port() const;
        bool getIsCoolerCam() const;
        bool getIsUSB3Host() const;

        // open and close the camera

        void open();
        void close();

        void startVideoCapture();
        void stopVideoCapture();

        void setROIFormat(int width, int height, int bin, ASI_IMG_TYPE img_type);
        void getROIFormat(int * p_width, int * p_height, int * p_bin, ASI_IMG_TYPE * p_img_type);

        int getNumberOfCameraControls() const;
        AsiCameraControl * getCameraControlByIndex(int controlIndex) const;
        AsiCameraControl * getCameraControlByControlType(ASI_CONTROL_TYPE controlType) const;

    private:

        int cameraIndex;
        ASI_CAMERA_INFO cameraInfo;
        std::vector<AsiCameraControl *> cameraControls;
};

#endif // AsiCamera_h
