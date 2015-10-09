#ifndef IMAGECACHE_FILEENTRY_H
#define IMAGECACHE_FILEENTRY_H

#include "illa/image.h"
#include "orz/types.h"

namespace Img {
	namespace Internal {
		class FileEntry {
		public:
			Img::Image::Ptr Image();

			const std::wstring& Name() const;
			void Rename(const std::wstring& newName);

			FileInt DateModified();
			FileInt DateAccessed();
			FileInt DateCreated();
			FileInt FileSize();

			FileEntry(std::wstring fullname);

		private:
			void QueryFile();

			Img::Image::Ptr m_image;
			std::wstring m_fullname;
			bool m_hasQueriedFile;
			FileInt m_dateCreate;
			FileInt m_dateAccess;
			FileInt m_dateModified;
			FileInt m_fileSize;

		};
	}
}

#endif
