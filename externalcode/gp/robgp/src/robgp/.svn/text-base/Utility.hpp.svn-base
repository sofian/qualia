#pragma once
#include <sstream>
#include <string>

/* Utility functions for simple commonly needed features
 */

template <class T>
inline std::string to_string (const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

std::string inline operator+(std::string a, int i)
{
	return a+to_string<int>(i);
}

