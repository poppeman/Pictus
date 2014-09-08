#include "StdAfx.h"
#include "filterstring.h"
#include "illa/codecmgr.h"
#include "app_types.h"
#include "orz/intl.h"

namespace App {
	FilterString::FilterString(Img::CodecFactoryStore* store):m_store(store) {}

	const std::wstring FilterString::GetFilterString() {
		std::wstring filterString = TX("");
		std::wstring match_all;
		std::wstring pretty_filter;

		const Img::CodecFactoryStore::InfoVector& codecs = m_store->CodecInfo();

		for (unsigned i = 0; i < codecs.size(); ++i) {
			const Img::CodecFactoryStore::Info& curr = codecs[i];
			const Img::CodecFactoryStore::Info::ExtensionVector& exts=curr.Extensions;
			bool first = true;

			pretty_filter += curr.Description+TX(" (");

			std::wstring filters;

			for (size_t j = 0; j < exts.size(); j++) {
				if (first == false) filters += TX(";");
				if (match_all != TX("")) match_all += TX(";");

				match_all	+= TX("*.") + exts[j];
				filters		+= TX("*.") + exts[j];
			}
			pretty_filter+=filters+TX(")|")+filters+TX("|");
		}
		pretty_filter += std::wstring(Intl::GetWString(App::SIDAllImages)) + TX(" (") + match_all + TX(")|") + match_all + TX("||");

		filterString.resize(pretty_filter.size());

		for (size_t i = 0; i < pretty_filter.length(); i++)
			filterString[i] = ((pretty_filter[i] == TX('|')) ? 0 : pretty_filter[i]);

		return filterString;
	}

	size_t FilterString::FilterCount() {
		return m_store->CodecInfo().size() + 1;
	}
}
