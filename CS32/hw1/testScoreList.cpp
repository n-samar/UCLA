//TESTING SCORELIST
#include "Sequence.h"
#include "ScoreList.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    ScoreList scores;
    scores.add(10); // Test add
    scores.add(35);
    scores.add(42);
    cerr<<scores.minimum(); // Test minimum
    assert(scores.maximum() == 42); // Test maximum
    scores.remove(42);  // Test remove
    assert(scores.size() == 2); // Test size()
    assert(scores.maximum() == 35);
    ScoreList scores2;
    scores2 = scores;   // Test assignment operator
    assert(scores2.size() == 2);
    assert(scores2.maximum() == 35);
    ScoreList scores3 = scores; // Test copy constructor
    assert(scores3.size() == 2);
    assert(scores3.maximum() == 35);
    
    cerr << "Passed all tests" << endl;
}