#include "Helper.h"
#include "orz/fileops.h"
#include "orz/folder.h"
#include "orz/types.h"

#include <windows.h>

void copyFiles( const std::string& source, const std::string dest ) {
	IO::Folder f;
	f.Path(source.c_str());
	auto files = f.CurrentContents();

	for (const auto& i : files) {
		if (i.Type == IO::TypeFile) {
			CopyFileW(
				UTF8ToWString(source + i.Name).c_str(),
				UTF8ToWString(dest + i.Name).c_str(),
				false);
		}
	}
}
