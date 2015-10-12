#include "Logger.h"
#include "orz/types.h"
#include <mutex>
#include <iostream>

std::mutex					g_mutexOutput;

#define DISABLEOUTPUT 

void OutputString( const std::string& toOutput, bool important )
{
#ifdef DISABLEOUTPUT
	if(!important) return;
#endif

	std::lock_guard<std::mutex> l(g_mutexOutput);
	std::wcout << UTF8ToWString(toOutput + "\n").c_str();
}
