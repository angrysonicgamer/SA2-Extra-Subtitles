#include "pch.h"
#include "ExtraSubs.h"
#include "Encoding.h"
#include "Json.h"
#include "Mod/Config/Config.h"
#include "UsercallFunctionHandler.h"


DataPointer(int, HudControl, 0x174AFE0);
DataPointer(int, SplitScreenControl, 0x1DD946C);
DataPointer(short, MenuOrGame, 0x1934BE0);


enum DisplayTextMode
{
	NoDisappear,	// 0 - 2 lines, doesn't disappear
	TopAlign,		// 1 - 2 lines, text aligned to the top
	BottomAlign,	// 2 - 2 lines, text aligned to the bottom
	Adjusting,		// 3 - number of lines depends on the text
	Animated,		// 4 - same as 3 but animated
	DisplayNothing, // 5 - empty
};

FunctionPointer(ObjectMaster*, DrawSubtitles, (DisplayTextMode mode, const char* text, int duration, int language), 0x6B6E20);
UsercallFunc(signed int, PlayVoice_Hook, (int idk, int id), (idk, id), 0x443130, rEAX, rEDX, stack4);

int SubtitleDuration = 0;
int SubtitleDisplayFrameCount = 0;
int CurrentDisplayCondition = 0;
const char* TextBuffer = nullptr;


// Subtitles maps

static std::map<int, SubtitleData> ExtraSubs_English;
static std::map<int, SubtitleData> ExtraSubs_Japanese;

static std::map<int, SubtitleData>* ExtraSubs[]
{
	&ExtraSubs_Japanese,
	&ExtraSubs_English,
	nullptr,				// French
	nullptr,				// Spanish
	nullptr,				// German
	nullptr,				// Italian
};


// Subtitles for normal voices

void DisplaySubtitleNormally(int id)
{
	DrawSubtitles(Adjusting, ExtraSubs[TextLanguage]->at(id).Text, ExtraSubs[TextLanguage]->at(id).Duration, TextLanguage);
}

void SetUp2PModeParameters()
{
	if (TwoPlayerMode)
	{
		HudControl = 0;
		SplitScreenControl = 1;
	}
}

void SetUpOnFrameBasedSubtitle(int id)
{
	TextBuffer = ExtraSubs[TextLanguage]->at(id).Text;
	SubtitleDuration = ExtraSubs[TextLanguage]->at(id).Duration;
	SubtitleDisplayFrameCount = 1;
	CurrentDisplayCondition = ExtraSubs[TextLanguage]->at(id).Condition;
	SetUp2PModeParameters();
}


void DisplayExtraSub(int id)
{
	if (ExtraSubs[TextLanguage] == nullptr) return;
	if (!ExtraSubs[TextLanguage]->count(id)) return;
	
	if (ExtraSubs[TextLanguage]->at(id).Condition == Normal)
	{
		DisplaySubtitleNormally(id);
	}
	else if (ExtraSubs[TextLanguage]->at(id).Condition == Victory)
	{
		SetUp2PModeParameters();
		DisplaySubtitleNormally(id);
	}
	else if (ExtraSubs[TextLanguage]->at(id).Condition == EmptyIntro)
	{
		if (MenuOrGame != 0)
		{
			SetUpOnFrameBasedSubtitle(id);
		}
	}
	else
	{
		SetUpOnFrameBasedSubtitle(id);
	}
}


signed int PlayVoice_ExtraSubs(int idk, int id)
{
	auto returnValue = PlayVoice_Hook.Original(idk, id);
	DisplayExtraSub(id);
	return returnValue;
}

void LoadExtraSubs(const char* modPath)
{
	json english = Json::Read(modPath, "EnglishDub.json");
	json japanese = Json::Read(modPath, "Japanese.json");

	if (Config::MenuSubsEnabled)
	{
		Json::ReadSubtitlesGroup(english, ExtraSubs_English, "Menu", Windows1252);
		Json::ReadSubtitlesGroup(japanese, ExtraSubs_Japanese, "Menu", ShiftJIS);
	}

	if (Config::IdleSubsEnabled)
	{
		Json::ReadSubtitlesGroup(english, ExtraSubs_English, "Idle", Windows1252);
		Json::ReadSubtitlesGroup(japanese, ExtraSubs_Japanese, "Idle", ShiftJIS);
	}

	if (Config::StageSpecificSubsEnabled)
	{
		Json::ReadSubtitlesGroup(english, ExtraSubs_English, "Stage specific voices", Windows1252);
		Json::ReadSubtitlesGroup(japanese, ExtraSubs_Japanese, "Stage specific voices", ShiftJIS);
	}

	if (Config::RankSubsEnabled)
	{
		Json::ReadSubtitlesGroup(english, ExtraSubs_English, "Rank voices", Windows1252);
		Json::ReadSubtitlesGroup(japanese, ExtraSubs_Japanese, "Rank voices", ShiftJIS);
	}

	if (Config::VictorySubsEnabled)
	{
		Json::ReadSubtitlesGroup(english, ExtraSubs_English, "Victory lines", Windows1252);
		Json::ReadSubtitlesGroup(japanese, ExtraSubs_Japanese, "Victory lines", ShiftJIS);
	}

	if (Config::GameplaySubsEnabled)
	{
		Json::ReadSubtitlesGroup(english, ExtraSubs_English, "Gameplay voices", Windows1252);
		Json::ReadSubtitlesGroup(japanese, ExtraSubs_Japanese, "Gameplay voices", ShiftJIS);
	}

	if (Config::BossSubsEnabled)
	{
		Json::ReadSubtitlesGroup(english, ExtraSubs_English, "Bosses", Windows1252);
		Json::ReadSubtitlesGroup(japanese, ExtraSubs_Japanese, "Bosses", ShiftJIS);
	}
	
	if (Config::TwoPlayerSubsEnabled)
	{
		Json::ReadSubtitlesGroup(english, ExtraSubs_English, "2P Battle", Windows1252);
		Json::ReadSubtitlesGroup(japanese, ExtraSubs_Japanese, "2P Battle", ShiftJIS);
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
			SplitScreenControl = 2;
			if (NonSplitScreen2PStage())
			{
				HudControl = 0;
				SplitScreenControl = 1;
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