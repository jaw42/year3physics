#ifndef GSL_MATRIX_H
#define GSL_MATRIX_H
#include <iostream>

/*
GSL matrix helper file
Written by Daniel Brown 2011

Copy and include this in your code by using #include "gsl_matrix_helper.h".
If this is used in any assessed work make sure you include it in your submission

*/

/* GSL does have an inbuilt method to print matrices but it prints each element
   row by row. This function provides a much nices output to the console */
void printMatrix(const gsl_matrix *m){
	if(m == NULL){
		std::cout << "Matrix pointer is nulled. Could not print" << std::endl;		
		return;
	}
	/*
		To get the numbers of rows and columns in a matrix you call the
		size1(rows) and size2(cols) members of the matrix.
	*/
	for(int i=0; i<m->size1; i++){
		std::cout << "| ";
		for(int j=0;j<m->size2;j++){
			std::cout << std::setiosflags(std::ios::fixed)<< std::setw(12) 
				 << std::setprecision(9) << gsl_matrix_get(m,i,j);
			if(j!=m->size2-1)
				std::cout  << ", ";
		}

		std::cout << "|" << std::endl;
	}
	std::cout << std::endl;
}

/*
	Calculates a^n. Does this using GSL BLAS functions so should be fairly fast.
	Theres probably a more efficient way of doing this without so many memcpy's...
*/
void matrix_pow(gsl_matrix *a, int n){
	gsl_matrix *b = gsl_matrix_calloc(a->size1,a->size2);
	gsl_matrix *ao = gsl_matrix_alloc(a->size1,a->size2);
	gsl_matrix_memcpy(ao,a);

	for(int i=1;i<n;i++){
		gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
						1.0, a,ao,0,b);
		gsl_matrix_memcpy(a,b);
	}

	gsl_matrix_free(b);
	gsl_matrix_free(ao);
}
#endif 
