#include "exception.h"
#include "types.h"

std::wstring DoThrowBuildDescription(const wchar_t* filename, int lineNum, const wchar_t* functionName, const std::wstring& description)
{
	return L"\nFile: " + ::std::wstring(filename) + L"\nLine: " + ::ToWString(lineNum) + L"\nFunction: " + ::std::wstring(functionName) + L"\n\nDescription: " + description;
}

namespace Err {
	Exception::Exception(const std::wstring& msg):
		m_message(msg)
	{}

	Exception::~Exception() {}

	const std::wstring& Err::Exception::Desc() const
	{
		return m_message;
	}

	Unsupported::Unsupported(const std::wstring& msg):
		Exception(L"Unsupported operation: " + msg)
	{}

	DuplicateInstance::DuplicateInstance():
		Exception(L"DuplicateInstance")
	{}

	CriticalError::CriticalError(const std::wstring& msg):
		Exception(L"Critical error: " + msg)
	{}

	InvalidCall::InvalidCall(const std::wstring& msg):
		Exception(L"Invalid call: " + msg)
	{}

	InvalidParam::InvalidParam(const std::wstring& msg):
		Exception(L"Invalid param: " + msg)
	{}

	NotYetImplemented::NotYetImplemented(const std::wstring& msg):
		Exception(L"Not yet implemented:" + msg)
	{}
}