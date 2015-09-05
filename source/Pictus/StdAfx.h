
#ifndef PICTUS_STDAFX_H
#define PICTUS_STDAFX_H

#define D3D_DEBUG_INFO
#define _WIN32_WINNT 0x0600
#define WINVER 0x0600
#define _WIN32_IE 0x0900

#include "illa/StdAfx.h"
#include "D3DWrap/StdAfx.h"

#include <ShellAPI.h>
#include <commctrl.h>
#include <commdlg.h>
#include <objbase.h>
#include <wininet.h>
#include <shobjidl.h>
#include <Uxtheme.h>

#include <shlobj.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#pragma warning(disable: 4512)
#pragma warning(disable: 4503)
#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include "orz/logger.h"

#endif // STDAFX_H
