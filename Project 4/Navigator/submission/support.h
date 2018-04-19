//
//  support.h
//  Navigator
//
//  Created by Andrew Musk on 2017/03/12.
//  Copyright © 2017 Andrew Musk. All rights reserved.
//

#ifndef support_h
#define support_h

#include "provided.h"
#include <iostream>

class newGeoCoord
{
public:
    
    newGeoCoord(GeoCoord a)
    {m_coord = a;}
    
    newGeoCoord(){};
    
    bool operator==(const newGeoCoord &point2) const;
    bool operator<(const newGeoCoord &point2) const;
    bool operator>(const newGeoCoord &point2) const;
    const GeoCoord getCoord() const {return m_coord;};
   

    
    GeoCoord m_coord;
};

class Node
{
public:
    Node();
    Node(std::string name, GeoCoord cur, GeoCoord end, Node* previous)
    {
        m_parent = previous;
        m_cur = cur;
        h = distanceEarthMiles(cur, end);
        if(m_parent!=nullptr)
        {
            g = distanceEarthMiles(m_cur.m_coord, m_parent->m_cur.m_coord) + m_parent -> g;
        }
        else{
            g=0;
        }
        f= (g+h)*-1;
        
    }
    bool operator<(const Node &node2) const;
    newGeoCoord m_cur;
    double g;
    double h;
    double f;
private:
    Node* m_parent;

};

std::string getDirection(newGeoCoord point1, newGeoCoord point2);
std::string directionOfLine(const GeoSegment& geoseg);


#endif /* support_h */
