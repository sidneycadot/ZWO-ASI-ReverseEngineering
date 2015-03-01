
//////////////////////////
// AsiCameraControl.cpp //
//////////////////////////

#include "AsiUtils.h"
#include "AsiCameraControl.h"

AsiCameraControl::AsiCameraControl(const int cameraID, const int cameraControlIndex) : cameraID(cameraID)
{
    ASI_ERROR_CODE errorcode = ASIGetControlCaps(cameraID, cameraControlIndex, &controlCaps);
    check_asi_errorcode(errorcode);
}

AsiCameraControl::~AsiCameraControl()
{
}

const char * AsiCameraControl::getName() const
{
    return controlCaps.Name;
}

const char * AsiCameraControl::getDescription() const
{
    return controlCaps.Description;
}

long AsiCameraControl::getMinValue() const
{
    return controlCaps.MinValue;
}

long AsiCameraControl::getMaxValue() const
{
    return controlCaps.MaxValue;
}

long AsiCameraControl::getDefaultValue() const
{
    return controlCaps.DefaultValue;
}

bool AsiCameraControl::getIsAutoSupported() const
{
    return (controlCaps.IsAutoSupported != ASI_FALSE);
}

bool AsiCameraControl::getIsWritable() const
{
    return (controlCaps.IsWritable != ASI_FALSE);
}

ASI_CONTROL_TYPE AsiCameraControl::getControlType() const
{
    return controlCaps.ControlType;
}

void AsiCameraControl::setControlValue(long value, bool automatic)
{
    ASI_ERROR_CODE errorcode = ASISetControlValue(cameraID, controlCaps.ControlType, value, automatic ? ASI_TRUE : ASI_FALSE);
    check_asi_errorcode(errorcode);
}

void AsiCameraControl::getControlValue(long * p_value, bool * p_automatic)
{
    long value;
    ASI_BOOL automatic;

    ASI_ERROR_CODE errorcode = ASIGetControlValue(cameraID, controlCaps.ControlType, &value, &automatic);
    check_asi_errorcode(errorcode);

    if (p_value != nullptr)
    {
        *p_value = value;
    }

    if (p_automatic != nullptr)
    {
        *p_automatic = (automatic != ASI_FALSE);
    }
}
