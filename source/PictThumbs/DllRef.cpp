#include "DllRef.h"

#include <windows.h>

long g_cRefModule = 0;

void DllRelease()
{
	InterlockedDecrement(&g_cRefModule);
}

void DllAddRef()
{
	InterlockedIncrement(&g_cRefModule);
}

long DllRefCount() {
	return g_cRefModule;
}
