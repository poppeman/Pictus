#include "filterstring.h"
#include "illa/codecmgr.h"
#include "app_types.h"
#include "orz/intl.h"

namespace App {
	FilterString::FilterString(Img::CodecFactoryStore* store):m_store(store) {}

	const std::wstring FilterString::GetFilterString() {
		// Awful
		std::wstring filterString = L"";
		std::wstring match_all;
		std::wstring pretty_filter;

		const Img::CodecFactoryStore::InfoVector& codecs = m_store->CodecInfo();

		for (unsigned i = 0; i < codecs.size(); ++i) {
			const Img::CodecFactoryStore::Info& curr = codecs[i];
			const Img::CodecFactoryStore::Info::ExtensionVector& exts=curr.Extensions;
			bool first = true;

			pretty_filter += curr.Description+L" (";

			std::wstring filters;

			for (size_t j = 0; j < exts.size(); j++) {
				if (first == false) filters += L";";
				if (match_all != L"") match_all += L";";

				match_all += L"*." + exts[j];
				filters += L"*." + exts[j];
			}
			pretty_filter += filters + L")|" + filters + L"|";
		}
		pretty_filter += std::wstring(Intl::GetWString(App::SIDOpenAllImages)) + L" (" + match_all + L")|" + match_all + L"||";

		filterString.resize(pretty_filter.size());

		for (size_t i = 0; i < pretty_filter.length(); i++)
			filterString[i] = ((pretty_filter[i] == L'|') ? 0 : pretty_filter[i]);

		return filterString;
	}

	size_t FilterString::FilterCount() {
		return m_store->CodecInfo().size() + 1;
	}
}
