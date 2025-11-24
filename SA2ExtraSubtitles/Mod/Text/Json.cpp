#include "pch.h"
#include "Json.h"
#include "Encoding.h"
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
	// The path is UTF-8 encoded so it might cause issues if it contains non-ASCII characters because functions reading a const char* string would likely expect a single-byte encoding, so I'm doing UTF-8 to UTF-16 conversion to avoid this
	std::wstring utf16JsonPath = EncodeUTF16(jsonPath);
	PrintDebug("[SA2 Extra Subtitles] Reading file: %s", jsonPath.c_str());
	std::ifstream jsonFile(utf16JsonPath);

	try
	{
		contents = json::parse(jsonFile);
	}
	catch (std::exception& ex)
	{
		Message::Error(L"Error reading file:\n" + utf16JsonPath + L"\n\nCaught exception:\n" + EncodeUTF16(ex.what()) + L"\n\nThe game will be closed.");
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