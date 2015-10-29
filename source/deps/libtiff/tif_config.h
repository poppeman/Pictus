#define ZIP_SUPPORT
#define JPEG_SUPPORT
#define OJPEG_SUPPORT

#ifdef _WIN32
#include "tif_config_win32.h"
#else
#include "tif_config_linux.h"
#endif
