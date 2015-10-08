#include "reg_resize_position_method_translator.h"

namespace Reg {
	namespace Internal {
		boost::optional<ResizePositionMethodTranslator::external_type> ResizePositionMethodTranslator::get_value(internal_type const sval) {
			if (sval == "0") return App::ResizePositionMethod::PositionToScreen;
			if (sval == "1") return App::ResizePositionMethod::PositionToCurrent;
			if (sval == "2") return App::ResizePositionMethod::PositionNothing;

			auto it = App::ResizePositionMethodIdentifier.left.find(sval);
			if (it != App::ResizePositionMethodIdentifier.left.end()) {
				return it->second;
			}

			return nullptr;
		}

		boost::optional<ResizePositionMethodTranslator::internal_type> ResizePositionMethodTranslator::put_value(external_type const& val) {
			return App::ResizePositionMethodIdentifier.right.find(val)->second;
		}
	}
}
