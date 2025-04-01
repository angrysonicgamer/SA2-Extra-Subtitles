#include "pch.h"
#include "ExtraSubs.h"
#include "Encoding.h"
#include "Json.h"
#include "Mod/Config/Config.h"
#include "UsercallFunctionHandler.h"


DataPointer(int, HudControl, 0x174AFE0);
FunctionPointer(ObjectMaster*, DrawSubtitles, (DisplayTextMode mode, const char* text, int duration, int language), 0x6B6E20);
UsercallFunc(signed int, PlayVoice_Hook, (int idk, int id), (idk, id), 0x443130, rEAX, rEDX, stack4);

static std::map<int, SubtitleData> ExtraSubs[6];


// For OnFrame setup

int SubtitleDuration = 0;
int SubtitleDisplayFrameCount = 0;
int CurrentDisplayCondition = 0;
const char* TextBuffer = nullptr;


// Subtitles for normal voices

void DisplaySubtitleNormally(int id)
{
	DrawSubtitles(Adjusting, ExtraSubs[TextLanguage][id].Text, ExtraSubs[TextLanguage][id].Duration, TextLanguage);
}

void SetUp2PModeParameters()
{
	if (TwoPlayerMode)
	{
		HudControl = 0;
		SplitscreenMode = 1;
	}
}

void SetUpOnFrameBasedSubtitle(int id)
{
	TextBuffer = ExtraSubs[TextLanguage][id].Text;
	SubtitleDuration = ExtraSubs[TextLanguage][id].Duration;
	SubtitleDisplayFrameCount = 1;
	CurrentDisplayCondition = ExtraSubs[TextLanguage][id].Condition;
	SetUp2PModeParameters();
}


void DisplayExtraSub(int id)
{
	if (!ExtraSubs[TextLanguage].count(id)) return;
	
	if (ExtraSubs[TextLanguage][id].Condition == Normal)
	{
		DisplaySubtitleNormally(id);
	}
	else if (ExtraSubs[TextLanguage][id].Condition == Victory)
	{
		SetUp2PModeParameters();
		DisplaySubtitleNormally(id);
	}
	else if (ExtraSubs[TextLanguage][id].Condition == EmptyIntro)
	{
		if (GameState != GameStates_Inactive)
		{
			SetUpOnFrameBasedSubtitle(id);
		}
	}
	else
	{
		SetUpOnFrameBasedSubtitle(id);
	}
}


// Function to hook

signed int PlayVoice_ExtraSubs(int idk, int id)
{
	auto returnValue = PlayVoice_Hook.Original(idk, id);
	DisplayExtraSub(id);
	return returnValue;
}


// Loading extra subs

void ReadGroup(const char* group, Json* languages)
{
	languages[Language_Japanese].GetSubtitlesGroup(ExtraSubs[Language_Japanese], group, ShiftJIS);
	languages[Language_English].GetSubtitlesGroup(ExtraSubs[Language_English], group, Windows1252);
	languages[Language_French].GetSubtitlesGroup(ExtraSubs[Language_French], group, Windows1252);
	languages[Language_Spanish].GetSubtitlesGroup(ExtraSubs[Language_Spanish], group, Windows1252);
	// just add more languages here if needed
}

void LoadExtraSubs(const char* modPath)
{
	Json languages[6];
	languages[Language_Japanese].Read(modPath, "Japanese.json");
	languages[Language_English].Read(modPath, "EnglishDub.json");
	languages[Language_French].Read(modPath, "French.json");
	languages[Language_Spanish].Read(modPath, "Spanish.json");

	if (Config::MenuSubsEnabled)
	{
		ReadGroup("Menu", languages);
	}

	if (Config::IdleSubsEnabled)
	{
		ReadGroup("Idle", languages);
	}

	if (Config::StageSpecificSubsEnabled)
	{
		ReadGroup("Stage specific voices", languages);
	}

	if (Config::RankSubsEnabled)
	{
		ReadGroup("Rank voices", languages);
	}

	if (Config::VictorySubsEnabled)
	{
		ReadGroup("Victory lines", languages);
	}

	if (Config::GameplaySubsEnabled)
	{
		ReadGroup("Gameplay voices", languages);
	}

	if (Config::BossSubsEnabled)
	{
		ReadGroup("Bosses", languages);
	}
	
	if (Config::TwoPlayerSubsEnabled)
	{
		ReadGroup("2P Battle", languages);
	}
}


void ExtraSubtitles::Init(const char* modPath)
{
	LoadExtraSubs(modPath);
	PlayVoice_Hook.Hook(PlayVoice_ExtraSubs);
}



// OnFrame stuff

bool NonSplitScreen2PStage()
{
	return CurrentLevel == LevelIDs_WeaponsBed2P
		|| CurrentLevel == LevelIDs_MissionStreet2P
		|| CurrentLevel == LevelIDs_SandOcean2P
		|| CurrentLevel == LevelIDs_HiddenBase2P
		|| CurrentLevel == LevelIDs_CosmicWall2P
		|| CurrentLevel == LevelIDs_IronGate2P;
}

void ClearSubtitle()
{
	SubtitleDisplayFrameCount = 0;
	SubtitleDuration = 0;
}

void DisplayBufferedText(DisplayTextMode mode)
{
	DrawSubtitles(mode, TextBuffer, 1, TextLanguage);
}

void DisplaySinglePlayerSubtitle()
{
	if (SubtitleDisplayFrameCount > 0 && SubtitleDisplayFrameCount <= SubtitleDuration)
	{
		DisplayBufferedText(Adjusting);
		SubtitleDisplayFrameCount++;
	}

	if (SubtitleDisplayFrameCount > SubtitleDuration)
	{
		ClearSubtitle();
	}
}

void Do2PThings()
{
	if (SubtitleDisplayFrameCount > 0)
	{
		if (SubtitleDisplayFrameCount++ > SubtitleDuration)
		{
			HudControl = 1;
			SplitscreenMode = 2;
			if (NonSplitScreen2PStage())
			{
				HudControl = 0;
				SplitscreenMode = 1;
			}
			ClearSubtitle();
		}
	}

	if (SubtitleDisplayFrameCount > 1 && SubtitleDisplayFrameCount < SubtitleDuration)
	{
		DisplayTextMode mode = CurrentDisplayCondition == EmptyIntro ? DisplayNothing : Adjusting;
		DisplayBufferedText(mode);
	}
}


void ExtraSubtitles::OnFrame()
{
	if (TwoPlayerMode)
	{
		Do2PThings();
	}
	else
	{
		DisplaySinglePlayerSubtitle();
	}
}
