#include "Helper.h"
#include "orz/fileops.h"
#include "orz/folder.h"

void copyFiles( const std::wstring& source, const std::wstring dest ) {
	IO::Folder f;
	f.Path(source.c_str());
	auto files = f.CurrentContents();

	for (const auto& i : files) {
		if (i.Type == IO::TypeFile) {
			CopyFileW(
				(source + i.Name).c_str(),
				(dest + i.Name).c_str(),
				false);
		}
	}
}
