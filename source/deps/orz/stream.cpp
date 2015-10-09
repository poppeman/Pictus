#include "stream.h"

namespace IO {
	std::wstring Stream::Name() const {
		return performName();
	}

	bool Stream::Open() {
		if (IsOpen()) {
			DO_THROW(Err::FileAlreadyOpen, "Stream is already open.");
		}
		return performOpen();
	}

	bool Stream::IsOpen() const {
		return performIsOpen();
	}

	void Stream::Close() {
		performClose();
	}

	size_t Stream::Read(void* buf, size_t size, size_t items) {
		if (IsOpen() == false) {
			DO_THROW(Err::FileNotOpen, "Stream is not open.");
		}
		return performRead(buf, size, items);
	}

	void Stream::Seek(FileInt position, SeekMethod m) {
		if (IsOpen() == false) {
			DO_THROW(Err::FileNotOpen, "Stream is not open.");
		}
		return performSeek(position, m);
	}

	FileInt Stream::Position() const {
		if (IsOpen() == false) {
			DO_THROW(Err::FileNotOpen, "Stream is not open.");
		}
		return performPosition();
	}

	FileInt Stream::Size() {
		if (IsOpen() == false) {
			DO_THROW(Err::FileNotOpen, "Stream is not open.");
		}
		return performSize();
	}

	Stream::~Stream() {}
}

std::string ToAString(IO::SeekMethod m) {
	switch (m) {
	case IO::SeekMethod::Begin:
		return "Begin";
	case IO::SeekMethod::Current:
		return "Current";
	case IO::SeekMethod::End:
		return "End";
	}
	return "Unknown";
}
