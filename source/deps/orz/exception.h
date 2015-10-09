#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

namespace Err {
	struct Exception {
		Exception(const std::wstring& msg);
		virtual ~Exception();

		const std::wstring& Desc() const;

	private:
		std::wstring m_message;
	};

	struct Unsupported:public Exception {
		Unsupported(const std::wstring& msg);
	};

	struct DuplicateInstance:public Exception {
		DuplicateInstance();
	};

	struct CriticalError:public Exception {
		CriticalError(const std::wstring& msg);
	};

	struct InvalidCall:public Exception {
		InvalidCall(const std::wstring& msg);
	};

	struct InvalidParam:public Exception {
		InvalidParam(const std::wstring& msg);
	};

	struct NotYetImplemented:public Exception {
		NotYetImplemented(const std::wstring& msg);;
	};
}

// Modified design by contract macros.
// I don't want to remove ALL pre/post checks in release build but it can be
// very nifty to remove the silliest ones

std::wstring DoThrowBuildDescription(const wchar_t* filename, int line, const wchar_t* functionName, const std::wstring& description);

#ifdef _MSC_VER
#define MAKE_STRING(x) L#x
#define DO_THROW(exception, description) throw exception(DoThrowBuildDescription(MAKE_STRING(__FILE__), __LINE__, MAKE_STRING(__FUNCTION__), description))
#else
#define DO_THROW(exception, description) throw exception(DoThrowBuildDescription(L"UNK_FILE", 0, L"UNK_FUNC", std::wstring(description)))
#endif
#define COND_STRICT(condition, exception, description) { if ((condition) == false) DO_THROW(exception, (description)); }


#endif
