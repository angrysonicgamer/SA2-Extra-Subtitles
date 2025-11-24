#include "pch.h"
#include "Encoding.h"
#include "TextConv.hpp"


const char* Encode(const std::string& utf8text, int codepage)
{
	return UTF8toCodepage(utf8text.c_str(), codepage);
}

const char* Encode1252(const wchar_t* utf16text)
{
	return UTF16toMBS(utf16text, Windows1252);
}

const char* EncodeSJIS(const wchar_t* utf16text)
{
	return UTF16toMBS(utf16text, ShiftJIS);
}

const wchar_t* EncodeUTF16(const std::string& utf8text)
{
	return MBStoUTF16(utf8text.c_str(), UTF8);
}