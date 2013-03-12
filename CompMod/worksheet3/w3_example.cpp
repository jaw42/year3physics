#include <iostream>
#include <iomanip>
using namespace std;

//the following include GSL header files, note how they follow the namespace directive above...
#include <gsl/gsl_errno.h> //defines the GSL status return values and convenience functions
#include <gsl/gsl_sf_airy.h> //includes definitions of the Airy function routines
#include <gsl/gsl_const_mksa.h> //defines the values of certain Physical constants

int main (void)
{
    
    /*=========================
     * Airy function Ai(x=0.8)
     *=========================*/
    double x = 0.8; //input argument to the Airy function
    gsl_sf_result result; //data type specialised to the output of Special Function routines. 
                          //see the manual for its definition.
    
    gsl_mode_t mode=0;

    /*
     * The following is a GSL function call . Note how it is prefixed with
     * 'gsl_' . Most routines return an integer status value which should be 
     * handled (see below).
     */
    int status = gsl_sf_airy_Ai_e(x,mode,&result);
    
    cout<<" *Airy Function, Ai(x=0.8,mode=0)* "<<endl
        /* Many GSL routines return pre-defined integer status codes which indicate
         * their success/failure . 
         * 
         * If the code is successful it will return a 0. A non-zero value
         * indicates a problem.
         * 
         * There are a large number of reasons a routine can fail and 
         * GSL defines a large number of (non-zero) status codes to help provide 
         * information about what went wrong
         * 
         * It is good practice to handle these status values in some way, 
         * at least testing to make sure the routine didnt return a non-zero 
         * value. 
         * 
         * The below uses a function gsl_strerror which automatically prints
         * an status/error message specific to the value of status that was returned. 
         */
        <<"GSL status  = "<<gsl_strerror(status)<<endl //note the use of status/error handling
        <<"Ai(0.8) = "<<setprecision(18)<<result.val<<endl //value of Ai(x=0.8)
        <<"      +/- "<<result.err<<endl; //estimate of error in value
    
    /*============================================
     * Value of Gravitational Constant in SI units
     *============================================*/
    cout<<endl
        <<" *Gravitational Constant,G* "<<endl
        <<"G = "<<GSL_CONST_MKSA_GRAVITATIONAL_CONSTANT<<endl;
        
    return 0;
}
