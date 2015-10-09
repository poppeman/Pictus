#include "exception.h"
#include "types.h"

std::string DoThrowBuildDescription(const char* filename, int lineNum, const char* functionName, const std::string& description)
{
	return "\nFile: " + ::std::string(filename) + "\nLine: " + ::ToAString(lineNum) + "\nFunction: " + ::std::string(functionName) + "\n\nDescription: " + description;
}

namespace Err {
	Exception::Exception(const std::string& msg):
		std::runtime_error(msg)
	{}

	Exception::~Exception() {}

	Unsupported::Unsupported(const std::string& msg):
		Exception("Unsupported operation: " + msg)
	{}

	DuplicateInstance::DuplicateInstance():
		Exception("DuplicateInstance")
	{}

	CriticalError::CriticalError(const std::string& msg):
		Exception("Critical error: " + msg)
	{}

	InvalidCall::InvalidCall(const std::string& msg):
		Exception("Invalid call: " + msg)
	{}

	InvalidParam::InvalidParam(const std::string& msg):
		Exception("Invalid param: " + msg)
	{}

	NotYetImplemented::NotYetImplemented(const std::string& msg):
		Exception("Not yet implemented:" + msg)
	{}
}
