#include "imagecache_fileentry.h"
#include "orz/types.h"
#include <windows.h>

namespace Img {
	namespace Internal {
		FileInt ToFileInt(const FILETIME& ft) {
			FileInt result = ft.dwHighDateTime;
			result <<= 32;
			return result + ft.dwLowDateTime;
		}

		void FileEntry::QueryFile() {
			if (m_hasQueriedFile) return;
			WIN32_FILE_ATTRIBUTE_DATA ad;
			if(GetFileAttributesEx(UTF8ToWString(m_fullname).c_str(), GetFileExInfoStandard, &ad) == 0)
				return;

			m_dateCreate = ToFileInt(ad.ftCreationTime);
			m_dateAccess = ToFileInt(ad.ftLastAccessTime);
			m_dateModified = ToFileInt(ad.ftLastWriteTime);
			m_fileSize = (static_cast<INT64>(ad.nFileSizeHigh) << 32) + ad.nFileSizeLow;

			m_hasQueriedFile = true;
		}

		FileInt FileEntry::DateModified() {
			QueryFile();
			return m_dateModified;
		}

		FileInt FileEntry::DateAccessed() {
			QueryFile();
			return m_dateAccess;
		}

		FileInt FileEntry::DateCreated() {
			QueryFile();
			return m_dateCreate;
		}

		FileInt FileEntry::FileSize() {
			QueryFile();
			return m_fileSize;
		}

		Img::Image::Ptr FileEntry::Image() {
			if (m_image.get() == 0) {
				m_image = std::make_shared<Img::Image>();
			}

			return m_image;
		}

		const std::string& FileEntry::Name() const {
			return m_fullname;
		}

		FileEntry::FileEntry(std::string fullname):
			m_fullname(fullname),
			m_dateModified(0),
			m_hasQueriedFile(false)
		{}

		void FileEntry::Rename(const std::string& newName) {
			m_fullname = newName;
		}
	}
}