# CS31-Project3

Nikola Samardzic

	•	As always, the biggest obstacle with these projects is finding correct test cases and making sure the output fits the spec.
	•	Functions and explanations: 	1. isValidUppercaseStateCode() - Checks if the two character parameter is a valid state code.
		2. getForecast() - updates the voteCount integer by parsing a state forecast code. This is done by first checking the state code(first two characters) against the isValidUppercaseStateCode() function. Then, the function reads the following block of digits and saves them in a variable. If the party code matches with the party code parameter the voteCount is increased.

		3. isValidForecast() - checks if specific state forecast code is correctly formatted. This is done by first checking the state code(first two characters) against the isValidUppercaseStateCode() function. Then, the function reads the following block of digits. If there are no digits after the state code, the function returns false. If the function reads a block of digits, then it check if there is only one letter left in the state forecast code (as it should be) representing the party code.

		4. hasCorrectSyntax() - checks if the specified poll data is correctly formatted. This function basically just divides the poll data code into separate state forecast codes and sends these to the isValidForecast() function for further inspection.

		5. countVotes() - goes through the poll data and counts the votes for the specified party. This function basically just divides the poll data code into separate state forecast codes and sends these to the getForecast() function that updates the voteCount value for each state forecast code. Also, we check if any state forecast is equal to zero, as the output is suppose to change for that case.
		
Pseudocode: 		countVotes(pollData, partyCode, voteCount) 		{
		Set all alpha characters to uppercase to avoid confussion; 			If(pollData syntax incorrect) return 1;
		for(all chars in pollData from first to last){
	Int test (stores the next forecast code);
	Add next char block to test;
	Add next digit block to test;
	Add one extra char to test which represents party code; 		Get the forecast value, iterate Count accordingly, if not getForecast() return 3;
}
If the party code is incorrect return 3;
Set the actual voteCount variable to appropriate value;
return 0;
}

	(c) Comprehensive test cases:
	•	Correct input with some votes being single digits, some triple,... all types of mixed upper and lower cases. Most importantly, different letter case for the party code that is being checked.
	•	Incorrect input (CHAR means any string without digits, <DIGITS> means a string with only digits as characters). Since checking string correctness is mostly the job of the isValidForecast() function we will divide test cases by all the possible parameter formats sent to that function rather than looking at the poll data:
	•	<CHAR1><DIGITS><CHAR2>
						I) <CHAR1> has more than two characters.
						II) <CHAR1> is an invalid state code, but is two characters long.
						III) <CHAR1> has less than two characters.
				      b.	<CHAR>
				      c. <DIGITS> 				      e. <DIGITS><CHAR> 				      f. Assuming proper functioning of the hasCorrectFunction() (Tested under 3) these are all possible test cases. 	3. Checking if hasCorrectFunction() sends proper param formats to isValidForecast(), given incorrect poll data code: 		This function should send to isValidForecast() a string consisting of 2 “new” characters the block of digits (if exists) after the 2 characters, and an additional character and return false if it is unable to do that before reaching the end of the string. Also it should return false whenever isValidForecast() returns false.
	•	isValidForecast() returns false.
	•	Reaches end of poll data before parsing the first char of the new state forecast code.
	•	Reaches end of poll data before parsing the second char of the new state forecast code.
	•	No digits after second char of new state forecast code.
	•	Reaches end of poll data after parsing the second char of the new state forecast code.
	•	Reaches end of poll data before encountering the end of the block of digits.
	•	Empty string.
 	  4. Checking if countVotes() notices all the incorrect inputs: 		      a. Electoral vote number equal to 0 for some state. 		      b. Incorrect poll data string. 		      d. Party not a letter. 		      e. Any and all combinations of a, b, c. 	5. Checking if isValidUppercaseStateCode() works:
		     a. Send all possible two char combos and check correctness of output.
