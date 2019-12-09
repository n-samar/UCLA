 #include <iostream>
 #include <cassert>
#include <cstring>

using namespace std;

const int MAX_WORD_LENGTH = 20;
const int MAX_DOC_LENGTH = 200;

//same behavior as rotateLeft() from Project 4 applied to char matrices
int rotateLeft(char a[][MAX_WORD_LENGTH+1], int n, int pos){
	
	cerr<< "in rotateLeft"<<endl;
	//check if input is valid
	if(n<0 || pos>n-1 || pos<0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
	 	return -1;
	}
	if(n==0){
		return 0;
	}
	char temp[MAX_WORD_LENGTH+1];	//store value that needs to be put on end
	strcpy(temp, a[pos]);
	//loop through, rotate left
	for(int i = pos; i < n-1; i++){
		strcpy(a[i], a[i+1]);
	}
	strcpy(a[n-1], temp);	//put temp on last position

	//return what needs to be returned
	return pos;
}


//same as other rotateLeft functions, just adapted for distance[]
void rotateLeft2(int a[], int n, int pos){
	int temp = a[pos];
	for(int i = pos; i<n-1; i++){
		a[i] = a[i+1];
	}
	a[n-1] = temp;
}

int normalizeRules(char word1[][MAX_WORD_LENGTH+1],
                   char word2[][MAX_WORD_LENGTH+1],
                   int distance[],
                   int nRules){
	cerr<< "in normalizeRules"<<endl;
	if(nRules < 0) nRules = 0;

	//go through all rules and set all words to lowercase
	//if word contains non-alpha characters, delete the rule that contains it
	for(int i = 0; i<nRules; i++){
		for(int j = 0; word1[i][j]!='\0'; j++){
			//cerr<<"1"<<endl;
			if(!isalpha(word1[i][j])){
			 	distance[i] = -1;
			 	cerr<<i<<": w1 NOT ALPHA"<<endl;
			 }
			word1[i][j] = tolower(word1[i][j]);
		}
		for(int j = 0; word2[i][j]!='\0'; j++){
			//cerr<<"2"<<endl;
			word2[i][j] = tolower(word2[i][j]);
			if(!isalpha(word2[i][j])){ 
				distance[i] = -1;
				cerr<<i<<": w2 NOT ALPHA"<<endl;
			}
		}
	}

	//delete all rules with w1 or w2 empty
	for(int i = 0; i<nRules; i++){
		//cerr<<"3"<<endl;
		if(word1[i][0] == '\0'){
			 distance[i] = -1;
			 cerr<<i<<": w1 EMPTY"<<endl;
			}
		if(word2[i][0] == '\0'){
		 distance[i] = -1;
		 cerr<<i<<": w2 EMPTY"<<endl;
		}
	}

	//delete all duplicate rules as defined by spec
	for(int i = 0; i<nRules; i++){
		for(int j = 0; j<i; j++){
			//cerr<<"4"<<endl;
			if((strcmp(word1[i], word1[j]) == 0) && (strcmp(word2[i], word2[j]) == 0)){
				if(distance[i]>=distance[j]){
					distance[j] = -1;
					cerr<<j<<": OVERWRITTEN BY SAME RULE OF GREATER DISTANCE: "<<i<<endl;
				}else{
					cerr<<i<<": OVERWRITTEN BY SAME RULE OF GREATER DISTANCE: "<<j<<endl;
					distance[i] = -1;
				}
			}
			if((strcmp(word1[i], word2[j]) == 0) && (strcmp(word2[i], word1[j]) == 0)){
				if(distance[i]>=distance[j]){
					cerr<<j<<": OVERWRITTEN BY SAME RULE OF GREATER DISTANCE: "<<i<<endl;
					distance[j] = -1;
				}else{
					cerr<<i<<": OVERWRITTEN BY SAME RULE OF GREATER DISTANCE: "<<j<<endl;
					distance[i] = -1;
				}
			}
		}
	}


	//delete all rules with negative distance variables
	for(int i = 0; i<nRules; i++){
		//cerr<<"5"<<endl;
		while(i<nRules && distance[i] < 0){
			cerr<<"DELETING: "<<i;
		 	rotateLeft(word1, nRules, i);
		 	rotateLeft(word2, nRules, i);
		 	rotateLeft2(distance, nRules, i);
		 	nRules--;
		}
	}


	cerr<< "AFTER FIX:::::::::::::::"<<endl;
	for(int i = 0; i<nRules; i++){
		cerr<<i<<":  "<<word1[i]<<", "<<word2[i]<<", "<<distance[i]<<endl;
	}


	return nRules;
}


