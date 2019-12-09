# include <iostream>
# include <string>
# include <cassert>

using namespace std;

//returns a nicely formated string that we can use to see values in array
string arrayToPrintable(const string a[], int n){
	if(n<1) return "-1";
	string printable = "[ ";
	for(int i = 0; i<n-1; i++)
		printable+=(a[i]+", ");
	printable+=(a[n-1]+"]");
	return printable;
}


int appendToAll(string a[], int n, string value){
	cerr<< "appendToAll("<< arrayToPrintable(a, n)<<", "<<n<<", "<<value<<");"<<endl;	//print function and parameters
	
	//check if input is valid
	if(n<0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
	 	return -1;
	}

	//append
	for(int i = 0; i < n; i++){
		a[i]+=value;
	}

	//print changes
	cerr<<"    "<<arrayToPrintable(a, n)<<endl;
	cerr<<"    returns: "<<n<<endl;
	return n;
}

int lookup(const string a[], int n, string target){
	cerr<< "lookup("<< arrayToPrintable(a, n)<<", "<<n<<", "<<target<<");"<<endl;	//print function and parameters
	
	//check if input is valid
	if(n<=0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
	 	return -1;
	}

	//loop through entire array
	for(int i = 0; i < n; i++)
		//check if we got a match
		if(target == a[i]){
			//print changes and return
			cerr<<"    "<<arrayToPrintable(a, n)<<endl;
			cerr<<"    returns: "<<i<<endl;
			return i;
		}

	//if no match return -1
	cerr<<"    Wrong input!"<<endl;
	cerr<<"    returns: -1"<<endl;
	return -1;
}

int positionOfMax(const string a[], int n){
	cerr<< "positionOfMax("<< arrayToPrintable(a, n)<<", "<<n<<");"<<endl;	//print function and parameters
	
	//check if input is valid
	if(n<0||n==0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
	 	return -1;
	}

	string max = a[0];	//track current max
	int maxPos = 0;		//track position of current max

	//loop through array
	for(int i = 0; i < n; i++){

		if(max<a[i]){
			maxPos = i;
			max = a[i];
		}
	}

	//Return position of maximum 
	cerr<<"    "<<arrayToPrintable(a, n)<<endl;
	cerr<<"    returns: "<<maxPos<<endl;
	return maxPos;
}

int rotateLeft(string a[], int n, int pos){
	cerr<< "rotateLeft("<< arrayToPrintable(a, n)<<", "<<n<<", "<<pos<<");"<<endl;	//print function and parameters
	
	//check if input is valid
	if(n<0 || pos>n-1 || pos<0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
	 	return -1;
	}
	if(n==0){
		return 0;
	}


	string temp = a[pos];	//store value that needs to be put on end
	
	//loop through, rotate left
	for(int i = pos; i < n-1; i++){
		a[i] = a[i+1];
	}
	a[n-1] = temp;	//put temp on last position

	//return what needs to be returned
	cerr<<"    "<<arrayToPrintable(a, n)<<endl;
	cerr<<"    returns: "<<pos<<endl;
	return pos;
}

int countRuns(const string a[], int n){
	cerr<< "countRuns("<< arrayToPrintable(a, n)<<", "<<n<<");"<<endl;	//print function and parameters
	
	//check if input is valid
	if(n<0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
	 	return -1;
	}
	if(n==0) return 0;


	int count = 0;	//stores relevant value
	int i = 0;

	//loop through array
	while(i<n){
		int j = i;
		bool test = false;

		//read out the block of strings that are the same
		while(a[j]==a[i]){
		 	j++;
		 	test = true;
		}
		i = j;
		count+=1;

		//if we didn't already iterate, iterate
		if(!test) i++;
	}

	//return the count
	cerr<<"    "<<arrayToPrintable(a, n)<<endl;
	cerr<<"    returns: "<<count<<endl;
	return count;
}

//swap values at position i and n-1-i in array a[] of size n
void swap(string a[], int n, int i){
	cerr<< "swap("<< arrayToPrintable(a, n)<<", "<<n<<");"<<endl;	//print function and parameters
	string temp = a[i];
	
	//check if input is valid
	if(n==0) return;

	a[i] = a[n-1-i];
	a[n-1-i] = temp;
}

