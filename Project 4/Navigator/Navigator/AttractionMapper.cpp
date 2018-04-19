#include "provided.h"
#include <string>
#include "MyMap.h"
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    //map of attraction names to geocoords
    MyMap<string,GeoCoord> m_attractionMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    //loop through all the segments in the maploader
    for(int i = 0; i< ml.getNumSegments() ; i++)
    {
        StreetSegment cur;
        ml.getSegment(i, cur);
        //if their are attractions
        if(cur.attractions.size()>0)
        {
            //loop through the attactions in the segment
            for(int j =0; j < cur.attractions.size(); j++)
            {
                string name;
                //convert to lower case
                for (int k = 0; k < cur.attractions[j].name.size(); k++)
                {
                    name += tolower(cur.attractions[j].name[k]);
                }
                //associate the name to the geocoord
                m_attractionMap.associate(name,cur.attractions[j].geocoordinates);
            }
        }
    }
    
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    string attr;
    for (int j = 0; j < attraction.size(); j++)
    {
        attr += tolower(attraction[j]);
    }
    const GeoCoord* value = m_attractionMap.find(attr);
    if(value == nullptr)
    {
        return false;  // This compiles, but may not be correct
    }
    else
    {
        gc = *value;
        return true;
    }
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
