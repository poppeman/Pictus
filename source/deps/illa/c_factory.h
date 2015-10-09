#ifndef ILLA_C_FACTORY_H
#define ILLA_C_FACTORY_H

#include "types.h"
#include <string>
#include <vector>

namespace Img {
	class AbstractCodec;

	class ICodecFactory {
	public:
		typedef std::vector<std::wstring>	ExtensionList;

	public:
		const ExtensionList& SupportedExtensions() const;

		const std::wstring& GetFormatName() const;

		virtual AbstractCodec* CreateCodec()=0;

		ICodecFactory(const std::wstring& name):m_name(name) {}
		virtual ~ICodecFactory() {}

	protected:
		void AddExtension(const std::wstring& extension);

	private:
		ExtensionList m_extensions;
		std::wstring m_name;
	};
}

#endif
