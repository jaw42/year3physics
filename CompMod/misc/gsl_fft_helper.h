#include <cstdlib>
#include <cstring>

#ifndef GSL_FFT_H
#define GSL_FFT_H

/*
GSL FFT helper file
Written by Daniel Brown 2011

Copy and include this in your code by using #include "gsl_fft_helper.h".
If this is used in any assessed work make sure you include it in your submission

*/

/*
	These macros are quick and more verbose way of setting the real and imaginary
	parts of a number into a complex array for fourier transforming. Macro got from
	gsl documentation.
*/
#define Re(z,i) ((z)[2*(i)])
#define Im(z,i) ((z)[2*(i)+1])

/*
	This orders the frequency in a more sensible way.negative frequencies go off to the left
	and positive to the right.
*/
void fftshift(double *data, int n){
	double *tmp;
	
	tmp = (double*) malloc(sizeof(double)*2*n); //tmp variable to hold data while copying
	memcpy(tmp,data, sizeof(double) * 2 * n); //copy data across

	for(int i=0;i<n;i++){
		int j = (i+(n/2-1))%(n); // the DC 0Hz frequency should be
								 // siuated in the middle of the spectrum
		Re(data,j) = Re(tmp,i);
		Im(data,j) = Im(tmp,i);
	}	

	free(tmp); // free up allocated memory
}
#endif
