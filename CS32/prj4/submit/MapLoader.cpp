#include "provided.h"
#include <fstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

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
	vector<StreetSegment> m_segments;
};

MapLoaderImpl::MapLoaderImpl()
{
	// TODO: Implement?
}

MapLoaderImpl::~MapLoaderImpl()
{
}

// TODO: Take care of case when mapFile doesn't open properly
bool MapLoaderImpl::load(string mapFile)
{
	char junk[10];
	char junkC;
	ifstream ifs(mapFile);
	char stupidCString[1000];
	string nameHolder;

	StreetSegment seg;
	while(getline(ifs, nameHolder))
	{
		seg.streetName = nameHolder;
		ifs.get(stupidCString, 1000, ',');
		string lat(stupidCString);

		ifs.get(junkC);	// get rid of ','
		
		string lon;
		ifs >> lon;

		GeoCoord s(lat, lon);

		ifs.get(junkC);	// get rid of ' '

		ifs.get(stupidCString, 1000, ',');
		lat = stupidCString;
		ifs.get(junkC);	// get rid of ','
		ifs >> lon;
		ifs.getline(junk, 1000);	// go to new line

		GeoCoord e(lat, lon);
		GeoSegment gSeg(s, e);


		seg.segment = gSeg;
		seg.attractions.clear();
		int attractionCount;
		string attractionCountStr;
		getline(ifs, attractionCountStr);
		attractionCount = stoi(attractionCountStr);

		for(int i = 0; i < attractionCount; i++)
		{
			Attraction atr;
			ifs.get(stupidCString, 1000, '|');
			ifs.get(junkC);	// get rid of '|'
			atr.name = stupidCString;
			ifs.get(stupidCString, 1000, ',');
			lat = stupidCString;
			ifs.get(junkC); 	// get rid of ','
			ifs >> lon;
			ifs.getline(junk, 10);	// go to new line

			GeoCoord atrCoord(lat, lon);
			atr.geocoordinates = atrCoord;
			seg.attractions.push_back(atr);
		}
		m_segments.push_back(seg);
	}
	ifs.close();
	return true;  // This compiles, but may not be correct
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_segments.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if(segNum > m_segments.size()-1) return false;
	seg.streetName = m_segments[segNum].streetName;
	seg.segment.start.latitude = m_segments[segNum].segment.start.latitude;
	seg.segment.start.latitudeText = m_segments[segNum].segment.start.latitudeText;
	seg.segment.start.longitude = m_segments[segNum].segment.start.longitude;
	seg.segment.start.longitudeText = m_segments[segNum].segment.start.longitudeText;

	seg.segment.end.latitude = m_segments[segNum].segment.end.latitude;
	seg.segment.end.latitudeText = m_segments[segNum].segment.end.latitudeText;
	seg.segment.end.longitude = m_segments[segNum].segment.end.longitude;
	seg.segment.end.longitudeText = m_segments[segNum].segment.end.longitudeText;

	seg.attractions.clear();
	for(int i = 0; i < m_segments[segNum].attractions.size(); i++)
		seg.attractions.push_back(m_segments[segNum].attractions[i]);

	return true;
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
