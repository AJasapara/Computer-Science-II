#include "provided.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <list>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	AttractionMapper am;
	SegmentMapper sm;
	struct Node {
		Node(Node* p, StreetSegment ss, double sc, double gsc, GeoCoord e) {
			prev = p;
			street = ss;
			score = sc;
			end = e;
			gscore = gsc;
		}
		Node* prev;
		StreetSegment street;
		GeoCoord end;
		double gscore, score;
	};

	bool compareCoords(const GeoCoord& g1, const GeoCoord& g2) const { // checks if 2 GeoCoords are equal
		return g1.latitudeText == g2.latitudeText && g1.longitudeText == g2.longitudeText;
	}

	string toString(const StreetSegment& s) const { // converts a StreetSegment into its coordinates' text
		return s.segment.start.latitudeText + s.segment.start.longitudeText + s.segment.end.latitudeText + s.segment.end.longitudeText;
	}

	string getDirection(double dir) const { // returns a direction based on the double inputted
		string direction;
		if (dir <= 22.5) direction = "east";
		else if (dir <= 67.5) direction = "northeast";
		else if (dir <= 112.5) direction = "north";
		else if (dir <= 157.5) direction = "northwest";
		else if (dir <= 202.5) direction = "west";
		else if (dir <= 247.5) direction = "southwest";
		else if (dir <= 292.5) direction = "south";
		else if (dir <= 337.5) direction = "southeast";
		else direction = "east";
		return direction;
	}

	vector<Node*> surrounding(Node* n, const GeoCoord& coord) const;

	void constructPath(Node* n, vector<NavSegment> &directions, const GeoCoord& start) const;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile) // loads the map data
{
	MapLoader ml;
	if (!ml.load(mapFile))
		return false;
	am.init(ml);
	sm.init(ml);
	return true;
}

void NavigatorImpl::constructPath(Node* n, vector<NavSegment> &directions, const GeoCoord& start) const { // reconstructs the optimal path
	if (n == nullptr) return;
	vector<NavSegment> temp;
	directions = temp;
	GeoSegment gs(n->prev->end,n->end); // manually adds first direction to account for attraction
	double dir = angleOfLine(gs);
	string direction = getDirection(dir);
	NavSegment first(direction, n->street.streetName, distanceEarthMiles(n->prev->end, n->end), gs);
	temp.push_back(first);
	n = n->prev;
	while (n->prev != nullptr) { // adds rest of movements as NavSegments based on StreetSegments
		dir = angleOfLine(GeoSegment(n->prev->end, n->end)); // Proceed-style NavSegments
		direction = getDirection(dir);
		NavSegment ns(direction, n->street.streetName, distanceEarthMiles(n->prev->end, n->end), n->street.segment);
		temp.push_back(ns);
		if (n->street.streetName != n->prev->street.streetName) { // Turn-style NavSegments
			GeoCoord ps = n->prev->street.segment.start, pe = n->prev->street.segment.end, ns = n->street.segment.start, ne = n->street.segment.end;
			if (compareCoords(ps, ne)) // accounts for lack of specification in what is start/end GeoCoord
				dir = angleBetween2Lines(GeoSegment(pe, ps), GeoSegment(ne, ns));
			else if (compareCoords(ne, pe))
				dir = angleBetween2Lines(GeoSegment(ps, pe), GeoSegment(ne, ns));
			else if (compareCoords(ns, ps))
				dir = angleBetween2Lines(GeoSegment(pe, ps), GeoSegment(ns, ne));
			else if (compareCoords(pe, ns))
				dir = angleBetween2Lines(GeoSegment(ps, pe), GeoSegment(ns, ne));
			if (dir < 180) direction = "left";
			else direction = "right";
			NavSegment ns2(direction, n->street.streetName);
			temp.push_back(ns2);
		}
		n = n->prev;
	}
	for (int i = temp.size() - 1; i >= 0; i--) // reverses the vector since path was constructed in reverse
		directions.push_back(temp[i]);
	GeoSegment gs2(start, directions[0].m_geoSegment.end); // manually changes first NavSegment to account for starting attraction
	directions[0].m_geoSegment = gs2;
	directions[0].m_direction = getDirection(angleOfLine(gs2));
}

