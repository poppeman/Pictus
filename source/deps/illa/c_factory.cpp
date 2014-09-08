#include "StdAfx.h"
#include "c_factory.h"

namespace Img {
	const ICodecFactory::ExtensionList& ICodecFactory::SupportedExtensions() const {
		return m_extensions;
	}

	const std::wstring& ICodecFactory::GetFormatName() const {
		return m_name;
	}

	void ICodecFactory::AddExtension(const std::wstring& extension) {
		m_extensions.push_back(extension);
	}
}
