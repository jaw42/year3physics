//******************************************************************************
// Author: Josh Wainwright                                                     *
// Date: 30/11/2012                                                            *
// File Number:07                                                             *
//                                                                             *
// Description: Header file for the "epubmk.cxx" with function prototypes      *
//******************************************************************************

#include <string>

#include "Meta.h"

using namespace std;

Meta mkcontent(Meta metadata);

Meta mkcontainer(Meta metadata);

Meta mktoc(Meta metadata);

void mkstructure(Meta metadata);
