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

std::pair<int, SubtitleData> ReadExtraSub(const json& value, Encoding targetEncoding)
{
	int voiceID = value["VoiceID"];
	std::string text = value["Text"];
	int duration = value["Duration"];
	std::string mode = value["Mode"];

	return { voiceID, { Encode("\a" + text, targetEncoding), duration, DisplayModes[mode] } };
}



void Json::Read(const char* modPath, const char* jsonName)
{
	std::string path = std::string(modPath) + "\\Text\\" + jsonName;
	PrintDebug("[SA2 Extra Subtitles] Reading file: %s", path.c_str());
	std::ifstream jsonFile(path);

	try
	{
		contents = json::parse(jsonFile);
	}
	catch (std::exception& ex)
	{
		std::string exText = std::string(ex.what());
		std::wstring jsonError = L"Error reading file:\n" + std::wstring(path.begin(), path.end()) + L"\n\nCaught exception:\n" + std::wstring(exText.begin(), exText.end()) + L"\n\nThe game will be closed.";
		Message::Error(jsonError);
	}
}

void Json::GetSubtitlesGroup(std::map<int, SubtitleData>& destination, const char* group, Encoding targetEncoding)
{
	for (auto& jsonObject : contents["Subtitles"][group])
	{
		for (auto& value : jsonObject)
		{
			destination.insert(ReadExtraSub(value, targetEncoding));
		}
	}
}