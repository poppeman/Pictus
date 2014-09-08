#ifndef GETEVENT_H
#define GETEVENT_H

#include "appreg.h"
#include "wintypes.h"

namespace App {
	App::MouseAction MouseStandardEvent(const Win::MouseEvent& e);
	App::MouseAction MouseDblEvent(const Win::MouseEvent& e);
}

#endif
