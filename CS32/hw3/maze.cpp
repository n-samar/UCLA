#include <iostream>
#include <streambuf>
#include <algorithm>
#include <cstdlib>
#include <cassert>
using namespace std;

# include <cassert>
# include <iostream>

using namespace std;

/*
    If the start location is equal to the ending location, then we've
        solved the maze, so return true.
    Mark the start location as visted.
    For each of the four directions,
        If the location one step in that direction (from the start
        location) is unvisited,
            then call pathExists starting from that location (and
                ending at the same ending location as in the
                current call).
             If that returned true,
                 then return true.
    Return false.
*/

#include <string>
#include <cassert>

using namespace std;

bool inBounds(int r, int c, int nRows, int nCols)
{
    if(r > -1 && r < nRows && c > -1 && c < nCols)
        return true;
    return false;
}

bool pathExists(string maze[], int nRows, int nCols, 
    int sr, int sc, int er, int ec)
{
    if(sr == er && sc == ec) return true;
    maze[sr][sc] = 'V';
    int r = sr;
    int c = sc;
    // NORTH
        if(inBounds(r-1, c, nRows, nCols)
            && maze[r-1][c] == '.')
            if(pathExists(maze, nRows, nCols, r-1, c, er, ec)) return true;
        // EAST
        if(inBounds(r, c+1, nRows, nCols)
            && maze[r][c+1] == '.')
            if(pathExists(maze, nRows, nCols, r, c+1, er, ec)) return true;
        // SOUTH
        if(inBounds(r+1, c, nRows, nCols)
            && maze[r+1][c] == '.')
            if(pathExists(maze, nRows, nCols, r+1, c, er, ec)) return true;
        // WEST
        if(inBounds(r, c-1, nRows, nCols)
            && maze[r][c-1] == '.')
            if(pathExists(maze, nRows, nCols, r, c-1, er, ec)) return true;
return false;
}


/**************************** MAZE.CPP TESTS ******************************/


#include <iostream>
#include <streambuf>
#include <algorithm>
#include <cstdlib>
#include <cassert>
# include <queue>

using namespace std;


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



 bool pathExistsQueue(string maze[], int nRows, int nCols, 
    int sr, int sc, int er, int ec)
 {
    queue<Coord> q;

    Coord beg(sr, sc);
    maze[sr][sc] = 'V';     // 'V' for Vendetta
    q.push(beg);
    int count = 1;
    while(!q.empty())
    {
        Coord curr = q.front();
        count++;
        q.pop();
         int r = curr.r();
        int c = curr.c();
        if(curr.r() == er && curr.c() == ec)
            return true;
        // NORTH
        if(inBounds(r-1, c, nRows, nCols)
            && maze[r-1][c] == '.')
        {
            maze[r-1][c] = 'V';
            Coord newC(r-1, c);
            q.push(newC);
        }
        // EAST
        if(inBounds(r, c+1, nRows, nCols)
            && maze[r][c+1] == '.')
        {
            maze[r][c+1] = 'V';
            Coord newC(r, c+1);
            q.push(newC);
        }
        // SOUTH
        if(inBounds(r+1, c, nRows, nCols)
            && maze[r+1][c] == '.')
        {
            maze[r+1][c] = 'V';
            Coord newC(r+1, c);
            q.push(newC);
        }
        // WEST
        if(inBounds(r, c-1, nRows, nCols)
            && maze[r][c-1] == '.')
        {
            maze[r][c-1] = 'V';
            Coord newC(r, c-1);
            q.push(newC);
        }
    }
    return false;
 }
int main()
{
	    static string maze1[10] = {
        "XXXXXXXXXX",
        "X.X..X...X",
        "X.XX.X.XXX",
        "X....X.X.X",
        "XX.X.X...X",
        "XXX..X.X.X",
        "X...X...XX",
        "X.XX..X.XX",
        "X....X...X",
        "XXXXXXXXXX",
    };
    
    static string maze2[10] = {
        "XXXXXXXXXX",
        "X.X..X...X",
        "X.XX.X.XXX",
        "X....X.X.X",
        "XX.X.X...X",
        "XXX.XX.X.X",
        "X...X...XX",
        "X.XX..X.XX",
        "X....X...X",
        "XXXXXXXXXX",
    };
    
    static string maze3[10] = {
        "XXXXXXXXXX",
        "XX.....XXX",
        "X..XX....X",
        "X...X...XX",
        "X.X.XXX..X",
        "XXXX..X..X",
        "XX....X..X",
        "X.......XX",
        "X..XXXXXXX",
        "XXXXXXXXXX",
    };
    
    static string maze4[10] = {
        "XXXXXXXXXX",
        "XX.....XXX",
        "X..XX....X",
        "X...X...XX",
        "X.X.XXX..X",
        "XXXX..X..X",
        "XX....X..X",
        "X.....X.XX",
        "X..XXXXXXX",
        "XXXXXXXXXX",
    };

            assert(pathExists(maze1, 10, 10, 8, 6, 1, 1));
            assert(!pathExists(maze2, 10, 10, 8, 6, 1, 1));
            assert(pathExists(maze3, 10, 10, 4, 3, 7, 1));
            assert(!pathExists(maze4, 10, 10, 4, 3, 7, 1));

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
        
            for(int i = 0; i < 10; i++)
                for(int j = 0; j < 10; j++)
                    for(int x = 0; x < 10; x++)
                        for(int y = 0; y < 10; y++)
                            assert(pathExistsQueue(maze, 10, 10, i, j, x, y) 
                        == pathExists(maze, 10, 10, i, j, x, y));


            cerr << "Passed all test cases." << endl;
}
