//******************************************************************************
// Author: Josh Wainwright                                                     *
// Date: 30/11/2012                                                            *
// File Number: 09                                                             *
//                                                                             *
// Description: Main file containing "main" and the declaration and position of*
//              all FLTK elements and their respective callback functions and  *
//              attributes.                                                    *
//******************************************************************************

#include <iostream>

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Input_Choice.H>

#include "epubfunctions.h"
#include "epubmk.h"
#include "about.h"
#include "images.cxx"

using namespace std;

Meta metadata;

Fl_File_Chooser* chooser;

Fl_Double_Window* mywindow;
Fl_Menu_Bar* mainmenu;
Fl_Tile* tile;
Fl_Group* lefthalf;
    Fl_Input* intitle;
        Fl_Button* info;
    Fl_Input* inid;
    Fl_Input_Choice* inlang;
    Fl_Input* inauthor;
    Fl_Input* inpublish;
    Fl_Input* indate;
        Fl_Button* today;

    Fl_Tabs* tabs;
        Fl_Group* htmls;
            Fl_Menu_Bar* htmlmenu;
            Fl_Text_Buffer* htmlbuff;
                Fl_Text_Display* htmltext;

        Fl_Group* images;
            Fl_Menu_Bar* imagemenu;
            Fl_Text_Buffer* imagebuff;
                Fl_Text_Display* imagetext;

    Fl_Box* hidden;
    Fl_Button* update;
    Fl_Button* create;
    Fl_Button* close;

Fl_Box* righthalf;
    Fl_Tabs* outputtabs;
        Fl_Group* outputwindow1;
            Fl_Text_Buffer* code1buff;
                Fl_Text_Editor* code1text;
        Fl_Group* outputwindow2;
            Fl_Text_Buffer* code2buff;
                Fl_Text_Editor* code2text;
        Fl_Group* outputwindow3;
            Fl_Text_Buffer* code3buff;
                Fl_Text_Editor* code3text;


//FLTK----------------------------------------------------------------------FLTK

void Meta::openimage_cb(Fl_Widget*, void*){
    string images = "";
    Fl_File_Chooser chooser("~", 
                            "Image Files (*.{bmp,gif,jpg,jpeg,png})", 
                            Fl_File_Chooser::MULTI, 
                            "Choose Image Files"
                            );
    chooser.show();

    while(chooser.shown()){
        Fl::wait();
    }

    if ( chooser.value() == NULL ){
        return;
    }
    for ( int t=1; t<=chooser.count(); t++ ) {
        metadata.set_images(chooser.value(t));
        images = images + chooser.value(t) + "\n";
    }
    imagebuff->text(images.c_str());
}

void Meta::openhtml_cb(Fl_Widget*, void*){
    string files = "";
    Fl_File_Chooser chooser("~", 
                            "Web (*.{html,htm,xhtml})\tText (*.txt)", 
                            Fl_File_Chooser::MULTI, 
                            "Choose HTML Files"
                            );
    chooser.show();

    while(chooser.shown()){
        Fl::wait();
    }

    if ( chooser.value() == NULL ){
        return;
    }
    for ( int t=1; t<=chooser.count(); t++ ) {
        metadata.set_files(chooser.value(t));
        files = files + chooser.value(t) + "\n";
    }
    htmlbuff->text(files.c_str());
}

void Meta::clearhtml_cb(Fl_Widget*, void*){
    metadata.clear("files_list");
    htmlbuff->text("");
}

void Meta::clearimages_cb(Fl_Widget*, void*){
    metadata.clear("images_list");
    imagebuff->text("");
}

void Meta::inlang_cb(Fl_Widget *w, void*){
    /*if(inlang->value()=="English"){metadata.set_lang("en");}
    else if(inlang->value()=="English - GB"){metadata.set_lang("en-gb");}
    else if(inlang->value()=="English - US"){metadata.set_lang("en-us");}
    else if(inlang->value()=="French"){metadata.set_lang("fr");}
    else if(inlang->value()=="French - Canada"){metadata.set_lang("fr-ca");}
    else if(inlang->value()=="Spanish"){metadata.set_lang("es");}
    else{metadata.set_lang("none");}*/
    metadata.set_lang(inlang->value());
}
void Meta::intitle_cb(Fl_Input* o){
    metadata.set_title(intitle->value());
    metadata.set_title_under(space_underscore(intitle->value()));
}
void Meta::inid_cb(Fl_Input* o){
    metadata.set_id(inid->value());
}
void Meta::inauthor_cb(Fl_Input* o){
    metadata.set_author(inauthor->value());
}
void Meta::inpublish_cb(Fl_Input* o){
    metadata.set_publish(inpublish->value());
}
void Meta::indate_cb(Fl_Input* o){
    metadata.set_date(indate->value());
}