vector<NavigatorImpl::Node*> NavigatorImpl::surrounding(Node* n, const GeoCoord& coord) const { // returns surrounding Node*
	vector<NavigatorImpl::Node*> nlist;
	vector<StreetSegment> ss = sm.getSegments(n->end);
	GeoCoord loc;
	for (StreetSegment s : ss) {
		if (!compareCoords(n->end, s.segment.start)) // checks to make sure the streetSegment is not same as surrounding
			loc = s.segment.start;
		if (!compareCoords(n->end, s.segment.end))
			loc = s.segment.end;
		int tempScore = n->gscore + distanceEarthMiles(n->end, loc); // surrounding g-score
		int fullScore = tempScore + distanceEarthMiles(loc, coord); // surrounding score
		nlist.push_back(new Node(n, s, fullScore, tempScore, loc));
		vector<Attraction> alist = s.attractions;
		for (Attraction a : alist) // if destination attraction is encountered, adds it as a Node*
			if (compareCoords(coord, a.geocoordinates)) 
				nlist.push_back(new Node(n, s, n->gscore, n->gscore, a.geocoordinates));
	}
	return nlist;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	GeoCoord startCoord, endCoord;
	if (!am.getGeoCoord(start, startCoord)) // checks to make sure both source and destination are valid
		return NAV_BAD_SOURCE;
	if (!am.getGeoCoord(end, endCoord))
		return NAV_BAD_DESTINATION;
	StreetSegment ss;
	if (!sm.getSegments(startCoord).empty()) // gets starting segment
		ss = sm.getSegments(startCoord)[0];
	list<Node*> open; // Nodes to visit
	list <Node*> closed; // Nodes already visited
	open.push_back(new Node(nullptr, ss, distanceEarthMiles(startCoord, endCoord), 0, startCoord)); // Manually add first Node
	while (!open.empty()) {
		Node* current = open.front(); // pops front and gets surroundings
		open.pop_front();
		vector<Node*> n = surrounding(current, endCoord);
		for (int i = 0; i < n.size(); i++) { // for each surrounding Node*
			if (compareCoords(n[i]->end, endCoord)) { // if destination is reached
				constructPath(n[i], directions, startCoord);
				delete current;
				for (int j = i; j < n.size(); j++) { // cleans up Nodes
					delete n[j];
					n[j] = nullptr;
				}
				for (list<Node*>::iterator it = closed.begin(); it != closed.end(); it++)
					if ((*it) != nullptr) {
						delete *it;
						*it = nullptr;
					}
				for (list<Node*>::iterator it = open.begin(); it != open.end(); it++)
					if ((*it) != nullptr) {
						delete *it;
						*it = nullptr;
					}
				return NAV_SUCCESS;
			}	
			bool found = false;
			for (Node* c : closed) // makes sure the surrounding Node* is not already in closed
				if (compareCoords(c->end, n[i]->end) && c->score <= n[i]->score) {
					found = true;
					delete n[i];
					n[i] = nullptr;
					break;
				}
			if(!found)
				for (Node* o: open) // makes sure the surrounding Node* is not already in open
					if (compareCoords(o->end, n[i]->end) && o->score <= n[i]->score) {
						found = true;
						delete n[i];
						n[i] = nullptr;
						break;
					}
			if (!found) { // adds Node* to respective score-sorted list, ensures first Node* in list has lowest score
				if (open.empty() || open.back()->score <= n[i]->score) // if empty or highest score, puts at end
					open.push_back(n[i]);
				else
					for (list<Node*>::iterator it = open.begin(); it != open.end(); it++)
						if ((*it)->score > n[i]->score) { // adds Node* right before the next highest score
							open.insert(it, n[i]);
							break;
						}
			}
		}
		closed.push_back(current);
	}
	for (list<Node*>::iterator it = closed.begin(); it != closed.end(); it++) // Cleans up Nodes
		if ((*it) != nullptr) {
			delete *it;
			*it = nullptr;
		}
	for (list<Node*>::iterator it = open.begin(); it != open.end(); it++)
		if ((*it) != nullptr) {
			delete *it;
			*it = nullptr;
		}
	return NAV_NO_ROUTE;
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
