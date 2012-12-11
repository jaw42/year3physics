//**********************************************************************************
// Author: Josh Wainwright
// Date: 11/10/2012
// Title: Numerical Integration
//
// Description:
//**********************************************************************************
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

int main (void){
	double e, s, sum, rad;

	ofstream data("data.txt");

	for(int x=0;x<=180;x++){
		rad = x*M_PI/180.0;
		e = exp(-rad);
		s = sin(rad);
		sum = e*s;
		cout << rad << " " << sum << endl;
		data << rad << " " << sum << endl;
	}
	data.close();

	double a=0, b=2, total, y, area;
	int N, n;
	cout << "The number of discrete panels, N: ";
	cin >> N;

	area += (exp(-y)*sin(y));
	for(n=1;n<=N;n++){
		y = b * n/N;
		area += 2*(exp(-y)*sin(y));
	}

	total = (b-a)/(2*N) * area;
	cout << total << endl;

	return 0;
}

