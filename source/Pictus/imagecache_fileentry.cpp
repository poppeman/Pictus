#include "imagecache_fileentry.h"
#include "orz/types.h"
#include <windows.h>
#include <boost/filesystem.hpp>

namespace Img {
	namespace Internal {
		FileInt ToFileInt(const FILETIME& ft)
		{
			FileInt result = ft.dwHighDateTime;
			result <<= 32;
			return result + ft.dwLowDateTime;
		}

		void FileEntry::QueryFile()
		{
			if (m_hasQueriedFile == false)
			{
				WIN32_FILE_ATTRIBUTE_DATA ad;
				if (GetFileAttributesEx(UTF8ToWString(m_fullname).c_str(), GetFileExInfoStandard, &ad) == 0)
				{
					return;
				}

				auto path = boost::filesystem::path(m_fullname);

				// Boost doesn't seem to supply a way to query creation time of files, so we need WinAPI for that
				m_dateCreate = ToFileInt(ad.ftCreationTime);

				m_dateModified = boost::filesystem::last_write_time(path);
				m_fileSize = boost::filesystem::file_size(path);

				m_hasQueriedFile = true;
			}
		}

		std::time_t FileEntry::DateModified() {
			QueryFile();
			return m_dateModified;
		}

		std::time_t FileEntry::DateCreated() {
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