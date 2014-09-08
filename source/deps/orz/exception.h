#ifndef EXCEPTION_H
#define EXCEPTION_H

namespace Err {
	struct Exception {
		Exception(const std::wstring& msg):m_message(msg) {}
		virtual ~Exception() {}

		const std::wstring& Desc() const {
			return m_message;
		}

	private:
		std::wstring				m_message;
	};

	struct Unsupported:public Exception {
		Unsupported(const std::wstring& msg):Exception(TX("Unsupported operation: ")+msg) {}
	};

	struct DuplicateInstance:public Exception {
		DuplicateInstance():Exception(TX("DuplicateInstance")) {}
	};

	struct CriticalError:public Exception {
		CriticalError(const std::wstring& msg):Exception(TX("Critical error: ")+msg) {}
	};

	struct InvalidCall:public Exception {
		InvalidCall(const std::wstring& msg):Exception(TX("Invalid call: ") + msg) {}
	};

	struct InvalidParam:public Exception {
		InvalidParam(const std::wstring& msg):Exception(TX("Invalid param: ")+msg) {}
	};

	struct NotYetImplemented:public Exception {
		NotYetImplemented(const std::wstring& msg):Exception(TX("Not yet implemented:") + msg) {};
	};
}

#endif
