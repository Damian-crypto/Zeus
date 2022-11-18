#pragma once

#ifdef _DEBUG
	#define DEBUG_MODE 1
#else
	#define DEBUG_MODE 0
#endif

#ifdef _MSC_VER
	#define BREAK_POINT __debugbreak()
#endif
