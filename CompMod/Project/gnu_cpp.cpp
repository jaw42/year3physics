#include <stdio.h>
#include <iostream>

using namespace std;

/*
	The idea of this code is to use a pipe to output directly to gnuplot, as if you were simply typing
	the commands in yourself. Thus you see common commands like 'set terminal' and 'plot' are used below.
	Whatever you can type into gnuplot you can use here. 

	This code shows a guassian curve evolving over time with a varying standard deviation.
*/

int main(){
	// Open a pipe to gnuplot. but we forward the output to a file err
	// and output the errors to that file too. To get output to the screen
	// remove the part '> ./output 2>&1'
	FILE *gnuplotPipe = popen("gnuplot -persist","w");  
	
	// If gnuplot is found  
	if (gnuplotPipe) {  
		 
        //By default gnuplot will pick the best terminal output it sees fit.
		//so when running it on the phymat servers it will default to X11 forwarding.
		//you can force it to output to png/jpeg/bmp or whatever you like.
		//fprintf(gnuplotPipe,"set terminal windows \n");  
		//fprintf(gnuplotPipe,"set terminal x11 \n");  
		fprintf(gnuplotPipe,"set xrange[0:200] \nset yrange[-1:1] \n");  
		
		//This is the main loop that updates the graph output
		for (int x=0; x<1000; x++){
			// fprintf(gnuplotPipe, "plot data%d\n",x);
			fprintf(gnuplotPipe, "%s%d%s\n","plot \"data",x,".txt\"");
			// gnuplot pause just slows down the animation so we can see it.	
			// update time in seconds		
			fprintf(gnuplotPipe, "pause 0.001\n");
			// flushing the pipe means that we push the commands straight away to
			//gnuplot rather than waiting for some internal buffer to be filled with
			//a certain number of commands
			fflush(gnuplotPipe);
		}
		
		fprintf(stderr,"Press enter to continue...\n");  
		fflush(stderr);
		//getchar(); //wait for user to press enter  
		
		// exit gnuplot  
		fprintf(gnuplotPipe,"exit \n");  
		pclose(gnuplotPipe);  
	}else{
		cout << endl << "ERROR: Could not open GnuPlot" << endl;
	}
}
