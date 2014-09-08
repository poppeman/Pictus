#include "StdAfx.h"
#include "logger.h"

namespace IO {
	void Logger::SetOutput(_In_ const std::wstring& filename) {
		m_filename = filename;
		m_wroteBOM = false;
		AttemptInitialize();
	}

	void Logger::Write(_In_ const std::wstring& message) {
		// TODO: Work better with the << operator. Currently a single message can get split into multiple rows.
		m_dbgCached += message;
		boost::replace_all(m_dbgCached, L"\r\n", L"\n");
		std::vector<std::wstring> lines;
		boost::split(lines, m_dbgCached, boost::is_any_of(L"\n"));
		if (lines.empty() == false) {
			m_dbgCached = lines.back();
			lines.pop_back();
			for (auto l : lines) { 
				OutputDebugStringW((L"Pictus: " + l + L"\r\n").c_str());
			}
		}

		AttemptInitialize();
		IO::FileWriter f;
		if (!f.Open(m_filename, true)) {
			return;  // Prefer losing logging messages than raising an error.
		}
		f.Write(message.c_str(), 2, message.length());
	}

	void Logger::AttemptInitialize() {
		if (m_filename.empty()) {
			return;
		}

		if (m_wroteBOM) {
			return;
		}

		IO::FileWriter f;
		if (!f.Open(m_filename)) {
			return;
		}

		const unsigned char bof[2] = {0xff, 0xfe};
		f.Write(bof, 1, 2);
		m_wroteBOM = true;
	}

	Logger::~Logger() {
		if (!m_dbgCached.empty()) {
			OutputDebugStringW((L"Pictus: " + m_dbgCached).c_str());
		}
	}

	std::wstring Internal::Cleanup(const std::wstring& input) {
		std::wstring output(input);

		size_t pos = 0;
		while((pos = output.find(TX("\n"), pos)) != std::wstring::npos) {
			output.insert(pos, TX("\r"));
			pos += 2;
		}
		return output;
	}
}
