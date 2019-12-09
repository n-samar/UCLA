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
        if(n2 <= 0) return 1;
        if(n1 <= 0) return 0;
        return a1[0] == a2[0] ? countIncludes(a1+1, n1-1, a2+1, n2-1) + countIncludes(a1+1, n1-1, a2, n2) 
        : countIncludes(a1+1, n1-1, a2, n2);
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
        return; 
    }

/************************ TREE.CPP TEST *********************/

#include <iostream>
#include <streambuf>
#include <algorithm>
#include <cstdlib>
#include <cassert>

using namespace std;

bool checkOrder(double a[], int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		if (a[i] < a[i + 1])
			return false;
	}
	return true;
}

int main()
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
            double f2[16] = { 40, 70, 10, 120, 30, 40, 50, 110,
        90, 20, 10, 60, 100, 110, 40, 80 };
            order(f2, 1);
            double f3[16] = { 40, 70, 10, 120, 30, 40, 50, 110,
        90, 20, 10, 60, 100, 110, 40, 80 };
            assert(f3[0] == 40);
            double f4[16] = { 40, 70, 10, 120, 30, 40, 50, 110,
        90, 20, 10, 60, 100, 110, 40, 80 };
            order(f4, 0);
            assert(f4[0] == 40);

	cout << "Passed all test cases." << endl;

}
