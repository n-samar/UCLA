# CS31-Project4

	•	Obstacles: Creating test cases. Running test cases. seperate () didn’t work for a long time. Taking care of the edge cases was the biggest issue.

	•	Test cases, divided by function:
	•	int appendToAll(string a[], int n, string value)
	•	Negative array size value
	•	0 array size value
	•	First argument is empty array
	•	Third argument is empty string
 
	•	int lookup(const string a[], int n, string target):
	•	Negative array size value
	•	0 array size value
	•	First argument is empty array
	•	Third argument is empty string

	•	int positionOfMax(const string a[], int n):
	•	Negative array size value
	•	0 array size value
	•	First argument is empty array
	•	More than one maximum value in array

	•	int rotateLeft(string a[], int n, int pos):
	•	Negative array size value
	•	0 array size value
	•	First argument is empty array
	•	Third argument negative
	•	Third argument greater than or equal to second
	•	Third argument equal to the index of the last element of array

	•	int countRuns(const string a[], int n):
	•	Negative array size value
	•	0 array size value
	•	First argument is empty array
	•	First argument contains some equal consecutive elements
	•	First argument contains the same string a couple of times
	•	First argument does not contain any equal consecutive elements

	•	int flip(string a[], int n):
	•	Negative array size value
	•	0 array size value
	•	First argument is empty array
	•	Second argument is odd
	•	Second argument is even

	•	int differ(const string a1[], int n1, const string a2[], int n2):
	•	Negative array size value
	•	0 array size value
	•	First argument is empty array
	•	The first element that differs has a smaller index in the first array
	•	The first element that differs has a smaller index in the second array


	•	int subsequence(const string a1[], int n1, const string a2[], int n2):
	•	Second argument negative
	•	Second argument 0
	•	Fourth argument negative
	•	Fourth argument zero
	•	Any combination of either ii. and jj. or either kk. and ll.
	•	 Second array is the subset of the first only once
	•	Second array is the subset of the first more than once
	•	Second array is not the subset of the first array
	•	First array is empty
	•	Second array is empty
	•	Any combination of qq. and rr.
	
	•	int lookupAny(const string a1[], int n1, const string a2[], int n2):
	•	Second argument negative
	•	Second argument 0
	•	Fourth argument negative
	•	Fourth argument zero
	•	Any combination of either ii. and jj. or either kk. and ll.
	•	First array is empty
	•	Second array is empty
	•	Any combination of qq. and rr.
	•	The two arrays don’t have common elements
	•	The two elements have one common element
	•	The two arrays have two common elements (say a and b) such that a comes before b in the first array and the opposite is true for the second array.
	•	The two arrays have more than two common elements with property ddd. applied to the first common element in the first and first common element in the second array.
	•	The first common element in the first array is repeated more than once in the first array.

	•	int separate(string a[], int n, string separator):
	•	Second argument negative
	•	Second argument 0
	•	Fourth argument negative
	•	Fourth argument zero
	•	Any combination of either ii. and jj. or either kk. and ll.
	•	First array is empty
	•	Second array is empty
	•	Any combination of qq. and rr.
	•	The separator is bigger than all array elements
	•	The separator is smaller than all array elements
	•	There are elements in the array bigger, smaller, and equal to the separator
	•	There are elements in the array bigger, smaller to the separator
	•	There are elements in the array that are bigger, equal to the separator
	•	There are elements in the array that are smaller, equal to the separator

	Along with these test cases, we must also test with valid input the functions that have not been tested with valid input.
	These are the test cases in C++ assert() statements:

    string empty[0];
	    string h[7] = { "jill", "hillary", "donald", "tim", "", "evan", "gary" };
	    
	    assert(appendToAll(h, -3, "!!!") == -1);
	    assert(appendToAll(h, 0, "!!!") == 0);
	    assert(appendToAll(empty, 0, "!!!") == 0);
	    assert(appendToAll(h, 7, "!!!"));

	    assert(lookup(h, 0, "f") == -1);
	    assert(lookup(h, 7, "evan!!!") == 5);
	    assert(lookup(h, 7, "donald!!!") == 2);
	    assert(lookup(h, 2, "donald!!!") == -1);

	    assert(positionOfMax(h, 7) == 3);
	    assert(positionOfMax(h, 0) == -1);	
	    assert(positionOfMax(empty, 0) == -1); 
	    h[0] = "tim";
	    assert(positionOfMax(h, 7) == 3);

	    assert(rotateLeft(h, 7, -1) == -1);
	    assert(rotateLeft(h, 7, 0) == 0);
	    assert(rotateLeft(empty, 0, 0) == -1);
	    assert(rotateLeft(h, 7, 7) == -1);
	    assert(rotateLeft(h, 7, 6) == 6);

	    string list1 [] = {"a", "b", "c", "d"};
	    string list2 [] = {"a","a","a","a", "b", "b","b","b","c", "d", "d", "d"};
	    string list3 [] = {"a", "a", "a", "a"};
	    assert(countRuns(list1, 4) == 4);
	    assert(countRuns(list1, 0) == 0);
	    assert(countRuns(list1, -1) == -1);
	    assert(countRuns(list2, 12) == 4);
	    assert(countRuns(list3, 4) == 1);

	    assert(flip(list1, -5) == -1);
	    assert(flip(list1, 0) == 0);	
	    assert(flip(empty, 0) == 0);	
	    assert(flip(list1, 3) == 3);
	    assert(flip(list1, 4) == 4);

	    string a1[] = {"a", "b", "c"};
	    string a2[] = {"c", "b", "a"};
	    string folks[6] = { "ajamu", "mike", "", "tim", "mindy", "bill" };
		string group[5] = { "ajamu", "mike", "bill", "", "tim" };
	    assert(differ(a1, 3, a2, 3) == 0);
	    assert(differ(folks, 6, group, 5) == 2);
	    assert(differ(folks, 2, group, 1) == 1);
	    assert(differ(folks, 2, group, 0) == 0);	
	    assert(differ(folks, 0, group, 2) == 0);	
	    assert(differ(empty, 0, group, 2) == 0);
	    assert(differ(folks, 5, empty, 0) == 0);

	    string names[10] = { "evan", "hillary", "mindy", "jill", "ajamu", "gary" };
		string names1[10] = { "hillary", "mindy", "jill" };
		assert(subsequence(names, 6, names1, 3) == 1); 
string names2[10] = { "evan", "jill" };
		assert(subsequence(names, 5, names2, 2) == -1);  
		assert(subsequence(names, 5, empty, 0) == 0);
		assert(subsequence(empty, 0, names, 5) == -1);

		string namess[10] = { "evan", "hillary", "mindy", "jill", "ajamu", "gary" };
		string set1[10] = { "bill", "ajamu", "jill", "hillary" };
		assert(lookupAny(namess, 6, set1, 4)==1); 
		string set2[10] = { "tim", "donald" };
		assert(lookupAny(namess, 6, set2, 2)==-1);  
		assert(lookupAny(namess, 6, empty, 0) == -1);
		assert(lookupAny(empty, 0, names, 5) == -1);

		string cand[6] = { "donald", "jill", "hillary", "tim", "evan", "bill" };
		assert(separate(cand, 6, "gary") == 3);  
		string cand2[4] = { "gary", "hillary", "jill", "donald" };
		assert(separate(cand2, 4, "hillary") == 2); 
		assert(separate(empty, 0, "hillary") == 0);

