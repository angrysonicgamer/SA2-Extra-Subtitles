#pragma once

#include "ExtraSubs.h"
#include "Encoding.h"
#include "nlohmann-json/json.hpp"
using json = nlohmann::json;

class Json
{
private:
	json contents;

public:
	/// <summary>
	/// Parses specified JSON file containing extra subtitles
	/// </summary>
	void Read(const char* modPath, const char* jsonName);

	/// <summary>
	/// Reads a group of subtitles from parsed JSON and adds it to the destination map
	/// </summary>
	void GetSubtitlesGroup(std::map<int, SubtitleData>& destination, const char* group, Encoding encoding);
};