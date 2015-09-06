#ifndef LOGGER
#define LOGGER

#include "file_writer.h"

namespace IO {
	class Logger {
	public:
		void SetOutput(_In_ const std::wstring& filename);
		~Logger();

	private:
		void Write(_In_ const std::wstring& message);
		void AttemptInitialize();
		std::wstring m_filename;
		bool m_wroteBOM;
		std::wstring m_dbgCached;

		template <typename T> friend  Logger& operator <<(Logger& log, T const& value);
	};

	namespace Internal {
		std::wstring Cleanup(const std::wstring& input);
	}

	template <typename T>
	Logger& operator <<(Logger& log, T const& value) {
		std::wstringstream ss;
		ss << value;

		log.Write(Internal::Cleanup(ss.str()));
		return log;
	}
}

#endif
