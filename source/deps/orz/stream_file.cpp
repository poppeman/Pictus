#include "stream_file.h"
#include "fileops.h"

#ifdef __linux__
#include <sys/stat.h>
#include <sys/types.h>
#endif


namespace IO {
	using std::recursive_mutex;

#ifdef _WIN32
	std::string StreamFile::Rename(const std::string& newFilename, HWND handle) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		bool wasOpen = IsOpen();
		FileInt prevPos = (wasOpen?performPosition():0);
		performClose();
		auto ret = IO::Rename(m_name, newFilename, handle);
		if(ret.empty())
			m_name = ret;

		if(wasOpen) {
			performOpen();
			performSeek(prevPos, SeekMethod::Begin);
		}
		return ret;
	}
#endif

	void StreamFile::Renamed(const std::string& newFilename) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		m_name = newFilename;

		if(!IsOpen()) return;
		FileInt prevPos = performPosition();
		performClose();
		performOpen();
		performSeek(prevPos, SeekMethod::Begin);
	}

#ifdef _WIN32
	bool StreamFile::Delete(bool doRecycle, HWND handle) {
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

	size_t StreamFile::performRead(void* buf, size_t size, size_t items) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		return fread(buf, size, items, m_file);
	}

	FileInt StreamFile::performPosition() const {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		// TODO: Support the same size of files on all platforms
#ifdef _WIN32
		return _ftelli64(m_file);
#else
		return ftell(m_file);
#endif
	}

	void StreamFile::performClose() {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		if (m_file) {
			fclose(m_file);
		}

		m_file = nullptr;
	}

	bool StreamFile::performOpen() {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		m_size = 0;
#ifdef _WIN32
		// _wfsopen fails on paths surrounded by quotes, and assure_folder can output just that
		auto fn = m_name;
		if (fn.length() > 2 && fn[0] == '"' && fn[fn.length() - 1] == '"')
		{
			fn = fn.substr(1, fn.length() - 2);
		}
		m_file = _wfsopen(UTF8ToWString(fn).c_str(), L"rb", _SH_DENYWR);
#elif __linux__
		// Linux is evil and allows you to fopen folders. Thus we need to make sure this is a file manually
		m_file = fopen(m_name.c_str(), "rb");
		if(m_file != nullptr)
		{
			struct stat stats;
			if (stat(m_name.c_str(), &stats) == -1)
			{
				performClose();
				return false;
			}
			if (S_ISDIR(stats.st_mode))
			{
				performClose();
				return false;
			}
		}
#endif
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

	void StreamFile::performSeek(FileInt position, SeekMethod m) {
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
				DO_THROW(Err::InvalidParam, "Attempted to use an unsupported seek method.");
		}

		// TODO: See ftell-stuff
#ifdef _WIN32
		int ret = _fseeki64(m_file, position, flag);
#else
		int ret = fseek(m_file, position, flag);
#endif

		if (ret != 0)
			DO_THROW(Err::IOException, "Couldn't seek as requested.");
	}

	bool StreamFile::performIsOpen() const {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		return m_file != 0;
	}

	FileInt StreamFile::performSize() {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		return m_size;
	}

	std::string StreamFile::performName() const {
		return m_name;
	}

	StreamFile::StreamFile(std::string filename):
		m_name(filename)
	{}

	StreamFile::~StreamFile() {
		performClose();
	}
}
