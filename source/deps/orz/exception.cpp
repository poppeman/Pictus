#include "exception.h"
#include "types.h"

std::string DoThrowBuildDescription(const char* filename, int lineNum, const char* functionName, const std::string& description)
{
	return "\nFile: " + ::std::string(filename) + "\nLine: " + ::ToAString(lineNum) + "\nFunction: " + ::std::string(functionName) + "\n\nDescription: " + description;
}

namespace Err {
	Unsupported::Unsupported(const std::string& msg):
		std::runtime_error("Unsupported operation: " + msg)
	{}

	DuplicateInstance::DuplicateInstance():
		std::runtime_error("DuplicateInstance")
	{}

	CriticalError::CriticalError(const std::string& msg):
		std::runtime_error("Critical error: " + msg)
	{}

	InvalidCall::InvalidCall(const std::string& msg):
		std::runtime_error("Invalid call: " + msg)
	{}

	InvalidParam::InvalidParam(const std::string& msg):
		std::runtime_error("Invalid param: " + msg)
	{}

	NotYetImplemented::NotYetImplemented(const std::string& msg):
		std::runtime_error("Not yet implemented:" + msg)
	{}
}
