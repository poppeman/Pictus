#ifndef PICTUS_IMAGECACHE_FILEENTRY_H
#define PICTUS_IMAGECACHE_FILEENTRY_H

#include "illa/image.h"
#include "orz/types.h"

#include <boost/date_time.hpp>

namespace Img {
	namespace Internal {
		class FileEntry {
		public:
			Img::Image::Ptr Image();

			const std::string& Name() const;
			void Rename(const std::string& newName);

			boost::posix_time::ptime DateModified();
			FileInt DateAccessed();
			FileInt DateCreated();
			FileInt FileSize();

			FileEntry(std::string fullname);

		private:
			void QueryFile();

			Img::Image::Ptr m_image;
			std::string m_fullname;
			bool m_hasQueriedFile;
			FileInt m_dateCreate;
			FileInt m_dateAccess;
			boost::posix_time::ptime m_dateModified;
			FileInt m_fileSize;

		};
	}
}

#endif
