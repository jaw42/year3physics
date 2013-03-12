//**********************************************************************************
// Author: Josh Wainwright
// Date: 29/10/2012
// Title: Integration of Ordinary Differential Equations
//
// Description: Examining two different methods for root finding and the advantages 
//              and the advantages and disadvantages of each depending on the system
//              being examined
//**********************************************************************************

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <stdio.h>

#include <gsl/gsl_errno.h> 
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

using namespace std;

// dy/dt for the first order differential equation
double dy(double y){ 
    return 1 + y*y;
}

// dx/dt for the second order differential equation
double dx(double v){
    return v;
}

// dv/dx for the second order differential equation
double dv(double x){
    //Let mass, m=1.
    return -x;
}

//Definition of the functions used for the GSL algorithm stored in separate elements of array
int func (double t, const double y[], double f[], void *params){
    //dx/dt = v
    f[0] = y[1];    //ref{first}
    //dv/dx = -x
    f[1] = -y[0];   //ref{second}
    return GSL_SUCCESS;
}

int jac (double t, const double y[], double *dfdy, double dfdt[], void *params){
  //  double mu = *(double *)params;
    gsl_matrix_view dfdy_mat = gsl_matrix_view_array (dfdy, 2, 2);
    gsl_matrix * m = &dfdy_mat.matrix; 
    gsl_matrix_set (m, 0, 0, 0.0);  //this is ref{first} differentiated wrt y[0] 
    gsl_matrix_set (m, 0, 1, 1.0);  //this is ref{first} differentiated wrt y[1] 
    gsl_matrix_set (m, 1, 0, -1.0); //this is ref{second} differentiated wrt y[0] 
    gsl_matrix_set (m, 1, 1, 0.0);  //this is ref{second} differentiated wrt y[1]
    dfdt[0] = 0.0;
    dfdt[1] = 0.0;
    return GSL_SUCCESS;
}