void Meta::today_cb(Fl_Input* o){
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%Y-%m-%d", timeinfo);
    metadata.set_date(buffer);
    indate->value(buffer);
}

static void info_cb(Fl_Input* o){
    const char *message =
            "Title - The title of the publication\n"
            "ID - A unique identifier for the book\n"
            "Language - The language of the publication\n"
            "Author - The author of the book\n"
            "Publisher - Name of publisher or your name if self published\n"
            "Date - Date the book was published\n";
    fl_message(message);
}

void Meta::create_cb(Fl_Input* o){
    if(metadata.get("title").empty()){
        const char *message = "Please enter a Title.";
        fl_message(message);
    }else if(metadata.get("id").empty()){
        const char *message = "Please enter an ID.";
        fl_message(message);
    }else if(metadata.get("lang").empty()){
        const char *message = "Please select a language.";
        fl_message(message);
    }else if(metadata.get("author").empty()){
        const char *message = "Please enter an author.";
        fl_message(message);
    }else if(metadata.get("date").empty()){
        const char *message = "Please enter a date.";
        fl_message(message);
    }else{
        mkstructure(metadata);
        const char *message = "Your file was created successfully.\n Enjoy.";
        fl_message(message);
    }
}

//PRINT
void Meta::print_cb(Fl_Input* o){
    metadata = mkcontent(metadata);
    metadata = mkcontainer(metadata);
    metadata = mktoc(metadata);
    code1buff->text(metadata.get("content_contents").c_str());
    code2buff->text(metadata.get("container_contents").c_str());
    code3buff->text(metadata.get("toc_contents").c_str());
    mywindow->redraw();
}

void Meta::clear_all_cb(Fl_Widget* o){
    if(fl_ask("Are you sure?") == 1){
        metadata.clear("files_list");
        metadata.clear("images_list");
        metadata.clear("content");
        metadata.clear("container");
        metadata.clear("toc");
        code1buff->text("");
        code2buff->text("");
        htmlbuff->text("");
        imagebuff->text("");
        intitle->value("");
        inid->value("");
        inlang->value("");
        inauthor->value("");
        inpublish->value("");
        indate->value("");
        mywindow->redraw();
    }
}

void close_cb( Fl_Widget* o, void*) {
   exit(0);
}

void os_linux_cb( Fl_Widget* o, void*){
    metadata.set_os(true);
    cout << "Linux" << endl;
}

void os_windows_cb( Fl_Widget* o, void*){
    metadata.set_os(false);
    cout << "Windows" << endl;
}

void about_cb(Fl_Widget* o, void*){
    about();
}

void drag_cb( Fl_Widget* o, void*){
    mywindow->redraw();
}

//MAIN----------------------------------------------------------------------MAIN

