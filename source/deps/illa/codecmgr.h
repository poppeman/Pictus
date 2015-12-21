#ifndef ILLA_CODECMGR_H
#define ILLA_CODECMGR_H

#include "types.h"
#include <deque>
#include <map>
#include <memory>
#include <vector>

namespace Img {
	class AbstractCodec;
	class ICodecFactory;

	class CodecFactoryStore {
	public:
		struct Info {
			typedef std::vector<std::string>	ExtensionVector;

			std::string Description;
			ExtensionVector Extensions;
		};
		typedef std::vector<Info> InfoVector;
		bool DoCodecExist(const char* ext);
		AbstractCodec* CreateCodec(const std::string& ext);
		AbstractCodec* CreateCodec(size_t index);
		const InfoVector& CodecInfo();

		bool AddCodecFactory(ICodecFactory* pCodecFactory);
		void ReleaseFactories();
		void AddBuiltinCodecs();

		~CodecFactoryStore();

	private:
		typedef std::shared_ptr<ICodecFactory> FactoryPtr;
		typedef std::deque<FactoryPtr> FactoryList;
		typedef std::map<std::string, FactoryPtr> ExtMap;
		typedef std::pair<std::string, FactoryPtr> ExtPair;
		FactoryList m_factories;
		ExtMap m_ext;
		InfoVector m_info;
	};
}

#endif
