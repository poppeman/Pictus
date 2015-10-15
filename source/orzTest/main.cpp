#include "main.h"
#include "orz/types.h"

#include <boost/locale.hpp>
#include <iostream>

#include <UnitTest++/UnitTest++.h>

std::string g_datapath;

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[]) {
	if (argc < 2) {
		std::cout << "Data path argument not set, test run failed\n";
		return 1;
	}
	g_datapath = WStringToUTF8(argv[1]);

    std::locale::global(boost::locale::generator().generate(""));
	return UnitTest::RunAllTests();
}
#else
int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Data path argument not set, test run failed\n";
		return 1;
	}
	g_datapath = argv[1];
	std::cout << "Starting orzTest ... " << g_datapath << std::endl;

    std::locale::global(boost::locale::generator().generate(""));
	return UnitTest::RunAllTests();
}
#endif
