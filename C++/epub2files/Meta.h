//******************************************************************************
// Author: Josh Wainwright                                                     *
// Date: 30/11/2012                                                            *
// File Number: 01                                                             *
//                                                                             *
// Description: Header file for the "Meta" class. Here all variable types are  *
//              declared along with functions.                                 *
//******************************************************************************

#include <string>
#include <vector>

#include <FL/Fl.H>
#include <FL/Fl_Input.H>

using namespace std;

class Meta{
    private:
        string title;
        string title_under;
        string id;
        string lang;
        string author;
        string publish;
        string date;
        string uuid;
        bool LINUX=true;
        vector<string> files_list;
        vector<string> images_list;

        string content_contents;
        string container_contents;
        string toc_contents;
    public:
        string get(string get);
        bool get_os();
        int get_files_size();
        int get_images_size();
        string get_file(int i);
        string get_image(int i);

        void clear(string list);

        void set_title(string title_set);
        void set_title_under(string title_under);
        void set_id(string id_set);
        void set_lang(string lang_set);
        void set_author(string author_set);
        void set_publish(string publish_set);   
        void set_date(string date_set);
        void set_files(string files);
        void set_images(string images);
        void set_uuid(string uuid_set);
        void set_os(bool os);

        void set_content(string content);
        void set_container(string container);
        void set_toc(string toc);

        static void openimage_cb(Fl_Widget*, void*);
        static void openhtml_cb(Fl_Widget*, void*);
        static void clearhtml_cb(Fl_Widget*, void*);
        static void clearimages_cb(Fl_Widget*, void*);
        static void clear_all_cb(Fl_Widget*);
        
        static void inlang_cb(Fl_Widget*, void*);
        static void intitle_cb(Fl_Input* o);
        static void inid_cb(Fl_Input* o);
        static void inauthor_cb(Fl_Input* o);
        static void inpublish_cb(Fl_Input* o);
        static void indate_cb(Fl_Input* o);
        static void today_cb(Fl_Input* o);
        static void create_cb(Fl_Input* o);
        static void print_cb(Fl_Input* o);
};
