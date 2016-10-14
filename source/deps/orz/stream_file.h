#ifndef ORZ_STREAM_FILE_H
#define ORZ_STREAM_FILE_H

#include "stream.h"

#include <mutex>
#include <wx/window.h>

namespace IO {
	class StreamFile:public Stream {
	public:
		explicit StreamFile(std::string filename);
		~StreamFile();

		std::string Rename(const std::string& newFilename, wxWindow* handle);
		void Renamed(const std::string& newFilename);
		bool Delete(bool doRecycle, wxWindow* handle);

	private:
		std::string performName() const;
		bool performOpen();
		bool performIsOpen() const;
		void performClose();
		size_t performRead(void* buf, size_t size, size_t items);
		void performSeek(FileInt position, SeekMethod m);
		FileInt performPosition() const;
		FileInt performSize();

		mutable std::recursive_mutex m_mutexAccess;

		std::FILE* m_file = nullptr;
		std::string m_name;
		FileInt m_size = 0;
		OpenErrorCode m_error = OpenErrorCode::Succeeded;
	};
}

#endif
