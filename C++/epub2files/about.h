//******************************************************************************
// Author: Josh Wainwright                                                     *
// Date: 30/11/2012                                                            *
// File Number:03                                                             *
//                                                                             *
// Description: Header file for the "About" dialogue.                          *
//******************************************************************************

#ifndef about_h
#define about_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
extern Fl_Double_Window *about_window;
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
extern Fl_Button *close_about;
void about();
#endif
