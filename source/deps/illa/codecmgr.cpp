#include "orz/intl.h"
#include "orz/types.h"
#include "codecmgr.h"
#include "c_factory.h"
#include "config.h"

// Codecs
#include "f_bmp.h"
#include "f_gif.h"
#include "f_jpeg.h"
#include "f_pcx.h"
#include "f_png.h"
#include "f_tga.h"
#include "f_wbmp.h"
#include "f_tiff.h"
#include "f_psp.h"
#include "f_psd.h"
#include "f_xyz.h"

namespace Img {
	bool CodecFactoryStore::DoCodecExist(const char* ext) {
		return (m_ext.find(ToUpper(ext)) != m_ext.end());
	}

	AbstractCodec* CodecFactoryStore::CreateCodec(const std::string& ext) {
		auto i = m_ext.find(ToUpper(ext));
		if (i != m_ext.end()) {
			return i->second->CreateCodec();
		}

		return nullptr;
	}

	AbstractCodec* CodecFactoryStore::CreateCodec(size_t index) {
		if (index >= m_factories.size()) {
			return nullptr;
		}

		return m_factories[index]->CreateCodec();
	}

	const CodecFactoryStore::InfoVector& CodecFactoryStore::CodecInfo() {
		return m_info;
	}

	bool CodecFactoryStore::AddCodecFactory(ICodecFactory* pCodecFactory) {
		FactoryPtr pFactory(pCodecFactory);

		for (auto i = 0; i < m_factories.size(); i++) {
			if (m_factories[i].get() == pCodecFactory) {
				return true;
			}
		}

		m_factories.push_back(pFactory);

		Info info;
		info.Description = pCodecFactory->GetFormatName();

		auto& exts = pCodecFactory->SupportedExtensions();

		for(auto i = 0; i < exts.size(); i++) {
			m_ext.insert(ExtPair(exts[i], pFactory));
			info.Extensions.push_back(exts[i]);
		}

		m_info.push_back(info);

		return true;
	}

	void CodecFactoryStore::ReleaseFactories() {
		m_info.clear();
		m_factories.clear();
		m_ext.clear();
	}

	void CodecFactoryStore::AddBuiltinCodecs() {
		AddCodecFactory(new Img::FactoryBMP());
		AddCodecFactory(new Img::FactoryGIF());
		AddCodecFactory(new Img::FactoryJPEG());
		AddCodecFactory(new Img::FactoryPCX());
		AddCodecFactory(new Img::FactoryPNG());
		AddCodecFactory(new Img::FactoryTGA());
		AddCodecFactory(new Img::FactoryWBMP());
		AddCodecFactory(new Img::FactoryPSP());
		AddCodecFactory(new Img::FactoryTIFF());
		AddCodecFactory(new Img::FactoryPSD());
		AddCodecFactory(new Img::FactoryXYZ());
	}

	CodecFactoryStore::CodecFactoryStore() {
		m_info.clear();
		m_factories.clear();
		m_ext.clear();
	}
}
