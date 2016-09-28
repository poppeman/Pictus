#ifndef ORZ_EXCEPTION_H
#define ORZ_EXCEPTION_H

#include <string>
#include <stdexcept>

namespace Err {
	struct Unsupported:public std::runtime_error {
		Unsupported(const std::string& msg);
	};

	struct DuplicateInstance:public std::runtime_error {
		DuplicateInstance();
	};

	struct CriticalError:public std::runtime_error {
		CriticalError(const std::string& msg);
	};

	struct InvalidCall:public std::runtime_error {
		InvalidCall(const std::string& msg);
	};

	struct InvalidParam:public std::runtime_error {
		InvalidParam(const std::string& msg);
	};

	struct NotYetImplemented:public std::runtime_error {
		NotYetImplemented(const std::string& msg);
	};
}

std::string DoThrowBuildDescription(const char* filename, int line, const char* functionName, const std::string& description);

// Works on MSVC, Clang, GCC
#define DO_THROW(exception, description) throw exception(DoThrowBuildDescription(__FILE__, __LINE__, __FUNCTION__, std::string(description)))
#define EXCEPTION(exception, description) exception(DoThrowBuildDescription(__FILE__, __LINE__, __FUNCTION__, std::string(description)))

#endif
