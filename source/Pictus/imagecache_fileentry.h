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

			std::time_t DateModified();
			std::time_t DateCreated();
			std::time_t FileSize();

			FileEntry(std::string fullname);

		private:
			void QueryFile();

			Img::Image::Ptr m_image;
			std::string m_fullname;
			bool m_hasQueriedFile;
			std::time_t m_dateCreate;
			std::time_t m_dateModified;
			uintmax_t m_fileSize;

		};
	}
}

#endif
