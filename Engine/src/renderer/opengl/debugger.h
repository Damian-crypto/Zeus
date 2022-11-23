#pragma once

#include "core.h"

#ifdef DEBUG_MODE
	#define GLCheck(x) x; Check(__FUNCTION__, __FILE__, __LINE__)
	#define GLVALIDATE if (not Validate()) BREAK_POINT
#else
	#define GLCheck(x) x;
	#define GLVALIDATE
#endif

bool Validate();
void Check(const char* function, const char* file, int line);
