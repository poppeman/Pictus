#ifndef ORZ_LOGGER_H
#define ORZ_LOGGER_H

#include <sstream>

#include "file_writer.h"

namespace IO {
	class Logger {
	public:
		void SetOutput(const std::string& filename);
		~Logger();

	private:
		void Write(const std::string& message);
		void AttemptInitialize();
		std::string m_filename;
		bool m_wroteBOM = false;
		std::string m_dbgCached;

		template <typename T> friend  Logger& operator <<(Logger& log, T const& value);
	};

	namespace Internal {
		std::string Cleanup(const std::string& input);
	}

	template <typename T>
	Logger& operator <<(Logger& log, T const& value) {
		std::stringstream ss;
		ss << value;

		log.Write(Internal::Cleanup(ss.str()));
		return log;
	}
}

extern IO::Logger Log;

#endif
