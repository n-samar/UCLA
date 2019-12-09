# include <cassert>
# include <iostream>

# include "Sequence.h"

using namespace std;

int main()
{

/*************************** EMPTY LINKED LIST *******************************/
    ItemType t;
    Sequence s;         // Construct empty linked list

    Sequence s2 = s;    // Copy constructor

    s2 = s;             // Assignment operator

    assert(s.empty());
    assert(s.size() == 0);
    assert(!s.erase(0) && !s.erase(1));     
    assert(s.remove(2) == 0);              
    assert(!s.get(0, t) && !s.get(1, t));   
    assert(!s.set(0, 1) && !s.set(1, 1));   
    assert(s.find(5) == -1);                
    s2 = s;
    s.swap(s2);                             // swap two empty lists

    assert(!s2.insert(1, 2));
    assert(s2.insert(0, 2) 
        && s2.insert(1, 3) 
        && s2.insert(2, 4));                // insert into empty list

    s.swap(s2);                             // swap where this is empty
                                            // but other is not
    assert(s2.empty());
    assert(s.get(0, t) && t == 2);          // first elem after swap
    assert(s.get(1, t) && t == 3);          // second elem after swap
    assert(s.get(2, t) && t == 4);          // ...

    s.swap(s2);                             // swap where this is not empty
                                            // but other is

    assert(s.empty());
    assert(s2.get(0, t) && t == 2);         // first elem after swap
    assert(s2.get(1, t) && t == 3);         // second elem after swap
    assert(s2.get(2, t) && t == 4);         // ...

    Sequence s3;
    assert(subsequence(s, s3) == -1);        // two empty lists
    assert(subsequence(s2, s) == -1);        // main list not empty, other empty
    assert(subsequence(s, s2) == -1);       // main list empty, other not empty

    interleave(s2, s, s3);                  // first not empty
                                            // second empty
                                            // third empty
    // interleave should copy s2 into s3 if s is empty
    assert(s3.get(0, t) && t == 2);         
    assert(s3.get(1, t) && t == 3);         
    assert(s3.get(2, t) && t == 4);     

    interleave(s, s2, s3);                  // first empty
                                            // second not empty
                                            // third not empty
    // interleave should copy s2 into s3 if s is empty
    assert(s3.get(0, t) && t == 2);         
    assert(s3.get(1, t) && t == 3);         
    assert(s3.get(2, t) && t == 4);

    interleave(s, s, s3);                   // first two empty should make empty
    assert(s3.empty());

/*************************** ALIASING ****************************************/
    
    s2 = s2;                                // Assignment operator aliasing
    assert(s2.get(0, t) && t == 2);         
    assert(s2.get(1, t) && t == 3);         
    assert(s2.get(2, t) && t == 4);         

    s2.swap(s2);                            // swap aliasing
    assert(s2.get(0, t) && t == 2);         
    assert(s2.get(1, t) && t == 3);         
    assert(s2.get(2, t) && t == 4);

    assert(subsequence(s2, s2) == 0);       // subsequence aliasing

    interleave(s2, s2, s3);         // interleave aliasing
                                            // should double up s2 into s3

    ItemType t1, t2;
    for(int i = 0; i < s2.size(); i++)
        assert(s2.get(i, t1) && s3.get(2*i, t2) && t1 == t2);

/************************** RANDOM BUT NORMAL TESTS *************************/

    unsigned long array[50];
 
    srand((unsigned)time(0)); 
    const int SIZE = 10;
    const int ITER = 40000;
    
    // Check everything ITER times with new rand input
    for(int x_c = 0; x_c < ITER; x_c++)
    {
        Sequence test;

        // TESTING INSERT( -, -)
        for(int i = 0; i<SIZE; i++)
        {
            array[i] = 100*i + (rand()%100)+1;
            assert(test.insert(i, array[i]));
        }

        // TESTING REMOVE
        for(int i = 0; i<SIZE; i++)
        {
            assert(test.remove(array[i]) == 1);
        }

        // TESTING INSERT(-)
        for(int i = 0; i<SIZE; i++)
        {
            array[i] = 100*i + (rand()%100)+1;
            assert(test.insert(array[i]) != -1);
        }

        // TESTING REMOVE
        for(int i = 0; i<SIZE; i++)
        {
            assert(test.remove(array[i]) != 0);
        }

        assert(test.empty());  // TESTING EMPTY (all deleted in prev. for)

        for(int i = 0; i<SIZE; i++)
        {
            array[i] = (rand()%100)+1;
            test.insert(i, array[i]);
        }

        // TESTING GET
        for(int i = 0; i<SIZE; i++)
            assert(test.get(i, t1) && t1 == array[i]);

        Sequence test2;
        for(int i = 0; i<SIZE; i++)
        {
            array[i] = (rand()%100)+1;
            test2.insert(array[i]);
        }

        // TESTING INSERT(-) positioning
        // should be sorted because insert was used
        for(int i = 0; i<SIZE-1; i++)
        {
            assert(test2.get(i, t1));
            assert(test2.get(i+1, t2));
            assert(t1 <= t2);
        }

        // TESTING SIZE as it slowly reduces to 0
        for(int i = 0; i<SIZE; i++)
        {
            assert(test2.erase(0));
            assert(test2.size() == SIZE-i-1);
        }
        assert(test2.empty());

        // Create linked list of non-repeating int's (using %)
        for(int i = 0; i < SIZE; i++)
        {
            array[i] = 100*i+(rand()%10)+1; // make them unique for find
            test2.insert(i, array[i]);
        }

        // TESTING FIND on non-repeating list
        for(int i = 0; i < SIZE; i++)
        {
            assert(test2.get(i, t1) && t1 == array[i]);
            assert(test2.find(array[i]) == i);
        }

        Sequence test3, test4;
        int array1[SIZE], array2[SIZE];
        int c = 0;

        // Setting up swap test using 2 linked lists of random content
        // test4 will also have random size
        for(int i = 0; i<SIZE; i++)
        {
            array1[i] = (rand()%100)+1;
            test3.insert(i, array1[i]);

            // variable size
            if(rand()%2)
            {
                array2[c] = (rand()%100)+1;
                test4.insert(c, array2[c]);
                c++;
            }
        }
        test3.swap(test4);

        // TESTING SWAP
        for(int i = 0; i < SIZE; i++)
        {
            assert(test4.get(i, t1) && t1 == array1[i]);
            // Extra check because test3.get is not defined for all i since it is smaller
            assert(i >= test3.size() || (test3.get(i, t1) && t1 == array2[i]));
        }

        // TESTING SUBSEQUENCE
        // erase some elements from original and check if subsequence becomes buggy
        Sequence test5 = test4;
        if(SIZE > 0)
        {
           int er = rand()%SIZE;
        for(int k = 0; k < er; k++)
            test5.erase(0);
        if(test5.size() != 0)
            assert(subsequence(test4, test5) != -1);
        else
            assert(subsequence(test4, test5) == -1);
        }
        // HOW TO TEST WHEN SOMETHING IS NOT A SUBSEQUENCE ??? HARD

        // TESTING INTERLEAVE
        Sequence res;
        interleave(test3, test4, res); // second is larger than first

        // check part with intertwined elements
        for(int i = 0; i < test3.size(); i++)
        {
            assert(res.get(2*i+1, t1) && test4.get(i, t2) && t1 == t2);
            assert(res.get(2*i, t1) && test3.get(i, t2) && t1 == t2);
        }

        // delete everything in res except the test4 left over
        for(int i = 0; i < 2*test3.size(); i++)
            res.erase(0);



        interleave(test4, test3, res); // second is smaller than first

        // check part with intertwined elements
        for(int i = 0; i < test3.size(); i++)
        {
            assert(res.get(2*i+1, t1) && test3.get(i, t2) && t1 == t2);
            assert(res.get(2*i, t1) && test4.get(i, t2) && t1 == t2);
        }

        // delete everything in res except the test 4 left over
        for(int i = 0; i < 2*test3.size(); i++)
            res.erase(0);

        // check if it is a proper leftover by using subsequence
        if(res.size() != 0)
            assert(subsequence(test4, res) != -1);
    }

    cerr << "Passed all test cases!" << endl;
}