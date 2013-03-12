//******************************************************************************
// Author: Josh Wainwright                                                     *
// Date: 30/11/2012                                                            *
// File Number: 04                                                             *
//                                                                             *
// Description: "About" dialogue, Here the window is created and drawn. All    *
//              FLTK elements are defined and used here. This file should      *
//              remain able to be run standalone.                              *
//******************************************************************************

#include "about.h"
#include "images.cxx"

Fl_Double_Window *about_window=(Fl_Double_Window *)0;
Fl_Button *close_about=(Fl_Button *)0;

static void cb_close_about(Fl_Button* o, void*) {
  about_window->hide();
}

void about() {
    about_window = new Fl_Double_Window(360, 270, "About");
    {
        about_window->set_modal();
            about_window->color(FL_LIGHT2);
        Fl_Box* picture = new Fl_Box(10, 10, 340, 128);
            picture->image(image_big);
        
        Fl_Box* title = new Fl_Box(10, 148, 340, 35, "EPUB Creator");
            title->labeltype(FL_ENGRAVED_LABEL);
            title->labelsize(24);
        
        Fl_Box* text1 = new Fl_Box(10, 188, 340, 20, 
                        "An application to build e-publication file for use");
            text1->labelsize(11);
        
        Fl_Box* text2 = new Fl_Box(10, 202, 340, 20, 
                        "with popular ebook readers and applications.");
            text2->labelsize(11);
        
        close_about = new Fl_Button(260, 230, 90, 30, " Close");
            close_about->image(image_window);
            close_about->callback((Fl_Callback*)cb_close_about);
            close_about->align(Fl_Align(256));
        
        about_window->size_range(360, 270, 360, 270);
        about_window->end();
    }
    about_window->show();
}
