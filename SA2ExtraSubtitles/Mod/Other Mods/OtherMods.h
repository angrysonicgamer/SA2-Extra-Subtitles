#pragma once

#include "Module.h"


class OtherMods
{
public:
	static Module RenderFix;
	static Module MenuOverhaul;
	static Module RetranslatedStoryComplete;

	static void Init();
	static void EnableMenuOverhaulTextFix();
	static void EnableRFExitMessagesFix(const HelperFunctions& helperFunctions);
	static void CheckRetranslatedStoryComplete();
};


struct PauseAndMessages
{
	const char* PauseMenu[10];
	const char* ExitMessages[8];
	const char* NetworkStuff[4];
};

DataArray(const char**, AdvertiseGamepad, 0xC70590, 6);
DataArray(const char**, AdvertiseKeyboard, 0xC705A8, 6);
DataArray(PauseAndMessages, PauseMenuAndMessages, 0x174A7D0, 6);