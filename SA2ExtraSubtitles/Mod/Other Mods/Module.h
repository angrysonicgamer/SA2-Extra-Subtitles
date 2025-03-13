#pragma once

class Module
{
public:
	HMODULE DLLHandle = nullptr;
	
	void Init(const wchar_t* dllName);
	bool IsLoaded();
};