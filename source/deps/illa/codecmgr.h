#ifndef CODECMGR_H
#define CODECMGR_H

#include "types.h"

namespace Img {
	class AbstractCodec;
	class ICodecFactory;

	class CodecFactoryStore {
	public:
		struct Info {
			typedef std::vector<std::wstring>	ExtensionVector;

			std::wstring Description;
			ExtensionVector Extensions;
		};
		typedef std::vector<Info> InfoVector;
		bool DoCodecExist(const wchar_t* ext);
		AbstractCodec* CreateCodec(const std::wstring& ext);
		AbstractCodec* CreateCodec(size_t index);
		const InfoVector& CodecInfo();

		bool AddCodecFactory(ICodecFactory* pCodecFactory);
		void ReleaseFactories();
		void AddBuiltinCodecs();

		CodecFactoryStore();

	private:
		typedef std::shared_ptr<ICodecFactory> FactoryPtr;
		typedef std::deque<FactoryPtr> FactoryList;
		typedef std::map<std::wstring, FactoryPtr> ExtMap;
		typedef std::pair<std::wstring, FactoryPtr> ExtPair;
		FactoryList m_factories;
		ExtMap m_ext;
		InfoVector m_info;
	};
}

#endif
