#ifndef ORZ_EXCEPTION_H
#define ORZ_EXCEPTION_H

#include <string>
#include <stdexcept>

namespace Err {
	struct Exception:public std::runtime_error {
		Exception(const std::string& msg);
		virtual ~Exception();
	};

	struct Unsupported:public Exception {
		Unsupported(const std::string& msg);
	};

	struct DuplicateInstance:public Exception {
		DuplicateInstance();
	};

	struct CriticalError:public Exception {
		CriticalError(const std::string& msg);
	};

	struct InvalidCall:public Exception {
		InvalidCall(const std::string& msg);
	};

	struct InvalidParam:public Exception {
		InvalidParam(const std::string& msg);
	};

	struct NotYetImplemented:public Exception {
		NotYetImplemented(const std::string& msg);;
	};
}

// Modified design by contract macros.
// I don't want to remove ALL pre/post checks in release build but it can be
// very nifty to remove the silliest ones

std::string DoThrowBuildDescription(const char* filename, int line, const char* functionName, const std::string& description);

#ifdef _MSC_VER
#define MAKE_STRING(x) #x
#define DO_THROW(exception, description) throw exception(DoThrowBuildDescription(MAKE_STRING(__FILE__), __LINE__, MAKE_STRING(__FUNCTION__), description))
#else
#define DO_THROW(exception, description) throw exception(DoThrowBuildDescription("UNK_FILE", 0, "UNK_FUNC", std::string(description)))
#endif
#define COND_STRICT(condition, exception, description) { if ((condition) == false) DO_THROW(exception, (description)); }


#endif
