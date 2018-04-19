#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>
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
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
    //create an infile of mapfile
    ifstream infile(mapFile);    // infile is a name of our choosing
    if ( ! infile )		        // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    //loop through all the lines in the map file
    std::string streetName;
    while (getline(infile, streetName))
    {
        StreetSegment seg;
        string lat1, lat2, lon1, lon2, trash;
        //get the first coordinate
        getline(infile,lat1,',');
        //check if there is a space
        char c;
        infile.get(c);
        if(c != ' ')
        {
            //if not then add character you just took
            getline(infile,lon1,' ');
            lon1 = c+ lon1;
        }
        //otherwise just get the coord
        else
        {
            getline(infile,lon1,' ');
        }
        //do the same for the second coordinate
        getline(infile,lat2,',');
        char j;
        infile.get(j);
        if(j != ' ')
        {
            getline(infile,lon2,'\n');
            lon2 = j + lon2;
            
        }
        else
        {
            getline(infile,lon2,'\n');
        }
        //create geocoords from the lats and longs
        GeoCoord c_start(lat1,lon1);
        GeoCoord c_end(lat2,lon2);
        GeoSegment c_seg = GeoSegment(c_start,c_end);
        
        //get the number of attractions
        int count;
        infile >> count;
        // If you want to consume and ignore the rest of the line the
        // number is found on, follow this with
        infile.ignore(10000, '\n');
        
        //loop through all of the attractions
        vector<Attraction> attracts;
        for(int j =0; j<count; j++)
        {
            //get the name and the coordinates
            string name, lat, lon;
            getline(infile,name,'|');
            getline(infile,lat,',');
            
            char k;
            infile.get(k);
            if(k != ' ')
            {
                getline(infile,lon,'\n');
                lon = k + lon;
                
            }
            else
            {
                getline(infile,lon,'\n');
            }
            //add the atractions to an attraction vector
            GeoCoord c_coord = GeoCoord(lat, lon);
            Attraction attr;
            attr.name = name;
            attr.geocoordinates = c_coord;
            attracts.push_back(attr);
        }
        //add the segments to a segment vector
        seg.streetName = streetName;
        seg.attractions=attracts;
        seg.segment = c_seg;
        
        m_segments.push_back(seg);
    }
    
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
    return m_segments.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    //if it is a valid num
    if(segNum > m_segments.size()-1)
    {
        return false;
    }
    //return the segment
    seg = m_segments[segNum];
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
