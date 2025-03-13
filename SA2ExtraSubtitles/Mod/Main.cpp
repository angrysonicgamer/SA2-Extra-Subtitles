#include "pch.h"
#include "Mod/Other Mods/OtherMods.h"
#include "Mod/Config/Config.h"
#include "Mod/Text/ExtraSubs.h"


extern "C"
{
	__declspec(dllexport) void Init(const char* modPath, const HelperFunctions& helperFunctions)
	{
		OtherMods::Init();
		OtherMods::CheckRetranslatedStoryComplete();
		Config::Init(modPath, helperFunctions);
		ExtraSubtitles::Init(modPath);
	}

	__declspec(dllexport) void OnFrame()
	{
		ExtraSubtitles::OnFrame();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}