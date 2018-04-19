#include "provided.h"
#include <string>
#include <vector>
#include <queue>
#include "MyMap.h"
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    AttractionMapper m_attrMap;
    SegmentMapper m_segMap;
    string getDirection(GeoCoord point1, GeoCoord point2) const;
    string getStreetName(GeoCoord first, GeoCoord second) const;
    bool nextIsTurn(GeoCoord first, GeoCoord second, GeoCoord third) const;
    string getDirectionForTurn(GeoCoord g1, GeoCoord g2, GeoCoord g3) const;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    MapLoader ml;
    if(!ml.load(mapFile))
    {
        return false;
    }
    m_attrMap.init(ml);
    m_segMap.init(ml);
    
    return false;  // This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    MyMap<GeoCoord,GeoCoord> routeLinks;
    queue<GeoCoord> routes;
    GeoCoord c_start;
    GeoCoord c_end;
    
    m_attrMap.getGeoCoord(start, c_start);
    m_attrMap.getGeoCoord(end, c_end);
    
    vector<StreetSegment>  startStreet = m_segMap.getSegments(c_start);
    vector<StreetSegment> endStreet = m_segMap.getSegments(c_end);
    
    if(startStreet.size()==0)
    {
        return NAV_BAD_SOURCE;
    }
    if(endStreet.size()==0)
    {
        return NAV_BAD_DESTINATION;
    }
    for(int i =0 ; i < startStreet.size(); i++)
    {
        routes.push(startStreet[i].segment.start);
        routeLinks.associate(startStreet[i].segment.start, c_start);
        routes.push(startStreet[i].segment.end);
        routeLinks.associate(startStreet[i].segment.end, c_start);
    }
    
    bool found = false;
    
    while(!routes.empty() || !found)
    {
        if(routes.empty())
        {
            return NAV_NO_ROUTE;
        }
        
        GeoCoord cur = routes.front();
        routes.pop();
        
        for(int i = 0; i<endStreet.size(); i++)
        {
            if((cur.latitude == endStreet[i].segment.end.latitude && cur.longitude == endStreet[i].segment.end.longitude)||(cur.latitude == endStreet[i].segment.start.latitude && cur.longitude == endStreet[i].segment.start.longitude))
            {
                routeLinks.associate(c_end, cur);
                found = true;
                continue;
            }
        }
       
        vector<StreetSegment>  curStreet = m_segMap.getSegments(cur);
        
        for(int i = 0; i < curStreet.size(); i++)
        {
            if(routeLinks.find(curStreet[i].segment.start) == nullptr)
            {
                routes.push(curStreet[i].segment.start);
                routeLinks.associate(curStreet[i].segment.start, cur);
            }
            if(routeLinks.find(curStreet[i].segment.end) == nullptr)
            {
                routes.push(curStreet[i].segment.end);
                routeLinks.associate(curStreet[i].segment.end, cur);
            }
        }
    }
    
    vector<GeoCoord> routeTrace;
    GeoCoord cur = c_end;
    
    while(!(cur.latitude == c_start.latitude && cur.longitude == c_start.longitude))
    {
        const GeoCoord* value = routeLinks.find(cur);
        routeTrace.push_back(*value);
        cur = *value;
    }
    
    while(directions.size()!=0)
    {
        directions.erase(directions.begin());
    }
    
    for(int k = routeTrace.size()-1; k>=0 ; k--)
    {
        string name = getStreetName(routeTrace[k],routeTrace[k-1]);
        double distance = distanceEarthMiles(routeTrace[k],routeTrace[k-1]);
        GeoSegment curSeg(routeTrace[k],routeTrace[k-1]);
        string direction;
        
        direction = getDirection(routeTrace[k], routeTrace[k-1]);
        NavSegment c_direction(direction,name,distance,curSeg);
        directions.push_back(c_direction);
        
        if(k>=2)
        {
            bool isTurn = nextIsTurn(routeTrace[k], routeTrace[k-1], routeTrace[k-2]);
            if(isTurn)
            {
                direction = getDirectionForTurn(routeTrace[k], routeTrace[k-1], routeTrace[k-2]);
                NavSegment c_direction(direction,name);
                directions.push_back(c_direction);
            }
        }
    }
    return NAV_SUCCESS;
}



string NavigatorImpl::getDirection(GeoCoord point1, GeoCoord point2) const
{
    GeoSegment seg(point1,point2);
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

string NavigatorImpl::getStreetName(GeoCoord first, GeoCoord second) const
{
    vector<StreetSegment> one = m_segMap.getSegments(first);
    vector<StreetSegment> two = m_segMap.getSegments(second);
    for (int k = 0; k < one.size(); k++)
    {
        for (int i = 0; i < two.size(); i++)
        {
            if (one[k].streetName == two[i].streetName)
                return one[k].streetName;
        }
    }
    return "name not found for street";
}

bool NavigatorImpl::nextIsTurn(GeoCoord first, GeoCoord second, GeoCoord third) const
{
    if (getStreetName(first, second) == getStreetName(second, third))
        return false;
    return true;
}


string NavigatorImpl::getDirectionForTurn(GeoCoord g1, GeoCoord g2, GeoCoord g3) const
{
    string dir;
    // make 2 geosegments
    GeoSegment nugs(g1, g2);
    GeoSegment nugs2(g2, g3);
    
    // pass into 2 lines
    double angle;
    angle = angleBetween2Lines(nugs, nugs2);
    
    // give it it's direction
    if ((angle >= 0 && angle <= 90) || (angle >= 180 && angle <= 270)){
        dir = "right";
    }
    if ((angle > 90 && angle <= 180) || (angle > 270 && angle <= 360)){
        dir = "left";
    }
    return dir;
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
