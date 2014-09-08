#include "StdAfx.h"
#include "stream_file.h"
#include "fileops.h"

namespace IO {
	using std::recursive_mutex;

#ifdef WIN32
	std::wstring StreamFile::Rename(_In_ const std::wstring& newFilename, _In_ HWND handle) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		bool wasOpen = IsOpen();
		FileInt prevPos = (wasOpen?performPosition():0);
		performClose();
		std::wstring ret = IO::Rename(m_name, newFilename, handle);
		if(ret != TX(""))
			m_name = ret;

		if(wasOpen) {
			performOpen();
			performSeek(prevPos, SeekMethod::Begin);
		}
		return ret;
	}

	void StreamFile::Renamed(_In_ const std::wstring& newFilename) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		m_name = newFilename;

		if(!IsOpen()) return;
		FileInt prevPos = performPosition();
		performClose();
		performOpen();
		performSeek(prevPos, SeekMethod::Begin);
	}

	_Check_return_ bool StreamFile::Delete(_In_ bool doRecycle, _In_ HWND handle) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);

		bool wasOpen = IsOpen();
		FileInt prevPos = (wasOpen?performPosition():0);
		performClose();

		if(doRecycle && IO::FileRecycle(m_name, handle))
			return true;
		if(!doRecycle && IO::FileDelete(m_name, handle))
			return true;

		if(wasOpen) {
			performOpen();
			performSeek(prevPos, SeekMethod::Begin);
		}
		return false;
	}
#endif

	_Check_return_ size_t StreamFile::performRead(_Out_writes_bytes_(size * items) void* buf, _In_ size_t size, _In_ size_t items) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		return fread(buf, size, items, m_file);
	}

	_Check_return_ FileInt StreamFile::performPosition() const {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		return _ftelli64(m_file);
	}

	void StreamFile::performClose() {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		if (m_file) {
			fclose(m_file);
		}

		m_file = nullptr;
	}

	_Check_return_ bool StreamFile::performOpen() {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		m_size = 0;
		m_file = _wfsopen(m_name.c_str(), TX("rb"), _SH_DENYWR);

		if (m_file == 0) {
			switch(errno) {
			case EACCES:
				m_error = OpenErrorCode::AccessViolation;
				break;
			default:
				m_error = OpenErrorCode::FileNotFound;
				break;
			}
		}
		else {
			m_error = OpenErrorCode::Succeeded;

			// Precache file size
			performSeek(0, SeekMethod::End);

			m_size = performPosition();
			performSeek(0, SeekMethod::Begin);
		}
		return (m_file != 0);
	}

	void StreamFile::performSeek(_In_ FileInt position, _In_ SeekMethod m) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		unsigned long flag = 0;

		switch(m) {
			case SeekMethod::Begin:
				flag = SEEK_SET;
				break;
			case SeekMethod::Current:
				flag = SEEK_CUR;
				break;
			case SeekMethod::End:
				flag = SEEK_END;
				break;
			default:
				DO_THROW(Err::InvalidParam, TX("Attempted to use an unsupported seek method."));
		}

		int ret = _fseeki64(m_file, position, flag);

		if (ret != 0)
			DO_THROW(Err::IOException, TX("Couldn't seek as requested."));
	}

	_Check_return_ bool StreamFile::performIsOpen() const {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		return m_file != 0;
	}

	_Check_return_ FileInt StreamFile::performSize() {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		return m_size;
	}

	_Check_return_ std::wstring StreamFile::performName() const {
		return m_name;
	}

	StreamFile::StreamFile(_In_ const std::wstring& filename) :m_name(filename), m_file(0), m_error(OpenErrorCode::Succeeded), m_size(0) {
	}

	StreamFile::~StreamFile() {
		performClose();
	}
}
