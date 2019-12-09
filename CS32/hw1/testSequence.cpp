
#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Sequence s;
    assert(s.empty());  // Testy empty when empty
    assert(s.find(42) == -1);   // Test find when there's no such value
    s.insert(42);   // Test insert
    assert(s.size() == 1  &&  s.find(42) == 0); // Test size()
    assert(s.set(0, 33));   // Test set in bounds
    assert(s.set(1, 420) == false); // Test set out of bounds
    unsigned long g = 0;
    s.get(0, g);    // Test get
    assert(g == 33);
    assert(s.empty() == false);  // Testy empty when not empty
    s.insert(1, 420);
    assert(s.insert(4, 69) == false);   // Test insert at pos > size
    assert(s.erase(0) == true); // Test erase
    assert(s.find(420) == 0);
    Sequence s2;
    s2.insert(555);
    s2.insert(999);
    s2.insert(888);
    s2.dump();
    s.swap(s2);
    assert(s2.find(420) == 0);
    assert(s.find(555) == 0);  // Test find when the value does exist
    assert(s.find(999) == 2);
    assert(s.find(888) == 1);
    cerr << s.find(999);
    cerr << s.find(888);
    cerr << s2.find(420) << endl;
    s.dump();
    s2.dump();
    //assert(s.find(999) == 1);
    //assert(s.find(888) == 2);
    
    cerr << "Passed all tests" << endl;
}