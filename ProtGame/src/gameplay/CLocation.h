#pragma once

#include "../../stdafx.h"

class CLocation
{
public:
	CLocation(std::string, std::string);
	~CLocation();

private:
	std::string mName;
	std::string mFilePath;

public:
	std::string getName() const { return mName; }
	std::string getFilePath() const { return mFilePath; }
};

