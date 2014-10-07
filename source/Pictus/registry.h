#ifndef REGISTRY_H
#define REGISTRY_H

#include "app_types.h"

// Place your registry keys in this file
#include "appreg.h"

namespace Reg {
	uint32_t Key(Reg::Keys::DWORDKey key);
	void Key(Reg::Keys::DWORDKey key, uint32_t value);

	void Load(const std::wstring& name);
	void Save(const std::wstring& name);
}

#endif
