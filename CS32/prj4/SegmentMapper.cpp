#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	void add(int seg, const GeoCoord& gc);
	MyMap<GeoCoord, vector<int>> m_gcMap;
	const MapLoader* m_ml;
};





SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

// add seg to m_gcMap with key gc
void SegmentMapperImpl::add(int seg, const GeoCoord &gc)
{
	if(!m_gcMap.find(gc))
	{
		vector<int> addMeHere;
		m_gcMap.associate(gc, addMeHere);
	}
	(*(m_gcMap.find(gc))).push_back(seg);	// TODO: SHould check if seg is already in vector
}
void SegmentMapperImpl::init(const MapLoader& ml)
{
	m_ml = &ml;
	int segPtr = 0;
	
	StreetSegment ss;
	int count = 0;
	while(ml.getSegment(segPtr, ss))
	{
		StreetSegment currSeg(ss);
		add(segPtr, currSeg.segment.start);	// add to gc of start
		add(segPtr, currSeg.segment.end);		// add to gc of end
		count+=2;
		for(int attrPtr = 0; attrPtr < currSeg.attractions.size(); attrPtr++)
		{
			//cerr << segPtr <<" atr: " << currSeg.attractions.size() << endl;
			add(segPtr, 
				currSeg.attractions[attrPtr].geocoordinates);	// add to gc of all attractions
			count++;
		}
		//cerr << "Depth: " << m_gcMap.getDepth() <<", " << segPtr << endl;
		segPtr++;
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> result;
	StreetSegment seg;
	const vector<int> vecPtr = *(m_gcMap.find(gc));
	for(int i = 0; i < vecPtr.size(); i++)
	{
		m_ml->getSegment(vecPtr[i], seg);
		result.push_back(seg);
	}
	return result;  // This compiles, but may not be correct
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
