#include "corepch.h"
#include "logger.h"

#include <iostream>
#include <utility>
#include <Windows.h>

namespace zeus
{
	static constexpr short WHITE	= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	static constexpr short RED		= FOREGROUND_RED;
	static constexpr short GREEN	= FOREGROUND_GREEN;
	static constexpr short BLUE		= FOREGROUND_BLUE;
	static constexpr short YELLOW	= FOREGROUND_RED | FOREGROUND_GREEN;

	void Logger::Log(LogLevel level, const char* filename, int line, const char* function, const char* msg, bool showInfo, bool engine)
	{
		unsigned short color = 0;
		switch (level)
		{
		case LogLevel::Info:
			color = GREEN;
			break;
		case LogLevel::Trace:
			color = WHITE;
			break;
		case LogLevel::Warn:
			color = YELLOW;
			break;
		case LogLevel::Error:
			color = RED;
			break;
		}

		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(consoleHandle, color);
		OutputDebugStringA(msg);
		uint64_t bufferLength = strlen(msg);
		LPDWORD charsWritten = nullptr;
		if (engine)
		{
			std::cout << "[ENGINE]::";
		}
		if (showInfo)
		{
			std::cout << filename << ":" << line << "->" << function << ": ";
		}
		WriteConsoleA(consoleHandle, msg, (DWORD)bufferLength, charsWritten, 0);
		SetConsoleTextAttribute(consoleHandle, WHITE);
	}
}
