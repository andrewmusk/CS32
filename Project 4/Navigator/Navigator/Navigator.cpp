#include "provided.h"
#include <string>
#include <vector>
#include <queue>
#include "MyMap.h"
#include "support.h"
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
    string getStreetName(newGeoCoord first, newGeoCoord second) const;
    bool nextIsTurn(newGeoCoord first, newGeoCoord second, newGeoCoord third) const;
    string getDirectionForTurn(newGeoCoord g1, newGeoCoord g2, newGeoCoord g3) const;
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
    //if the mapfile failed loading
    if(!ml.load(mapFile))
    {
        return false;
    }
    //initialise
    m_attrMap.init(ml);
    m_segMap.init(ml);
    
    return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    MyMap<newGeoCoord,newGeoCoord> routeLinks;
    priority_queue<Node> routes;
    newGeoCoord c_start;
    newGeoCoord c_end;
    
    //getting the geocoord of the of the attractions
    m_attrMap.getGeoCoord(start, c_start.m_coord);
    m_attrMap.getGeoCoord(end, c_end.m_coord);
    //getting the segments of the start and end
    vector<StreetSegment>  startStreet = m_segMap.getSegments(c_start.m_coord);
    vector<StreetSegment> endStreet = m_segMap.getSegments(c_end.m_coord);
    
    //push the first attraction and add to map to check
    Node startNode(start,c_start.m_coord,c_end.m_coord,nullptr);
    routes.push(startNode);
    
    //if it is not on segment
    if(startStreet.size()==0)
    {
        return NAV_BAD_SOURCE;
    }
    //if end is not on segment
    if(endStreet.size()==0)
    {
        return NAV_BAD_DESTINATION;
    }
    
    bool found = false;
    
    //while it hasn't been found
    while(!found)
    {
        //if empty
        if(routes.empty())
        {
            return NAV_NO_ROUTE;
        }
        
        //pop the top value
        Node cur = routes.top();
        routes.pop();
        
        //loop throuh end street segments
        for(int i = 0; i<endStreet.size(); i++)
        {
            //if current is equal to the start or end of the end street segment
            if((cur.m_cur.m_coord.latitude == endStreet[i].segment.end.latitude && cur.m_cur.m_coord.longitude == endStreet[i].segment.end.longitude)||(cur.m_cur.m_coord.latitude == endStreet[i].segment.start.latitude && cur.m_cur.m_coord.longitude == endStreet[i].segment.start.longitude))
            {
                //add to the map and set found to true
                routeLinks.associate(c_end, cur.m_cur);
                found = true;
                break;
            }
        }
       
        vector<StreetSegment>  curStreet = m_segMap.getSegments(cur.m_cur.m_coord);
        
        //loop through the current segment
        for(int i = 0; i < curStreet.size(); i++)
        {
            //check if it is already in the map for the start coord
            if(routeLinks.find(curStreet[i].segment.start) == nullptr)
            {
                //add to the queue and push to map
                Node startNode(curStreet[i].streetName, curStreet[i].segment.start, c_end.m_coord, &cur);
                routes.push(startNode);
                routeLinks.associate(curStreet[i].segment.start, cur.m_cur);
            }
            //check if it is already in the map for the end coord
            if(routeLinks.find(curStreet[i].segment.end) == nullptr)
            {
                //add to the queue and push to map
                Node endNode(curStreet[i].streetName, curStreet[i].segment.end, c_end.m_coord, &cur);
                routes.push(endNode);
                routeLinks.associate(curStreet[i].segment.end, cur.m_cur);
            }
        }
    }
    
    vector<newGeoCoord> routeTrace;
    newGeoCoord cur = c_end;
    routeTrace.push_back(c_end);
    
    //loop through the map to find the successful root
    while(!(cur.m_coord.latitude == c_start.m_coord.latitude && cur.m_coord.longitude == c_start.m_coord.longitude))
    {
        const newGeoCoord* value = routeLinks.find(cur);
        routeTrace.push_back(*value);
        cur = *value;
    }
    //clear the vector
    while(directions.size()!=0)
    {
        directions.erase(directions.begin());
    }
    //loop backwards through troot
    for(int k = routeTrace.size()-1; k>=0 ; k--)
    {
        string direction;
        string name;
        if(k>=1)
        {
            //create the direction if not turn
            string name = getStreetName(routeTrace[k],routeTrace[k-1]);
            double distance = distanceEarthMiles(routeTrace[k].m_coord,routeTrace[k-1].m_coord);
            GeoSegment curSeg(routeTrace[k].m_coord,routeTrace[k-1].m_coord);
            
            direction = getDirection(routeTrace[k], routeTrace[k-1]);
            NavSegment c_direction(direction,name,distance,curSeg);
            directions.push_back(c_direction);
        }
        //if it is possibly a turn
        if(k>=2)
        {
            bool isTurn = nextIsTurn(routeTrace[k], routeTrace[k-1], routeTrace[k-2]);
            //check if it is a trun
            if(isTurn)
            {
                direction = getDirectionForTurn(routeTrace[k], routeTrace[k-1], routeTrace[k-2]);
                name = getStreetName(routeTrace[k-1], routeTrace[k-2]);
                NavSegment c_direction(direction,name);
                directions.push_back(c_direction);
            }
        }
    }
    return NAV_SUCCESS;
}



string NavigatorImpl::getStreetName(newGeoCoord first, newGeoCoord second) const
{
    //get both street segments
    vector<StreetSegment> one = m_segMap.getSegments(first.m_coord);
    vector<StreetSegment> two = m_segMap.getSegments(second.m_coord);
    //if any match then it is the street name
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

//check if the next is a turn. This is done if the names are different
bool NavigatorImpl::nextIsTurn(newGeoCoord first, newGeoCoord second, newGeoCoord third) const
{
    if (getStreetName(first, second) == getStreetName(second, third))
        return false;
    return true;
}


string NavigatorImpl::getDirectionForTurn(newGeoCoord g1, newGeoCoord g2, newGeoCoord g3) const
{
    string dir;
    GeoSegment geo1(g1.m_coord, g2.m_coord);
    GeoSegment geo2(g2.m_coord, g3.m_coord);
    
    double angle;
    angle = angleBetween2Lines(geo1, geo2);
    
    if(angle >=0 && angle<=180)
    {
        dir = "left";
    }
    if(angle >=  180 && angle <=360)
    {
        dir = "right";
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
