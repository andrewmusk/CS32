//
//  support.cpp
//  Navigator
//
//  Created by Andrew Musk on 2017/03/12.
//  Copyright © 2017 Andrew Musk. All rights reserved.
//

#include "support.h"
using namespace std;


bool newGeoCoord::operator==(const newGeoCoord& other) const
{
    if (m_coord.latitude == other.m_coord.latitude && m_coord.longitude == other.m_coord.longitude)
    {
        return true;

    }
    return false;
}
bool newGeoCoord::operator<(const newGeoCoord& other) const
{
    if (m_coord.latitude < other.m_coord.latitude)
        return true;
    if (m_coord.longitude < other.m_coord.longitude)
        return true;
    return false;
}
bool newGeoCoord::operator>(const newGeoCoord& other) const
{
    if (m_coord.latitude > other.m_coord.latitude)
        return true;
    if (m_coord.longitude > other.m_coord.longitude)
        return true;
    return false;
}


string directionOfLine(const GeoSegment& geoseg)
{
    return getDirection(geoseg.start, geoseg.end);
}

bool Node::operator<(const Node& other) const
{
    return (this->f < other.f);
}

string getDirection(newGeoCoord point1, newGeoCoord point2) 
{
    GeoSegment seg(point1.m_coord,point2.m_coord);
    double angle = angleOfLine(seg);
    
    if(angle >= 0 && angle <=22.5)
        return "east";
    if(angle > 22.5 && angle <= 67.5)
        return "northeast";
    if(angle > 67.5 && angle <= 112.5)
        return "north";
    if(angle > 112.5 && angle <= 157.5)
        return "northwest";
    if(angle > 157.5 && angle <= 202.5)
        return "west";
    if(angle > 202.5 && angle <= 247.5)
        return "southwest";
    if(angle > 247.5 && angle <= 292.5)
        return "south";
    if(angle > 292.5 && angle <= 337.5)
        return "southeast";
    if(angle > 337.5 && angle <= 360)
        return "east";
    return "";
}




