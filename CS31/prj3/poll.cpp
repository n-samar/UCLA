
//CORRRECT
#include <iostream>
#include <cassert>
#include <string>
#include <cstring>
#include <cctype>

using namespace std;

//*************************************
//  isValidUppercaseStateCode
//*************************************

// Return true if the argument is a two-uppercase-letter state code, or
// false otherwise.

bool isValidUppercaseStateCode(string stateCode)
{
	//cerr << "in isValidUppercaseStateCode("<<stateCode<<")"<<endl;
    const string codes =
	"AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
        "KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."
        "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2  &&
	    stateCode.find('.') == string::npos  &&  // no '.' in stateCode
	    codes.find(stateCode) != string::npos);  // match found
}

//Returns the number of recieved votes from a speficied state by a specified party
	//identical to isValidForecast(), just updates voteCount and assumes code correctness
	bool getForecast(string forecast, int &voteCount, char partyCode){
		cerr << "in getForecast("<<forecast<<")";
			int k = 2;
			int cVote = 0;
	    		while(k<forecast.size() && isdigit(forecast[k])){
	    			cVote*=10;
	    			cVote+=(forecast[k] - '0');
	    			k++;
	    		}
	    	if(partyCode == forecast[forecast.size()-1]) voteCount+=cVote;
	    	cerr << " --> "<<cVote<<endl;
	    	if(cVote==0) return false;
	    	return true;
	}

// Checks if the specific state forecast is correctly formated

	bool isValidForecast(string forecast){
		cerr << "in isValidForecast("<<forecast<<")"<<endl;
			int k = 0;
			char secondL, firstL = forecast[k];	//will hold state code
	    	if(k+1 >= forecast.size()){
	    		return false;
	    	}
	    		secondL = forecast[k+1];
	    		string stateTest = "";
	    		stateTest += firstL;
	    		stateTest += secondL;
	    		if(!isValidUppercaseStateCode(stateTest))
	    			return false;	//check if state code is valid
	    		cerr << "    valid state code: "<< stateTest <<endl;
	    		k++; k++;
	    		bool hasVotes = false;
	    		string voteCount = "";

	    		//iterate until you hit a non-digit and save the digits as voteCount
	    		while(k<forecast.size() && isdigit(forecast[k])){
	    			
	    			voteCount+=forecast[k];
	    			hasVotes = true;
	    			k++;
	    		}
	    		cerr << "    valid vote count: "<<voteCount<<endl;
	    		if(!hasVotes) return false;	//if there were no digits after state code
	    		if(k+1 != forecast.size()) return false; //if forecast code is correct, it should have one extra letter left (party code)
	    		if(!isalpha(forecast[k])) return false;
	    		cerr << "    valid party code: "<<forecast[k]<<endl;
	    		return true;
	}

// Checks if poll code is syntactically correct

	bool hasCorrectSyntax(string pollData)
	{
		cerr << "in hasCorrectSyntax("<< pollData<<")"<<endl;
		if(pollData.size()==0) return false;
		for(int k = 0; k < pollData.size(); k++){
			pollData[k] = toupper(pollData[k]);	//Convert everything to upper case to avoid confussion.
		}
	    for(int k = 0; k < pollData.size(); k++){

	    	//exact same logic as count votes, but checks data instead of counting votes
	    	string test = "";
	    	while(k<pollData.size() && isalpha(pollData[k])){
	    		test+=pollData[k];
	    		k++;
	    	}
	    	while((k<pollData.size()) && isdigit(pollData[k])){
	    		test+=pollData[k];
	    		k++;
	    	}
	    	if(k<pollData.size())
	    		test+=pollData[k];
	    	if(!isValidForecast(test)) return false;
	    }
	    return true;
	}

//Counts the number of votes given specific poll data and party

	int countVotes(string pollData, char party, int& voteCount1)
	{

		
		for(int k = 0; k < pollData.size(); k++){
			pollData[k] = toupper(pollData[k]);	//Convert everything to upper case to avoid confussion.
		}
		if(!hasCorrectSyntax(pollData)) return 1;  //Check if data is well formated
		party = toupper(party);	//To upper case just in case
		
		int voteCount = 0;	//just in case
		cerr << "in countVotes("<< pollData<<")"<<endl;
	    for(int k = 0; k < pollData.size(); k++){
	    	string test = "";

	    	//All of this in the loop just iterates k until it hits the first character of the next state forecast code
	    	//and then adds the forecast to the voteCount through getForecast()
	    	while(isalpha(pollData[k])){
	    		test+=pollData[k];
	    		k++;
	    	}
	    	while(isdigit(pollData[k])){
	    		test+=pollData[k];
	    		k++;
	    	}
	    		test+=pollData[k];
	    	if(!getForecast(test, voteCount, party)) return 2;	//Check if number of votes is 0
	    }
	    if(!isalpha(party)) return 3;	//Check if the party char is a letter
	    voteCount1 = voteCount;
	    return 0;
	}

	int main()
	{
		/*
	    assert(hasCorrectSyntax("TX38RCA55D"));
	    assert(!hasCorrectSyntax("MX38RCA55D"));
	    int votes;
	    votes = -999;    // so we can detect whether countVotes sets votes
	    assert(countVotes("TX38RCA55DMs6rnY29dUT06L", 'd', votes) == 0  &&  votes == 84);
	    votes = -999;    // so we can detect whether countVotes sets votes
	    assert(countVotes("TX38RCA55D", '%', votes) == 3  &&  votes == -999);
	    
 		cout << "All tests succeeded" << endl;
	    */
	    string s;
	    s="ny29dtx38+hi4d";
	    int voteCount;
	    //cout<<isValidForecast("CA0d")<<endl;
	   	cout << countVotes("ny29dtx38rhi4d", 'd', voteCount)<<endl;
	}
