#include "reg_filter_mode_translator.h"
#include "orz/types.h"

namespace Reg {
	namespace Internal {
		boost::optional<FilterModeTranslator::external_type> FilterModeTranslator::get_value(internal_type const sval) {
			if (sval.empty()) return boost::none;
			if (sval == "Bilinear") {
				return Filter::Mode::Bilinear;
			}
			if (sval == "Nearest") {
				return Filter::Mode::NearestNeighbor;
			}
			if (sval == "Lanczos3") {
				return Filter::Mode::Lanczos3;
			}

			switch (FromAString<int>(sval)) {
			case 1:
				return Filter::Mode::NearestNeighbor;
			case 2:
				return Filter::Mode::Bilinear;
			case 3:
				return Filter::Mode::Lanczos3;
			}

			return nullptr;
		}

		boost::optional<FilterModeTranslator::internal_type> FilterModeTranslator::put_value(external_type const& val) {
			switch (val) {
			case Filter::Mode::NearestNeighbor:
				return "Nearest";
			case Filter::Mode::Bilinear:
				return "Bilinear";
			case Filter::Mode::Lanczos3:
				return "Lanczos3";
			}

			return nullptr;
		}
	}
}