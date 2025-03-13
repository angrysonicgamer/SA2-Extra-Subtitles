#pragma once

class Module
{
public:
	HMODULE ModHandle = nullptr;
	
	void Init(const wchar_t* dllName);
	bool IsLoaded();
};