int main(){
    int w=1000; // Width
    int h=600;  // Height
    int tx=30;  // Text height, for single line
    mywindow = new Fl_Double_Window(w,h,"EPUB Creator");
    mywindow->color(Fl_Color(fl_rgb_color(222, 222, 222)));
    {
        mainmenu = new Fl_Menu_Bar(0, 0, w, tx);
            mainmenu->color(Fl_Color(fl_rgb_color(222, 222, 222)));     
            Fl_Menu_Item menu_menubar[] = {
                {"File", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
                {"OS",  0,   0, 0, FL_SUBMENU},
                    {"Linux", 0, (Fl_Callback*)os_linux_cb, 0, FL_MENU_RADIO|FL_MENU_VALUE},
                    {"Windows", 0, (Fl_Callback*)os_windows_cb, 0, FL_MENU_RADIO},
                {0},
                {"Exit", 0,  (Fl_Callback*)close_cb, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
                {0,0,0,0,0,0,0,0,0},
                {"Edit", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
                {"Update",0, (Fl_Callback*)Meta::print_cb,0,0, FL_NORMAL_LABEL,0,14,0},
                {"Clear",0, (Fl_Callback*)Meta::clear_all_cb,0,0, FL_NORMAL_LABEL,0,14,0},
                {0,0,0,0,0,0,0,0,0},
                {"About",0, (Fl_Callback*)about_cb,0,0, FL_NORMAL_LABEL,0, 14,0},
                {0,0,0,0,0,0,0,0,0}
            };
            mainmenu->menu(menu_menubar);

        tile = new Fl_Tile(0,0,w,h);
            tile->callback(drag_cb,0);
        {
            lefthalf = new Fl_Group(0, 0, w/2, h);
            {
                intitle = new Fl_Input(80, 40, w/2-100, tx, "Title");
                    intitle->callback((Fl_Callback*)Meta::intitle_cb);
                    intitle->tooltip("Please enter a title");

                inid = new Fl_Input(80, 80, w/2-100, tx, "ID");
                    inid->callback((Fl_Callback*)Meta::inid_cb);

                inlang = new Fl_Input_Choice(80, 120, w/2-100, tx, "Language");
                    inlang->add("English");
                    inlang->add("English - GB");
                    inlang->add("English - US");
                    inlang->add("French");
                    inlang->add("French - Canada");
                    inlang->add("Spanish");
                    inlang->add("German");
                    inlang->callback((Fl_Callback*)Meta::inlang_cb);

                inauthor = new Fl_Input(80, 160, w/2-100, tx, "Author");
                    inauthor->callback((Fl_Callback*)Meta::inauthor_cb);

                inpublish = new Fl_Input(80, 200, w/2-100, tx, "Publish");
                    inpublish->callback((Fl_Callback*)Meta::inpublish_cb);

                indate = new Fl_Input(80, 240, w/2-175, tx, "Date");
                    indate->callback((Fl_Callback*)Meta::indate_cb);
                    today = new Fl_Button(410, 240, 70, tx, "Today");
                        today->callback((Fl_Callback*)Meta::today_cb);
                        today->color(Fl_Color(fl_rgb_color(222, 222, 222)));

                tabs = new Fl_Tabs(80, 280, w/2-100, 210);
                {
                    // HTML tab
                    htmls = new Fl_Group(80, 310, w/2-100, 180, "HTML FIles");
                    htmls->color(Fl_Color(fl_rgb_color(222, 222, 222)));
                    {
                        htmlmenu = new Fl_Menu_Bar(90, 320, 200, tx);
                            htmlmenu->add("Select HTML Files", 0, Meta::openhtml_cb);
                            htmlmenu->add("Clear", 1, Meta::clearhtml_cb);
                        htmltext = new Fl_Text_Display(90, 360, w/2-120, 120);
                        htmlbuff = new Fl_Text_Buffer();
                            htmltext->buffer(htmlbuff);
                    }
                    htmls->end();
                    
                    // Images tab
                    images = new Fl_Group(80, 310, w/2-100, 180, "Image FIles");
                    images->color(Fl_Color(fl_rgb_color(222, 222, 222)));
                    {
                        imagemenu = new Fl_Menu_Bar(90, 320, 200, tx);
                            imagemenu->add("Select Image Files", 0, Meta::openimage_cb);
                            imagemenu->add("Clear", 1, Meta::clearimages_cb);
                        imagetext = new Fl_Text_Display(90, 360, w/2-120, 120);
                        imagebuff = new Fl_Text_Buffer();
                            imagetext->buffer(imagebuff);
                            imagetext->resizable();
                    }
                    images->end();
                }
                tabs->end();

                close = new Fl_Button(80, h-50, 100, tx, " &Quit");
                    close->callback(close_cb);
                    close->image(image_exit);
                    close->align(Fl_Align(256));
                update = new Fl_Button(190, h-50, 100, tx, " &Update");
                    update->callback((Fl_Callback*)Meta::print_cb);
                    update->image(image_update);
                    update->align(Fl_Align(256));
                create = new Fl_Button(300, h-50, 100, tx, " &Create!");
                    create->callback((Fl_Callback*)Meta::create_cb);
                    create->image(image_create);
                    create->align(Fl_Align(256));                   
                info = new Fl_Button(410, h-50, 30, tx, "?");
                    info->callback((Fl_Callback*)info_cb);
                    info->labelcolor(FL_BLUE);
                hidden = new Fl_Box(440, h-50, 30, tx);
                    hidden->hide();
            }
            lefthalf->end();
            lefthalf->resizable(hidden);

            righthalf = new Fl_Box(500, 0, w/2, h);
            {
                outputtabs = new Fl_Tabs(500, 40, w/2, h-40);
                {
                    outputwindow1 = new Fl_Group(500, 70, w/2, h-70, "content.opf");
                    {
                        code1text = new Fl_Text_Editor(500, 70, w/2, h-70);
                        code1buff = new Fl_Text_Buffer();
                            code1text->buffer(code1buff);
                    }
                    outputwindow1->end();
                    outputwindow2 = new Fl_Group(500, 70, w/2, h-70, "container.xml");
                    {
                        code2text = new Fl_Text_Editor(500, 70, w/2, h-70);
                        code2buff = new Fl_Text_Buffer();
                            code2text->buffer(code2buff);                   
                    }
                    outputwindow2->end();
                    outputwindow3 = new Fl_Group(500, 70, w/2, h-70, "toc.ncx");
                    {
                        code3text = new Fl_Text_Editor(500, 70, w/2, h-70);
                        code3buff = new Fl_Text_Buffer();
                            code3text->buffer(code3buff);                   
                    }
                    outputwindow3->end();
                }
                tabs->end();
            }
        }
        tile->end();
    }
    mywindow->resizable(tile);
    mywindow->end();
    Fl::scheme("gtk+");
    mywindow->show();  

    Fl::run();
}
