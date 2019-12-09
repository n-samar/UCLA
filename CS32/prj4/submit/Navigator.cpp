#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <algorithm>
#include <string>
#include <vector>
#include <stack>

using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    string getTurnSide(GeoSegment g1, GeoSegment g2) const;
    string getDirection(GeoCoord A, GeoCoord B) const;
    MapLoader m_ml;
};
bool operator< (const pair<double, GeoCoord> &lhs, const pair<double, GeoCoord> &rhs)
{
    return (lhs.first > rhs.first);     // this make a min heap instead of the default max heap
}

string NavigatorImpl::getTurnSide(GeoSegment g1, GeoSegment g2) const
{
    return angleBetween2Lines(g1, g2) < 180? "left":"right";
}

string NavigatorImpl::getDirection(GeoCoord A, GeoCoord B) const
{
    string s;
    double angle = angleOfLine(GeoSegment(A, B));
    if(angle <= 22.5) return "east";
    if(angle <= 67.5) return "northeast";
    if(angle <= 112.5) return "north";
    if(angle <= 157.5) return "northwest";
    if(angle <= 202.5) return "west";
    if(angle <= 247.5) return "southwest";
    if(angle <= 292.5) return "south";
    if(angle <= 337.5) return "southeast";
    if(angle <= 360)   return "east";

    return "ERROR: BAD ANGLE";
}
NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if(m_ml.load(mapFile))
        return true;

	return false;  
}

/*
void operator= (GeoCoord lhs, GeoCoord rhs)
{
    lhs.latitudeText = rhs.latitudeText;
    lhs.longitudeText = rhs.longitudeText;
    lhs.latitude = rhs.latitude;
    lhs.longitude = rhs.longitude;
}*/


NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
        MyMap<GeoCoord, GeoCoord> trackback;
    vector<pair<double, GeoCoord>> candidates;
    MyMap<GeoCoord, NavSegment> trackSeg;   // tells us which navigation segment led us to this node
    stack<GeoCoord> trackStack; // helps with trackback since trackback will give
                                // us GeoCoord's in reverse order than what we want
    MyMap<GeoCoord, double> visited;

    directions.clear();

    SegmentMapper segMap;
    segMap.init(m_ml);

    AttractionMapper atrMap;
    atrMap.init(m_ml);

    GeoCoord gcStart, gcEnd;
    if(!atrMap.getGeoCoord(start, gcStart)) return NAV_BAD_SOURCE;
    if(!atrMap.getGeoCoord(end, gcEnd)) return NAV_BAD_DESTINATION;

    candidates.push_back(make_pair(distanceEarthMiles(gcStart, gcEnd), gcStart));

    trackback.associate(gcStart, gcStart);  

    while(candidates.size() > 0)
    {
        /*cerr << "Candidates: " << endl;
        for(int x = 0; x < candidates.size(); x++)
            cerr << candidates[x].first << endl;
        getchar();*/
        // get best candidate from heap
        pop_heap(candidates.begin(), candidates.end());

        GeoCoord curr = candidates.back().second; 
        double currTravelled = candidates.back().first - distanceEarthMiles(curr, gcEnd);
        candidates.pop_back();

        visited.associate(curr, currTravelled);
       // cout << "visited size:" << visited.size() << endl;
        // cerr << distanceEarthKM(curr, gcEnd) << endl;
        // see if we are done
        if (curr == gcEnd)
        {
            //cerr << "DONE" << endl;
            while(curr != gcStart)
            {
                trackStack.push(curr);
                curr = *(trackback.find(curr));
                //cerr << "survived0" << endl;
            }
            //trackStack.push(gcStart);
            //cerr << "survived1" << endl;
            while(!trackStack.empty())
            {
                NavSegment nextSeg = *(trackSeg.find(trackStack.top()));
                // check to see if TURN direction should be added
                if(directions.size() > 0 
                    && directions[directions.size() - 1].m_streetName != nextSeg.m_streetName)
                {
                    directions.push_back(NavSegment(getTurnSide(directions[directions.size() - 1].m_geoSegment, 
                        nextSeg.m_geoSegment),
                        nextSeg.m_streetName));
                }
                //cerr << "survived2" << endl;
                directions.push_back(nextSeg);  // Now add NavSegments using stack which lets us insert everything in proper order.

                trackStack.pop();
            }
            return NAV_SUCCESS;
        }


        vector<StreetSegment> newGuys = segMap.getSegments(curr);
        for(int segPtr = 0; segPtr < newGuys.size(); segPtr++)
        {
            vector<GeoCoord> addUs;
            vector<string>   streetNames;
            
            // is the end visited?
            if(!visited.find(newGuys[segPtr].segment.end))
            {
                addUs.push_back(newGuys[segPtr].segment.end);
                streetNames.push_back(newGuys[segPtr].streetName);
            }

            // is the start visited?
            if(!visited.find(newGuys[segPtr].segment.start))
            {
                addUs.push_back(newGuys[segPtr].segment.start);
                streetNames.push_back(newGuys[segPtr].streetName);
            }

            // which attractions were not visited?
            for(int i = 0; i < newGuys[segPtr].attractions.size(); i++)
                if(!visited.find(newGuys[segPtr].attractions[i].geocoordinates))
                {
                    addUs.push_back(newGuys[segPtr].attractions[i].geocoordinates);
                    streetNames.push_back(newGuys[segPtr].streetName);
                }

            // add all unvisited GeoCoord's to heap
            for(int i = 0; i < addUs.size(); i++)
            {
                bool labelled = false;  // this means addUs[i] is already in the candidates
                for(int j = 0; j < candidates.size(); j++)
                {
                    if(candidates[j].second == addUs[i] 
                        && candidates[j].first < distanceEarthMiles(addUs[i], gcEnd) + currTravelled + distanceEarthMiles(addUs[i], curr))
                    {
                        labelled = true;
                    }else if(candidates[j].second == addUs[i])
                    {
                        labelled = true;
                        candidates[j].first = distanceEarthMiles(addUs[i], gcEnd) + currTravelled 
                        + distanceEarthMiles(addUs[i], curr);
                        trackback.associate(addUs[i], curr);
                    trackSeg.associate(addUs[i], 
                        NavSegment(getDirection(curr, addUs[i]), streetNames[i], 
                        distanceEarthMiles(addUs[i], curr), GeoSegment(curr, addUs[i])));
                    make_heap(candidates.begin(), candidates.end());
                    }
                }
                
                if(!labelled)
                {
                    candidates.push_back(make_pair(distanceEarthMiles(addUs[i], gcEnd) + currTravelled 
                        + distanceEarthMiles(addUs[i], curr), 
                                                               addUs[i])); push_heap(candidates.begin(), candidates.end());
                    trackback.associate(addUs[i], curr);
                    trackSeg.associate(addUs[i], 
                        NavSegment(getDirection(curr, addUs[i]), streetNames[i], 
                        distanceEarthMiles(addUs[i], curr), GeoSegment(curr, addUs[i])));    // TODO: direction
                }
            }

        }

    }
	return NAV_NO_ROUTE;  // This compiles, but may not be correct
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
