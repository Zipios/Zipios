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
    Header file that defines VirtualSeeker.
*/

#include "zipios++/zipios-config.h"

#include <stdexcept>


namespace zipios
{


/** VirtualSeeker is a simple class that keeps track of a set of
    specified 'virtual' file endings that mark a subset of a real
    file. An example of its use (and its reason for existence) is to
    keep track of the file endings of a Zip file embedded in another
    file. */
class VirtualSeeker
{
public:
  inline VirtualSeeker( int start_offset = 0, int end_offset = 0) ;

  inline void setOffsets( int  start_offset, int  end_offset ) ;
  inline void getOffsets( int& start_offset, int& end_offset ) const ;
  inline int startOffset() const ;
  inline int endOffset() const ;
  inline void vseekg( std::istream& is, int offset, std::ios::seekdir sd ) const ;
  inline int vtellg( std::istream& is ) const ;

private:
  // start and end offsets
  int _s_off, _e_off ;
};



VirtualSeeker::VirtualSeeker( int start_offset, int end_offset ) 
  : _s_off( start_offset )
  , _e_off( end_offset   )
{
}


void VirtualSeeker::setOffsets( int start_offset, int end_offset )
{
  _s_off = start_offset ;
  _e_off = end_offset   ;
}


void VirtualSeeker::getOffsets( int& start_offset, int& end_offset ) const
{
  start_offset = _s_off ;
  end_offset   = _e_off ;
}


int VirtualSeeker::startOffset() const
{
  return _s_off ;
}


int VirtualSeeker::endOffset() const
{
  return _e_off ;
}


void VirtualSeeker::vseekg( std::istream &is, int offset, std::ios::seekdir sd ) const
{
  switch ( sd )
  {
  case std::ios::cur :
    is.seekg( offset, sd ) ;
    break;

  case std::ios::beg :
    is.seekg( offset + _s_off,  sd ) ;
    break;

  case std::ios::end :
    is.seekg( offset - _e_off, sd ) ;
    break;

  default :
    throw std::logic_error("VirtualSeekManager::vseekg(): error - unknown seekdir");

  }
}


int  VirtualSeeker::vtellg( std::istream &is ) const
{
  return static_cast< int >( is.tellg() ) - _s_off ;
}


} // namespace

// vim: ts=2 sw=2 et