int readNext(const char document[], int i, char s[]){
	//cerr<< "in readNext"<<endl;
	int indx = 0;
	while(document[i] != '\0' && document[i] == ' ') i++;
	while(document[i] != '\0' && document[i] != ' '){
		//cerr<<"1: "<<i<<endl;
		s[indx] = document[i];
		indx++;
		i++;
	}
	s[indx] = '\0';
	return i;
}

// find char array s in char matrix word1
int findWord(const char word1[][MAX_WORD_LENGTH+1], char s[], int n){
	//cerr<< "in findWord "<<s<<endl;
	for(int i = 0; i<n; i++){
		//cerr<<"word[i]: "<<word1[i]<<endl;
		if(strcmp(word1[i], s)==0){
			//cerr<<" returns: "<<i<<endl;
			return i;
		}
	}
	//cerr<<" returns: "<<-1<<endl;
	return -1;
}

//returns 1 if the rule (w1, w2, dist) is in the document 
//described by the word array words of size wordCount
int searchDoc(char words[][MAX_DOC_LENGTH+1], char w1[], char w2[], int dist, int wordCount){
	cerr<< "in searchDoc( "<<w1<<", "<<w2<<", "<<dist<<", "<<wordCount<<")"<<endl;
	int min, max;

	for(int pos = 0; pos < wordCount; pos++){
		//check for all words if they equal w1
		if(strcmp(words[pos], w1) == 0){
			//define limits within which we will search for w2
			min = pos - dist;
			max = pos + dist;
			if(min<0) min = 0;
			if(max>wordCount-1) max = wordCount-1;
			cerr<<"min/max: "<<min<<", "<<max<<endl;
			//check for all words within dist if they match w2
			for(int i = min; i<=max; i++){
			//cerr<<words[i]<<endl;
			if(i!=pos){
			if(strcmp(words[i], w2)==0){
				cerr<<"FOUND: "<<i<<endl<<endl;
				//if match found
				return 1;
			}
	}
	}
	}
}
	cerr<<"Didn't find anything"<<endl<<endl;
	//if nothing is found
	return 0;
}

//put the char in position pos at end of char array a
int rotateLeft1(char a[], int n, int pos){
	//cerr<< "rotateLeft1("<<n<<", "<<pos<<")"<<endl;
	//check if input is valid
	if(n<0 || pos>n-1 || pos<0){
		cerr<<"    Wrong input!"<<endl;
		cerr<<"    returns: -1"<<endl;
	 	return -1;
	}
	if(n==0){
		return 0;
	}


	char temp = a[pos];	//store value that needs to be put on end
	
	//loop through, rotate left
	for(int i = pos; i < n-1; i++){
		a[i] = a[i+1];
	}
	a[n-1] = temp;	//put temp on last position

	//return what needs to be returned
	return pos;
}

