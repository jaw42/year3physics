#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <cstring>

#define SIZE 200
#define LOSS 0.001

using namespace std;

double ex[SIZE], hy[SIZE], ceze[SIZE], cezh[SIZE], imp0=2.0, absorbingcoef;
int qTime, maxTime=210, mm, frame=0, startcord=0, function_choice=2, 
	infiniteplane=2;
string option="n", temp, func;
bool nocombine=true;

// PROTOTYPES ------------------------------------------------------- PROTOTYPES
string intToString(int t);
int StringToInt(string t);
void calculatefields(int startcord);
void GNU();
void infopane();

// MAIN ------------------------------------------------------------------- MAIN
int main(){
	while(true){
		string menu;
		infopane();
		cout << "What would you like to do?\n";
		cout << "\t(1) Recalculate the fields and animate,\n"
				"\t(2) Change the seeding point and number of iterations,\n"
				"\t(3) Choose the seeding function,\n"
				"\t(4) Define the type of system (infinite, bounded etc.)\n"
				"\t(v) View the animation,\n"
				"\t(0) Exit.\n";
		getline(cin,menu);
		if(menu=="1"){ //------------------------------------------------------1
			calculatefields(startcord);
			GNU();

		}else if(menu=="2"){ //------------------------------------------------2
			do{
				cout << "Choose the starting point for the wave "
							"(between 0 and 200): ";
				getline(cin,temp);
				startcord = StringToInt(temp);
			}while( (startcord < 0) || (startcord >= 200) );
			do{
				cout << "Choose the number of iterations "
							"(advised 200 to 2000): ";
				getline(cin,temp);
				maxTime = StringToInt(temp);
			}while(maxTime < 0);

		}else if(menu=="3"){ //------------------------------------------------3
			cout << "Choose the function to seed the waves:\n"
					"\t(1) Gaussian,\n"
					"\t(2) Sine,\n"
					"\t(3) Cosine,\n";
			getline(cin,temp);
			function_choice = StringToInt(temp);

		}else if(menu=="4"){ //------------------------------------------------4
			cout << "Define type of system: \n"
					"\t(1) Bounded at 0 and 200,\n"
					"\t(2) Infinite system to -inf and +inf\n"
					"\t(3) Absorbing\n";
			cin >> infiniteplane;
			if(infiniteplane==3){
				cout << "Enter the absorbing coefficient, [0,1]: ";
				cin >> absorbingcoef;
			}

		}else if(menu=="v"){ //------------------------------------------------v
			GNU();

		}else if(menu=="9"){ //------------------------------------------------9
			system("rm /tmp/files/data*");
		}else if(menu=="12"){
			char c;
			puts("Enter text here: ");
				c = getchar();

			cout << c << endl;

		}else if(menu=="0" || menu=="q"){ //-----------------------------------0
			system("rm /tmp/fdtd/data*");
			return 0;

		} else{ //--------------------------------------------------------------
			cout << "Option not recognised.\n";
		}
	}
}

// FUNCTIONS --------------------------------------------------------- FUNCTIONS
string intToString(int integer){ //---------------------------------intoString()
	string ch;
	ostringstream outs; 
	outs << integer;   // Convert value into a string.
	ch = outs.str();   
	return ch;
}

int StringToInt(string ch){ //-------------------------------------StringtoInt()
	int integer;
	stringstream(ch) >> integer;
	return integer;
}

