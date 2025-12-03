#include "pch.h"
#include "ExtraSubs.h"
#include "Encoding.h"
#include "Json.h"
#include "Mod/Global/MyMod.h"
#include "Mod/Config/Config.h"
#include "Mod/API/API.h"
#include "UsercallFunctionHandler.h"


DataPointer(int, HudControl, 0x174AFE0);
FunctionPointer(task*, DrawSubtitles, (EfMsgMode mode, const char* text, int duration, int language), 0x6B6E20);
UsercallFunc(int, PlayVoice_Hook, (int pri, int id), (pri, id), 0x443130, rEAX, rEDX, stack4);
UsercallFunc(char, PlaySE_Hook, (int se, int id, int pri, int volofs), (se, id, pri, volofs), 0x437260, rAL, rESI, stack4, stack4, stack4);
UsercallFunc(char, PlaySE2_Hook, (int se, NJS_VECTOR* pos, int id, int pri, int volofs), (se, pos, id, pri, volofs), 0x4372E0, rAL, rEDI, rESI, stack4, stack4, stack4);


class SubtitlesSet
{
private:
	std::map<int, SubtitleData> subtitles[6];

public:
	void LoadGroup(const char* group, Json* languages)
	{
		languages[Language_Japanese].GetSubtitlesGroup(subtitles[Language_Japanese], group, ShiftJIS);
		languages[Language_English].GetSubtitlesGroup(subtitles[Language_English], group, Windows1252);
		languages[Language_French].GetSubtitlesGroup(subtitles[Language_French], group, Windows1252);
		languages[Language_Spanish].GetSubtitlesGroup(subtitles[Language_Spanish], group, Windows1252);
		//languages[Language_German].GetSubtitlesGroup(subtitles[Language_German], group, Windows1252);
		//languages[Language_Italian].GetSubtitlesGroup(subtitles[Language_German], group, Windows1252);
	}

	std::map<int, SubtitleData>& operator[](int index)
	{
		return subtitles[index];
	}
};

static SubtitlesSet MainSubs;
static SubtitlesSet SoundEffectSubs;


// For OnFrame setup

int SubtitleDuration = 0;
int SubtitleDisplayFrameCount = 0;
int CurrentDisplayMode = 0;
const char* TextBuffer = nullptr;


// Subtitles for normal voices

void DisplaySubtitleNormally(int id)
{
	DrawSubtitles(EfMsg_Adjusting, MainSubs[TextLanguage][id].Text, MainSubs[TextLanguage][id].Duration, TextLanguage);
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
	TextBuffer = MainSubs[TextLanguage][id].Text;
	SubtitleDuration = MainSubs[TextLanguage][id].Duration;
	SubtitleDisplayFrameCount = 1;
	CurrentDisplayMode = MainSubs[TextLanguage][id].Mode;
	SetUp2PModeParameters();
}