//remove all redundant characters from document and put all alpha to lowercase
void correctDocument(char document[]){
	for(int i = 0; i<strlen(document); i++){
			document[i] = tolower(document[i]);	//set to lower case
			//if neither alpha nor space then remove
			while(!isalpha(document[i]) && document[i]!=' ' && i<strlen(document)){
				rotateLeft1(document, strlen(document), i);
				document[strlen(document)-1] = '\0';
			}
	}
}
int calculateSatisfaction(const char word1k[][MAX_WORD_LENGTH+1],
                          const char word2k[][MAX_WORD_LENGTH+1],
                          const int distancek[],
                          int nRules,
                          const char documentk[]){
	cerr<<"-----------------------------------------------------------"<<endl;
	cerr<< "in calculateSatisfaction:"<<endl;
	cerr<< "document: "<< documentk<<endl;
	cerr<<"word1: "<<endl;
	for(int i = 0; i<nRules; i++){
		cerr<<"    "<<word1k[i]<<endl;
	}
	cerr<<"word2: "<<endl;
	for(int i = 0; i<nRules; i++){
		cerr<<"    "<<word2k[i]<<endl;
	}
	cerr<<"-------"<<endl;
	char word1[MAX_WORD_LENGTH+1][MAX_WORD_LENGTH+1];
	char word2[MAX_WORD_LENGTH+1][MAX_WORD_LENGTH+1];
	int distance[MAX_WORD_LENGTH+1];
	for(int i = 0; i<nRules; i++){
		strcpy(word1[i], word1k[i]);
		strcpy(word2[i], word2k[i]);
		distance[i] = distancek[i];
	}
	char document[MAX_DOC_LENGTH+1];
	strcpy(document, documentk);
	correctDocument(document);
	cerr<< "Corrected doc: "<< document <<endl;
	nRules = normalizeRules(word1, word2, distance, nRules);
	/*(cerr<<"word1:"<<endl;
	for(int i = 0; i<nRules; i++){
		cerr<< "    "<<word1[i]<<endl;
	}
	cerr<<"word2:"<<endl;
	for(int i = 0; i<nRules; i++){
		cerr<< "    "<<word2[i]<<endl;
	}*/
	char s[MAX_DOC_LENGTH+1];
	char temp[MAX_DOC_LENGTH+1];
	int i = 0;
	int satisfaction = 0;
	int wordCount = 0;
	char words[MAX_DOC_LENGTH+1][MAX_DOC_LENGTH+1];

	//read all words within document into an array
	while(document[i]!='\0'){
		//cerr<<"Yo4"<<endl;
		i = readNext(document, i, s);
		
		strcpy(words[wordCount], s);
		//cerr<<"----s:"<<s<<"----"<<endl;
		//cerr<<"----"<<words[wordCount]<<"----"<<endl;
		wordCount++;
	}
	cerr<< "Word count: "<<wordCount<<endl<<endl;
	//cerr<<"!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	//for(i = 0; i<wordCount; i++)
		//cerr<<words[i]<<" ";
	cerr<<endl;

	//go through all rules and check if they are satisfied by the document
	for(i = 0; i<nRules; i++){
			satisfaction += searchDoc(words, word1[i], word2[i], distance[i], wordCount);
	}
	cerr<<"Satisfaction: "<<satisfaction<<endl;
	return satisfaction;
}
int main()
        {
        	char doc[] = "The mad ..... . UCLA scientist unleashed a deranged evil giant robot.";
        	correctDocument(doc);
        	const int TEST1_NRULES = 4;
            char test1w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "mad",       "deranged", "nefarious", "have"
            };
            char test1w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "scientist", "robot",    "plot",      "mad"
            };
            int test1dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "The mad UCLA scientist unleashed a deranged evil giant robot.") == 2);
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "The mad UCLA scientist unleashed    a deranged robot.") == 2);
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "**** 2016 ****") == 0);
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "  That plot: NEFARIOUS!") == 1);
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "deranged deranged robot deranged robot robot") == 1);
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "That scientist said two mad scientists suffer from deranged-robot fever.") == 0);
            
            char test2w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "a", "nefarious", "have"
            };

            char test2w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "b", "nefarious", "have"
            };
            int test2dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test2w1, test2w2, test2dist, TEST1_NRULES,
                "a b c d e f g h i j k l m n") == 1);


            char test3w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test3w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test3dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test3w1, test3w2, test3dist, TEST1_NRULES,
                "a b c d e f g h i j k l m n") == 1);

            char test4w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test4w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test4dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test4w1, test4w2, test4dist, TEST1_NRULES,
                "a b c d e f g h i j k l m n") == 1);

            char test5w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test5w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test5dist[TEST1_NRULES] = {
                -5,           4,          1,           13
            };
            assert(calculateSatisfaction(test5w1, test5w2, test5dist, TEST1_NRULES,
                "a b c d e f g h i j k l m n") == 1);



            char test6w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "B",       ".A.", "nefarious", "have"
            };

            char test6w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a.. $",       "B!", "nef#@arious", "have"
            };
            int test6dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test6w1, test6w2, test6dist, TEST1_NRULES,
                "A b c d e f g h i j k l m n") == 0);


            char test7w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "",       "a", "nefarious", ""
            };

            char test7w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "", ""
            };
            int test7dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test7w1, test7w2, test7dist, TEST1_NRULES,
                "a b c d e f g h i j k l m n") == 1);



            char test8w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test8w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a.!. :-D",       "a", "nefarious", "have"
            };
            int test8dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test8w1, test8w2, test8dist, TEST1_NRULES,
                "a b c d e f g h i j k l m n") == 0);




            char test9w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "a", "nefarious", "have"
            };

            char test9w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test9dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test9w1, test9w2, test9dist, TEST1_NRULES,
                "a b c d e f g h i j k l m n") == 1);



            char test10w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "b", "nefarious", "have"
            };

            char test10w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "a", "nefarious", "have"
            };
            int test10dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test10w1, test10w2, test10dist, TEST1_NRULES,
                "a b a a a f g h i j k l m n") == 1);





            char test11w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test11w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test11dist[TEST1_NRULES] = {
                2,           2,          1,           13
            };
            assert(calculateSatisfaction(test11w1, test11w2, test11dist, TEST1_NRULES,
                "a b c d e a b h i j k l m n") == 1);



            char test12w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test12w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test12dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test12w1, test12w2, test12dist, TEST1_NRULES,
                "a b c d e f a b i j a b m n") == 1);



            char test13w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "aaaaaaaaaaaaaaaaaaaa"
            };

            char test13w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "i"
            };
            int test13dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test13w1, test13w2, test13dist, TEST1_NRULES,
                "a b c d e f g h aaaaaaaaaaaaaaaaaaaa i j k l m n") == 2);



            char test14w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test14w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test14dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test14w1, test14w2, test14dist, 0,
                "a b c d e f g h i j k l m n") == 0);


            char test15w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test15w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test15dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test15w1, test15w2, test15dist, -5,
                "a b c d e f g h i j k l m n") == 0);



            char test16w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test16w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test16dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test16w1, test16w2, test16dist, TEST1_NRULES,
                "a b c d e f g h i j k l m n aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") == 1);


            char test17w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test17w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test17dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test17w1, test17w2, test17dist, TEST1_NRULES,
                "a  b  c d   e f g h    i j k    l m n") == 1);

            


            char test18w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test18w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test18dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test18w1, test18w2, test18dist, TEST1_NRULES,
                "a  <=>b  c???!??? d   e f g5318008 h .   i j k..    l m n 8-)  =3") == 1);
           
            char test19w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test19w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test19dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test19w1, test19w2, test19dist, TEST1_NRULES,
                "a  b  c d   e f g h    i j k    l m n") == 1);


            char test20w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test20w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test20dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test20w1, test20w2, test20dist, TEST1_NRULES,
                "UCLABCDEFGHIJKLMNOPQRSTUVWXYZ") == 0);



            char test21w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "b",       "a", "nefarious", "have"
            };

            char test21w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "a",       "b", "nefarious", "have"
            };
            int test21dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            assert(calculateSatisfaction(test21w1, test21w2, test21dist, TEST1_NRULES,
                "   a  b  c d   e f g h    i j k    l m n   ") == 1);

            cout << "All tests succeeded" << endl;
            /*const int TEST1_NRULES = 4;
            char test1w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "mad",       "deranged", "nefarious", "have"
            };
            char test1w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "scientist", "robot",    "plot",      "mad"
            };
            int test1dist[TEST1_NRULES] = {
                2,           4,          1,           13
            };
            int k = normalizeRules(test1w1, test1w2, test1dist, TEST1_NRULES);
            cerr<<"Result: "<< k<< endl;*/
        }