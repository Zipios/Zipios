
#include "zipios++/zipios-config.h"

#if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM)
#include <iostream>
#else
#include <iostream.h>
#endif

#include "zipios++/fcollexceptions.h"

namespace zipios {

using std::cerr ;
using std::endl ;

IOException::IOException() throw () 
  : _what( "I/O exception" ) {} //{ cerr << "IOException()" << endl ; }

IOException::IOException( const string &msg ) throw () 
  : _what( msg ) {} //{ cerr << "IOException()" << endl ; }

IOException::IOException( const IOException &src ) throw () 
  : _what( src._what ) {}


IOException &IOException::operator= ( const IOException &src ) throw () {
  _what = src._what ;
  return *this ;
}

  
const char *IOException::what() const throw () {
  return _what.c_str() ;
}

IOException::~IOException() throw () {} //{ cerr << "~IOException()" << endl ; }






FCollException::FCollException() throw () 
  : _what( "FileCollection exception" ) {} //{ cerr << "FCollException()" << endl ; }

FCollException::FCollException( const string &msg ) throw () 
  : _what( msg ) {} //{ cerr << "FCollException()" << endl ; }

FCollException::FCollException( const FCollException &src ) throw () 
  : _what( src._what ) {}


FCollException &FCollException::operator= ( const FCollException &src ) throw () {
  _what = src._what ;
  return *this ;
}

  
const char *FCollException::what() const throw () {
  return _what.c_str() ;
}

FCollException::~FCollException() throw () {} //{ cerr << "~FCollException()" << endl ; }






InvalidStateException::InvalidStateException() throw () 
  : _what( "InvalidState exception" ) {} //{ cerr << "InvalidStateException()" << endl ; }

InvalidStateException::InvalidStateException( const string &msg ) throw () 
  : _what( msg ) {} //{ cerr << "InvalidStateException()" << endl ; }

InvalidStateException::
InvalidStateException( const InvalidStateException &src ) throw () 
  : _what( src._what ) {}


InvalidStateException &InvalidStateException::
operator= ( const InvalidStateException &src ) throw () {
  _what = src._what ;
  return *this ;
}

  
const char *InvalidStateException::what() const throw () {
  return _what.c_str() ;
}

InvalidStateException::~InvalidStateException() throw () {} 
//{ cerr << "~InvalidStateException()" << endl ; }

} // namespace

/** \file
    Implementation of a number of Exceptions used by FileCollection and its
    subclasses.
*/

/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000  Thomas Søndergaard
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
