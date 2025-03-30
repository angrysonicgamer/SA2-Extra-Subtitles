#include "pch.h"
#include "ExtraSubs.h"
#include "Encoding.h"
#include "Json.h"
#include "Mod/Config/Config.h"
#include "UsercallFunctionHandler.h"


DataPointer(int, HudControl, 0x174AFE0);
FunctionPointer(ObjectMaster*, DrawSubtitles, (DisplayTextMode mode, const char* text, int duration, int language), 0x6B6E20);
UsercallFunc(signed int, PlayVoice_Hook, (int idk, int id), (idk, id), 0x443130, rEAX, rEDX, stack4);

int SubtitleDuration = 0;
int SubtitleDisplayFrameCount = 0;
int CurrentDisplayCondition = 0;
const char* TextBuffer = nullptr;


// Subtitles maps

static std::map<int, SubtitleData> ExtraSubs_English;
static std::map<int, SubtitleData> ExtraSubs_Japanese;
static std::map<int, SubtitleData> ExtraSubs_French;

static std::map<int, SubtitleData>* ExtraSubs[]
{
	&ExtraSubs_Japanese,
	&ExtraSubs_English,
	& ExtraSubs_French,				// French
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
		SplitscreenMode = 1;
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

void ReadGroup(const char* group, const json& english, const json& japanese, const json& french) // just add more languages here if needed
{
	Json::ReadSubtitlesGroup(english, ExtraSubs_English, group, Windows1252);
	Json::ReadSubtitlesGroup(japanese, ExtraSubs_Japanese, group, ShiftJIS);
	Json::ReadSubtitlesGroup(french, ExtraSubs_French, group, Windows1252);
}

void LoadExtraSubs(const char* modPath)
{
	json english = Json::Read(modPath, "EnglishDub.json");
	json japanese = Json::Read(modPath, "Japanese.json");
	json french = Json::Read(modPath, "French.json");

	if (Config::MenuSubsEnabled)
	{
		ReadGroup("Menu", english, japanese, french);
	}

	if (Config::IdleSubsEnabled)
	{
		ReadGroup("Idle", english, japanese, french);
	}

	if (Config::StageSpecificSubsEnabled)
	{
		ReadGroup("Stage specific voices", english, japanese, french);
	}

	if (Config::RankSubsEnabled)
	{
		ReadGroup("Rank voices", english, japanese, french);
	}

	if (Config::VictorySubsEnabled)
	{
		ReadGroup("Victory lines", english, japanese, french);
	}

	if (Config::GameplaySubsEnabled)
	{
		ReadGroup("Gameplay voices", english, japanese, french);
	}

	if (Config::BossSubsEnabled)
	{
		ReadGroup("Bosses", english, japanese, french);
	}
	
	if (Config::TwoPlayerSubsEnabled)
	{
		ReadGroup("2P Battle", english, japanese, french);
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
