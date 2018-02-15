#pragma once
#include "CLocation.h"

class CLocationParser
{
public:
	CLocationParser();
	~CLocationParser();

	static std::vector<CLocation> loadLocations(std::string);
};

