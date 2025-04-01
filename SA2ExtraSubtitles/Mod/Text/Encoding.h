#pragma once

enum Encoding
{
	Windows1252 = 1252,
	ShiftJIS = 932
};

const char* Encode(const std::string& utf8text, int codepage);
const char* Encode1252(const wchar_t* utf16text);
const char* EncodeSJIS(const wchar_t* utf16text);