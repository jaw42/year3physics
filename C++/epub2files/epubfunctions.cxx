//******************************************************************************
// Author: Josh Wainwright                                                     *
// Date: 30/11/2012                                                            *
// File Number: 06                                                             *
//                                                                             *
// Description: various functions used by the program to manipulate strings    *
//              etc.                                                           *
//******************************************************************************

#include <stdlib.h>

#include "epubfunctions.h"

//Converts all of the spaces in a string to underscores so that the string can 
// be used in functions.
string space_underscore(string text){
    for(int unsigned i=0;i<text.length();i++){
        if(text[i] == ' ') text[i] = '_';
    }
    return text;
}

//Adds a backslash before all spaces to escape the space, allows the string to 
// be used as a file location.
string space_backslash(string text){
    string newtext;
    for(int unsigned i=0;i<text.length();i++){
        if(text[i]==' '){
        //  text[i] = '\\';
            newtext += "\\ ";
        } else {
            newtext += text[i];
        }
    }
    return newtext;
}

//Removes all instances of a newline character from a string so that a different
// deliminator can be used to separate the string when read.
string removenewline(string text){
    for(int unsigned i=0;i<text.length();i++){
        if(text[i]=='\n') text[i] = ' ';
    }
    return text;
}

//Removes all of the string apart from the last section that contains a filename
// so that that filename can be used independantly.
string catfile(string filename){
    size_t found;
    found = filename.find_last_of("/\\");
    filename = filename.substr(found+1);
    return filename;
}

//Use system commands to copy a file from one location to 
void copy_file(string fromfile, string tofile){
//  fromfile = space_backslash(fromfile);
    fromfile = removenewline(fromfile);
//  tofile = space_backslash(tofile);
    string cp = "cp \"" + fromfile + "\" \"" + tofile + "\"";
    system(cp.c_str());
}
