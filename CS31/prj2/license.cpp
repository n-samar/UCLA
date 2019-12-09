#include <iostream>
#include <string>

using namespace std;

int main(){
	string id, country;
	double revenue;
	
	//Reading data
	
	cout << "Identification: ";
	getline(cin, id);
	cout << "Expected revenue (in millions): ";
	cin >> revenue;
	cin.ignore(1000, '\n');
	cout << "Country: ";
	getline(cin, country);
	cout << "---" << endl;
	
	//Checking if logical bounds are satisfied
	
	if(id.empty()){
		cout << "You must enter a property identification." << endl;
	}else if(revenue < 0){
		cout << "The expected revenue must be nonnegative." << endl;
	}else if(country.empty()){
		cout << "You must enter a country." << endl;
	}else{
	
		// This branch for correct input
	
		double fee = 0;
		if(revenue > 50){
			fee += (revenue-50)*.23;
			revenue = 50;
		}
		if(revenue > 20 ){
			if(country.compare("Turkey") == 0 || country.compare("UAE") == 0){
				fee += (revenue-20)*0.217;
			}else{
				fee += (revenue-20)*0.203;
			}revenue = 20;
		}
		fee += revenue*0.181;
	
		//Printing and formatting
	
		cout.setf(ios::fixed);
		cout.precision(3);
		cout << "The license fee for "<< id << " " << country << " is $" << fee << " million.";
	}
}
