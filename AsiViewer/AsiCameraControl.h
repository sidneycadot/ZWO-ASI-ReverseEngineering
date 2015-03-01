
////////////////////////
// AsiCameraControl.h //
////////////////////////

#ifndef AsiCameraControl_h
#define AsiCameraControl_h

// Include the proprietary C interface
#include "ASICamera2.h"

class AsiCameraControl
{
    public:
        AsiCameraControl(const int cameraID, const int cameraControlIndex);
        ~AsiCameraControl();

        const char * getName() const;
        const char * getDescription() const;
        long getMinValue() const;
        long getMaxValue() const;
        long getDefaultValue() const;
        bool getIsAutoSupported() const;
        bool getIsWritable() const;
        ASI_CONTROL_TYPE getControlType() const;

        void setControlValue(long value, bool automatic);
        void getControlValue(long * p_value, bool * p_automatic);

    private:

        int cameraID;
        ASI_CONTROL_CAPS controlCaps;
};

#endif // AsiCameraControl_h
