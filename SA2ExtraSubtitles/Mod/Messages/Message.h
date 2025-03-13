#pragma once

class Message
{
public:
	static void Warning(const std::wstring& message);
	static void Error(const std::wstring& message);
};