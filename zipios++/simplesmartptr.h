#ifndef SIMPLESMARTPTR_H
#define SIMPLESMARTPTR_H

#include "zipios++/zipios-config.h"

namespace zipios {

/** SimpleSmartPointer is a simple reference counting smart pointer
    template. The type pointed to must keep a reference count that is
    initialized to zero and accessible through the two methods void
    ref() const and unsigned int unref() const.
*/
template< class Type >
class SimpleSmartPointer {
public:
  Type *operator-> () const { return _p ;  }

  Type &operator* ()  const { return *_p ; }

  SimpleSmartPointer( Type *p = 0 ) : _p( p ) { ref() ; }

  SimpleSmartPointer( const SimpleSmartPointer &src ) : _p( src.get() ) { 
    ref() ; 
  }

  template< class T2 > SimpleSmartPointer( const SimpleSmartPointer< T2 > &src ) 
    : _p( src.get() ) { ref() ; }

  ~SimpleSmartPointer () { if ( unref() == 0 ) deleteIt() ; }

  SimpleSmartPointer &operator= ( const SimpleSmartPointer &src ) {
    ref( src.get() ) ;
    if ( unref() == 0 )
      deleteIt() ;
    _p = src.get() ;
    return *this ;
  }

  template< class T2 > 
  SimpleSmartPointer &operator= ( const SimpleSmartPointer< T2 > &src ) {
    ref( src.get() ) ;
    if ( unref() == 0 )
      deleteIt() ;
    _p = src.get() ;
    return *this ;
  }

  bool operator== ( const Type *p )                const { return _p == p     ; }
  bool operator!= ( const Type *p )                const { return _p != p     ; }
  bool operator== ( const SimpleSmartPointer &sp ) const { return _p == sp.get() ; }
  bool operator!= ( const SimpleSmartPointer &sp ) const { return _p != sp.get() ; }
  bool operator!  ()                               const { return ! _p        ; }
  // This next method is inspired by iostream, and is for use with 
  // if ( some_smart_pointer ).
  operator void*() const { return _p ? (void *)(-1) : (void *)(0) ; }

  Type *get() const { return _p ; }
private:
  template< class T2 >
  void ref( const T2 *ptr ) { if ( ptr ) ptr->ref() ; }

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
