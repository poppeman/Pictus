#ifndef PICTUS_VERSION_H
#define PICTUS_VERSION_H

#include "res_version.h"
#include <string>

namespace App {
	struct Version {
		int Major, Minor, Revision;
		bool operator >(const Version& rhs);
	};

	const Version CurrentVersion = {PICTUS_VER_MAJOR, PICTUS_VER_MINOR, PICTUS_VER_REVISION};
}

std::string ToAString(const App::Version ver);

#endif
