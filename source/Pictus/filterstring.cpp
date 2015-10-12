#include "filterstring.h"
#include "illa/codecmgr.h"
#include "app_types.h"
#include "orz/intl.h"

namespace App {
	FilterString::FilterString(Img::CodecFactoryStore* store):m_store(store) {}

	const std::string FilterString::GetFilterString()
{
		// Awful
		std::string filterString = "";
		std::string match_all;
		std::stringstream pretty_filter;

		const Img::CodecFactoryStore::InfoVector& codecs = m_store->CodecInfo();

		for (unsigned i = 0; i < codecs.size(); ++i) {
			const Img::CodecFactoryStore::Info& curr = codecs[i];
			const Img::CodecFactoryStore::Info::ExtensionVector& exts=curr.Extensions;
			bool first = true;

			pretty_filter << curr.Description << " (";

			std::string filters;

			for (size_t j = 0; j < exts.size(); j++) {
				if (first == false) filters += ";";
				if (match_all.empty() == false) match_all += ";";

				match_all += "*." + exts[j];
				filters += "*." + exts[j];
			}
			pretty_filter << filters << ")" << 0 << filters << 0;
		}
		pretty_filter << Intl::GetString(App::SIDOpenAllImages) << " (" << match_all << ")" << 0 << match_all << 0 << 0;

		return pretty_filter.str();
	}

	size_t FilterString::FilterCount() {
		return m_store->CodecInfo().size() + 1;
	}
}
