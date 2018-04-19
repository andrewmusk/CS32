////
////  main.cpp
////  Mazes
////
////  Created by Andrew Musk on 2017/02/05.
////  Copyright © 2017 Andrew Musk. All rights reserved.
////
//
//#include <iostream>
//#include <queue>
//using namespace std;
//
//bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
//
//class Coord
//{
//public:
//    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
//    int r() const { return m_r; }
//    int c() const { return m_c; }
//private:
//    int m_r;
//    int m_c;
//};
//
//
//bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
//{
//    queue<Coord> coordQueue;
//    Coord begin(sr,sc);
//    Coord end(er,ec);
//    coordQueue.push(begin);
//    maze[sr][sc] = '#';
//    while(!coordQueue.empty())
//    {
//        Coord current = coordQueue.front();
//        coordQueue.pop();
//        
//        int c_r = current.r();
//        int c_c = current.c();
//        
//        cerr << "The coordinates popped were (" << c_r << ", " << c_c << ")" << endl;
//        
//        
//        if(c_r==er&&c_c==ec)
//        {
//            return true;
//        }
//        
//        if(maze[c_r-1][c_c]=='.')
//        {
//            maze[c_r-1][c_c] = '#';
//            coordQueue.push(Coord(c_r-1,c_c));
//        }
//        if(maze[c_r][c_c+1]=='.')
//        {
//            maze[c_r][c_c+1] = '#';
//            coordQueue.push(Coord(c_r, c_c+1));
//        }
//        if(maze[c_r+1][c_c]=='.')
//        {
//            maze[c_r+1][c_c] = '#';
//            coordQueue.push(Coord(c_r+1, c_c));
//        }
//        if(maze[c_r][c_c-1]=='.')
//        {
//            maze[c_r][c_c-1] = '#';
//            coordQueue.push(Coord(c_r,c_c-1));
//        }
//        
//    }
//    return false;
//}
