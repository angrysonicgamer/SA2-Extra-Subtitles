#include "pch.h"
#include "Module.h"


void Module::Init(const wchar_t* dllName)
{
	ModHandle = GetModuleHandle(dllName);
}

bool Module::IsLoaded()
{
	return ModHandle != nullptr;
}