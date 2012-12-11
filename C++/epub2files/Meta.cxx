//******************************************************************************
// Author: Josh Wainwright                                                     *
// Date: 30/11/2012                                                            *
// File Number: 02                                                             *
//                                                                             *
// Description: Function prototypes for the "Meta" class                       *
//******************************************************************************

#include <stdlib.h>
#include <iostream>

#include "Meta.h"

//Allows the retrieval of any of the listed variables from the Meta class. The 
// name of the variable must be used as the argument within quote marks/
string Meta::get(string get){
    if(get == "title"){return title;}
    else if(get == "title_under"){return title_under;}
    else if(get == "id"){return id;}
    else if(get == "lang"){return lang;}
    else if(get == "author"){return author;}
    else if(get == "publish"){return publish;}
    else if(get == "date"){return date;}
    else if(get == "content_contents"){return content_contents;}
    else if(get == "container_contents"){return container_contents;}
    else if(get == "toc_contents"){return toc_contents;}
    else if(get == "uuid"){return uuid;}
    else{return 0;}
}

//Allows the retrieval of some of the more varied variables. These all simply 
// return the same data type as the included variable.
int Meta::get_files_size(){return files_list.size();}
int Meta::get_images_size(){return images_list.size();}
bool Meta::get_os(){return LINUX;}
string Meta::get_file(int i){return files_list[i];}
string Meta::get_image(int i){return images_list[i];}

//Allows the large strings to be set to blank if the user wants to clear 
// everything that they have entered. There is no clear function for the other
// variables as the relevant input can be set to zero individually.
void Meta::clear(string list){
    if(list == "files_list"){files_list.clear();}
    else if(list == "images_list"){images_list.clear();}
    else if(list == "content"){content_contents = "";}
    else if(list == "container"){container_contents = "";}
    else if(list == "toc"){toc_contents = "";}
}

//Functions to set the value of each of the different variables. Most simply 
// replace the existing data with the new, but some concatenate the new onto the
// existing, hence the clear functions for these cases.
void Meta::set_title(string title_set){title = title_set;}
void Meta::set_title_under(string title_under_set){title_under=title_under_set;}
void Meta::set_id(string id_set){id = id_set;}
void Meta::set_lang(string lang_set){lang = lang_set;}
void Meta::set_author(string author_set){author = author_set;}
void Meta::set_publish(string publish_set){publish = publish_set;}
void Meta::set_date(string date_set){date = date_set;}
void Meta::set_files(string file){files_list.push_back(file);}
void Meta::set_images(string image){images_list.push_back(image);}
void Meta::set_uuid(string uuid_set){uuid = uuid_set;}
void Meta::set_os(bool os){LINUX = os;}

void Meta::set_content(string content){content_contents += content;}
void Meta::set_container(string container){container_contents += container;}
void Meta::set_toc(string toc){toc_contents += toc;}

