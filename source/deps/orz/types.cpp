#include "types.h"

#include <algorithm>
#include <cwctype>
#include <memory>

#include <boost/locale.hpp>

// This include is to force boost.system to be linked. Boost.locale requires it, but the auto-linking seems to be broken.
#include <boost/thread.hpp>

int RoundCast(float rhs) {
	return static_cast<int>(rhs + 0.5f);
}

std::wstring UTF8ToWString(const std::string& utf8) {
	return boost::locale::conv::utf_to_utf<wchar_t>(utf8);
}

std::string WStringToUTF8(const std::wstring& utf16) {
	return boost::locale::conv::utf_to_utf<char>(utf16);
}

std::wstring ToWString( uint32_t i ) {
	std::wstringstream ss;
	ss << i;
	return ss.str();
}

std::wstring ToWString( int i ) {
	std::wstringstream ss;
	ss << i;
	return ss.str();
}

std::wstring ToWString( uint8_t i ) {
	std::wstringstream ss;
	ss << i;
	return ss.str();
}

std::wstring ToWString( const char* i ) {
	std::wstringstream ss;
	ss << i;
	return ss.str();
}

std::string ToAString(const std::wstring& s) {
	return std::string(s.begin(), s.end());
}

std::string ToAString(int i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}
