# include <iostream>
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

bool inBounds(int r, int c, int nRows, int nCols)
{
	if(r > -1 && r < nRows && c > -1 && c < nCols)
		return true;
	return false;
}

 bool pathExists(string maze[], int nRows, int nCols, 
 	int sr, int sc, int er, int ec)
 {
 	queue<Coord> q;

 	Coord beg(sr, sc);
 	maze[sr][sc] = 'V';		// 'V' for Vendetta
 	q.push(beg);
 	int count = 1;
 	while(!q.empty())
 	{
 		Coord curr = q.front();
 		cerr << count << ": (" << curr.r() << ", " << curr.c() << ")" << endl;
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