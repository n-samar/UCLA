# CS31-Project5

Report

	•	Debugging was the biggest obstacle of this project
	•	Pseudocode: 	normalizeRules(word1, word2, distance, nRules){ 		if(nRules negative) nRules = 0;
for all the rules: 	for all the characters in w1: 		if char not alpha set distance = -1 		Put all chars into lowercase  	for all the characters in w2: 		if char not alpha set distance = -1 		Put all chars into lowercase for all the rules: 	if w1 is empty set distance = -1 for all the rules: 	if w2 is empty set distance = -1 	Put all chars into lowercase
for all the rules:
	for all the rules that come before it: 		if  the two rules have the same {w1, w2}: 			set the distance of the one with the smaller distance to -1  for all rules:
	If the distance for the rule is negative: 		remove the rule 		nRules--  return nRules
}

//returns 1 if the rule (w1, w2, dist) is in the document //described by the word array words of size wordCount
int searchDoc(words, w1, w2, dist, wordCount){
	for all words in words:
		if the word matches w1: 				for all the words within dist of that word: 					if the word matches w2:
					return 1 		return 0;
} 
int calculateSatisfaction(word1[], word2[], distance[], document){
words; //will store all words in document in array
counter; //counts # of rules satisfied by document
while(not last char of document){
	Read next word into words;
}
for all the rules{
	Iterate counter if document satisfies the rule
}
return counter;
}
	•	Test cases:
	•	A rule with same two words and non-zero distance
	•	A rule with same two words and zero distance
	•	Two rules have same words but the first words match the second words.
	•	Set of rules with a rule with a negative distance
	•	Rule with uppercase letters
	•	Rule with non-alpha characters
	•	Empty word in rule
	•	Word with only non-alpha characters
	•	Document with two different ways of satisfying a rule, with the first word in both ways being the same, yet the position of second word is different
	•	Document with two different ways of satisfying a rule, with the first word in both ways being the same, yet the position of second word is different. Also, the distance in both rules is the same.
	•	 Document with two different ways of satisfying a rule, with one way out of the distance range of the second way
	•	 Document with two different ways of satisfying a rule, with one way out of the distance range of the second way. Also, the distance in both rules is the same.
	•	A word of length 20 in a rule
	•	Zero rules in rule set
	•	A set of rules with all rules not in normal form
	•	Negative nRules value
	•	Document of size equal to 200
	•	Words in document separated with more than one space
	•	Document with non-alpha characters
	•	Document with non-alpha characters, such that a block of non-alpha characters is between spaces.
	•	Last character is non-alpha in document
	•	Document with alpha words longer than 20 characters
	•	Document with only one long word
	•	Document with space as first character.
	•	Document with spaces at end

Corresponding asserts:

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
                -1,           4,          1,           13
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


