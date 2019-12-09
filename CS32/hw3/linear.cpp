# include <cassert>
# include <iostream>

using namespace std;

bool somePredicate(double x);


      // Return false if the somePredicate function returns false for at
      // least one of the array elements; return true otherwise.
    bool allTrue(const double a[], int n)
    {
        if(n <= 0) return true;
        if(!somePredicate(a[0])) return false;
        return allTrue(a+1, n-1);
    }

      // Return the number of elements in the array for which the
      // somePredicate function returns false.
    int countFalse(const double a[], int n)
    {
        if(n <= 0) return 0;
        return somePredicate(a[0]) ? countFalse(a+1, n-1) : countFalse(a+1, n-1)+1;
    }

      // Return the subscript of the first element in the array for which
      // the somePredicate function returns false.  If there is no such
      // element, return -1.
    int firstFalse(const double a[], int n)
    {
        if(n <= 0) return -1;
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
        if(n <= 0) return -1;
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
        if(n2 <= 0) return true;
        if(n1 <= 0) return false;
        return a1[0] == a2[0] ? includes(a1+1, n1-1, a2+1, n2-1) : includes(a1+1, n1-1, a2, n2);  // This is not always correct.
    }


/********************* LINEAR.CPP TESTS *****************************/

# include <cassert>
# include <iostream>

using namespace std;

bool somePredicate(double x)
{
    return x >= 0;
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

        cerr << "Passed all test cases." << endl;
}