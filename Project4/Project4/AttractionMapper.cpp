#include "provided.h"
#include "MyMap.h"
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> m_map;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < ml.getNumSegments(); i++) { // runs through all segments in the MapLoader
		StreetSegment s;
		ml.getSegment(i, s);
		for (int j = 0; j < s.attractions.size(); j++) { // runs through the attractions in each StreetSegment
			string name, temp = s.attractions[j].name;
			for (int k = 0; k < temp.size(); k++) // makes the name lowercase
				name += tolower(temp[k]);
			m_map.associate(name, s.attractions[j].geocoordinates); // adds attraction to map
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	string temp;
	for (int k = 0; k < attraction.size(); k++) // makes attraction name lowercase
		temp += tolower(attraction[k]);
	const GeoCoord* gcptr = m_map.find(temp); // uses MyMap's find function to find attraction
	if (gcptr == nullptr)
		return false;  
	gc = *gcptr;
	return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
