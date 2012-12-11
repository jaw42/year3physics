//******************************************************************************
// Author: Josh Wainwright                                                     *
// Date: 30/11/2012                                                            *
// File Number: 05                                                             *
//                                                                             *
// Description: Header file for the "epubfunctions.cxx" with function          *
//              prototypes                                                     *
//******************************************************************************

#include <string>

using namespace std;

string space_underscore(string text);

string space_backslash(string text);

string removenewline(string text);

string catfile(string filename);

void copy_file(string fromfile, string tofile);
