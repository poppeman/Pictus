#include "stream.h"

namespace IO {
	_Check_return_ std::wstring Stream::Name() const {
		return performName();
	}

	_Check_return_ bool Stream::Open() {
		if (IsOpen()) {
			DO_THROW(Err::FileAlreadyOpen, L"Stream is already open.");
		}
		return performOpen();
	}

	_Check_return_ bool Stream::IsOpen() const {
		return performIsOpen();
	}

	void Stream::Close() {
		performClose();
	}

	_Check_return_ size_t Stream::Read(_Out_writes_bytes_(size * items) void* buf, _In_ size_t size, _In_ size_t items) {
		if (IsOpen() == false) {
			DO_THROW(Err::FileNotOpen, L"Stream is not open.");
		}
		return performRead(buf, size, items);
	}

	void Stream::Seek(_In_ FileInt position, _In_ SeekMethod m) {
		if (IsOpen() == false) {
			DO_THROW(Err::FileNotOpen, L"Stream is not open.");
		}
		return performSeek(position, m);
	}

	_Check_return_ FileInt Stream::Position() const {
		if (IsOpen() == false) {
			DO_THROW(Err::FileNotOpen, L"Stream is not open.");
		}
		return performPosition();
	}

	_Check_return_ FileInt Stream::Size() {
		if (IsOpen() == false) {
			DO_THROW(Err::FileNotOpen, L"Stream is not open.");
		}
		return performSize();
	}

	Stream::~Stream() {}
}

std::wstring ToWString(IO::SeekMethod m) {
	switch (m) {
	case IO::SeekMethod::Begin:
		return L"Begin";
	case IO::SeekMethod::Current:
		return L"Current";
	case IO::SeekMethod::End:
		return L"End";
	}
	return L"Unknown";
}
