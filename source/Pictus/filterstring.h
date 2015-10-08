#ifndef FILTERSTRING_H
#define FILTERSTRING_H

#include <string>

namespace Img {
	class CodecFactoryStore;
}

namespace App {
	class FilterString {
	public:
		FilterString(Img::CodecFactoryStore* store);

		const std::wstring GetFilterString();
		size_t FilterCount();

	private:
		Img::CodecFactoryStore* m_store;
	};
}

#endif
