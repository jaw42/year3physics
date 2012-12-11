//**********************************************************************************
// Author: Josh Wainwright
// Date: 04/10/2012
// Title: Know Your Computer's Limitations
//
// Description: Calculate the value of the Silver Ratio using direct multiplication
//				and iteration using the recurance relation.
//**********************************************************************************
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>

using namespace std;

int main (void){

	// Define the initial value for the silver ratio and phi.
	double phin, doublephin=0, doublephii = (-1 + sqrt(5)) / 2;
	float floatphii = (-1 + sqrt(5)) / 2;
	int number, i;

	// Define the output file to be written to. Use comma separated values file for easy use when creating graphs etc.
	ofstream out ("output_FD.csv");

	cout << "Input number, n, for phi^n (~50): ";
	cin >> number;
	double doublephi[number+1];
	float floatphi[number+1];

	// Specify the inital two values for the recursion relation in both float and double.
	doublephi[0] = 1;
	floatphi[0] = 1;
	doublephi[1] = doublephii;
	floatphi[1] = floatphii;

	doublephin = pow(doublephii,number);

	cout << "The answer to the silver ratio to the power of " << number << " is " << doublephin << endl;

	cout <<	endl << "  n |  Double Phi^n  |  Float Phi^n  |  Direct Multiplication" << endl;
	cout << "----|----------------|---------------|------------------------" << endl;

	// Calculate the recusion relation for each value 0-50
	for(i=1;i<=number;i++){
		doublephi[i+1] = doublephi[i-1] - doublephi[i];
		floatphi[i+1] = floatphi[i-1] - floatphi[i];
		phin = pow(doublephii,i);

		// write to screen and file
		out << i << "," << doublephi[i] << "," << floatphi[i] << "," << phin<< endl;
		cout << setw(3) << i << " |" << setw(15) << doublephi[i] << " | " << setw(13) << floatphi[i] << " |" << setw(17) << phin << endl;
	}
	cout << endl << "The file output_FD.csv has been written." << endl;

	// close the file stream
	out.close();
	return 0;
}