int flip(string a[], int n){
	cerr<< "flip("<< arrayToPrintable(a, n)<<", "<<n<<");"<<endl;	//print function and parameters
	
	//check if input is valid
	if(n<0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
	 	return -1;
	}
	if(n==0) return 0;

	//swap all combinations i, n-1-i
	for(int i = 0; i < n/2; i++){
		swap(a, n, i);
	}

	//return what needs to be returned
	cerr<<"    "<<arrayToPrintable(a, n)<<endl;
	cerr<<"    returns: "<<n<<endl;
	return n;
}

int differ(const string a1[], int n1, const string a2[], int n2){
	cerr<< "differ("<< arrayToPrintable(a1, n1)<<", "<<n1<<arrayToPrintable(a2, n2)<<", "<<n2<<", "<<");"<<endl;	//print function and parameters
	
	//check if input is valid
	if(n1<0 || n2<0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
	 	return -1;
	}

	//see which array is bigger
	int n;
	if(n1<n2) n = n1;
	else n = n2;

	// go through arrays side by side and compare until you hit two different
	for(int i = 0; i < n; i++){
		if(a1[i]!=a2[i]){ 
			cerr<<"    const"<<endl;
			cerr<<"    returns: "<<i<<endl;
			return i;
		}
	}

	//return what needs to be returned
	cerr<<"    const"<<endl;
	cerr<<"    returns: "<<n<<endl;
	return n;
}

int subsequence(const string a1[], int n1, const string a2[], int n2){
		cerr<< "subsequence("<< arrayToPrintable(a1, n1)<<", "<<n1<<arrayToPrintable(a2, n2)<<", "<<n2<<", "<<");"<<endl;	//print function and parameters
	
	//check if input is valid
	if(n1<0 || n2<0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
	 	return -1;
	}
	if(n2 == 0) return 0;

	//loop through a1[] and search for subarrays that match a2[]
	for(int i = 0; i < n1; i++){
		int j = 0;
		int temp = i;
		//loop through equal values in strings as long as they match
		while(a2[j]==a1[temp]){
			j++;
			temp++;
			//if all elements up to n2-1 match return the starting position
			if(j==n2){ 
				cerr<<"    const"<<endl;
				cerr<<"    returns: "<<i<<endl;
				return i;
			}
		}
	}

	//if no match found, return -1
	cerr<<"    Wrong input!"<<endl;
	cerr<<"    returns: -1"<<endl;
	return -1;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2){
	cerr<< "lookupAny("<< arrayToPrintable(a1, n1)<<", "<<n1<<arrayToPrintable(a2, n2)<<", "<<n2<<", "<<");"<<endl;	//print function and parameters
	
	//check if input is valid
	if(n1<=0 || n2<=0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
	 	return -1;
	 }

	 //loop through all combinations of elements from the arrays
	for(int i = 0; i < n1; i++)
		for(int j = 0; j < n2; j++){
			//if two elements are equal return the position of the element in a1[]
			if(a1[i] == a2[j]){
				cerr<<"    const"<<endl;
				cerr<<"    returns: "<<i<<endl;
				return i;
			}
		}

	//if no match return -1
	cerr<<"    Wrong input!"<<endl;
	cerr<<"    returns: -1"<<endl;
	return -1;
}

//swap array a[] values at position i and j
void swap(string a[], int n, int i, int j){
	string temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}
int separate(string a[], int n, string separator){
		cerr<< "separate("<< arrayToPrintable(a, n)<<", "<<n<<", "<<separator<<");"<<endl;	//print function and parameters
	
	//check if input is valid
	if(n<0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
		return -1;
	}
	if(n==0) return 0;

	//sort list using bubblesort
	bool sorted = false;
	while(!sorted){
		sorted = true;
		for(int i = 0; i<n-1; i++){
			if(a[i]>a[i+1]){
				swap(a, n, i, i+1);
				sorted = false;
			}
		}
	}

	//go through array and find first element >=seperator
	for(int i = 0; i<n; i++)
		if(a[i]>=separator){
			cerr<<"    separate: "<< arrayToPrintable(a, n)<<endl;
			cerr<<"    returns: "<<i<<endl;
		 	return i;
		}

	//if all elements less than seperator return size of array
	return n;
}



	int main()
	{
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
	    assert(rotateLeft(h, 1, 0) == 0);

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
		string s[40] = {"d", "a", "c", "d", "e", "c", "b", "b", "b" ,"c", "c"};
		assert(separate(cand2, 4, "hillary") == 2); 
		assert(separate(empty, 0, "hillary") == 0);
		assert(separate(s, 11, "c") == 4);

	    cout << "All tests succeeded" << endl;
	}