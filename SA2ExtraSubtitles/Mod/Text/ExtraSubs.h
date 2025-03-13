#pragma once

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