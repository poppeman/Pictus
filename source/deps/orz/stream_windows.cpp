#include "StdAfx.h"
#include "stream_windows.h"

namespace IO {
	_Use_decl_annotations_ StreamWindows::StreamWindows(IStream* i) :m_pStream(i) {
	}

	_Use_decl_annotations_ StreamWindows::StreamWindows(const std::wstring& filename) : m_filename(filename), m_pStream(nullptr) {}

	StreamWindows::~StreamWindows() {
		if (m_pStream) {
			m_pStream->Release();
		}
		m_pStream = nullptr;
	}

	_Use_decl_annotations_ bool StreamWindows::performOpen() {
		if (m_filename.empty() == false) {
			if (FAILED(SHCreateStreamOnFileW(m_filename.c_str(), STGM_READ | STGM_SHARE_DENY_WRITE, &m_pStream))) {
				//DO_THROW(Err::InvalidParam, TX("Could not create stream for file: ") + filename);
				return false;
			}
		}

		return true;
	}

	_Use_decl_annotations_ bool StreamWindows::performIsOpen() const {
		return m_pStream != nullptr;
	}

	void StreamWindows::performClose() {}

	_Use_decl_annotations_ size_t StreamWindows::performRead(void* buf, size_t size, size_t items) {
		ULONG bytesRead;
		auto bytesToRead = size * items;
		auto hr = m_pStream->Read(buf, bytesToRead, &bytesRead);
		if (hr != S_OK && hr != S_FALSE) {
			// IStream::Read returns S_FALSE for incomplete reads, which is totally bonkers but oh well.
			Log << L"(StreamWindows::performRead) Failed reading data\n";
			return 0;
		}

		if (bytesRead != bytesToRead) {
			return bytesRead / size;
		}
		return items;
		//m_pStream->Revert()
	}

	_Use_decl_annotations_ void StreamWindows::performSeek(FileInt position, SeekMethod m) {
		DWORD dwOrigin;

		if (m == IO::SeekMethod::Begin) {
			dwOrigin = STREAM_SEEK_SET;
		}
		else if (m == IO::SeekMethod::Current) {
			dwOrigin = STREAM_SEEK_CUR;
		}
		else if (m == IO::SeekMethod::End) {
			dwOrigin = STREAM_SEEK_END;
		}
		else {
			DO_THROW(Err::InvalidParam, L"Unsupported seek method: " + ToWString(m));
		}
		LARGE_INTEGER pos;
		pos.QuadPart = position;
		if (FAILED(m_pStream->Seek(pos, dwOrigin, 0))) {
			DO_THROW(Err::CriticalError, L"Seek failed");
		}
	}

	_Use_decl_annotations_ FileInt StreamWindows::performPosition() const {
		ULARGE_INTEGER pos;
		LARGE_INTEGER dummy;
		dummy.QuadPart = 0;
		m_pStream->Seek(dummy, STREAM_SEEK_CUR, &pos);
		return pos.QuadPart;
	}

	_Use_decl_annotations_ FileInt StreamWindows::performSize() {
		STATSTG ssg;
		if (m_pStream->Stat(&ssg, STATFLAG_NONAME) != S_OK) {
			return 0;
		}

		return ssg.cbSize.QuadPart;
	}

	_Use_decl_annotations_ std::wstring StreamWindows::performName() const {
		STATSTG ssg;
		if (m_pStream->Stat(&ssg, STATFLAG_DEFAULT) != S_OK) {
			return std::wstring(L"");
		}

		std::wstring toReturn(ssg.pwcsName);
		CoTaskMemFree(ssg.pwcsName);
		return toReturn;
	}
}
