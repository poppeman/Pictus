#include "StdAfx.h"
#include "registry.h"
#include "w32_assure_folder.h"

namespace Reg {
	extern Reg::DWordNode c_DWordSettings[Reg::Keys::FinalDWORD];

	void Load(const std::wstring& name) {
		std::wstring full_name = assure_folder(name);

		IO::FileReader r(full_name);
		r.Open();
		auto sz = r.Size();
		std::string data;
		data.resize(sz);
		r.ReadFull(&data[0], sz);
		std::stringstream ss(data);

		try {
			boost::property_tree::ptree pt;
			boost::property_tree::ini_parser::read_ini(ss, pt);

			for (int i = 0; i < Reg::Keys::FinalDWORD; ++i) {
				std::string name = std::string("Settings.") + c_DWordSettings[i].Name;
				c_DWordSettings[i].Value = pt.get<int>(name.c_str(), c_DWordSettings[i].Value);
			}
		}
		catch (std::exception const& ex) {
			Log << L"(Reg::Load) Failed parsing configuration file: " << ex.what() << L"\n";
		}
	}

	void Save(const std::wstring& name) {
		std::wstring full_name = assure_folder(name);
		std::stringstream ss;
		boost::property_tree::ptree pt;

		for (int i = 0; i < Reg::Keys::FinalDWORD; ++i) {
			std::string name = std::string("Settings.") + c_DWordSettings[i].Name;
			pt.put<int>(name.c_str(), c_DWordSettings[i].Value);
		}

		boost::property_tree::ini_parser::write_ini(ss, pt);
		IO::FileWriter wrt;
		if (wrt.Open(full_name)) {
			auto str = ss.str();
			wrt.Write(str.c_str(), sizeof(char), str.size());
		}
	}
}
