#include "logger.h"

#include "types.h"

#include <boost/algorithm/string.hpp>
#include <vector>

#include <windows.h>
#include <debugapi.h>

namespace IO {
	void Logger::SetOutput(const std::string& filename) {
		m_filename = filename;
		m_wroteBOM = false;
		AttemptInitialize();
	}

	void Logger::Write(const std::string& message) {
		// TODO: Work better with the << operator. Currently a single message can get split into multiple rows.
		m_dbgCached += message;
		boost::replace_all(m_dbgCached, "\r\n", "\n");
		std::vector<std::string> lines;
		boost::split(lines, m_dbgCached, boost::is_any_of("\n"));
		if (lines.empty() == false) {
			m_dbgCached = lines.back();
			lines.pop_back();
			for (auto l : lines) {
				auto u8str = "Pictus: " + l + "\r\n";
				OutputDebugStringW(UTF8ToWString(u8str.c_str()).c_str());
			}
		}

		AttemptInitialize();
		IO::FileWriter f;
		if (!f.Open(UTF8ToWString(m_filename.c_str()), true)) {
			return;  // Prefer losing logging messages than raising an error.
		}
		f.Write(message.c_str(), 1, message.length());
	}

	void Logger::AttemptInitialize() {
		if (m_filename.empty()) {
			return;
		}

		if (m_wroteBOM) {
			return;
		}

		IO::FileWriter f;
		if (!f.Open(UTF8ToWString(m_filename.c_str()))) {
			return;
		}

		const unsigned char bof[3] = {0xef, 0xbb, 0xbf};
		f.Write(bof, 1, 3);
		m_wroteBOM = true;
	}

	Logger::~Logger() {
		if (!m_dbgCached.empty()) {
			OutputDebugStringW(UTF8ToWString(("Pictus: " + m_dbgCached).c_str()).c_str());
		}
	}

	std::string Internal::Cleanup(const std::string& input) {
		std::string output(input);

		size_t pos = 0;
		while((pos = output.find("\n", pos)) != std::string::npos) {
			output.insert(pos, "\r");
			pos += 2;
		}
		return output;
	}
}

IO::Logger Log;
