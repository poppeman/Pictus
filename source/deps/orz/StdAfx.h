#ifndef ORZ_STDAFX_H
#define ORZ_STDAFX_H

#ifdef NDEBUG
#ifdef _SECURE_SCL
#undef _SECURE_SCL
#endif
#define _SECURE_SCL 0
#endif

#include <codeanalysis\warnings.h>

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