int main (void){ 
    while(true){
        int option=0;
        cout << "Choose an option\n"
                << "(1) Estimate y(pi/4) using Euler's Method for the equation dy/dx=1+y^2\n"
                << "(2) Estimate the velocity and position for the SHO potential using Runge-Kutta 2nd order\n"
                << "(3) Estimate the velocity and position for the SHO potential using Runge-Kutta 4th order\n"
                << "(4) Estimate for the SHO potential using Runge-Kutta 4th order with step size control\n"
                << "(0) Exit" << endl; 
        cin >> option;

        if(option == 1){//----------------------------------------------------------------------------------------------
        //  y1 = y0 + h * f(x0, y0)
            
            ofstream outfile("data.txt");
            double yold=0,          // the value of y for the iteration number n
                   ynew=0,          // the value of y for the iteration number n+1
                   yprev,           // a value of y at n used for error calculation
                   yerror_est=0,    // estimate of the error on y from current and previous results
                   yerror,          // actual error on y from analytical calculation
                   h,               // step width
                   final = M_PI/4;  // end point - on this case the value of x to find y(x)
            int n=0, count; 
            cout << "Choose the number of intervals: ";
            cin >> count;
            h = final/count;
            cout << "\n  n         ynew   error(est)        error\n";
            cout << "--------------------------------------------\n";
            do{
                yprev = yold; // set the comparison value equal to the value from the previous iteration 
                ynew = yold + h*dy(yold); // apply Euler's method to find the new value
                n++;
                yold=ynew; //set current value as previous value for next iteration
                yerror_est = ynew - yprev;
                yerror = 1-ynew;

                cout << setw(3) << n << " " << 
                        setw(12) << ynew << " " << 
                        setw(12) << yerror_est << " " << 
                        setw(12) << yerror << endl;
                outfile << n << " " << ynew << " " << yerror_est << " " << yerror << endl;
            }while(n<count);

            cout << "h = " << h << endl;
            cout << "Final estimate = " << ynew << endl;

        }else if(option == 2){//----------------------------------------------------------------------------------------
        //  y1 = y0 + h * f(x0, y0)
            
            ofstream outfile("data.txt");
            double vold=0,//  starting value for the velocity of 0
                   vnew=0,
                   vprev,
                   xold=2,// Set an initial value for the starting point
                   xnew=0,
                   xprev,
                   k1, k2, k3, k4,// initialise each of the intermediate steps for the Runge-Kutta Method
                   verror_est=0,// estimated and actual errors, from results and know value respectively
                //   verror,
                   xerror_est,
                //   xerror,
                   h;// step size
            int n=0, count=200; 
            cout << "Choose the width of the intervals: ";
            cin >> h;
            cout << "\n  n |       vnew   verror_est |       xnew   xerror_est";
            cout << "\n---------------------------------------------------------" << endl;
            do{
                vprev = vold;         ////
                k1 = h*dx(xold);        //Apply the Runge-Kutta Method - the value of the position, x, from the last
                k2 = h*dx(xold + k1/2); // iteration is used to calculate the new position of velocity, then the midway
                vnew = vold + k2;       // point of this is used to calculate the value for the next iteration.
                vold = vnew;          ////

                xprev = xold;         ////
                k3 = h*dv(vold);        //Same as above but for position. The value of the velocity is fed in to find
                k4 = h*dv(vold + k3/2); // the new value of position.
                xnew = xold + k4;       //
                xold = xnew;          ////

                verror_est = vnew - vprev; //calculate the estimated and actual errors using the know value of 1.
                xerror_est = xnew - xprev;

                cout << setw(3) << n << " | " << 
                        setw(10) << vnew << 
                        setw(13) << verror_est << " | " << 
                        setw(10) << xnew << 
                        setw(13) << xerror_est << endl;
                outfile << n << " " << vnew << " " << verror_est << " " << xnew << " " << xerror_est << endl;
                n++;
            }while(n<count);

            cout << "h = " << h << endl << endl;
        }else if(option == 3){//----------------------------------------------------------------------------------------
            
            //GSL library - Runge KUtta method
            ofstream outfile("data.txt");
            const gsl_odeiv_step_type * T = gsl_odeiv_step_rk4; //Define the step type as being Runge-Kutta method to 
                                                                // the 4th order - rk4

            gsl_odeiv_step * s = gsl_odeiv_step_alloc (T, 2); //Initialise a pointer to the step type defined above

            double mu = 1; //set the parameter mu to 1 so that there is no effect on the function

            //Define the ordinary differential equation in terms of
            gsl_odeiv_system sys = {func, // functions above,
                                    jac,  // a maxtrix containing the Jacobian of the functions used,
                                    1,    // the number of dimensions of the system and
                                    &mu}; // the arbitrary parameters of the system, used as 1.

            double t = 0.0, t1=20.0; //define the start and end time for the calculation to be performed over
          //  cout << "Enter the end time: ";
          //  cin >> t1;
            double h;
            cout << "Choose the width of the intervals: ";
            cin >> h;

            double y[2] = { 1.0, 0.0 }, // array to hold the values as calculated, initiated with position and velocity
                   y_err[2],
                   dydt_in[2],
                   dydt_out[2];

            GSL_ODEIV_FN_EVAL(&sys, t, y, dydt_in);

            while (t < t1){
                int status = gsl_odeiv_step_apply (s,       //ODE step algorithm
                                                   t,       //Initial value of time, increases for each iteration
                                                   h,       //Step size
                                                   y,       //(array) function values
                                                   y_err,   //Array to hold the errors
                                                   dydt_in, //Value for the derivatives for the system input
                                                   dydt_out,//Derivatives for the system output
                                                   &sys     //The differential system to be calculated
                                                );

                if (status != GSL_SUCCESS){break;}

                dydt_in[0] = dydt_out[0]; // Set the input for the next iteration as the output from the current
                dydt_in[1] = dydt_out[1];

                t += h;

                cout << t << " " << y[0] << " " << y[1] << endl;
                outfile << t << " " << y[0] << " " << y[1] << endl;
            }

            gsl_odeiv_step_free (s);

        }else if (option == 4){//---------------------------------------------------------------------------------------

            ofstream outfile("data.txt");
            const gsl_odeiv_step_type * T = gsl_odeiv_step_rk4; //Define the step type as being Runge-Kutta method to 
                                                                // the 4th order - rk4

            gsl_odeiv_step * s = gsl_odeiv_step_alloc (T, 2); //Initialise a pointer to the step type defined above
            gsl_odeiv_control * c = gsl_odeiv_control_y_new (1e-6, 0.0); //Variable to hold local error for each step
            gsl_odeiv_evolve * e = gsl_odeiv_evolve_alloc (2); //Initialise and set a pointer to evolution function, set
                                                               // with number of dimensions

            double mu = 1; //Parameters to generalise the function, set to 1 to make no change
            gsl_odeiv_system sys = {func, //The functions to be calculated
                                    jac,  // a maxtrix containing the Jacobian of the functions used,
                                    2,    //Number of dimensions of the system
                                    &mu   //Pointer to the parameters
                                   };

            double t = 0.0, t1=20.0;
            double h;
            cout << "Choose the width of the intervals: ";
            cin >> h;

            //y[0] = { 1.0, 0.0 } are the start positions - position and velocity.
            double y[2] = { 1.0, 0.0 };//, y_err[2];

            while (t < t1){
                int status = gsl_odeiv_evolve_apply (e,   //the evolve function defining the number of dimensions of 
                                                          // the system
                                                     c,   //the control for the adaptive step size of h
                                                     s,   //the type of solver to use
                                                     &sys,//system containing the functions to be calculated
                                                     &t,  //pointer to the initial...
                                                     t1,  //...and final times for the interval
                                                     &h,  //the step width telling the function how much to advance the
                                                          //time on each iteration
                                                     y    //functions to be calculated
                                                );

                if (status != GSL_SUCCESS){break;}

                cout << t << " " << y[0] << " " << y[1] << endl;
                outfile << t << " " << y[0] << " " << y[1] << endl;
            }
            
            gsl_odeiv_evolve_free(e);  //Free the memory used for the solver
            gsl_odeiv_control_free(c);
            gsl_odeiv_step_free(s);
        }else if (option == 5){

        }else if (option == 0){//---------------------------------------------------------------------------------------
            // Exit the program.
            return 0;
        }else {//-------------------------------------------------------------------------------------------------------
            // Error in option selection.
            cout << "Option not recognised.\n";
        }
    }
    return 0;
}
