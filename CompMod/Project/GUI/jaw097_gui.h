#ifndef gui_h
#define gui_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
extern Fl_Double_Window *win;
#include <FL/Fl_Button.H>
extern Fl_Button *animate;
extern Fl_Button *Calculate;
extern Fl_Button *calcani;
#include <FL/Fl_Value_Slider.H>
extern Fl_Value_Slider *startcord_in;
extern Fl_Value_Slider *maxTime_in;
#include <FL/Fl_Choice.H>
extern Fl_Choice *seedfunction;
extern Fl_Choice *systemtype;
#include <FL/Fl_Check_Button.H>
extern Fl_Check_Button *dielectric;
#include <FL/Fl_Progress.H>
extern Fl_Progress *progressbar;
extern Fl_Button *killgnu;
extern Fl_Button *deletefiles;
extern Fl_Button *close;
extern Fl_Check_Button *d3;
extern Fl_Value_Slider *dielectricleft;
extern Fl_Value_Slider *dielectricright;
#include <FL/Fl_Counter.H>
extern Fl_Counter *lossvalue;
extern Fl_Counter *permvalue;
extern Fl_Check_Button *TFSF;
extern Fl_Menu_Item menu_seedfunction[];
extern Fl_Menu_Item menu_systemtype[];
#endif
