
////////////////
// AsiUtils.h //
////////////////

#ifndef AsiUtils_h
#define AsiUtils_h

// Include the proprietary C interface
#include "ASICamera2.h"

// Check the error code as returned by an ASI function.
// Throw exception if the call was unsuccesful.

void check_asi_errorcode(const ASI_ERROR_CODE errorcode);

const char * toString(const ASI_BAYER_PATTERN bayerPattern);
const char * toString(const ASI_IMG_TYPE imageType);
const char * toString(const ASI_GUIDE_DIRECTION direction);
const char * toString(const ASI_FLIP_STATUS flipStatus);
const char * toString(const ASI_ERROR_CODE errorcode);
const char * toString(const ASI_BOOL boolean);
const char * toString(const ASI_CONTROL_TYPE controlType);
const char * toString(const ASI_EXPOSURE_STATUS exposureStatus);

int bytes_per_pixel(const ASI_IMG_TYPE img_type);

#endif // AsiUtils_h
