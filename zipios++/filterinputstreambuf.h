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
    Header file that defines FilterInputStreambuf.
*/

#include "zipios++/meta-iostreams.h"

namespace zipios
{



/** An input streambuf filter is a streambuf that filters the input it
 gets from the streambuf it is attached to. FilterInputStreambuf is a base class to
 derive input streambuf filters from. */
class FilterInputStreambuf : public std::streambuf
{
public:
  /** Constructor.
      @param inbuf the streambuf to use for input.
      @param del_inbuf if true is specified inbuf will be deleted, when 
      the FilterInputStreambuf is destructed.
  */
  explicit FilterInputStreambuf( std::streambuf *inbuf, bool del_inbuf = false ) ;
  /** Destructor. */
  virtual ~FilterInputStreambuf() ;

protected:
  int _s_pos ; // Position in this streambuf - FIXME: is this used?
  std::streambuf *_inbuf ;
  bool _del_inbuf ;
private:

  /** Copy-constructor is private to prevent copying. */
  FilterInputStreambuf( FilterInputStreambuf const& src ) ;

  /** Copy-assignment operator is private to prevent copying.  */
  const FilterInputStreambuf& operator = ( FilterInputStreambuf const& src ) ;
};


} // namespace 

// vim: ts=2 sw=2 et
