#ifndef ORZ_FILE_WRITER_H
#define ORZ_FILE_WRITER_H

#include <string>

namespace IO {
	class FileWriter {
	public:
		bool Open(std::string name, bool append = false);
		void Close();

		size_t Write(const void* buf, size_t size, size_t items);

		FileWriter();
		~FileWriter();

	private:
		std::FILE* m_file;
	};
}

#endif
