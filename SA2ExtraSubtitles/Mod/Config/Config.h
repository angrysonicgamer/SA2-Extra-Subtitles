#pragma once

class Config
{
private:
	static void Read();

public:
	static bool MenuSubsEnabled;
	static bool IdleSubsEnabled;
	static bool StageSpecificSubsEnabled;
	static bool RankSubsEnabled;
	static bool VictorySubsEnabled;
	static bool GameplaySubsEnabled;
	static bool BossSubsEnabled;
	static bool TwoPlayerSubsEnabled;	
	static bool FinalSceneSubsEnabled;
	static bool SoundEffectSubsEnabled;

	static bool MenuOverhaulTextFixEnabled;
	static bool RFExitMessagesFixEnabled;
	
	static void Init();
};