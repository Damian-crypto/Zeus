#pragma once
//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#define LOG(level, ...)																	\
{																						\
	char buffer[1024];																	\
	sprintf_s(buffer, __VA_ARGS__);														\
	zeus::Logger::Log(zeus::LogLevel::level, __FILE__, __LINE__, __FUNCTION__, buffer);	\
	puts("");																			\
}																						\

#define LOG_ENGINE(level, ...)																	\
{																								\
	char buffer[1024];																			\
	sprintf_s(buffer, __VA_ARGS__);																\
	zeus::Logger::Log(zeus::LogLevel::level, __FILE__, __LINE__, __FUNCTION__, buffer, false);	\
	puts("");																					\
}																								\


namespace zeus
{
	enum class LogLevel
	{
		Info	= 0,
		Trace	= 1,
		Warn	= 2,
		Error	= 3
	};

	class Logger
	{
	public:
		static void Log(LogLevel level, const char* filename, int line, const char* function, const char* msg, bool showInfo = true);
	};
}
