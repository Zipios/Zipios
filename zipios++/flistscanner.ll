%{
  #include "zipios++/zipios-config.h"

  #if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM)
  #include <iostream>
  #else
  #include <iostream.h>
  #endif
  #include <flistscanner.h>

  using namespace zipios ;
%}
%%

ROOTDIR[ \t]*\=[ \t]*     {
                            #if TDEBUG > 3
                              cerr << "Reading a ROOTDIR_EQ \"keyword\"" << endl ;
                            #endif
			      return S_KW_ROOTDIR_EQ ;
                          }

[a-zA-Z0-9\/\_\.\-]+      {
                            #if TDEBUG > 3
                              cerr << "Reading a S_STRING" << endl ;
                            #endif
			      return S_STRING ;
                          }

\"[a-zA-Z0-9\/\_\.\- ]+\" {
                            #if TDEBUG > 3
                              cerr << "Reading a S_STRING (quoted)" << endl ;
                            #endif
			      return S_STRING ;
                          }
                         
\:.*                     {
                            #if TDEBUG > 3
                              cerr << "Reading a comment" << endl ;
                            #endif
			      return S_COMMENT ;
                          }

"#".*                     {
                            #if TDEBUG > 3
                              cerr << "Reading a shell script # comment" << endl ;
                            #endif
			      return S_IGNORE_COMMENT ;
                          }

[ \t]+                    { 
                            #if TDEBUG > 3
                              cerr << "reading whitespace\n" << endl ;
                            #endif
			    break;
                          }
\n                        { 
                            #if TDEBUG > 3
                              cerr << "reading newline" << endl ;
                            #endif
			    return S_NEWLINE ;
                          }


.                         {
                            #if TDEBUG > 3
                              cerr << "reading unrecognized token" << endl ;
                            #endif
			    return S_UNRECOGNIZED ;
                          }
%%

int yywrap() {
  return 1;
}

/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000  Thomas Søndergaard
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/
