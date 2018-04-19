//
//  main.cpp
//  Homework 2
//
//  Created by Andrew Musk on 2017/01/25.
//  Copyright © 2017 Andrew Musk. All rights reserved.
//


#include <iostream>
#include "Sequence.h"
#include <cassert>
using namespace std;

void test();

int main()
{
    test();
}


void test()

{
    
    Sequence s;
    assert(s.empty()==true);
    assert(s.insert(0, 10)==true);
    assert(s.insert(0, 20)==true);
    assert(s.size() == 2);
    ItemType x;
    assert(s.get(0, x)  &&  x == 20);
    assert(s.get(1, x)  &&  x == 10);
    assert (s.insert(15) == 0);
    assert (s.insert(6, 4) == false);
    
    //S=(15, 20, 10)
    
    assert(s.remove(15)==1);
    assert(s.get(0, x) && x == 20 && s.size()==2);
    assert(s.insert(10)==0);
    assert(s.insert(0, 10)==true);
    assert(s.remove(10) == 3);
    assert(s.insert(6)==0);
    
    assert(s.insert(7)==1);
    s.insert(0, 15);
    
    
    //s = (15, 6, 7, 20)
    
    s.erase(0);
    assert(s.get(0, x) && x == 6 && s.size()==3);
    assert(s.erase(4)==false);
    s.erase(1);
    assert(s.get(1, x) && x == 20 && s.size()==2);
    assert(s.insert(10)==1);
    assert(s.insert(5)==0);
    assert(s.get(2, x) && x == 10 && s.size()==4);
    assert(s.insert(25)==4 && s.size()==5);
    
    assert(s.insert(25)==4 && s.size()==6);
    assert(s.insert(21)==4 && s.size()==7);
    
    //s = (5, 6, 10, 20, 21, 25, 25)
    
    assert(s.remove(25)==2 && s.size()==5);
    s.erase(4);
    s.erase(3);
    assert(s.get(2, x) && x == 10 && s.size()==3);
    assert(s.remove(6)==1 && s.get(1, x) && x == 10 && s.size()==2);
    
    //s = (5, 10)
    
    assert(s.insert(2, 15) && s.get(2, x) && x == 15 && s.size()==3);
    assert(s.insert(12)==2 && s.size()==4);
    assert(s.insert(15)==3);
    assert(s.insert(15)==3);
    assert(s.remove(15)==3 && s.size()==3);
    for (int i = 0; i < s.size(); i++)
    {
        ItemType y;
        s.get(i, y);
        cerr << y << " ";
    }
    cout << " ???=== 5 10 12" << endl;
    //should print out (5, 10, 12)
    
    
    
    assert(s.set(2, 24)==true && s.size()==3);
    assert(s.set(3, 31)==false);
    s.set(0, 0);
    s.insert(3, 10);
    assert(s.find(10)==1);
    assert(s.find(24)==2);
    assert(s.find(31)==-1);
    assert(s.find(0)==0);
    for (int i = 0; i < s.size(); i++)
    {
        ItemType y;
        s.get(i, y);
        cerr << y << " ";
    }
    cout << " ???=== 0 10 24 10" << endl;
    //should print out (0, 10, 24, 10)
    
    
    
    Sequence m;
    m.swap(s);
    assert(s.empty()==true && m.empty()==false);
    assert(m.get(2, x) && x == 24 && m.size()==4);
    m.swap(m);
    assert(m.get(2, x) && x == 24 && m.size()==4);
    
    s.insert(0, 1);
    s.insert(10);
    s.insert(20);
    s.insert(30);
    s.insert(40);
    
    s = m;
    assert(s.get(2, x) && x == 24 && s.size()==4);
    assert(m.get(2, x) && x == 24 && m.size()==4);
    assert(s.get(5, x)==false && x == 24);
    
    Sequence w = s;
    assert(w.get(0, x) && x == 0 && w.size()==4);
    assert(w.get(3, x) && x == 10 && w.size()==4);
    
    
    
    Sequence t;
    t.insert(0, 1);
    t.insert(1, 3);
    t.insert(2, 5);     //(1, 3, 5,)
    
    Sequence r;
    r.insert(0, 2);
    r.insert(1, 4);
    r.insert(2, 6);     //(2, 4, 6)
    
    Sequence q;
    q.insert(0, 999);
    interleave(t, r, q);
    
    assert(q.get(0, x) && x == 1 && q.size()==6);
    assert(q.get(1, x) && x == 2);
    assert(q.get(2, x) && x == 3);
    assert(q.get(3, x) && x == 4);
    assert(q.get(4, x) && x == 5);
    assert(q.get(5, x) && x == 6);
    
    q.insert(7);
    q.insert(8);
    q.insert(9);
    interleave(t, q, r);
    assert(q.get(0, x) && x == 1 && q.size()==9);
    assert(q.get(1, x) && x == 2);
    assert(q.get(2, x) && x == 3);
    assert(q.get(3, x) && x == 4);
    assert(q.get(4, x) && x == 5);
    assert(q.get(5, x) && x == 6);
    assert(q.get(6, x) && x == 7);
    assert(q.get(7, x) && x == 8);
    assert(q.get(8, x) && x == 9);
    
    r.remove(7);
    r.remove(8);
    r.remove(9);
    
}
