#pragma once

enum EfMsgMode
{
	EfMsg_NoDisappear,		// 0 - 2 lines, doesn't disappear
	EfMsg_TopAlign,			// 1 - 2 lines, text aligned to the top
	EfMsg_BottomAlign,		// 2 - 2 lines, text aligned to the bottom
	EfMsg_Adjusting,		// 3 - number of lines depends on the text
	EfMsg_Animated,			// 4 - same as 3 but animated
	EfMsg_DisplayNothing,	// 5 - empty
};

enum SubtitleModes
{
	Normal,
	OnFrameBased,
	EmptyIntro,
	Victory,
	LightAttackSE,
	KartBoostSE
};

struct SubtitleData
{
	const char* Text;
	int Duration;
	SubtitleModes Mode;
};

class ExtraSubtitles
{
public:
	static void Init();
	static void OnFrame();
};