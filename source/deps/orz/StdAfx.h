#ifndef ORZ_STDAFX_H
#define ORZ_STDAFX_H

#ifdef NDEBUG
#ifdef _SECURE_SCL
#undef _SECURE_SCL
#endif
#define _SECURE_SCL 0
#endif

#ifdef _MSC_VER
#include <codeanalysis\warnings.h>
#else
#define _Check_return_
#define _In_
#define _In_reads_bytes_(x)
#define _Out_writes_bytes_(x)
#define _Out_writes_bytes_all_(x)
#define _Out_writes_all_(x)
#define _Ret_z_
#define _Use_decl_annotations_
#endif

#pragma warning (push)
#pragma warning (disable: 4503)

#pragma warning (push)
#pragma warning (disable: 4512)
#pragma warning (disable: 4100)
#pragma warning (disable : ALL_CODE_ANALYSIS_WARNINGS)
#include <boost/signals2.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#pragma warning (pop)

#include <windows.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include <share.h>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cwctype>
#include <sstream>
#include <string>

#include <errno.h>
#include <io.h>
#include <sys/timeb.h>

#pragma warning (pop)

#include "types.h"
#include "exception.h"
#include "Logger.h"

extern IO::Logger Log;

#endif
