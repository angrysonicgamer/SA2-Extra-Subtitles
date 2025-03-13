#include "pch.h"
#include "Encoding.h"
#include "TextConv.hpp"


const char* Encode(const std::string& utf8text, Encoding targetEncoding)
{
	return UTF8toCodepage(utf8text.c_str(), targetEncoding);
}

const char* Encode1252(const wchar_t* utf16text)
{
	return UTF16toMBS(utf16text, Windows1252);
}

const char* EncodeSJIS(const wchar_t* utf16text)
{
	return UTF16toMBS(utf16text, ShiftJIS);
}