#include "pch.h"
#include "Config.h"
#include "Mod/Other Mods/OtherMods.h"
#include "IniFile.hpp"


bool Config::MenuSubsEnabled;
bool Config::IdleSubsEnabled;
bool Config::StageSpecificSubsEnabled;
bool Config::RankSubsEnabled;
bool Config::VictorySubsEnabled;
bool Config::GameplaySubsEnabled;
bool Config::BossSubsEnabled;
bool Config::TwoPlayerSubsEnabled;
bool Config::FinalSceneSubsEnabled;

bool Config::MenuOverhaulTextFixEnabled;
bool Config::RFExitMessagesFixEnabled;


void Config::Read(const char* modPath)
{
	IniFile config(std::string(modPath) + "\\config.ini");

	MenuSubsEnabled = config.getBool("DisplaySubtitles", "Menu", true);
	IdleSubsEnabled = config.getBool("DisplaySubtitles", "Idle", true);
	StageSpecificSubsEnabled = config.getBool("DisplaySubtitles", "Stages", true);
	RankSubsEnabled = config.getBool("DisplaySubtitles", "Rank", true);
	VictorySubsEnabled = config.getBool("DisplaySubtitles", "Victory", true);
	GameplaySubsEnabled = config.getBool("DisplaySubtitles", "Gameplay", true);
	BossSubsEnabled = config.getBool("DisplaySubtitles", "Boss", true);
	TwoPlayerSubsEnabled = config.getBool("DisplaySubtitles", "2P", true);
	FinalSceneSubsEnabled = config.getBool("DisplaySubtitles", "Cutscene", true);

	MenuOverhaulTextFixEnabled = config.getBool("OtherMods", "EnableMenuOverhaulTextFix", true);
	RFExitMessagesFixEnabled = config.getBool("OtherMods", "EnableRFExitMessagesFix", true);
}

void Config::Init(const char* modPath, const HelperFunctions& helperFunctions)
{
	Read(modPath);

	if (!FinalSceneSubsEnabled)
	{
		helperFunctions.UnreplaceFile("resource\\gd_PC\\event\\e0210_j.prs");
		helperFunctions.UnreplaceFile("resource\\gd_PC\\event\\e0210_1.prs");
	}
	
	if (MenuOverhaulTextFixEnabled)
	{
		OtherMods::EnableMenuOverhaulTextFix();
	}

	if (RFExitMessagesFixEnabled)
	{
		OtherMods::EnableRFExitMessagesFix(helperFunctions);
	}
}