#include "StdAfx.h"
#include "registry.h"

namespace Reg {
#include "values_dword.inc"

	uint32_t Key(Reg::Keys::DWORDKey key) {
		COND_STRICT((key >= 0) && (key < Reg::Keys::FinalDWORD), Err::CriticalError, TX("Attempted to access an invalid setting."));
		return Reg::c_DWordSettings[key].Value;
	}

	void Key(Reg::Keys::DWORDKey key, uint32_t value) {
		COND_STRICT((key >= 0) && (key < Reg::Keys::FinalDWORD), Err::CriticalError, TX("Attempted to access an invalid setting."));
		Reg::c_DWordSettings[key].Value = value;
	}
}
