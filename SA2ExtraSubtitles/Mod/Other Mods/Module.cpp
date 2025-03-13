#include "pch.h"
#include "Module.h"


void Module::Init(const wchar_t* dllName)
{
	DLLHandle = GetModuleHandle(dllName);
}

bool Module::IsLoaded()
{
	return DLLHandle != nullptr;
}