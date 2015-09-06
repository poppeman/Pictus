#ifndef ORZ_FILE_WRITER_H
#define ORZ_FILE_WRITER_H

namespace IO {
	class FileWriter {
	public:
		_Check_return_ bool Open(_In_ const std::wstring& name, _In_ bool append = false);
		void Close();

		_Check_return_ size_t Write(_In_reads_bytes_(size * items) const void* buf, _In_ size_t size, _In_ size_t items);

		FileWriter();
		~FileWriter();

	private:
		std::FILE* m_file;
	};
}

#endif
