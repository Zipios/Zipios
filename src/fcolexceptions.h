#ifndef FCOLEXCEPTIONS_H
#define FCOLEXCEPTIONS_H

#include "config.h"

#include <stdexcept>
#include <string>

namespace zipios {

using std::string ;
using std::exception ;

/** An IOException is used to signal an I/O error.
 */
class IOException : public exception {
public:
  explicit IOException() throw () ;
  explicit IOException( const string &msg ) throw () ;
  explicit IOException( const IOException &src ) throw () ;
  IOException &operator= ( const IOException &src ) throw () ;
  
  virtual const char *what() const throw () ;
  virtual ~IOException() throw () ;
private:
  string _what ;
};

/** An FColException is used to signal a problem with a
 FileCollection. */
class FColException : public exception {
public:
  explicit FColException() throw () ;
  explicit FColException( const string &msg ) throw () ;
  explicit FColException( const FColException &src ) throw () ;
  FColException &operator= ( const FColException &src ) throw () ;
  
  virtual const char *what() const throw () ;
  virtual ~FColException() throw () ;
private:
  string _what ;
};

/** An object member function may throw this exception, if the
    operation it normally performs is inappropriate or impossible to
    perform because of the current state of the object. */
class InvalidStateException : public exception {
public:
  explicit InvalidStateException() throw () ;
  explicit InvalidStateException( const string &msg ) throw () ;
  explicit InvalidStateException( const InvalidStateException &src ) throw () ;
  InvalidStateException &operator= ( const InvalidStateException &src ) throw () ;
  
  virtual const char *what() const throw () ;
  virtual ~InvalidStateException() throw () ;
private:
  string _what ;
};

} // namespace
#endif

/** \file
    Header file that defines a number of exceptions used by FileCollection and
    its subclasses.
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
