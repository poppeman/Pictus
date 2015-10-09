#ifndef STREAM_H
#define STREAM_H

#include <memory>
#include "exception.h"
#include "types.h"

namespace Err {
	struct IOException:public Exception {
		IOException(const std::string& msg) :Exception("IO: " + msg) {}
	};

	struct FileNotOpen:public IOException {
		FileNotOpen(const std::string& msg) :IOException("FileNotOpen: " + msg) {}
	};

	struct FileAlreadyOpen:public IOException {
		FileAlreadyOpen(const std::string& msg) :IOException("FileAlreadyOpen: " + msg) {}
	};

	struct EndOfFile:public IOException {
		EndOfFile(const std::string& msg) :IOException("End of file: " + msg) {}
	};
}


namespace IO {
	enum class SeekMethod {
		Begin,
		Current,
		End
	};

	enum class OpenErrorCode {
		AccessViolation,
		FileNotFound,
		Succeeded
	};

	class Stream {
	public:
		std::wstring Name() const;
		bool Open();
		bool IsOpen() const;
		void Close();
		size_t Read(void* buf, size_t size, size_t items);
		void Seek(FileInt position, SeekMethod m);
		FileInt Position() const;
		FileInt Size();

		Stream() = default;
		virtual ~Stream();

		typedef std::shared_ptr<Stream> Ptr;

	private:
		Stream(const Stream&) = delete;
		Stream& operator=(const Stream&) = delete;

		virtual std::wstring performName() const=0;
		virtual bool performOpen()=0;
		virtual bool performIsOpen() const = 0;
		virtual void performClose()=0;
		virtual size_t performRead(void* buf, size_t size, size_t items) = 0;
		virtual void performSeek(FileInt position, SeekMethod m) = 0;
		virtual FileInt performPosition() const = 0;
		virtual FileInt performSize() = 0;
	};
}

std::string ToAString(IO::SeekMethod m);

#endif
