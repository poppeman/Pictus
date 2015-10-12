#include "main.h"
#include "orz/types.h"
#include <iostream>

#include <UnitTest++/UnitTest++.h>

std::string g_datapath;

int wmain(int argc, wchar_t* argv[]) {
	if (argc < 2) {
		std::cout << "Data path argument not set, test run failed\n";
		return 1;
	}
	g_datapath = WStringToUTF8(argv[1]);

	return UnitTest::RunAllTests();
}