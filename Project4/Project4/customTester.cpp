// The main.cpp you can use for testing will replace this file soon.

#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
	~KeepWindowOpenUntilDismissed()
	{
		DWORD pids[1];
		if (GetConsoleProcessList(pids, 1) == 1)
		{
			std::cout << "Press any key to continue . . . ";
			_getch();
		}
	}
} keepWindowOpenUntilDismissed;
#endif


#include "MyMap.h"
#include <string>
#include <iostream>
#include "provided.h"
using namespace std;

int main()
{
	MyMap<string, int> map;
	map.associate("hello", 10);
	cerr << *map.find("hello") << endl;
	map.associate("less than 10", 9);
	map.associate("more than 10", 11);
	map.associate("hello", 9);
	map.associate("yee", 8);
	map.associate("oite", 7);
	int* hi = map.find("hello");
	if (hi != nullptr)
	cerr << *hi << endl;
	else
	cerr << "your a wizard Harry" << endl;
	cerr << map.size() << endl;
	MapLoader ml;
	if (ml.load("mapdata.txt"))
	cerr << "something worked" << endl;
	AttractionMapper am;
	am.init(ml); // let our object build its internal data structures
	// by iterating thru all segments from the MapLoader object
	cerr << "Done Loading!" << endl;
	GeoCoord fillMe;
	string attraction = "The Coffee Bean & Tea Leaf";
	bool found = am.getGeoCoord(attraction, fillMe);
	if (!found)
	{
	cout << "No geolocation found for " << attraction << endl;
	return 0;
	}
	cout << "The location of " << attraction << " is " << fillMe.latitudeText << ", " << fillMe.longitudeText << endl;

	SegmentMapper sm;
	sm.init(ml);
	cerr << "Done Loading!" << endl;
	GeoCoord lookMeUp("34.0572000", "-118.4417620");
	std::vector<StreetSegment> vecOfAssociatedSegs(sm.getSegments(fillMe));
	if (vecOfAssociatedSegs.empty())
	{
	cout << "Error - no segments found matching this coordinate\n";
	return 0;
	}
	cout << "Here are all the segments associated with your coordinate:" << endl;
	for (auto s : vecOfAssociatedSegs)
	{
	cout << "Segment's street: " << s.streetName << endl;
	cout << "Segment's start lat/long: " << s.segment.start.latitudeText << ", " << s.segment.start.longitudeText << endl;
	cout << "Segment's end lat/long: " << s.segment.end.latitudeText << ", " << s.segment.end.longitudeText << endl;
	cout << "This segment has " << s.attractions.size() << " attractions on it." << endl;
	}
	Navigator n;
	n.loadMapData("mapdata.txt");
	vector<NavSegment> nv;
	cerr << "Starting Navigation Now" << endl;
	if (n.navigate("1061 Broxton Avenue", "Headlines!", nv) == NAV_NO_ROUTE)
		cerr << "Fail!" << endl;
	for (NavSegment ns : nv) {
		if (ns.m_command == NavSegment::PROCEED) {
			cerr << "type: PROCEED" << endl;
			cerr << "start: " << ns.m_geoSegment.start.latitudeText << ", " << ns.m_geoSegment.start.longitudeText << endl;
			cerr << "end: " << ns.m_geoSegment.end.latitudeText << ", " << ns.m_geoSegment.end.longitudeText << endl;
			cerr << "direction: " << ns.m_direction << endl;
			cerr << "distance: " << ns.m_distance << endl;
			cerr << "street: " << ns.m_streetName << endl;
		}
		else {
			cerr << "type: TURN" << endl;
			cerr << "direction: " << ns.m_direction << endl;
			cerr << "street: " << ns.m_streetName << endl;
		}
		cerr << endl << endl;
	}
}
