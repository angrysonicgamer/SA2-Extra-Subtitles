#pragma once

enum DisplayTextMode
{
	NoDisappear,	// 0 - 2 lines, doesn't disappear
	TopAlign,		// 1 - 2 lines, text aligned to the top
	BottomAlign,	// 2 - 2 lines, text aligned to the bottom
	Adjusting,		// 3 - number of lines depends on the text
	Animated,		// 4 - same as 3 but animated
	DisplayNothing, // 5 - empty
};

enum DisplayConditions
{
	Normal,
	OnFrameBased,
	EmptyIntro,
	Victory
};

struct SubtitleData
{
	const char* Text;
	int Duration;
	DisplayConditions Condition;
};

class ExtraSubtitles
{
public:
	static void Init(const char* modPath);
	static void OnFrame();
};