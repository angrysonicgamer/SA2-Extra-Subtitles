#pragma once

class Config
{
private:
	static void Read(const char* modPath);
public:
	static bool MenuSubsEnabled;
	static bool IdleSubsEnabled;
	static bool StageSpecificSubsEnabled;
	static bool RankSubsEnabled;
	static bool VictorySubsEnabled;
	static bool GameplaySubsEnabled;
	static bool BossSubsEnabled;
	static bool TwoPlayerSubsEnabled;

	static bool MenuOverhaulTextFixEnabled;
	static bool RFExitMessagesFixEnabled;
	
	static void Init(const char* modPath, const HelperFunctions& helperFunctions);
};