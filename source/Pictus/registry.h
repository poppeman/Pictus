#ifndef REGISTRY_H
#define REGISTRY_H

#include "app_types.h"

// Place your registry keys in this file
#include "appreg.h"

namespace Reg {
	Settings Load(const std::wstring& name);
	void Save(const std::wstring& name, const Settings& settings);
}

#endif
