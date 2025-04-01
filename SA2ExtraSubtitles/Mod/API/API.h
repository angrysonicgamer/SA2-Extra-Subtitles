#pragma once

extern "C"
{
	/// <summary>
	/// A function to use in other mods. Loads set of extra subtitles from a JSON file. 
	/// </summary>
	/// <param name="jsonPath">: Path to JSON file containing extra subtitles. The JSON format should match the one used in this mod.</param>
	/// <param name="language">: Text language (subtitles will be displayed if this language is set in the game).</param>
	/// <param name="codepage">: Target codepage. 932 (Shift-JIS) for Japanese, 1252 (Windows-1252) for other languages.</param>
	__declspec(dllexport) void LoadCustomExtraSubs(const char* jsonPath, Languages language, int codepage);
}