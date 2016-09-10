#include "file_writer.h"
#include "stream.h"

namespace IO {
	bool FileWriter::Open(std::string name, bool append) {
		Close();   // Might be re-using the object.

		if (name.empty()) {
			return false;
		}

#ifdef _WIN32
		if (name.length() > 2 && name[0] == '"' && name[name.length() - 1] == '"')
		{
			name = name.substr(1, name.length() - 2);
		}
		return _wfopen_s(&m_file, UTF8ToWString(name).c_str(), append ? L"ab" : L"wb") == 0;
#else
		m_file = fopen(name.c_str(), append ? "ab" : "wb");
		return m_file != nullptr;
#endif
	}

	void FileWriter::Close() {
		if (m_file != nullptr)
		{
			fclose(m_file);
		}

		m_file = nullptr;
	}

	size_t FileWriter::Write(const void* buf, size_t size, size_t items) {
		if (m_file == nullptr)
		{
			DO_THROW(Err::FileNotOpen, "File is not open.");
		}

		return fwrite(buf, size, items, m_file);
	}

	FileWriter::FileWriter() :
		m_file{ nullptr }
	{}

	FileWriter::~FileWriter()
	{
		Close();
	}
}
