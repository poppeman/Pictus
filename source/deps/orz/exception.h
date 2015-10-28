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
		NotYetImplemented(const std::string& msg);
	};
}

std::string DoThrowBuildDescription(const char* filename, int line, const char* functionName, const std::string& description);

#ifdef _MSC_VER
#define DO_THROW(exception, description) throw exception(DoThrowBuildDescription(__FILE__, __LINE__, __FUNCTION__, description))
#else
#define DO_THROW(exception, description) throw exception(DoThrowBuildDescription("UNK_FILE", 0, "UNK_FUNC", std::string(description)))
#endif

#endif
