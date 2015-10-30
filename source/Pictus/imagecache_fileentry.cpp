#include "imagecache_fileentry.h"
#include "orz/types.h"
#include <windows.h>
#include <boost/date_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/filesystem.hpp>

namespace Img {
	namespace Internal {
		FileInt ToFileInt(const FILETIME& ft) {
			FileInt result = ft.dwHighDateTime;
			result <<= 32;
			return result + ft.dwLowDateTime;
		}


		boost::posix_time::ptime ToLocal(std::time_t in) {
			typedef boost::date_time::c_local_adjustor<boost::posix_time::ptime> local_adj;
			auto pt = boost::posix_time::from_time_t(in);
			return local_adj::utc_to_local(pt);
		}

		// TODO: Stop using WinAPI timestamps entirely
		void FileEntry::QueryFile() {
			if (m_hasQueriedFile) return;
			WIN32_FILE_ATTRIBUTE_DATA ad;
			if(GetFileAttributesEx(UTF8ToWString(m_fullname).c_str(), GetFileExInfoStandard, &ad) == 0)
				return;

			m_dateCreate = ToFileInt(ad.ftCreationTime);
			m_dateAccess = ToFileInt(ad.ftLastAccessTime);
			m_dateModified = ToLocal(boost::filesystem::last_write_time(boost::filesystem::path(m_fullname)));
			m_fileSize = (static_cast<INT64>(ad.nFileSizeHigh) << 32) + ad.nFileSizeLow;


			m_hasQueriedFile = true;
		}

		boost::posix_time::ptime FileEntry::DateModified() {
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
			m_dateModified(boost::posix_time::not_a_date_time),
			m_hasQueriedFile(false)
		{}

		void FileEntry::Rename(const std::string& newName) {
			m_fullname = newName;
		}
	}
}