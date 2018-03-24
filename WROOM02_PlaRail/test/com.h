#ifndef com_h
#define com_h

#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

typedef int boolean;
typedef std::string String;
#define String(str) (std::to_string(str))

#include <vector>
#include <sstream>      // std::ostringstream
std::vector<std::string> split(const std::string &str, char sep)
{
	std::vector<std::string> v;
	std::stringstream ss(str);
	std::string buffer;
	while( std::getline(ss, buffer, sep) ) {
		v.push_back(buffer);
	}
	return v;
}

#endif
