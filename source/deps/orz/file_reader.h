#ifndef FILE_H
#define FILE_H

#include "stream.h"

namespace IO {
	class FileReader {
	public:
		const std::wstring Name() const;

		bool Open();
		void Close();
		FileInt Size();

		Stream* GetStream();

		size_t Read(void* buf, size_t size, size_t items);
		void ReadFull(void* buf, size_t size);

		void Seek(FileInt position, SeekMethod m);

		FileInt Position() const;

		bool IsOpen() const;

		typedef std::shared_ptr<FileReader> Ptr;

		FileReader(const std::wstring& name);
		FileReader(Stream::Ptr s);

	private:
		FileReader(const FileReader& b)=delete;
		Stream::Ptr m_stream;
	};

	std::vector<uint8_t> ReadAll(FileReader::Ptr file);

	uint32_t ReadNet32(FileReader::Ptr reader);
	uint16_t ReadNet16(FileReader::Ptr reader);

	uint32_t ReadLE32(FileReader::Ptr reader);
	uint16_t ReadLE16(FileReader::Ptr reader);
}

#endif
