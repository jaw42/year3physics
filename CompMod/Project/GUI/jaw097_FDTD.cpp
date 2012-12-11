//******************************************************************************
// Author: Josh Wainwright
// Date: 07/12/2012
// Title: The Finite Difference Time Domain Method
//
// Description: An implementation of the finite difference time domain method
//              using a FLTK interface and GNUPlot to draw animation of the
//              simulated waves.

// Requirements:- FLTK
//              - GNUPlot
//
// Makefile: Included is a makefile that can be used to build the program. In 
//           the program directory, simply use the command "make" to build. The 
//           generated file, "jaw097_FDTD" can be run with "./jaw097_FDTD".
//******************************************************************************

    
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
#include "jaw097_gui.h"

#define SIZE 200  //The size of the grid. Defined in the preprocessor stage so
                  // that can be carried into the definitions of arrays below.
using namespace std;

//Setting the variables for the program. These are the default values and many 
// will be changed by the user when using the program.
double ez[SIZE], hy[SIZE], cexe[SIZE], cezh[SIZE], chye[SIZE], imp0=2.0,    ////
        absorbingcoef;                                                        //
int qTime, maxTime=500, mm, frame=0, startcord=50, function_choice=0,         //
    infiniteplane=0;                                                          //
bool nocombine=true, diele=false;                                           ////

// PROTOTYPES ------------------------------------------------------- PROTOTYPES
//Several functions to perform general manipulations as well as specific 
// functions. One contains the code for the calculations of the fields, the 
// other opens GNUPlot to view it.
string intToString(int t);                                                  ////
int StringToInt(string t);                                                    //
void calculatefields(int startcord);                                          //
void GNU();                                                                 ////
    
// FLTK --------------------------------------------------------------------FLTK
//Below is the code for the GUI interface to the program. The declarations are 
// included in the separate file, "gui.h".
Fl_Double_Window *win=(Fl_Double_Window *)0;

Fl_Button *animate=(Fl_Button *)0;

static void cb_animate(Fl_Button*, void*) {
    GNU();
}

Fl_Button *Calculate=(Fl_Button *)0;

static void cb_Calculate(Fl_Button*, void*) {
    calculatefields(startcord);
}

Fl_Button *calcani=(Fl_Button *)0;

static void cb_calcani(Fl_Button*, void*) {
    calculatefields(startcord);
    GNU();
}

Fl_Value_Slider *startcord_in=(Fl_Value_Slider *)0;

static void cb_startcord_in(Fl_Value_Slider*, void*) {
    startcord = startcord_in->value();
}

Fl_Value_Slider *maxTime_in=(Fl_Value_Slider *)0;

static void cb_maxTime_in(Fl_Value_Slider*, void*) {
    maxTime = maxTime_in->value();
}

Fl_Choice *seedfunction=(Fl_Choice *)0;

static void cb_seedfunction(Fl_Choice*, void*) {
    function_choice = seedfunction->value();
}

