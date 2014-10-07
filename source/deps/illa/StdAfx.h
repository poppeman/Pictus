#ifndef ILLA_STDAFX_H
#define ILLA_STDAFX_H

#include "orz/StdAfx.h"

#include <shlwapi.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#pragma warning (push)
#pragma warning (disable : ALL_CODE_ANALYSIS_WARNINGS)
#pragma warning (disable: 4913)
#pragma warning (disable: 4503)
#include <boost/thread.hpp> // for thread_group
#pragma warning (pop)

#include <zlib/zlib.h>
#include <libtiff/tiff.h>
#include <libtiff/tiffio.h>
#include <libtiff/tiffiop.h>
#include <libpng/png.h>

#include <cstdio>

#endif