void DisplayMainSub(int id)
{
	if (!MainSubs[TextLanguage].count(id)) return;
	
	if (MainSubs[TextLanguage][id].Mode == Normal)
	{
		DisplaySubtitleNormally(id);
	}
	else if (MainSubs[TextLanguage][id].Mode == Victory)
	{
		SetUp2PModeParameters();
		DisplaySubtitleNormally(id);
	}
	else if (MainSubs[TextLanguage][id].Mode == EmptyIntro)
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


// Soundbank subtitles

void DisplaySoundEffectSub(int se)
{
	DrawSubtitles(EfMsg_Adjusting, SoundEffectSubs[TextLanguage][se].Text, SoundEffectSubs[TextLanguage][se].Duration, TextLanguage);
}

void DisplayLightAttackSub(int se)
{
	if (!SoundEffectSubs[TextLanguage].count(se)) return;

	if (SoundEffectSubs[TextLanguage][se].Mode == LightAttackSE)
	{
		DisplaySoundEffectSub(se);
	}
}

void DisplayKartBoostSub(int se)
{
	if (!SoundEffectSubs[TextLanguage].count(se)) return;

	if (CurrentLevel == LevelIDs_Route101280 || CurrentLevel == LevelIDs_KartRace)
	{
		DisplaySoundEffectSub(se);
	}
}


// Functions to hook

int PlayVoice_ExtraSubs(int pri, int id)
{
	auto returnValue = PlayVoice_Hook.Original(pri, id);
	DisplayMainSub(id);
	return returnValue;
}

char PlaySE_ExtraSubs(int se, int id, int pri, int volofs) // this is only for kart boost voices
{
	auto returnValue = PlaySE_Hook.Original(se, id, pri, volofs);
	DisplayKartBoostSub(se);
	return returnValue;
}

char PlaySE2_ExtraSubs(int se, NJS_VECTOR* pos, int id, int pri, int volofs) // this is only for light attack voices
{
	auto returnValue = PlaySE2_Hook.Original(se, pos, id, pri, volofs);
	DisplayLightAttackSub(se);
	return returnValue;
}


// Loading extra subs

void LoadExtraSubs()
{
	std::string folder = MyMod::Path + "\\Text\\";
	Json languages[6];
	languages[Language_Japanese].Read(folder + "Japanese.json");
	languages[Language_English].Read(folder + "EnglishDub.json");
	languages[Language_French].Read(folder + "French.json");
	languages[Language_Spanish].Read(folder + "Spanish.json");
	//languages[Language_German].Read(folder + "German.json");
	//languages[Language_Italian].Read(folder + "Italian.json");

	if (Config::MenuSubsEnabled)
	{
		MainSubs.LoadGroup("Menu", languages);
	}

	if (Config::IdleSubsEnabled)
	{
		MainSubs.LoadGroup("Idle", languages);
	}

	if (Config::StageSpecificSubsEnabled)
	{
		MainSubs.LoadGroup("Stage specific voices", languages);
	}

	if (Config::RankSubsEnabled)
	{
		MainSubs.LoadGroup("Rank voices", languages);
	}

	if (Config::VictorySubsEnabled)
	{
		MainSubs.LoadGroup("Victory lines", languages);
	}

	if (Config::GameplaySubsEnabled)
	{
		MainSubs.LoadGroup("Gameplay voices", languages);
	}

	if (Config::BossSubsEnabled)
	{
		MainSubs.LoadGroup("Bosses", languages);
	}
	
	if (Config::TwoPlayerSubsEnabled)
	{
		MainSubs.LoadGroup("2P Battle", languages);
	}

	if (Config::SoundEffectSubsEnabled)
	{
		SoundEffectSubs.LoadGroup("SE", languages);
	}	
}


void ExtraSubtitles::Init()
{
	LoadExtraSubs();
	PlayVoice_Hook.Hook(PlayVoice_ExtraSubs);
	PlaySE_Hook.Hook(PlaySE_ExtraSubs);
	PlaySE2_Hook.Hook(PlaySE2_ExtraSubs);
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

void DisplayBufferedText(EfMsgMode mode)
{
	DrawSubtitles(mode, TextBuffer, 1, TextLanguage);
}

void DisplaySinglePlayerSubtitle()
{
	if (SubtitleDisplayFrameCount > 0 && SubtitleDisplayFrameCount <= SubtitleDuration)
	{
		DisplayBufferedText(EfMsg_Adjusting);
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
		EfMsgMode mode = CurrentDisplayMode == EmptyIntro ? EfMsg_DisplayNothing : EfMsg_Adjusting;
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



// DLL export

void LoadCustomExtraSubs(const char* jsonPath, Languages language, int codepage)
{
	Json custom;
	custom.Read(jsonPath);
	MainSubs[language].clear();

	if (Config::MenuSubsEnabled)
	{
		custom.GetSubtitlesGroup(MainSubs[language], "Menu", codepage);
	}

	if (Config::IdleSubsEnabled)
	{
		custom.GetSubtitlesGroup(MainSubs[language], "Idle", codepage);
	}

	if (Config::StageSpecificSubsEnabled)
	{
		custom.GetSubtitlesGroup(MainSubs[language], "Stage specific voices", codepage);
	}

	if (Config::RankSubsEnabled)
	{
		custom.GetSubtitlesGroup(MainSubs[language], "Rank voices", codepage);
	}

	if (Config::VictorySubsEnabled)
	{
		custom.GetSubtitlesGroup(MainSubs[language], "Victory lines", codepage);
	}

	if (Config::GameplaySubsEnabled)
	{
		custom.GetSubtitlesGroup(MainSubs[language], "Gameplay voices", codepage);
	}

	if (Config::BossSubsEnabled)
	{
		custom.GetSubtitlesGroup(MainSubs[language], "Bosses", codepage);
	}

	if (Config::TwoPlayerSubsEnabled)
	{
		custom.GetSubtitlesGroup(MainSubs[language], "2P Battle", codepage);
	}

	if (Config::SoundEffectSubsEnabled)
	{
		custom.GetSubtitlesGroup(SoundEffectSubs[language], "SE", codepage);
	}
}