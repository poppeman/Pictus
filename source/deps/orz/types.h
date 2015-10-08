#ifndef ORZ_TYPES_H
#define ORZ_TYPES_H

#include <string>
#include <cstdint>
#include <vector>
#include <sstream>

// TODO: Remove these and use uint8_t, etc instead.
typedef __int64 FileInt;

// TODO: Remove this bullshit.
#define TX_(t) L##t
#define TX(t) TX_(t)

std::wstring ToWString(uint8_t b);
std::wstring ToWString(int i);
std::wstring ToWString(uint32_t d);
std::wstring ToWString(const char* i);

template <typename T>
std::wstring ToWString(T i) {
	std::wstringstream ss;
	ss << i;
	return ss.str();
}

std::string ToAString(int i);
std::string ToAString(const std::wstring& s);

template <typename T>
T FromAString(const std::string& s) {
	std::stringstream ss(s);
	T i;
	ss >> i;
	return i;
}

template <typename T>
T FromWString(const std::wstring& s) {
	std::wstringstream ss(s);
	T i;
	ss >> i;
	return i;
}

std::wstring UTF8ToWString(const char* utf8);

int RoundCast(float rhs);

bool IsDecimal(const std::wstring& s);

// Modified design by contract macros.
// I don't want to remove ALL pre/post checks in release build but it can be
// very nifty to remove the silliest ones

std::wstring DoThrowBuildDescription(const wchar_t* filename, int line, const wchar_t* functionName, const std::wstring& description);

#ifdef _MSC_VER
#define DO_THROW(exception, description) throw exception(DoThrowBuildDescription(TX(__FILE__), __LINE__, TX(__FUNCTION__), description))
#else
#define DO_THROW(exception, description) throw exception(DoThrowBuildDescription(L"UNK_FILE", 0, L"UNK_FUNC", std::wstring(description)))
#endif
#define COND_STRICT(condition, exception, description) { if ((condition) == false) DO_THROW(exception, (description)); }

namespace Util {
	template <typename T> const T& Min(const T& a, const T& b) { return a<b?a:b; }
	template <typename T> const T& Min(const T& a, const T& b, const T& c) {
		if(a < b) {
			if(c < a)
				return c;
			return a;
		}
		else if(c < b)
			return c;
		return b;
	}
	template <typename T> const T& Max(const T& a, const T& b) { return a>b?a:b; }
	template <typename T> const T& Max(const T& a, const T& b, const T& c) { return Max(a, Max(b, c)); }


	template <typename T> const T& Constrain(const T& lower, const T& val, const T& upper) {
		return Util::Min<T>(Util::Max<T>(lower, val), upper);
	}
}

namespace Num {
	const float Pi = 3.14159265358979323846f;
	const float Pi_2 = 1.57079632679489661923f;
	const float Pi_4 = 0.785398163397448309616f;
}

std::wstring ToUpper(const std::wstring& s);
std::wstring ToLower(const std::wstring& s);

template <typename C>
std::basic_string<C> Implode(const std::vector<std::basic_string<C>> arr, const C* glue = 0) {
	std::basic_stringstream<C> toret;
	bool first = true;
	for (const auto& item : arr) {
		if (!first) {
			toret << glue;
		}
		toret << item;
		first = false;
	}
	return toret.str();
}


#endif
