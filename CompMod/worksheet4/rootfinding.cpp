//**********************************************************************************
// Author: Josh Wainwright
// Date: 29/10/2012
// Title: Using the GNU Scientific Library
//
// Description: Examining two different methods for root finding and the advantages 
//				and the advantages and disadvantages of each depending on the system
//				being examined
//**********************************************************************************

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>

#include <gsl/gsl_errno.h> 
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>

using namespace std;

//Define the function f that shall be used for the first problem
double f(double theta){
	double k=1000, L=0.6, m=5, g=9.807;
	return k*L*(tan(theta) - sin(theta)) - m*g;
}

//Define the function y that shall be used for the second problem
double y(double x){
	double y = pow(x,3) + 8* pow(x,2)  - 5*x + 22;
	return y;
}

// Define the parameters of the quartic when used for the gsl function.
struct ygsl_params { double a; double b; double c; double d;};

// Redefine the function y as ygsl for use with the gsl. The parameters are used from above and are defined in main.
double ygsl (double x, void * params) {
	struct ygsl_params * p = (struct ygsl_params *)params;
	double a = (p->a);
	double b = (p->b);
	double c = (p->c);
	double d = (p->d);
	return  ((a * x + b) * x + c) * x + d;
}

// Also define the first derivative of the above function for use with the gsl function
double ygsl_deriv (double x, void * params){
	struct ygsl_params * p = (struct ygsl_params *)params;
	double a = (p->a);
	double b = (p->b);
	double c = (p->c);
	return (a * x + 2.0*b) * x + c;
}

//Define the general function for use with the gsl which contains the function to be solved and its derivative
void ygsl_fdf (double x, void *params, double *y, double *dy){
	struct ygsl_params * p = (struct ygsl_params *)params;
	double a = (p->a);
	double b = (p->b);
	double c = (p->c);
	double d = (p->d);

	*y = ((a * x + b) * x + c) * x + d;
	*dy = (3 * a * x + 2.0*b) * x + c;
}

int main (void){ 
	while(true){
		int option=0;
		cout << "Choose an option\n"
				<< "(1) Use Bisection method to solve for Theta, kL(tan(theta) - sin(theta)) -mg = 0\n"
				<< "(2) Use Bisection method to solve for x, x^3 + 8x^2 - 5x + 22 =0\n"
				<< "(3) Use the GSL with Newton-Raphson method to solve equation2\n"
				<< "(0) Exit" << endl; 
		cin >> option;
		
		if(option == 1){//---------------------------------------------------------------Bisection Method
			//Function kL(tan0-sin0)-mg=zero
			ofstream ofile("bisectiondata1.txt");

			double compare, xb = -1, xt = 1, xm;
			int count=0;
			cout << setw(3) << "#" << " " << setw(12) << "f(xt)" << " " << setw(12) << "f(xb)" << " " << setw(8) << "xb" << " " << setw(8) << "xt" << " " << setw(12) << "xt-xb" << endl << endl; 
			
			//Perform a loop that operates until the difference between the staring and ending values is smaller than a set error.
			do{
				xm = (xb + xt)/2;		//set a value for the midpoint of the iteration region to be used as the next bound.
				compare = f(xt)*f(xm);	//create a compare value, the sign of this value will be used to tell if the function lies in the first or second half of the range.
				if( compare<0 ){
					xb = xm;
				} else if(compare>0){
					xt = xm;
				}
				count++;
				cout << setw(3) << count << " " << setw(12) << f(xt) << " " << setw(12) << f(xb) << " " << setw(8) << xb << " " << setw(8) << xt << " " << setw(12) << xt-xb << endl; 
				ofile << count << " " << f(xt) << " " << f(xb) << " " << xb << " " << xt << " " << xt-xb << endl; 
			}while(xt-xb > 1e-3);
			cout << "The data has been written to the output file." << endl << endl;

		}else if(option == 2){//---------------------------------------------------------------Bisection Method
			ofstream ofile("bisectiondata2.txt");

			//Same as above, perform the bisection method, this time using the second function.
			double compare, xb = -10, xt = 10, xm;
			int count=0;
			cout << setw(3) << "#" << " " << setw(12) << "y(xt)" << " " << setw(12) << "y(xb)" << " " << setw(8) << "xb" << " " << setw(8) << "xt" << " " << setw(12) << "xt-xb" << endl << endl; 
			do{
				xm = (xb + xt)/2;
				compare = y(xt)*y(xm);
				if( compare<0 ){
					xb = xm;
				} else if(compare>0){
					xt = xm;
				}
				count++;
				cout << setw(3) << count << " " << setw(12) << y(xt) << " " << setw(12) << y(xb) << " " << setw(8) << xb << " " << setw(8) << xt << " " << setw(12) << xt-xb << endl; 
				ofile << count << " " << y(xt) << " " << y(xb) << " " << xb << " " << xt << " " << xt-xb << endl; 
			}while(xt-xb > 1e-3);

			ofile.close();			

		}else if(option == 3){//---------------------------------------------------------------Newton-Raphson Method
			ofstream ofile("GSLdata.txt");

			int complete, count=0, stop=100;

			//initiate the gsl function with the type of solver to use, in this case newton for Newton-Raphson.
			const gsl_root_fdfsolver_type * T = gsl_root_fdfsolver_newton;
			gsl_root_fdfsolver * s = gsl_root_fdfsolver_alloc (T);

			//Define the actual value for the root for use with calculating actual errors.
			double x0, x, r_expected=-8.84583;

			//Link the gsl function to the function that is being solved, as well with its first derivative 
			//and the parameters for the quartic that will actually define the equation.
			gsl_function_fdf FDF;

			struct ygsl_params params = { 1.0, 8.0, -5.0, 22.0 };

			FDF.f = &ygsl;
			FDF.df = &ygsl_deriv;
			FDF.fdf = &ygsl_fdf;
			FDF.params = &params;

			cout <<"Choose initial guess: \n"; 
			cin >> x;

			//Initialise the solver with the function and the initial guess to use.
			gsl_root_fdfsolver_set(s, &FDF, x);
			//Check the type of function being used and print it and the table headers
			cout << "using method " << gsl_root_fdfsolver_name(s) << endl;
			cout << "iter  root    err     err(est)" << endl;
			
			//Perform the loop until the function has converged or until the maximum number of iterations has been reached, 
			// - prevents the loop from continuing if the solver does not find a root, or the series diverges.
			do{
				count++;
				complete = gsl_root_fdfsolver_iterate(s);

				//Set the start value to the value from the previous loop for comparison and set the new value to the value returned by the solver
				x0 = x;
				x = gsl_root_fdfsolver_root(s);

				//Check if the function has converged, in which case finish the loop.
				complete = gsl_root_test_delta(x, x0, 0, 1e-3);
				if(complete == GSL_SUCCESS){cout << "Converged at: \n";}

				cout << setw(4) << count << " " << setprecision(6) //
					<< setw(10) << x << " " //
					<< setw(10) << x - r_expected << " " //
					<< setw(10) << x-x0 << endl;
				
				ofile << count << " " << x << " " << x - r_expected << " " << x-x0 << endl;

			}while( (complete != GSL_SUCCESS) && (count < stop) );

			//Free the space used by the solver once the loop is finished and close the output file.
			gsl_root_fdfsolver_free(s);

			ofile.close();

		}else if (option == 4){//---------------------------------------------------------------

		}else if (option == 5){

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
