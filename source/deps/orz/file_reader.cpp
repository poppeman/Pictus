#include "stream_file.h"
#include "file_reader.h"
#include "ByteOrder.h"

namespace IO {
	using std::mutex;

	const std::string FileReader::Name() const {
		return m_stream->Name();
	}

	bool FileReader::Open() {
		return m_stream->Open();
	}

	void FileReader::Close() {
		m_stream->Close();
	}

	 FileInt FileReader::Size() {
		return m_stream->Size();
	}

	Stream* FileReader::GetStream() {
		return m_stream.get();
	}

	size_t FileReader::Read(void* buf, size_t size, size_t items) {
		return m_stream->Read(buf, size, items);
	}

	void FileReader::ReadFull(void* buf, size_t size) {
		if (IsOpen() == false) {
			DO_THROW(Err::FileNotOpen, "File is not open.");
		}

		if (size > 0 && m_stream->Read(buf, size, 1) != 1) {
			DO_THROW(Err::EndOfFile, "Reached EOF in a call to ReadFull");
		}
	}

	void FileReader::Seek(FileInt position, SeekMethod m) {
		switch (m)
		{
			case SeekMethod::Begin:
				if(position > Size())
				{
					DO_THROW(Err::InvalidParam, u8"Argument position (" + ToAString(position) +") outside of file (" + ToAString(Size()) + ") for SeekMethod::Begin");
				}
				break;
			case SeekMethod::End:
				if(position != 0)
				{
					DO_THROW(Err::InvalidParam, u8"Argument position must be zero for SeekMethod::End");
				}
				break;
			case SeekMethod::Current:
				if(position + Position() > Size())
				{
					DO_THROW(Err::InvalidParam, u8"Attempted to seek past end of file for SeekMethod::Current");
				}
				break;
			default:
				DO_THROW(Err::InvalidParam, u8"Unrecognized seek method");
		}
		return m_stream->Seek(position, m);
	}

	FileInt FileReader::Position() const {
		return m_stream->Position();
	}

	bool FileReader::IsOpen() const {
		return m_stream->IsOpen();
	}

	FileReader::FileReader(const std::string& name ):
		m_stream(std::make_shared<StreamFile>(name))
	{}

	FileReader::FileReader(Stream::Ptr s):
		m_stream(s)
	{}

	std::vector<uint8_t> ReadAll(FileReader::Ptr file) {
		if (!file->IsOpen()) {
			file->Open();
		}
		auto sz = file->Size();
		std::vector<uint8_t> vec(sz);
		file->ReadFull(&vec[0], sz);
		return vec;
	}

	uint32_t ReadNet32(FileReader::Ptr reader) {
		uint32_t pre;
		if (reader->Read(&pre, 4, 1) != 1) throw std::runtime_error("EOF encountered");
		return Util::NToHl(pre);
	}

	uint16_t ReadNet16(FileReader::Ptr reader) {
		uint16_t pre;
		if (reader->Read(&pre, 2, 1) != 1) throw std::runtime_error("EOF encountered");
		return Util::NToHs(pre);
	}


	// TODO: Support for non-intel
	uint16_t ReadLE16(FileReader::Ptr reader) {
		uint16_t pre;
		if (reader->Read(&pre, 2, 1) != 1) throw std::runtime_error("EOF encountered");
		return pre;
	}

	uint32_t ReadLE32(FileReader::Ptr reader) {
		uint32_t pre;
		if (reader->Read(&pre, 4, 1) != 1) throw std::runtime_error("EOF encountered");
		return pre;
	}
}
