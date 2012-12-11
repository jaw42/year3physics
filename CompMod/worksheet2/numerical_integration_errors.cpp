//**********************************************************************************
// Author: Josh Wainwright
// Date: 11/10/2012
// Title: Numerical Integration
//
// Description: Use the Trapezium and Simpson's Rules to calculate the value of the 
//				integral of a function numerically.
//**********************************************************************************
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

// Function for calculating e^(-x)sin(x), used throughout
double y(double x){
	return exp(-x) * sin(x);
}

int main (void){
	while(true){
		int option=0;
		cout << "Choose an option\n (1) Generate data file for e^(-x)sin(x)\n (2) Trapezium Rule\n (3) Simpson's Rule\n (0) Exit" << endl; 
		cin >> option;
		
		if(option == 1){
			
		// DRAW FUNCTION -------------------------------------------------------------
			ofstream data("data.txt");

			// Iterate from 0 to multiple of pi writing to screen and file. Return to pi within loop
			for(double x=0;x<=M_PI*1000;x++){
			//	cout << x/1000 << " " << y(x/1000) << endl;
				data << x/1000 << " " << y(x/1000) << endl;
			}
			data.close();
			cout << "The data file, data.txt, has been written.\n\n";
		}	
		else if(option == 2){
		// TRAPEZIUM RULE -------------------------------------------------------------
			double a=0, b=2, total=0, x, area, error=0;
			int N, n;
			cout << "\nUsing the trapezium method with N panels the area is calculated to be: \n";
			// Iterate through different numbers of panels to see change in error.
			for(N=100;N<=1000;N+=100){
				area = 0;
				area += y(x);
				cout << "For " << N << " panels: ";
				// Perform the trapezium rule for each separate number of panels.
				for(n=1;n<=N;n++){
					x = b * n/N;
					area += 2*y(x);
				}
				total = (b-a)/(2*N) * area;

				//calculate difference between value and analytically calculated result
				error = abs(1 - total / 0.46662966259);
				cout << "Integral: " << setw(10) << total << ",   error: " << setw(10) << error << endl;
			}
			cout << endl;
		}
		// SIMPSON'S RULE -------------------------------------------------------------
		else if(option == 3){
			double h, a=0, b=2, simpson, n=1000000, S, nearest, decimal1, decimal2, dec;
			int i, dec1;
		
			cout << "Choose degree of accuracy (number of decimal places)\n:";
			cin >> dec;
			
			// Perform the loop until the initial and final values of the area, decimal1 and decimal2 are the same.
			do{
				simpson=0; i=0;

				// Define initial value for comparison
				decimal1 = nearest;

				//define the width of the panel, as in trapezium rule
				h = (b-a)/n;

				// Perform the loop that will sum each of the panels
				for(i=1;i<n-2;i++){
					simpson += 4*y(i*h);
					i++;
					simpson += 2*y(i*h);
				}

				// Also add the final values which do not follow the pattern.
				i++;
				simpson += 4*y(i*h);
				i++;
				simpson+= y(i*h);

				S = (b-a) * simpson/(3*n);

				// Round the value calculated to the number of decimal places as specified by the user.
				nearest = floor(S * pow(10.0,dec) + 0.5)/pow(10.0,dec);
				decimal2 = nearest;
				n+=1;
				dec1 = (int) dec;
			}while(decimal1 != decimal2);
			cout << "The integral is calculated to be: " << showpoint << setprecision(dec1) << S << endl << endl;

		} 
		else if (option == 0){
			// Exit the program.
			return 0;
		}
		else {
			// Error in option selection.
			cout << "Option not recognised.\n";
		}
	}
	return 0;
}
