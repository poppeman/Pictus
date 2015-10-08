#include "version.h"
#include "orz/types.h"

std::wstring ToWString(const App::Version ver) {
	return ToWString(ver.Major) + L"." + ToWString(ver.Minor) + L"." + ToWString(ver.Revision);
}

namespace App {
	bool Version::operator>(const Version& rhs) {
		if (Major > rhs.Major) return true;
		if (Major < rhs.Major) return false;
		if (Minor > rhs.Minor) return true;
		if (Minor < rhs.Minor) return false;
		if (Revision > rhs.Revision) return true;
		return false;
	}
}
