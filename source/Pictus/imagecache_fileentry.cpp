#include "imagecache_fileentry.h"
#include "orz/types.h"

#ifdef ENABLE_CREATED_TIME
#include <windows.h>
#endif

#include <boost/filesystem.hpp>

namespace Img {
	namespace Internal {
#ifdef ENABLE_CREATED_TIME
		FileInt ToFileInt(const FILETIME& ft)
		{
			FileInt result = ft.dwHighDateTime;
			result <<= 32;
			return result + ft.dwLowDateTime;
		}
#endif
		void FileEntry::QueryFile()
		{
			if (m_hasQueriedFile == false)
			{
#ifdef ENABLE_CREATED_TIME
				WIN32_FILE_ATTRIBUTE_DATA ad;
				if (GetFileAttributesEx(UTF8ToWString(m_fullname).c_str(), GetFileExInfoStandard, &ad) == 0)
				{
					return;
				}
#endif
				auto path = boost::filesystem::path(m_fullname);

#ifdef ENABLE_CREATED_TIME
				// Boost doesn't seem to supply a way to query creation time of files, so we need WinAPI for that
				m_dateCreate = ToFileInt(ad.ftCreationTime);
#endif

				m_dateModified = boost::filesystem::last_write_time(path);
				m_fileSize = boost::filesystem::file_size(path);

				m_hasQueriedFile = true;
			}
		}

		std::time_t FileEntry::DateModified() {
			QueryFile();
			return m_dateModified;
		}

#ifdef ENABLE_CREATED_TIME
		std::time_t FileEntry::DateCreated() {
			QueryFile();
			return m_dateCreate;
		}
#endif

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
			m_fullname(fullname)
		{}

		void FileEntry::Rename(const std::string& newName) {
			m_fullname = newName;
		}
	}
}