#ifndef FILE_H
#define FILE_H

#include "stream.h"

namespace IO {
	class FileReader {
	public:
		_Check_return_ const std::wstring Name() const;

		_Check_return_ bool Open();
		void Close();
		_Check_return_ FileInt Size();

		_Check_return_ Stream* GetStream();

		_Check_return_ size_t Read(_Out_writes_bytes_(size*items) void* buf, _In_ size_t size, _In_ size_t items);
		void ReadFull(_Out_writes_bytes_all_(size) void* buf, _In_ size_t size);

		void Seek(_In_ FileInt position, _In_ SeekMethod m);

		_Check_return_ FileInt Position() const;

		_Check_return_ bool IsOpen() const;

		typedef std::shared_ptr<FileReader> Ptr;

		FileReader(_In_ const std::wstring& name);
		FileReader(_In_ Stream::Ptr s);

	private:
		FileReader(const FileReader& b)=delete;
		Stream::Ptr m_stream;
	};

	std::vector<uint8_t> ReadAll(FileReader::Ptr file);

	uint32_t ReadNet32(FileReader::Ptr reader);
	uint16_t ReadNet16(FileReader::Ptr reader);

	uint16_t ReadLE32(FileReader::Ptr reader);
	uint16_t ReadLE16(FileReader::Ptr reader);
}

#endif