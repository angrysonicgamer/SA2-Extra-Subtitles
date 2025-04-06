#pragma once

class MyMod
{
public:
	static std::string Path;
	static HelperFunctions Helper;

	static void Init(const char* modPath, const HelperFunctions& helperFunctions);
};