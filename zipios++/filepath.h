#ifndef FILEPATH_H
#define FILEPATH_H

#include "zipios++/zipios-config.h"

#include <stdexcept>
#include <string>

namespace zipios {

using namespace std    ;

class FilePathException : public exception {
public:
  FilePathException() throw ()                               : _what( "I/O exception" ){}
  explicit FilePathException( const string &msg ) throw ()   : _what( msg ) {}
  FilePathException( const FilePathException &src ) throw () : _what( src._what ) {}
  FilePathException &operator= ( const FilePathException &src ) throw () { _what = src._what ; return *this ; }
  virtual const char *what() const throw ()                  { return _what.c_str() ; }
  virtual ~FilePathException() throw ()                      {}
private:
  string _what ;
};


/**  */
class FilePath {
public:
  FilePath( const string &path = "", bool check_exists = false, 
	    bool must_exist = false ) throw ( FilePathException ) ;

  inline FilePath &operator= ( const string &rhs ) ;

  inline operator string() const ;

  inline FilePath operator+ ( const FilePath &name ) const ;

  inline FilePath filename() const ;

  inline bool exists()         const ;
  inline bool isRegular()      const ;
  inline bool isDirectory()    const ;
  inline bool isCharSpecial()  const ;
  inline bool isBlockSpecial() const ;
  inline bool isSocket()       const ;
  inline bool isFifo()         const ;
protected:
  inline void pruneTrailingSeparator() ;
  void check() const ;

  static const char _separator = '/' ;

  // FIXME: Should be bitfield
  mutable bool   _checked   ;
  mutable bool   _exists    ;
  mutable bool   _is_reg    ;
  mutable bool   _is_dir    ;
  mutable bool   _is_char   ;
  mutable bool   _is_block  ;
  mutable bool   _is_socket ;
  mutable bool   _is_fifo   ;
  string _path              ;
};


//
// Inline member functions
//

FilePath &FilePath::operator= ( const string &rhs ) {
  _path = rhs ;
  pruneTrailingSeparator() ;
  return *this ;
}

void FilePath::pruneTrailingSeparator() {
  if ( _path.size() > 0 )
    if ( _path[ _path.size() -1 ] == _separator )
      _path.erase( _path.size() - 1 ) ; 
}

FilePath::operator string() const { 
  return _path ;
} 


FilePath FilePath::operator+ ( const FilePath &name ) const { 
  if ( _path.size() > 0 )
    return _path + _separator + name._path ; 
  else
    return name._path ;
}


FilePath FilePath::filename() const {
  string::size_type pos ;
  pos = _path.find_last_of( _separator ) ;
  if ( pos != string::npos )
    return string( _path, pos + 1 ) ;
  else 
    return _path ;
}


bool FilePath::exists() const {
  if ( ! _checked )
    check() ;
  return _exists ;
}


bool FilePath::isRegular() const {
  if ( ! _checked )
    check() ;
  return _is_reg ;
}


bool FilePath::isDirectory() const {
  if ( ! _checked )
    check() ;
  return _is_dir ;
}


bool FilePath::isCharSpecial() const {
  if ( ! _checked )
    check() ;
  return _is_char ;
}


bool FilePath::isBlockSpecial() const {
  if ( ! _checked )
    check() ;
  return _is_block ;
}


bool FilePath::isSocket() const {
  if ( ! _checked )
    check() ;
  return _is_socket ;
}


bool FilePath::isFifo() const {
  if ( ! _checked )
    check() ;
  return _is_fifo ;
}


} // namespace
#endif

/** \file
    Header file that defines FilePath.
*/

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
