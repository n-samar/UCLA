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



bool somePredicate(double x)
{
    return x >= 0;
}


      // Return false if the somePredicate function returns false for at
      // least one of the array elements; return true otherwise.
    bool allTrue(const double a[], int n)
    {
        if(n == 0) return true;
        if(!somePredicate(a[0])) return false;
        return allTrue(a+1, n-1);
    }

      // Return the number of elements in the array for which the
      // somePredicate function returns false.
    int countFalse(const double a[], int n)
    {
        if(n == 0) return 0;
        return somePredicate(a[0]) ? countFalse(a+1, n-1) : countFalse(a+1, n-1)+1;
    }

      // Return the subscript of the first element in the array for which
      // the somePredicate function returns false.  If there is no such
      // element, return -1.
    int firstFalse(const double a[], int n)
    {
        if(n == 0) return -1;
        int curr = firstFalse(a+1, n-1);
        if(curr == -1) return somePredicate(a[0]) ? -1 : 0;
        return somePredicate(a[0])? curr + 1 : 0;
    }

      // Return the subscript of the smallest double in the array (i.e.,
      // the one whose value is <= the value of all elements).  If more
      // than one element has the same smallest value, return the smallest
      // subscript of such an element.  If the array has no elements to
      // examine, return -1.
    int indexOfMin(const double a[], int n)
    {
        if(n == 0) return -1;
        int curr = indexOfMin(a+1, n-1);
        return (a[0] <= a[curr+1] || curr == -1) ? 0 : (1 + curr);
    }

      // If all n2 elements of a2 appear in the n1 element array a1, in
      // the same order (though not necessarily consecutively), then
      // return true; otherwise (i.e., if the array a1 does not include
      // a2 as a not-necessarily-contiguous subsequence), return false.
      // (Of course, if a2 is empty (i.e., n2 is 0), return true.)
      // For example, if a1 is the 7 element array
      //    10 50 40 20 50 40 30
      // then the function should return true if a2 is
      //    50 20 30
      // or
      //    50 40 40
      // and it should return false if a2 is
      //    50 30 20
      // or
      //    10 20 20
    bool includes(const double a1[], int n1, const double a2[], int n2)
    {
        if(n2 == 0) return true;
        if(n1 == 0) return false;
        return a1[0] == a2[0] ? includes(a1+1, n1-1, a2+1, n2-1) : includes(a1+1, n1-1, a2, n2);  // This is not always correct.
    }



# include <cassert>
# include <iostream>

using namespace std;

      // Return the number of ways that all n2 elements of a2 appear
      // in the n1 element array a1 in the same order (though not
      // necessarily consecutively).  The empty sequence appears in a
      // sequence of length n1 in 1 way, even if n1 is 0.
      // For example, if a1 is the 7 element array
      //    10 50 40 20 50 40 30
      // then for this value of a2     the function must return
      //    10 20 40            1
      //    10 40 30            2
      //    20 10 40            0
      //    50 40 30            3
    int countIncludes(const double a1[], int n1, 
        const double a2[], int n2)
    {
        if(n2 == 0) return 1;
        if(n1 == 0) return 0;
        return a1[0] == a2[0] ? countIncludes(a1+1, n1-1, a2+1, n2-1) + countIncludes(a1+1, n1-1, a2, n2) 
        : countIncludes(a1+1, n1-1, a2, n2);  // This is incorrect.
    }

      // Exchange two doubles
    void exchange(double& x, double& y)
    {
        double t = x;
        x = y;
        y = t;
    }

      // Rearrange the elements of the array so that all the elements
      // whose value is > separator come before all the other elements,
      // and all the elements whose value is < separator come after all
      // the other elements.  Upon return, firstNotGreater is set to the
      // index of the first element in the rearranged array that is
      // <= separator, or n if there is no such element, and firstLess is
      // set to the index of the first element that is < separator, or n
      // if there is no such element.
      // In other words, upon return from the function, the array is a
      // permutation of its original value such that
      //   * for 0 <= i < firstNotGreater, a[i] > separator
      //   * for firstNotGreater <= i < firstLess, a[i] == separator
      //   * for firstLess <= i < n, a[i] < separator
      // All the elements > separator end up in no particular order.
      // All the elements < separator end up in no particular order.
    void separate(double a[], int n, double separator,
                                        int& firstNotGreater, int& firstLess)
    {
        if (n < 0)
            n = 0;
    
          // It will always be the case that just before evaluating the loop
          // condition:
          //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
          //  Every element earlier than position firstNotGreater is > separator
          //  Every element from position firstNotGreater to firstUnknown-1 is
          //    == separator
          //  Every element from firstUnknown to firstLess-1 is not known yet
          //  Every element at position firstLess or later is < separator
    
        firstNotGreater = 0;
        firstLess = n;
        int firstUnknown = 0;
        while (firstUnknown < firstLess)
        {
            if (a[firstUnknown] < separator)
            {
                firstLess--;
                exchange(a[firstUnknown], a[firstLess]);
            }
            else
            {
                if (a[firstUnknown] > separator)
                {
                    exchange(a[firstNotGreater], a[firstUnknown]);
                    firstNotGreater++;
                }
                firstUnknown++;
            }
        }
    }

      // Rearrange the elements of the array so that
      // a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
      // If n <= 1, do nothing.
    void order(double a[], int n)
    {
        if(n <= 1) return;
        int firstNotGreater;
        int firstLess;
        separate(a, n, a[0], firstNotGreater, firstLess);
        order(a, firstNotGreater);
        order(a+firstLess, n-firstLess);
        return;  // This is not always correct.
    }

