#include "stdafx.h"
#include "CLocationParser.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

using boost::property_tree::ptree;

CLocationParser::CLocationParser()
{
}


CLocationParser::~CLocationParser()
{
}

std::vector<CLocation> CLocationParser::loadLocations(std::string path)
{
	std::ifstream is;
	is.open(path.c_str(), std::ifstream::in);

	ptree pt;
	std::vector<CLocation> locations;

	read_xml(is, pt);

	BOOST_FOREACH(ptree::value_type const& v, pt.get_child("locations")) 
	{
		if (v.first == "location") 
		{
			std::string filePath = v.second.get<std::string>("file");
			std::string name = v.second.get("<xmlattr>.name", "NoName");
			CLocation l(name, filePath);
			locations.push_back(l);
		}
	}

	for (auto & i : locations)
	{
		std::cout << i.getName() << ": " << i.getFilePath() << std::endl;
	}

	return locations;
}