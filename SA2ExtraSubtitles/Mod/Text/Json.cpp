#include "pch.h"
#include "Json.h"
#include "Mod/Messages/Message.h"
#include <fstream>
#include <exception>


static std::map<std::string, DisplayConditions> DisplayModes
{
	{ "Normal", Normal },
	{ "OnFrame", OnFrameBased },
	{ "EmptyIntro", EmptyIntro },
	{ "Victory", Victory }
};

std::pair<int, SubtitleData> ReadExtraSub(const json& value, int codepage)
{
	int voiceID = value["VoiceID"];
	std::string text = value["Text"];
	int duration = value["Duration"];
	std::string mode = value["Mode"];

	return { voiceID, { Encode("\a" + text, codepage), duration, DisplayModes[mode] } };
}



void Json::Read(const std::string& jsonPath)
{
	PrintDebug("[SA2 Extra Subtitles] Reading file: %s", jsonPath.c_str());
	std::ifstream jsonFile(jsonPath);

	try
	{
		contents = json::parse(jsonFile);
	}
	catch (std::exception& ex)
	{
		std::string exText = std::string(ex.what());
		std::wstring jsonError = L"Error reading file:\n" + std::wstring(jsonPath.begin(), jsonPath.end()) + L"\n\nCaught exception:\n" + std::wstring(exText.begin(), exText.end()) + L"\n\nThe game will be closed.";
		Message::Error(jsonError);
	}
}

void Json::GetSubtitlesGroup(std::map<int, SubtitleData>& destination, const char* group, int codepage)
{
	for (auto& jsonObject : contents["Subtitles"][group])
	{
		for (auto& value : jsonObject)
		{
			destination.insert(ReadExtraSub(value, codepage));
		}
	}
}