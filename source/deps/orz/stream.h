#ifndef STREAM_H
#define STREAM_H

namespace Err {
	struct IOException:public Exception {
		IOException(const std::wstring& msg):Exception(TX("IO: ") + msg) {};
	};

	struct FileNotOpen:public IOException {
		FileNotOpen(const std::wstring& msg):IOException(TX("FileNotOpen: ")+msg) {}
	};

	struct FileAlreadyOpen:public IOException {
		FileAlreadyOpen(const std::wstring& msg):IOException(TX("FileAlreadyOpen: ")+msg) {}
	};

	struct EndOfFile:public IOException {
		EndOfFile(const std::wstring& msg):IOException(TX("End of file: ")+msg) {}
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

	class Stream:boost::noncopyable {
	public:
		_Check_return_ std::wstring Name() const;
		_Check_return_ bool Open();
		_Check_return_ bool IsOpen() const;
		void Close();
		_Check_return_ size_t Read(_Out_writes_bytes_(size * items) void* buf, _In_ size_t size, _In_ size_t items);
		void Seek(_In_ FileInt position, _In_ SeekMethod m);
		_Check_return_ FileInt Position() const;
		_Check_return_ FileInt Size();

		virtual ~Stream();

		typedef std::shared_ptr<Stream> Ptr;

	private:
		virtual _Check_return_ std::wstring performName() const=0;
		virtual _Check_return_ bool performOpen()=0;
		virtual _Check_return_ bool performIsOpen() const = 0;
		virtual void performClose()=0;
		virtual _Check_return_ size_t performRead(_Out_writes_bytes_(size * items) void* buf, _In_ size_t size, _In_ size_t items) = 0;
		virtual void performSeek(_In_ FileInt position, _In_ SeekMethod m) = 0;
		virtual _Check_return_ FileInt performPosition() const = 0;
		virtual _Check_return_ FileInt performSize() = 0;
	};
}

std::wstring ToWString(IO::SeekMethod m);

#endif
