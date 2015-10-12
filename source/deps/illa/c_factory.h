#ifndef ILLA_C_FACTORY_H
#define ILLA_C_FACTORY_H

#include "types.h"
#include <string>
#include <vector>

namespace Img {
	class AbstractCodec;

	class ICodecFactory {
	public:
		typedef std::vector<std::string>	ExtensionList;

	public:
		const ExtensionList& SupportedExtensions() const;

		const std::string& GetFormatName() const;

		virtual AbstractCodec* CreateCodec()=0;

		ICodecFactory(const std::string& name):m_name(name) {}
		virtual ~ICodecFactory() {}

	protected:
		void AddExtension(const std::string& extension);

	private:
		ExtensionList m_extensions;
		std::string m_name;
	};
}

#endif
