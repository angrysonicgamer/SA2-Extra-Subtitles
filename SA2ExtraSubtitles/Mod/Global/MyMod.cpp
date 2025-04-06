#include "pch.h"
#include "MyMod.h"


std::string MyMod::Path;
HelperFunctions MyMod::Helper;

void MyMod::Init(const char* modPath, const HelperFunctions& helperFunctions)
{
	Path = modPath;
	Helper = helperFunctions;
}