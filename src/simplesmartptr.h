#ifndef SIMPLESMARTPTR_H
#define SIMPLESMARTPTR_H

#include "config.h"

namespace fcol {

/** SimpleSmartPointer is a simple reference counting smart pointer
    template. The type pointed to must keep a reference count that is
    initialized to zero and accessible through the two methods void
    ref() const and unsigned int unref() const.
*/
template< class Type >
class SimpleSmartPointer {
  template< class T > friend class SimpleSmartPointer ;
public:
  Type *operator-> () const { return _p ;  }

  Type &operator* ()  const { return *_p ; }

  SimpleSmartPointer( Type *p = 0 ) : _p( p ) { ref() ; }

  SimpleSmartPointer( const SimpleSmartPointer &src ) : _p( src._p ) { 
    ref() ; 
  }

  template< class T2 > SimpleSmartPointer( const SimpleSmartPointer< T2 > &src ) 
    : _p( src._p ) { ref() ; }

  ~SimpleSmartPointer () { if ( unref() == 0 ) deleteIt() ; }

  SimpleSmartPointer &operator= ( const SimpleSmartPointer &src ) {
    src.ref() ;
    if ( unref() == 0 )
      deleteIt() ;
    _p = src._p ;
    return *this ;
  }

  template< class T2 > 
  SimpleSmartPointer &operator= ( const SimpleSmartPointer< T2 > &src ) {
    src.ref() ;
    if ( unref() == 0 )
      deleteIt() ;
    _p = src._p ;
    return *this ;
  }

  bool operator== ( const Type *p ) { return _p == p ; }
  bool operator!= ( const Type *p ) { return _p != p ; }
  bool operator== ( const SimpleSmartPointer &sp ) { return _p == sp._p ; }
  bool operator!= ( const SimpleSmartPointer &sp ) { return _p != sp._p ; }
  bool operator!  () { return ! _p ; }
  // This next method is inspired by iostream, and is for use with 
  // if ( some_smart_pointer ).
  operator void*() { return _p ? (void *)0 : (void *)(-1) ; }

  Type *get() const { return _p ; }
private:
  void ref() const { if ( _p ) _p->ref() ; }
  unsigned int unref() const {
    if ( _p )
      return _p->unref();
    else
      return 0 ;
  }
  void deleteIt() {
//      if( _p )
//        cerr << "SimpleSmartPointer: Deleting object!" << endl ;
    delete _p ;
  }
  Type *_p ;
};


} // namespace

#endif

/** \file
    Header file that defines SimpleSmartPointer.
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