Fl_Menu_Item menu_seedfunction[] = {
    {"Gaussian", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
    {"Sine", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
    {"Cosine", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
    {"Wave Pulse", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
    {0,0,0,0,0,0,0,0,0}
};

Fl_Choice *systemtype=(Fl_Choice *)0;

static void cb_systemtype(Fl_Choice*, void*) {
    infiniteplane = systemtype->value();
}

Fl_Menu_Item menu_systemtype[] = {
    {"Infinite", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
    {"Bounded", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
    {"Absorbing", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
    {0,0,0,0,0,0,0,0,0}
};

Fl_Progress *progressbar=(Fl_Progress *)0;

Fl_Button *killgnu=(Fl_Button *)0;

static void cb_killgnu(Fl_Button*, void*) {
    system("killall gnuplot");
}

Fl_Button *deletefiles=(Fl_Button *)0;

static void cb_deletefiles(Fl_Button*, void*) {
    system("rm -v /tmp/fdtd/data*; mkdir -p /tmp/fdtd/");
    progressbar->value(0);
    progressbar->label("Files Deleted");
}

Fl_Button *close=(Fl_Button *)0;

static void cb_close(Fl_Button*, void*) {
    exit(0);
}

Fl_Check_Button *d3=(Fl_Check_Button *)0;

Fl_Value_Slider *dielectricleft=(Fl_Value_Slider *)0;

Fl_Value_Slider *dielectricright=(Fl_Value_Slider *)0;

static void cb_dielectricleft(Fl_Value_Slider*, void*) {
    cout << dielectricleft->value() << endl;
    dielectricleft->maximum(dielectricright->value()-10);
}

static void cb_dielectricright(Fl_Value_Slider*, void*) {
    dielectricright->minimum(dielectricleft->value()-10);
}

Fl_Counter *lossvalue=(Fl_Counter *)0;

Fl_Check_Button *dielectric=(Fl_Check_Button *)0;

Fl_Counter *permittivitycheck=(Fl_Counter *)0;

static void cb_dielectric(Fl_Check_Button*, void*) {
    if(dielectric->value()){
        dielectricleft->show();
        dielectricright->show();
        lossvalue->show();
        permittivitycheck->show();
    }else{
        dielectricleft->hide();
        dielectricright->hide();
        lossvalue->hide();
        permittivitycheck->hide();
    }
}

Fl_Check_Button *TFSF=(Fl_Check_Button *)0;

// MAIN ------------------------------------------------------------------- MAIN
//Main function including the user interface, all callbacks to functions and 
// begnning of the FLTK loop to run the program.
int main(int argc, char **argv) {                                               
  { win = new Fl_Double_Window(605, 425, "FDTD");
    { animate = new Fl_Button(80, 304, 100, 30, "Animate");
        animate->tooltip("Just animate the field");
        animate->callback((Fl_Callback*)cb_animate);
    } // Fl_Button* animate
    { Calculate = new Fl_Button(249, 304, 100, 30, "Calculate");
        Calculate->tooltip("Just calculate the field");
        Calculate->callback((Fl_Callback*)cb_Calculate);
    } // Fl_Button* Calculate
    { calcani = new Fl_Button(420, 304, 100, 30, "Calc + Ani");
        calcani->tooltip("Calculate and then animate the field");
        calcani->callback((Fl_Callback*)cb_calcani);
    } // Fl_Button* calcani
    { startcord_in = new Fl_Value_Slider(150, 20, 300, 30, "Seed Co-Ordinate");
        startcord_in->tooltip("The co-ordinate of the starting point");
        startcord_in->type(1);
        startcord_in->maximum(SIZE);
        startcord_in->step(1);
        startcord_in->value(50);
        startcord_in->callback((Fl_Callback*)cb_startcord_in);
        startcord_in->align(Fl_Align(FL_ALIGN_LEFT));
        startcord_in->when(FL_WHEN_RELEASE);
    } // Fl_Value_Slider* startcord_in
    { maxTime_in = new Fl_Value_Slider(150, 60, 300, 30,"Number of Iterations");
        maxTime_in->tooltip("How long should the fields be calculated for?");
        maxTime_in->type(1);
        maxTime_in->maximum(5000);
        maxTime_in->step(100);
        maxTime_in->value(500);
        maxTime_in->callback((Fl_Callback*)cb_maxTime_in);
        maxTime_in->align(Fl_Align(FL_ALIGN_LEFT));
        maxTime_in->when(FL_WHEN_RELEASE);
    } // Fl_Value_Slider* maxTime_in
    { seedfunction = new Fl_Choice(150, 100, 215, 30, "Seed Function");
        seedfunction->tooltip("What sort of function should be used to seed \
                                                the calculations?");
        seedfunction->callback((Fl_Callback*)cb_seedfunction);
        seedfunction->menu(menu_seedfunction);
    } // Fl_Choice* seedfunction
    { systemtype = new Fl_Choice(150, 140, 215, 30, "System Type");
        systemtype->tooltip("How should the boundaries of the system behave?");
        systemtype->callback((Fl_Callback*)cb_systemtype);
        systemtype->menu(menu_systemtype);
    } // Fl_Choice* systemtype
    { dielectric = new Fl_Check_Button(150, 180, 20, 30, "Dielectric");
        dielectric->tooltip("Should a dielectric material be modelled?");
        dielectric->align(Fl_Align(FL_ALIGN_LEFT));
        dielectric->callback((Fl_Callback*)cb_dielectric);
    } // Fl_Check_Button* dielectric
    { progressbar = new Fl_Progress(80, 353, 440, 20, "Graphing...");
        progressbar->value((frame+50)/maxTime);
        progressbar->color(0x88888800);
        progressbar->selection_color(0x4444ff00);
        progressbar->labelcolor(FL_WHITE);
    } // Fl_Progress* progressbar
    { killgnu = new Fl_Button(500, 223, 85, 20, "Kill GNUPlot");
        killgnu->tooltip("Kill all instances of GNUPlot");
        killgnu->box(FL_THIN_UP_BOX);
        killgnu->labelsize(10);
        killgnu->callback((Fl_Callback*)cb_killgnu);
    } // Fl_Button* killgnu
    { deletefiles = new Fl_Button(500, 248, 85, 20, "Delete Files");
        deletefiles->tooltip("Delete the generated files from previous run");
        deletefiles->box(FL_THIN_UP_BOX);
        deletefiles->labelsize(10);
        deletefiles->callback((Fl_Callback*)cb_deletefiles);
    } // Fl_Button* deletefiles
    { close = new Fl_Button(495, 380, 100, 30, "Close");
        close->callback((Fl_Callback*)cb_close);
    } // Fl_Button* close
    { d3 = new Fl_Check_Button(234, 180, 20, 30, "3D Plot");
        d3->tooltip("Should a 3D plot be produced? (Does not require \
                                                recalculating)");
        d3->down_box(FL_DOWN_BOX);
        d3->align(Fl_Align(FL_ALIGN_LEFT));
    } // Fl_Check_Button* d3
    { dielectricleft = new Fl_Value_Slider(75, 213, 205, 30, "Dielectric Left");
        dielectricleft->tooltip("Position of lefthand side of the dielectric");
        dielectricleft->type(1);
        dielectricleft->minimum(0);
        dielectricleft->maximum(SIZE-10);
        dielectricleft->value(SIZE/2);
        dielectricleft->step(10);
        dielectricleft->callback((Fl_Callback*)cb_dielectricleft);
        dielectricleft->when(FL_WHEN_RELEASE);
        dielectricleft->hide();
    } // Fl_Value_Slider* dielectricleft
    { dielectricright = new Fl_Value_Slider(285,213,205,30, "Dielectric Right");
        dielectricright->tooltip("Position of righthand side of the dielectric");
        dielectricright->type(1);
        dielectricright->minimum(110);
        dielectricright->maximum(SIZE);
        dielectricright->value(SIZE);
        dielectricright->step(10);
        dielectricright->callback((Fl_Callback*)cb_dielectricright);
        dielectricright->when(FL_WHEN_RELEASE);
        dielectricright->hide();
    } // Fl_Slider* dielectricright
    { lossvalue = new Fl_Counter(150, 265, 140, 20, "Lossy Material");
        lossvalue->tooltip("Set to 0 for a lossless material");
        lossvalue->minimum(0);
        lossvalue->maximum(1);
        lossvalue->step(0.001);
        lossvalue->lstep(0.01);
        lossvalue->value(0.01);
        lossvalue->align(Fl_Align(FL_ALIGN_LEFT));
        lossvalue->when(FL_WHEN_RELEASE);
        lossvalue->hide();
    } // Fl_Counter* lossvalue
    { permittivitycheck = new Fl_Counter(390, 265, 100, 20, "Permittivity");
        permittivitycheck->tooltip("Relative permittivity of lossy dielectric");
        permittivitycheck->minimum(1);
        permittivitycheck->maximum(20);
        permittivitycheck->step(0.5);
        permittivitycheck->value(5.0);
        permittivitycheck->align(Fl_Align(FL_ALIGN_LEFT));
        permittivitycheck->when(FL_WHEN_RELEASE);
        permittivitycheck->hide();
    } // Fl_Counter* permittivitycheck
    { TFSF = new Fl_Check_Button(306, 180, 20, 30, "TFSF");
        TFSF->tooltip("Use Total Field/Scattered Field boundary as the source");
        TFSF->down_box(FL_DOWN_BOX);
        TFSF->align(Fl_Align(FL_ALIGN_LEFT));
    } // Fl_Check_Button* TFSF
    win->end();
    } // Fl_Double_Window* o
    win->show(argc, argv);
    return Fl::run();
}

// FUNCTIONS --------------------------------------------------------- FUNCTIONS
string intToString(int integer){ //---------------------------------intoString()
// A general function to convert an integer to a string
    string ch;
    ostringstream outs; 
    outs << integer;
    ch = outs.str();   
    return ch;
}

int StringToInt(string ch){ //-------------------------------------StringtoInt()
// A general function to convert an string to an integer
    int integer;
    stringstream(ch) >> integer;
    return integer;
}

void calculatefields(int startcord){ //------------------------calculatefields()
//The main function that calculates the electric and magnetic fields at all of 
// the nodes and stores these values in the arrays defined above.

    //Remove any previous data files and create a location in the root temp 
    // folder to hold data files. Use /tmp for added space.
    system("rm /tmp/fdtd/data*; mkdir -p /tmp/fdtd/");
    frame=0;

    //Initialise the electric and magnetic fields to 0. This is quicker than
    // creating the arrays with a 0 value.
    for (mm = 0; mm < SIZE; mm++){
        ez[mm] = 0.0;
        hy[mm] = 0.0;
    }
    
    //Get the value for variabilities from the relavent FLTK components.
    double LOSS = lossvalue->value();
    double permittivity = permittivitycheck->value();
    double permeability = 1.0/*permeabilitycheck->value()*/;

    //Increment over the size of the grid area setting values for the lossyness 
    // and permittivity.
    for(mm = 0; mm < SIZE; mm++){
        if(mm<dielectricleft->value() || mm>dielectricright->value()){
            cexe[mm] = 1.0;
            cezh[mm] = imp0;
            chye[mm] = 1.0 / imp0;
        }else{
            cexe[mm] = (1.0 - LOSS) / (1.0 + LOSS);
            cezh[mm] = imp0 / permittivity / (1.0 + LOSS);
            chye[mm] = permeability / imp0;
        }
    }

    //Do time stepping.
    for (qTime = 0; qTime < maxTime; qTime++) {

        // update magnetic field 
        if(infiniteplane==0){
            hy[SIZE-1] = hy[SIZE-2];
        }
        for (mm = 0; mm < SIZE-1; mm++){
            // hy[mm] = hy[mm] + (ez[mm + 1] - ez[mm]) / imp0;
            hy[mm] = hy[mm] + (ez[mm + 1] - ez[mm]) *chye[mm];
        }

        //Only perform if the user has checked "true" for a TFSF boundary
        if(TFSF->value()){
            if(function_choice==0){
                hy[startcord-1] -= exp(-(qTime-31.) * (qTime-31.) /100.) / imp0;
            }
            else if(function_choice==1){
                hy[startcord-1] = sin((10*qTime-1)/(SIZE*1.0));
            }
            else if(function_choice==2){
                hy[startcord] = cos(sqrt(qTime));
            }
            else if(function_choice==3){}
        }

        //If the infinite plane is true add an absorbing boundary
        if(infiniteplane==0){
            ez[0] = ez[1];
            // ez[SIZE-1] = ez[SIZE-2];
        }

        //Update electric field.
        for (mm = 1; mm < SIZE; mm++){
            if(dielectric->value()){
                ez[mm] = cexe[mm] * ez[mm] + (hy[mm] - hy[mm - 1]) * cezh[mm];
            }else{
                ez[mm] = ez[mm] + (hy[mm] - hy[mm - 1]) * imp0;
            }
        }

        // Choose the type of function to be modelled.
        //-------------------------------------------------------FUNCTION CHOICE
        if(function_choice==0){ //----Gaussian
            if(SIZE==200){
                ez[startcord] += 2*exp(-(qTime - 30.) * (qTime - 30.) / 100.);
            }else if(SIZE==2000){
                ez[startcord] += 2*exp(-(qTime-400.) * (qTime-400.) / 10000.);
            }
        }
        else if(function_choice==1){ //----Sine
            ez[startcord] = sin(10*qTime/(SIZE*1.0));
        }
        else if(function_choice==2){ //----Cosine
            // ez[startcord] = cos(sqrt(qTime));
            ez[startcord] = cos(10*qTime/(SIZE*1.0));
        }
        else if(function_choice==3){ //----Pulse
            if(SIZE==200){
                ez[startcord] += 2*exp(-(qTime - 30.) * (qTime - 30.) / 100.) \
                                                            * cos(qTime);
            }else if(SIZE==2000){
                ez[startcord] += 2*exp(-(qTime-400.) * (qTime-400.) / 10000.) \
                                                            * cos(0.08*qTime);
            }
        }

        string name = "/tmp/fdtd/data" + intToString(frame) + ".txt";  
        ofstream write (name.c_str());

        progressbar->value((frame+50)*100.0/maxTime);
        char percent[10];
        sprintf(percent, "%d%%", int((frame+1)*100.0/maxTime));
        progressbar->label(percent);
        Fl::check();

        for(int x=0; x<SIZE; x++){
            write << x << " " << ez[x] << " " << hy[x] << " " << "0" << endl;
        }
        frame++;
        write.close();
    } // end of time-stepping
}

void GNU(){ //-------------------------------------------------------------GNU()
//Function that reads the files created by the calculations and draws them to 
// the screen using GNUPlot. The speed of the animation can be altered using the
// "pause" line below.
//Adapted from the example file provided in the Computational Modelling Git 
// Repository.
    FILE *gnuplotPipe = popen("gnuplot -persist","w");  
    
    // If gnuplot is found  
    if (gnuplotPipe) {           
        fprintf(gnuplotPipe,"%s%d%s" , "set xrange[0:", SIZE,    //y and x range
                            "] \nset yrange[-1:1] \nset zrange[-1:1]\n");  
        fprintf(gnuplotPipe,"set xlabel \"x\" \n \
                            set ylabel \"Electric Field\" \n"); //y and x labels
        fprintf(gnuplotPipe,"set sample 1001\n");                      //samples
        
        //Count number of files generated to use as the number of frames
        system("ls -l /tmp/fdtd |wc -l > temp");
        ifstream in("temp");
        in >> frame;
        system("rm temp");

        //The main loop that updates the graph output
        for (int x=0; x<frame-1; x++){
            if(d3->value()){
                if(dielectric->value()){
                    fprintf(gnuplotPipe, "%s%d%s%d%s",
                                "splot \"/tmp/fdtd/data", x, 
                                ".txt\" using 1:2:4 w l, \"/tmp/fdtd/data", x, 
                                ".txt\" using 1:4:3 w l\n ");
                }else{
                    fprintf(gnuplotPipe, "%s%d%s%d%s",
                                "splot \"/tmp/fdtd/data", x, 
                                ".txt\" using 1:2:4 w l, \"/tmp/fdtd/data", x, 
                                ".txt\" using 1:4:3 w l\n ");
                }
            }else{
                if(dielectric->value()){
                    fprintf(gnuplotPipe, "%s%f%s%f%s", 
                                                "set object 1 rect from ",
                                                dielectricleft->value(), 
                                                ",-1 to ",
                                                dielectricright->value(), 
                                                ",1 fc rgb \"#F0F3F5\"\n"
                                        ); 
                    fprintf(gnuplotPipe, "%s%d%s%d%s",
                                    "plot \"/tmp/fdtd/data", x, 
                                    ".txt\" using 1:2 w l, \"/tmp/fdtd/data", x, 
                                    ".txt\" using 1:3 w l\n ");
        
                }else{
                    fprintf(gnuplotPipe, "%s%d%s%d%s",
                                    "plot \"/tmp/fdtd/data", x, 
                                    ".txt\" using 1:2 w l, \"/tmp/fdtd/data", x, 
                                    ".txt\" using 1:3 w l\n ");
                }
            }
            // update time in seconds
            if(SIZE==200){
                fprintf(gnuplotPipe, "pause 0.01\n");
            }else if(SIZE==2000){
                fprintf(gnuplotPipe, "pause 0.001\n");
            }
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

