#include "pch.h"
#include "Mod/Global/MyMod.h"
#include "Mod/Other Mods/OtherMods.h"
#include "Mod/Config/Config.h"
#include "Mod/Text/ExtraSubs.h"


extern "C"
{
	__declspec(dllexport) void Init(const char* modPath, const HelperFunctions& helperFunctions)
	{
		MyMod::Init(modPath, helperFunctions);
		OtherMods::Init();
		OtherMods::CheckRetranslatedStoryComplete();
		Config::Init();
		ExtraSubtitles::Init();
	}

	__declspec(dllexport) void OnFrame()
	{
		ExtraSubtitles::OnFrame();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}