void testone()
{

	
	double a[10] = { 3, 6, -5, 5, -2, 1, -7, -4, 8, 2 };
	double b[4] = { 6, -5, -2, -4 };
	double c[8] = { 10, 50, 40, 20, 50, 40, 30 };
	double d[12] = { 10, 20, 40, 10, 40, 30, 20, 10, 40, 50, 40, 30 };
	double e[12] = { 10, 10, 10, 20, 20, 30, 30, 40, 40, 40, 40, 50 };
	double f[16] = { 40, 70, 10, 120, 30, 40, 50, 110,
        90, 20, 10, 60, 100, 110, 40, 80 };
	double g[16] = { 120, 110, 110, 100, 90, 80, 70, 60,
        50, 40, 40, 40, 30, 20, 10, 10 };


    double test[4] = {3, 5, 8, 9};
    double test2[3] = {10, 20, 40};
    double test3[3] = {10, 40, 30};
    double test4[3] = {20, 10, 40};
    double test5[3] = {50, 40, 30};
    double aa[12] = {3, 5, 5, 8, 9, 8, 5, 3, 3, 1, 0, 6};
    double bb[7] = {10, 50, 40, 20, 50, 40, 30};
    double tester[3] = {2, -1, -3};

            assert(allTrue(aa, 12));
            assert(allTrue(bb, 7));
            assert(!allTrue(tester, 3));
            assert(!allTrue(a, 10));
            assert(!allTrue(a, 8));
            assert(!allTrue(a+2, 8));
            assert(allTrue(a+8, 2));
            assert(!allTrue(a+6, 2));
            assert(allTrue(a, 1));
            assert(!allTrue(a+2, 1));
            assert(allTrue(a, 0)  &&  allTrue(a+2, 0));

            assert(countFalse(a, 10) == 4);
            assert(countFalse(a, 8) == 4);
            assert(countFalse(a+2, 8) == 4);
            assert(countFalse(a+8, 2) == 0);
            assert(countFalse(a+6, 2) == 2);
            assert(countFalse(a, 1) == 0);
            assert(countFalse(a+2, 1) == 1);
            assert(countFalse(a, 0) == 0  &&  countFalse(a+2, 0) == 0);
            assert(countFalse(tester, 3) == 2);
            assert(countFalse(aa, 12) == 0);

            assert(firstFalse(aa, 12) == -1);
            assert(firstFalse(tester, 3) == 1);
            assert(firstFalse(a, 10) == 2);
            assert(firstFalse(a, 8) == 2);
            assert(firstFalse(a+2, 8) == 0);
            assert(firstFalse(a+8, 2) == -1);
            assert(firstFalse(a+6, 2) == 0);
            assert(firstFalse(a, 1) == -1);
            assert(firstFalse(a+2, 1) == 0);
            assert(firstFalse(a, 0) == -1  &&  firstFalse(a+2, 0) == -1);

            assert(indexOfMin(a, 10) == 6);
            assert(indexOfMin(a, 7) == 6);
            assert(indexOfMin(a+6, 4) == 0);
            assert(indexOfMin(a+8, 2) == 1);
            assert(indexOfMin(a+7, 2) == 0);
            assert(indexOfMin(a, 1) == 0);
            assert(indexOfMin(a+2, 1) == 0);
            double x[8] = { 3, 6, -5, 2, -5, -5, -5, 2 };
            assert(indexOfMin(x, 8) == 2);
            assert(indexOfMin(a, 0) == -1  &&  indexOfMin(a+2, 0) == -1);
            assert(indexOfMin(test, 4) == 0);
            assert(indexOfMin(aa, 12) == 10);
            assert(indexOfMin(bb, 7) == 0);

            assert(includes(a, 10, b, 4));
            assert(includes(a+1, 9, b, 4));
            assert(!includes(a+2, 8, b, 4));
            assert(!includes(a+1, 6, b, 4));
            assert(includes(a+1, 7, b, 4));
            assert(includes(a, 10, b+2, 1));
            assert(includes(a+1, 9, b, 1));
            assert(!includes(a+2, 8, b, 1));
            assert(includes(a, 10, b, 0));
            assert(includes(a, 0, b, 0));
            assert(includes(a+1, 2, b, 2));
            assert(!includes(a+2, 2, b+1, 2));
            assert(!includes(a+2, 2, b+1, 3));
            assert(!includes(a+1, 1, b, 2));
            assert(!includes(a+1, 0, b, 1));
            assert(includes(c, 7, d+3, 3));
            assert(!includes(c, 7, d+6, 3));
            assert(includes(c, 7, d+9, 3));

            assert(includes(aa, 12, test, 4));
            assert(includes(bb, 7, test2, 3));
            assert(includes(bb, 7, test3, 3));
            assert(!includes(bb, 7, test4, 3));
            assert(includes(bb, 7, test5, 3));

            assert(countIncludes(c, 7, d, 3) == 1);
            assert(countIncludes(aa, 12, test, 4) == 2);
            assert(countIncludes(bb, 7, test2, 3) == 1);
            assert(countIncludes(bb, 7, test3, 3) == 2);
            assert(countIncludes(bb, 7, test4, 3) == 0);
            assert(countIncludes(bb, 7, test5, 3) == 3);
            assert(countIncludes(c, 7, d+3, 3) == 2);
            assert(countIncludes(c, 7, d+6, 3) == 0);
            assert(countIncludes(c, 7, d+9, 3) == 3);
            assert(countIncludes(c, 7, d+2, 1) == 2);
            assert(countIncludes(c, 2, d+2, 1) == 0);
            assert(countIncludes(c, 7, d, 0) == 1);
            assert(countIncludes(c, 0, d, 0) == 1);

            order(f, 16);
            assert(equal(f, f+16, g));
            order(f, 1);
            assert(f[0] == 40);
            order(f, 0);
            assert(f[0] == 40);


/*********************** MAZE.CPP *****************************/
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



int main()
{
    testone();
}