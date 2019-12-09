//TESTING NEWSEQUENCE
#include "newSequence.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Sequence s;
    assert(s.empty());
    assert(s.find(42) == -1);
    s.insert(42);
    assert(s.size() == 1  &&  s.find(42) == 0);
    assert(s.set(0, 33));
    assert(s.set(1, 420) == false);
    unsigned long g = 0;
    s.get(0, g);
    assert(g == 33);
    s.insert(1, 420);
    assert(s.insert(4, 69) == false);
    assert(s.erase(0) == true);
    assert(s.find(420) == 0);
    Sequence s2;
    s2.insert(555);
    s2.insert(999);
    s2.insert(888);
    s2.dump();
    s.swap(s2);
    assert(s.find(555) == 0);
    assert(s.find(999) == 2);
    assert(s.find(888) == 1);
    cerr << s.find(999) << endl;
    cerr << s.find(888) << endl;
    cerr << s2.find(420) << endl;
    s.dump();
    s2.dump();
    assert(s.find(999) == 2);
    assert(s.find(888) == 1);
    
    Sequence a(1000);   // a can hold at most 1000 items
    Sequence b(5);      // b can hold at most 5 items
    Sequence c;         // c can hold at most DEFAULT_MAX_ITEMS items
    ItemType v = 777;
    
    // No failures inserting 5 items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(k, v));
    
    // Failure if we try to insert a sixth item into b
    assert(!b.insert(5, v));
    
    // When two Sequences' contents are swapped, their capacities are
    // swapped as well:
    a.swap(b);
    assert(!a.insert(5, v));
    for (int k = 0; k < 1000; k++)
        assert(b.insert(k, v));
    
    
    
    
    cerr << "Passed all tests" << endl;
}