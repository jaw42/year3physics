//**********************************************************************************
// Author: Josh Wainwright
// Date: 21/10/2012
// Title: Using the GNU Scientific Library
//
// Description: Introduction and practice to some of the concepts and advantages of 
//				the huge number of functions in the GSL.
//**********************************************************************************

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>

#include <gsl/gsl_errno.h> 
#include <gsl/gsl_const_mksa.h>
#include <gsl/gsl_const_num.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_integration.h>

using namespace std;

//Function used in the trapezium rule for the first equation.
double ytrap(double x){
	double y = exp(-x) * sin(x);
	return y;
}

//Function used in the GSL function for the first equation, alpha is the weighting of the function and is set to 1 below.
double ygsl(double x, void* params){
	double alpha = *(double *) params;
	double ygsl = exp(-alpha * x) * sin(alpha * x);
	return ygsl;
}

//Function used in GSL function for the second equation.
double wgsl(double x, void* params){
	double alpha = *(double *) params;
	double wgsl = x * sin(30 * alpha * x) * cos(x);
	return wgsl;
}

int main (void){ 
	while(true){
		int option=0;
		cout << "Choose an option\n"
				<< "(1) Regular Cylindrical Bessel Function\n"
				<< "(2) Mass of the Muon\n"
				<< "(3) Trapezium Rule with the integral of exp(-x)sin(x)\n"
				<< "(4) GSL_Integration with the integral of exp(-x)sin(x)\n"
				<< "(5) GSL_Integration with the integral of xsin(30x)cos(x)\n"
				<< "(0) Exit" << endl; 
		cin >> option;
		
		if(option == 1){//---------------------------------------------------------------
			double x = 0.8;

			gsl_sf_result result;
			// As example, print out the value of the Bessel function for the specified x value.
			int status = gsl_sf_bessel_J0_e(x, &result);

			cout<< "Regular Cylindrical Bessel Function, x=0.8"<<endl
				<< "Bessel = " << gsl_sf_bessel_J0(x) << endl
				<< "Status = " << gsl_strerror(status) << endl
				<< "Error = " << result.err << endl << endl;
		}else if(option == 2){//---------------------------------------------------------------

			// As example use different library to print physical constant, mass of muon.
			cout<< "Mass of the Muon"<<endl
				<< "SI/MKS units = " << GSL_CONST_MKSA_MASS_MUON << " kg" << endl << endl;
		
		}else if(option == 3){//---------------------------------------------------------------
			// Calculate integral using trapezium rule
			ofstream ofile("outfile.txt");

			double a=0, b=2, total=0, area, error=0,  h;
			int n, number=0;
	
			// Perform the trapezium rule using 10, 100, 1000 etc. intervals.
			for(double power=1;power<=8;power++){
				double lognum = pow(10, power);
				area = 0;
				// Perform the trapezium rule for each separate number of panels.
				for(n=1;n<lognum;n++){
					h = (b-a)/(lognum);

					// Calculate the height of the curve at two adjacent points, average and multiply by with distance between them to get an area
					// when all of the separate areas are calculated and summed, the area of the whole curve will be produced. 
					double height1 = ytrap(n*h);
					double height2 = ytrap((n+1)*h);

					area += (height1 + height2);
					number++;
				}
				total = 0.5 * h * area;

				//calculate difference between value and analytically calculated result
				error = abs(1 - total / 0.4666296625931755725981756335);
				ofile << setprecision(15) << pow(10,power) << ", " << total << " " << error << endl;
				cout << setprecision(10) << pow(10,power) << ", " << total << " " << error << endl;
			}
			cout << "\nThe output file has been written\n";
			cout << "The function was called: " << number << " times"<< endl;
			cout << endl;
			ofile.close();

		}else if (option == 4){//---------------------------------------------------------------
			
			//GSL integration for integration 1
			gsl_integration_workspace * w = gsl_integration_workspace_alloc(1000);

			double result;
			double error;
			double alpha = 1.0;
			int key=6;

			// Initialise and convert the function as a gsl function for use in the integral below.
			gsl_function Ygsl;
			Ygsl.function = &ygsl;
			Ygsl.params = &alpha;
			
			// Perform the integration using the GSL library function.
			gsl_integration_qag(&Ygsl,		//function
								0.0,		//double a - starting value
								2.0,		//double b - end value
								0.0,		//double epsabs - absolute error bound
								1e-7,		//double epsrel - relative error bound
											// the function will calculate a value for which the absolute error or the relative error are smaller than those specified.
											// because the absolute error is zero, it will use the relative error.
								1000,		//size_t limit - maximum number of subintervals for the function to use, more gives a more accurate result.
								key,		//int key - option for choosing Gauss-Kronrod rule for integration (1-6), higher gives more accurate result.
								w,			//workspace - a workspace with enough space to hold the integration results and their errors.
								&result,	//double* result - pointer to the variable for the result.
								&error		//double* abserr - same for error value.
							);

			//This particular routine uses the parameters passed by the function to split the integration area into a number of 
			// intervals, much like the trapezium rule. The area of these intervals is calculate along with an estimate of the
			// error of this estimate. The intervals with the largest errors are then split down further and the iterations
			// repeated. By repeating this method, sections of the integration area that provide difficulty for the algorithm
			// are revisited, hence reducing the overall error very quickly and minimising unnecessary iterations. The workspace
			// that is initialised is used to hold the iterations, hence the size of the limit on the number of 
			// intervals cannot exceed the size of the workspace created.

			cout << "result = " << setprecision(14) << result << endl;
			cout << " error = " << setprecision(5) << error << endl;
			cout << "Number of intervals = " << w->size << endl << endl;

			//clear the workspace and so free memory.
			gsl_integration_workspace_free(w);

		}else if (option == 5){

			//GSL integration for integration 2
			const double TAU = 2 * M_PI;

			// Create workspace and QAWO table
			gsl_integration_workspace * w = gsl_integration_workspace_alloc(1000);
			// Create a table to describe the oscillating wavefunction
			gsl_integration_qawo_table * wf = gsl_integration_qawo_table_alloc(1, 				//the weight of the function defined by either sin(\weight x) or cos(\weight x)
																			TAU,				//the length of the interval that the function will be integrated over
																			GSL_INTEG_COSINE,	//SINE or COSINE depending on the type of function,
																								// in this case, the function varies in both sine and cosine, 
																								// cosine is chosen as it is the governing function, it is the envelope.
																			1000				//the number of levels of coefficients, the higher the better the accuracy.
																		);

			double result;
			double error;
			double alpha = 1.0;

			// Initialise and convert the function as a gsl function for use in the integral below.
			gsl_function Wgsl;
			Wgsl.function = &wgsl;
			Wgsl.params = &alpha;
			
			gsl_integration_qawo(&Wgsl,		//function
								0.0,		//double a - starting value
								0.0,		//const double epsabs - absolute error bound
								1e-7,		//const double epsrel - relative error bound
								1000,		//size_t limit - maximum number of subintervals for the function to use, more gives a more accurate result.
								w,			//workspace - a workspace with enough space to hold the integration results and their errors.
								wf,			//table - a table to describe the oscillating wave function.
								&result,	//double* result
								&error		//double* abserr
							);
			cout << "result = " << setprecision(8) << result << endl;
			cout << " error = " << setprecision(5) << error << endl << endl;

			//This GSL routine calculates a set of Chebyshev moments which are stored in the table initiated above. The function 
			// then uses these values to compute the integral repeating the iterations for regions that produce a large error, 
			// until the integral converges. The degree to which the area is split depends on the frequency of the oscillations, in 
			// this case 30, and so a higher point integration rule is used.

			// Clear the workspace and table to free memory.
			gsl_integration_workspace_free(w);
			gsl_integration_qawo_table_free(wf);
		}else if (option == 0){//---------------------------------------------------------------
			// Exit the program.
			return 0;
		}else {//---------------------------------------------------------------
			// Error in option selection.
			cout << "Option not recognised.\n";
		}
	}
	return 0;
}
