#include "timeconvert.h"
#include "orz/exception.h"

#include <boost/scoped_array.hpp>
#include <windows.h>

namespace App {
	FILETIME ToFILETIME(FileInt i) {
		FILETIME f;
		f.dwLowDateTime = i & 0xffffffff;
		f.dwHighDateTime = (i >> 32) & 0xffffffff;
		return f;
	}

	SYSTEMTIME ToSYSTEMTIME(FileInt date) {
		SYSTEMTIME st;
		FILETIME ft = ToFILETIME(date);
		if(FileTimeToSystemTime(&ft, &st) == false) {
			DO_THROW(Err::InvalidCall, "Failed converting to system time.");
		}
		return st;
	}

	std::wstring FormattedDate(FileInt date) {
		SYSTEMTIME st = ToSYSTEMTIME(date);
		int len=GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, 0, 0, 0);
		boost::scoped_array<wchar_t> tmpStr(new wchar_t[len]);
		GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, 0, tmpStr.get(), len);
		return std::wstring(tmpStr.get());
	}

	std::wstring FormattedTime(FileInt date) {
		SYSTEMTIME st = ToSYSTEMTIME(date);
		int len=GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, 0, 0, 0);
		boost::scoped_array<wchar_t> tmpStr(new wchar_t[len]);
		GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, 0, tmpStr.get(), len);
		return std::wstring(tmpStr.get());
	}
}
