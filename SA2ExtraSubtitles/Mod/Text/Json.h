#pragma once

#include "ExtraSubs.h"
#include "Encoding.h"
#include "nlohmann-json/json.hpp"
using json = nlohmann::json;

class Json
{
public:
	static json Read(const char* modPath, const char* jsonName);
	static void ReadSubtitlesGroup(const json& j, std::map<int, SubtitleData>& extraSubs, const char* group, Encoding encoding);
};