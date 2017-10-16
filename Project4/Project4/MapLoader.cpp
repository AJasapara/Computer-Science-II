#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment> segmentMap;
	int segmentsLoaded;
	int cutString(int start, char c, string whole, string &result);
};

MapLoaderImpl::MapLoaderImpl()
{
	segmentMap.reserve(1000);
	segmentsLoaded = 0;
}

MapLoaderImpl::~MapLoaderImpl()
{
	
}

int MapLoaderImpl::cutString(int start, char c, string whole, string & result) { // cuts string from index to desired char, and returns it
	result = "";
	for (; start < whole.size() && whole[start] != c; start++)
		result += whole[start];
	return start;
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);
	if (!infile)
		return false; 
	string name;
	while (getline(infile, name)) {
		StreetSegment ss;
		ss.streetName = name; // gets name
		string coords, coords1, coords2;
		if (!getline(infile, coords))
			break;
		int k = 0;
		k = 1 + cutString(k, ',', coords, coords1); // gets starting coordinates
		if (coords[k] == ' ') k++;
		k = 1 + cutString(k, ' ', coords, coords2);
		GeoCoord gc1(coords1, coords2);
		k = 1 + cutString(k, ',', coords, coords1);
		if (coords[k] == ' ') k++;
		cutString(k, '|', coords, coords2); // gets ending coordinates
		GeoCoord gc2(coords1, coords2);
		GeoSegment gs(gc1, gc2);
		ss.segment = gs;
		string attr;
		if (!getline(infile, attr)) // gets number of attractions
			break;
		int numAttractions = stoi(attr);
		for (int i = 0; i < numAttractions; i++) { // adds number of attractions
			string attraction;
			if (!getline(infile, attraction))
				return true;
			k = 0;
			k = 1 + cutString(k, '|', attraction, name); // adds name and coordinates
			k = 1 + cutString(k, ',', attraction, coords1);
			if (attraction[k] == ' ') k++;
			cutString(k, '|', attraction, coords2);
			GeoCoord gc(coords1, coords2);
			Attraction a;
			a.name = name;
			a.geocoordinates = gc;
			ss.attractions.push_back(a);
		}
		segmentMap.push_back(ss);
		segmentsLoaded++;
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return segmentsLoaded; // This compiles, but may not be correct
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < 0 || segNum >= getNumSegments()) // finds segment using MyMap's find
		return false;
	seg = segmentMap[segNum];
	return true;  // This compiles, but may not be correct
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
