#include "stream_file.h"
#include "file_reader.h"
#include "ByteOrder.h"

namespace IO {
	using std::mutex;

	const std::wstring FileReader::Name() const {
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
		return m_stream->Seek(position, m);
	}

	FileInt FileReader::Position() const {
		return m_stream->Position();
	}

	bool FileReader::IsOpen() const {
		return m_stream->IsOpen();
	}

	FileReader::FileReader( _In_ const std::wstring& name )
		:m_stream(std::make_shared<StreamFile>(name))
	{}

	FileReader::FileReader(_In_ Stream::Ptr s)
		:m_stream(s)
	{}

	// TODO: Replace this with Boost.filesystem or something else that doesn't scream of NIH.
	void replace_substrings(std::wstring& str, const std::wstring& toFind, const std::wstring& toWrite, size_t offset=0) {
		std::size_t pos = offset;
		while(pos != std::wstring::npos) {
			pos = str.find(toFind, pos);
			if (pos != std::wstring::npos) {
				str.replace(
					str.begin() + pos, str.begin() + pos + toFind.length(),
					toWrite.begin(), toWrite.end());
			}
		}
	}

	std::wstring GetPath(const std::wstring& s) {
		std::wstring ret(s);
		replace_substrings(ret, L"/", L"\\");
		replace_substrings(ret, L"\\\\", L"\\", 1);

		std::size_t dot = ret.find_last_of(L"/\\");
		if (dot != std::wstring::npos) {
			return ret.substr(0, dot) + L"\\";
		}

		return std::wstring(L"");
	}

	std::wstring GetFile(const std::wstring& s) {
		std::size_t dot = s.find_last_of(L"/\\");
		if (dot != std::wstring::npos) {
			return s.substr(dot + 1, std::wstring::npos);
		}

		return std::wstring(L"");
	}

	std::wstring GetTitle(const std::wstring& s) {
		std::size_t start	= s.find_last_of(L"/\\");
		std::size_t end		= s.find_last_of(TX('.'));

		if (start != std::wstring::npos) {
			return s.substr(start + 1, end - start - 1);
		}

		return s.substr(0, end);
	}

	std::wstring GetExtension(const std::wstring& s) {
		std::size_t dot = s.find_last_of(TX('.'));
		if (dot != std::wstring::npos) {
			// Make sure there were no / or \ afterwards
			if (s.find_first_of(L"/\\", dot) != std::wstring::npos) return std::wstring(L"");
			std::wstring ext = s.substr(dot + 1);
			return ext;
		}
		return std::wstring(L"");
	}

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
