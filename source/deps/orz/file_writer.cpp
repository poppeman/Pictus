#include "file_writer.h"
#include "stream.h"

namespace IO {
	bool FileWriter::Open(const std::string& name, bool append) {
		Close();   // Might be re-using the object.

		if (name.empty()) {
			return false;
		}

		return _wfopen_s(&m_file, UTF8ToWString(name).c_str(), append ? L"ab" : L"wb") == 0;
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
