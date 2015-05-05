
////////////////////////
// asi-enum-strings.h //
////////////////////////

#ifndef ASI_TYPES_h
#define ASI_TYPES_h

#include "ASICamera2.h"

// Functions to obtain string representation of the enum constants defined in the API.
// These function return the associated enum label, or "unknown" if the value doesn't
// correspond to a known value.

const char * toString(const ASI_BAYER_PATTERN bayerPattern);
const char * toString(const ASI_IMG_TYPE imageType);
const char * toString(const ASI_GUIDE_DIRECTION direction);
const char * toString(const ASI_FLIP_STATUS flipStatus);
const char * toString(const ASI_ERROR_CODE errorcode);
const char * toString(const ASI_BOOL boolean);
const char * toString(const ASI_CONTROL_TYPE controlType);
const char * toString(const ASI_EXPOSURE_STATUS exposureStatus);

#endif // ASI_TYPES_h
