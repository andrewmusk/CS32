#include "provided.h"
#include <vector>
#include "MyMap.h"
#include "support.h"
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<newGeoCoord,vector<StreetSegment>> m_segmentMap;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i< ml.getNumSegments() ; i++)
    {
        StreetSegment cur;
        ml.getSegment(i, cur);
        string cur_start = cur.segment.start.latitudeText + cur.segment.start.longitudeText;
        string cur_end = cur.segment.end.latitudeText + cur.segment.end.longitudeText;
        
        vector<StreetSegment>* streetSegsStart = m_segmentMap.find(cur.segment.start);
        vector<StreetSegment>* streetSegsEnd = m_segmentMap.find(cur.segment.end);
        
        
        if(streetSegsStart != nullptr)
        {
            (*streetSegsStart).push_back(cur);
        }
        else
        {
            vector<StreetSegment> n_segs;
            n_segs.push_back(cur);
            m_segmentMap.associate(cur.segment.start, n_segs);
        }
        if(streetSegsEnd != nullptr)
        {
            (*streetSegsEnd).push_back(cur);
        }
        else
        {
            vector<StreetSegment> n_segs;
            n_segs.push_back(cur);
            m_segmentMap.associate(cur.segment.end, n_segs);
        }
        
        //attractions
        for(int i = 0; i < cur.attractions.size() ; i++)
        {
            string attr_Coord = cur.attractions[i].geocoordinates.latitudeText + cur.attractions[i].geocoordinates.longitudeText;
            vector<StreetSegment>* streetSegsAttr = m_segmentMap.find(cur.attractions[i].geocoordinates);
            
            if(streetSegsAttr != nullptr)
            {
                streetSegsAttr->push_back(cur);
                m_segmentMap.associate(cur.attractions[i].geocoordinates, *streetSegsAttr);
                //(*streetSegsAttr).push_back(cur);
            }
            else
            {
                vector<StreetSegment> n_attr_segs;
                n_attr_segs.push_back(cur);
                m_segmentMap.associate(cur.attractions[i].geocoordinates, n_attr_segs);
            }
        }
    }
    
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    //const vector <StreetSegment>* n_seg = m_segmentMap.find(coord);
    if(m_segmentMap.find(gc) == nullptr)
    {
        vector<StreetSegment> v;
        return v;
    }
    else
        return *m_segmentMap.find(gc);
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
