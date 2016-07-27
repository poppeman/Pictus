#ifndef PICTUS_IMAGECACHE_FILEENTRY_H
#define PICTUS_IMAGECACHE_FILEENTRY_H

#include "illa/image.h"
#include "orz/types.h"

#include <boost/date_time.hpp>

#if _WIN32
#define ENABLE_CREATED_TIME
#endif


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
#ifdef ENABLE_CREATED_TIME
			std::time_t m_dateCreate;
#endif
			std::time_t m_dateModified;
			bool m_hasQueriedFile;
			uintmax_t m_fileSize;

		};
	}
}

#endif
