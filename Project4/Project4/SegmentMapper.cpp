#include "provided.h"
#include "MyMap.h"
#include <vector>
#include <string>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	string toString(const GeoCoord& gc) const;
	MyMap<string, vector<StreetSegment>> m_map;
};

string SegmentMapperImpl::toString(const GeoCoord& gc) const { // returns a string version of a GeoCoord
	return gc.latitudeText + " " + gc.longitudeText;
}

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < ml.getNumSegments(); i++) {
		StreetSegment s;
		ml.getSegment(i, s);
		vector<StreetSegment> vss; // creates new vector with current StreetSegment
		vss.push_back(s);
		vector<StreetSegment>* vssptr = m_map.find(toString(s.segment.start));
		if (vssptr != nullptr)
			vssptr->push_back(s); // if starting coordinate already exists, adds segment to its vector
		else
			m_map.associate(toString(s.segment.start), vss); // otherwise creates new association
		vssptr = m_map.find(toString(s.segment.end));
		if (vssptr != nullptr)
			vssptr->push_back(s); // if ending coordinate already exists, adds segment to its vector
		else
			m_map.associate(toString(s.segment.end), vss); // otherwise creates new association
		for (int j = 0; j < s.attractions.size(); j++) {
			vssptr = m_map.find(toString(s.attractions[j].geocoordinates));
			if (vssptr != nullptr) 
				vssptr->push_back(s); // if attraction coordinate already exists, adds segment to its vector
			else
				m_map.associate(toString(s.attractions[j].geocoordinates), vss); // otherwise creates new association
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	const vector<StreetSegment>* vssptr = m_map.find(toString(gc)); // finds vector of StreetSegments using MyMap's find function
	vector<StreetSegment> segments;
	if (vssptr != nullptr)
		segments = *vssptr;
	return segments; 
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
