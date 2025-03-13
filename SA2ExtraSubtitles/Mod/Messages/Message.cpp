#include "pch.h"
#include "Message.h"


void Message::Warning(const std::wstring& message)
{
	int returnValue = MessageBox(NULL, message.c_str(), L"SA2 Extra Subtitles: Warning", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2);

	if (returnValue == IDCANCEL)
	{
		exit(EXIT_FAILURE);
	}
}

void Message::Error(const std::wstring& message)
{
	MessageBox(NULL, message.c_str(), L"SA2 Extra Subtitles: Error", MB_ICONERROR);
	exit(EXIT_FAILURE);
}