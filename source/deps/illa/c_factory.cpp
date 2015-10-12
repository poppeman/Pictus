#include "c_factory.h"

namespace Img {
	const ICodecFactory::ExtensionList& ICodecFactory::SupportedExtensions() const {
		return m_extensions;
	}

	const std::string& ICodecFactory::GetFormatName() const {
		return m_name;
	}

	void ICodecFactory::AddExtension(const std::string& extension) {
		m_extensions.push_back(extension);
	}
}
