#include <string>

// String related utility functions for removing white spaces of a string(trailing/leading)
static inline void left_trim(std::string& s)
{
	s.erase(s.begin(), std::find_if(
		s.begin(),
		s.end(),
		[](unsigned char c) { return !std::isspace(c); }
	)
	);
}

static inline void right_trim(std::string& s)
{
	s.erase(std::find_if(
		s.rbegin(),
		s.rend(),
		[](unsigned char c) { return !std::isspace(c); }
	).base(),
		s.end());
}

static inline void trim(std::string& s)
{
	left_trim(s);
	right_trim(s);
}
