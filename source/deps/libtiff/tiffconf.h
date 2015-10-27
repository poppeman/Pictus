#define ZIP_SUPPORT
#define JPEG_SUPPORT

#ifdef _WIN32
#include "tiffconf_win32.h"
#else
#include "tiffconf_linux.h"
#endif
