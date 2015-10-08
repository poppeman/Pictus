#include "Logger.h"
#include <mutex>
#include <iostream>

std::mutex					g_mutexOutput;

#define DISABLEOUTPUT 

void OutputString( const std::wstring& toOutput, bool important )
{
#ifdef DISABLEOUTPUT
	if(!important) return;
#endif

	std::lock_guard<std::mutex> l(g_mutexOutput);
	std::wcout << (toOutput + L"\n").c_str();
}
