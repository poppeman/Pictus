#include "StdAfx.h"
#include "registry.h"
#include "w32_assure_folder.h"

namespace Reg {
	extern Reg::DWordNode c_DWordSettings[Reg::Keys::FinalDWORD];

	void Load(const std::wstring& name) {
		std::wstring full_name = assure_folder(name);

		for (int i = 0; i < Reg::Keys::FinalDWORD; ++i)
			c_DWordSettings[i].Value=GetPrivateProfileInt(TX("Settings"), c_DWordSettings[i].Name, c_DWordSettings[i].Value, full_name.c_str());
	}

	void Save(const std::wstring& name) {
		std::wstring full_name = assure_folder(name);

		for (int i = 0; i < Reg::Keys::FinalDWORD; ++i)
			WritePrivateProfileString(TX("Settings"), c_DWordSettings[i].Name, ToWString(c_DWordSettings[i].Value).c_str(), full_name.c_str());
	}
}
