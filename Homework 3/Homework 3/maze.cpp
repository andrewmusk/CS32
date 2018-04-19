

//
//  main.cpp
//  Mazes
//
//  Created by Andrew Musk on 2017/02/05.
//  Copyright © 2017 Andrew Musk. All rights reserved.
//

#include <iostream>
#include <stack>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);



class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    maze[sr][sc] = '#';
    
    if(sr==er&&sc==ec)
    {
        return true;
    }
    bool result;
    
    if(maze[sr-1][sc]=='.')
    {
        result = pathExists(maze, nRows, nCols, sr-1, sc, er, ec);
        if(result)
            return true;
    }
    if(maze[sr][sc+1]=='.')
    {
        result = pathExists(maze, nRows, nCols, sr, sc+1, er, ec);
        if(result)
            return true;
    }
    if(maze[sr+1][sc]=='.')
    {
        result = pathExists(maze, nRows, nCols, sr+1, sc, er, ec);
        if(result)
            return true;
    }
    if(maze[sr][sc-1]=='.')
    {
        result = pathExists(maze, nRows, nCols, sr, sc-1, er, ec);
        if(result)
            return true;
    }
    
    return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}


