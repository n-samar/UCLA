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
	MyMap<string, GeoCoord> m_attrMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	int segPtr = 0;
	StreetSegment currSeg;
	while(ml.getSegment(segPtr++, currSeg))
	{
		for(int attrPtr = 0; attrPtr < currSeg.attractions.size(); attrPtr++)
		{
			// Convert to lowercase
			string name = currSeg.attractions[attrPtr].name;
			//cerr << "Attraction: " << name << endl;
			for(int i = 0; i < name.size(); i++)
    			name[i] = std::tolower(name[i]);

			m_attrMap.associate(name, 
				currSeg.attractions[attrPtr].geocoordinates);
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	const GeoCoord* gcMaybe;

	// Convert to lowercase
	string name = attraction;
	for(int i = 0; i < name.size(); i++)
    	name[i] = std::tolower(name[i]);

	if((gcMaybe = m_attrMap.find(name)))
	{
		gc = *gcMaybe;
		return true;
	}else return false;
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