void calculatefields(int startcord){ //------------------------calculatefields()
	system("rm /tmp/fdtd/data*; mkdir -p /tmp/fdtd/");
	cout << "Calculating frame:\n";
	frame=0;
	// do time stepping 
	if(nocombine){
		for (mm = 0; mm < SIZE - 1; mm++){
			ex[mm] = 0;
			hy[mm] = 0;
		}
	}

	for(mm = 0; mm < SIZE; mm++){
		if(mm<200){
			ceze[mm] = 1.0;
			cezh[mm] = imp0;
		}else{
			ceze[mm] = (1.0 - LOSS) / (1.0 + LOSS);
			cezh[mm] = imp0 / 9.0 / (1.0 + LOSS);
		}
	}

	for (qTime = 0; qTime < maxTime; qTime++) {

		// update magnetic field 
		if(infiniteplane==2){
			hy[SIZE-1] = hy[SIZE-2];
		}
		for (mm = 0; mm < SIZE - 1; mm++){
			hy[mm] = hy[mm] + (ex[mm + 1] - ex[mm]) / imp0;
		}

		hy[startcord-1] -= exp(-((qTime+0.5) * 100.) * ((qTime+0.5) * 100.)/100.0) / imp0;

		// update electric field 
		if(infiniteplane==2){
			ex[0] = ex[1];
		}
		for (mm = 1; mm < SIZE; mm++){
			ex[mm] = ceze[mm] * ex[mm] + (hy[mm] - hy[mm - 1]) * cezh[mm];
		}

		// Choose the type of function to be modelled.
		if(function_choice==1){
			ex[startcord] += exp(-(qTime - 100.) * (qTime - 100.) / 100.);
			// ex[startcord] += exp(-(qTime + 0.5 - (-0.5) -30.0) * (qTime + 0.5 - (-0.5) -30.0) / 100.0);
		}
		else if(function_choice==2){
			// ex[startcord] += sin(sqrt(qTime/2.0))/20;
			ex[startcord] = sin(qTime*10)/2;
		}
		else if(function_choice==3){
			ex[startcord] = cos(sqrt(qTime))/2;
		}
		else if(function_choice==4){}
		// ex[50] += qTime/10.0;
		// qTime is int so dividing it by anything performs an integer division
		// ex[0] += sin(sqrt(qTime/2.0))/20;

		// string name = "files/data" + intToString(frame) + ".txt";  
		string name = "/tmp/fdtd/data" + intToString(frame) + ".txt";  
		ofstream write (name.c_str());

		if(frame%50==0){
			cout << endl << setw(4) << frame;
		}
		cout << ".";

		for(int x=0; x<SIZE; x++){
			write << x << " " << ex[x] << " " << hy[x] << endl;
		}
		frame++;
		write.close();
	} // end of time-stepping
}

void GNU(){ //-------------------------------------------------------------GNU()
	FILE *gnuplotPipe = popen("gnuplot -persist","w");  
	cout << "\nGraphing...\n";
	
	// If gnuplot is found  
	if (gnuplotPipe) {  
		fprintf(gnuplotPipe,"set xrange[0:200] \nset yrange[-1:1] \n");  
		fprintf(gnuplotPipe,"set xlabel \"x\" \n set ylabel \"Electric Field\" \n");  
		fprintf(gnuplotPipe,"set sample 1001\n");  
		
		system("ls -l /tmp/fdtd |wc -l > temp");
		ifstream in("temp");
		in >> frame;
		system("rm temp");

		//This is the main loop that updates the graph output
		for (int x=0; x<frame-1; x++){
			// fprintf(gnuplotPipe, "%s%d%s%d%s",
			// 		"plot x > 100 ? -1 : 1 with filledcurve y1=100, \"/tmp/fdtd/data", x, 
			// 		".txt\" using 1:2 w l, \"/tmp/fdtd/data", x, 
			// 		".txt\" using 1:3 w l\n ");
			fprintf(gnuplotPipe, "%s%d%s%d%s",
					"plot \"/tmp/fdtd/data", x, 
					".txt\" using 1:2 w l, \"/tmp/fdtd/data", x, 
					".txt\" using 1:3 w l\n ");
			// update time in seconds		
			fprintf(gnuplotPipe, "pause 0.01\n");
			fflush(gnuplotPipe);
		}
		
		fflush(stderr);
		// exit gnuplot  
		fprintf(gnuplotPipe,"exit \n");  
		pclose(gnuplotPipe);  
	}else{
		cout << endl << "ERROR: Could not open GnuPlot" << endl;
	}
}

void infopane(){ //---------------------------------------------------infopane()
	string plane = "bounded";
	switch (function_choice) {
		case 1:
			func = "Gaussian";
			break;
		case 2:
			func = "Sine";
			break;
		case 3:
			func = "Cosine";
			break;
		default:
			func = "other";
	}
	if(infiniteplane==2) plane = "infinite";
	else if(infiniteplane==3) plane = "absorbing";
	cout << "***********************************************************\n"
			"*     Start: " << setw(3) << startcord << 
			",   Number: " << setw(4) << maxTime << 
			",   Function: "<< setw(8) << func << "    *\n"
			"*     Plane: " << setw(8) << plane << "                           *\n"
			"***********************************************************\n";
}
