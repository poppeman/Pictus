#include "../deps/UnitTest++/UnitTest++.h"
#include "main.h"
#include <iostream>

std::wstring g_datapath;

int wmain(int argc, wchar_t* argv[]) {
	if (argc < 2) {
		std::cout << "Data path argument not set, test run failed\n";
		return 1;
	}
	g_datapath = argv[1];

	return UnitTest::RunAllTests();
}