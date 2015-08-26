#include "StdAfx.h"
#include "reg_color_translator.h"

namespace Reg {
	namespace Internal {
		uint8_t ToHex(char a, char b) {
			std::stringstream ss;
			ss << a << b;
			int v;
			ss >> std::hex >> v;
			return v % 0x100;
		}

		boost::optional<ColorTranslator::external_type> ColorTranslator::get_value(internal_type const sval) {
			if (sval.empty()) return boost::none;
			if (sval[0] == '#') {
				if (sval.length() == 7) {
					// #RRGGBB
					return Img::Color(0x00, ToHex(sval[1], sval[2]), ToHex(sval[3], sval[4]), ToHex(sval[5], sval[6]));
				}
			}
			return Img::Color::FromDWord(FromAString<uint32_t>(sval));
		}

		boost::optional<ColorTranslator::internal_type> ColorTranslator::put_value(external_type const& val) {
			std::stringstream ss;
			ss << '#';
			ss << std::hex << std::setfill('0') << std::setw(2) << val.R 
				<< std::hex << std::setfill('0') << std::setw(2) << val.G 
				<< std::hex << std::setfill('0') << std::setw(2) << val.B;
			return ss.str();
		}
	}
}
