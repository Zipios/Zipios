#pragma once
/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000-2015  Thomas Sondergaard
  
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

/** \file
    Header file that defines SimpleSmartPointer and ReferenceCount.
*/

#include "zipios++/zipios-config.h"

namespace zipios
{

/** \brief A simple smart pointer using a reference count.
 *
 * SimpleSmartPointer is a simple reference counting smart pointer
 * template. The type pointed to must keep a reference count that is
 * accessible through the two methods void ref() const and unsigned
 * int unref() const. The type must also handle the reference count
 * properly. The easiest way to do that is to use the ReferenceCount
 * template class.
 *
 * \note
 * This implementation is not thread safe.
 */
template< class Type >
class SimpleSmartPointer
{
public:
  SimpleSmartPointer( Type *p = 0 )
    : _p( p )
  {
    ref() ;
  }

  template< class T2 > SimpleSmartPointer( SimpleSmartPointer< T2 > const& src )
    : _p( src.get() )
  {
    ref() ;
  }

  SimpleSmartPointer( SimpleSmartPointer const& src )
    : _p( src.get() )
  { 
    ref() ; 
  }

  ~SimpleSmartPointer ()
  {
    unref() ;
  }

  template< class T2 > 
  SimpleSmartPointer& operator = ( SimpleSmartPointer< T2 > const& src )
  {
    ref( src.get() ) ;
    unref() ;
    _p = src.get() ;
    return *this ;
  }

  SimpleSmartPointer& operator = ( SimpleSmartPointer const& src )
  {
    ref( src.get() ) ;
    unref() ;
    _p = src.get() ;
    return *this ;
  }

  SimpleSmartPointer& operator = ( Type *src )
  {
    ref( src ) ;
    unref() ;
    _p = src ;
    return *this ;
  }

  Type *operator -> () const
  {
    return _p ;
  }

  Type& operator * () const
  {
    return *_p ;
  }

  bool operator == ( const Type *p ) const
  {
    return _p == p ;
  }

  bool operator != ( const Type *p ) const
  {
    return _p != p ;
  }

  bool operator == ( SimpleSmartPointer const& rhs ) const
  {
    return _p == rhs.get() ;
  }

  bool operator != ( SimpleSmartPointer const& rhs ) const
  {
    return _p != rhs.get() ;
  }

  operator bool () const
  {
    return _p != nullptr ;
  }

  bool operator ! () const
  {
    return ! _p ;
  }

  // This next method is inspired by iostream, and is for use with 
  // if ( some_smart_pointer ).
  operator void * () const
  {
    return _p ? (void *)(-1) : (void *)(0) ;
  }

  Type *get() const
  {
    return _p ;
  }

  /** Returns the reference count - For debugging purposes. */
  unsigned int getReferenceCount() const
  {
    return _p->getReferenceCount();
  }

private:
  template< class T2 >
  void ref( T2 const *ptr )
  {
    if ( ptr )
    {
      ptr->ref() ;
    }
  }

  void ref() const
  {
    if ( _p )
    {
      _p->ref() ;
    }
  }

  void unref() const
  {
    if ( _p )
    {
      if ( _p->unref() == 0 )
      {
        delete _p ;
        const_cast<SimpleSmartPointer *>(this)->_p = 0;
      }
    }
  }

  Type *_p ;
};


/** \brief Helper class to add a reference count to your objects.
 *
 * ReferenceCount is useful to ensure proper handling of the
 * reference count for (objects of) classes handled through a
 * SimpleSmartPointer. Subclassing ReferenceCount is all a class
 * needs to become ready for being handled by
 * SimpleSmartPointer. Another way is to add a ReferenceCount member
 * variable to a class and write two methods 'void ref() const' and
 * 'unsigned int unref() const' that invoke the same methods in the
 * ReferenceCount variable.
 */
template< class Type >
class ReferenceCount
{
  /** SimpleSmartPointer needs to be a friend to invoke the private
   * ref() and unref() methods.
   */
  friend class SimpleSmartPointer< Type > ;
  friend class SimpleSmartPointer< const Type > ;

  /** Type also needs to be a friend to invoke the private ref() and
   *  unref() methods, in case Type doesn't want to inherit
   *  ReferenceCount and thus needs to invoke ref() and unref()
   *  through forwarding member functions.
   */
  //
  //  Originally the following template parameter was made a friend.
  //  This is not allowed by the standard so comment it out:
  //
  // friend Type ;
  //
  //  Initially hack things by making the necessary classes friends
  //  even though we don't know really which they are.  This is an
  //  Hideous Hack.
  friend class FileEntry ;
  friend class Bogus ;
  
public:
  /** Constructor intializes count to zero. */
  ReferenceCount()
    : _ref_count( 0 )
  {
  }

  /** Copy-constructor intializes count to zero. It doesn't copy it
      from src. */
  ReferenceCount( ReferenceCount const& )
    : _ref_count( 0 )
  {
  }

  /** The assignment operator does not copy the reference count, it
      leaves it unchanged.  */
  ReferenceCount const& operator = ( ReferenceCount const& src )
  {
    return *this;
  }

private:

  /** Increases the reference count. */
  void ref() const
  {
    ++_ref_count ;
  }

  /** Decreases the reference count. */
  unsigned int unref() const
  {
    return --_ref_count ;
  }

  /** Returns the reference count - For debugging purposes. */
  unsigned int getReferenceCount() const
  {
    return _ref_count;
  }

  /** Holds the actual reference count */
  mutable unsigned short _ref_count ;
};



} // zipios namespace

// vim: ts=2 sw=2 et
