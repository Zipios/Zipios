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
    Implementation of FilterInputStreambuf.
*/

#include "zipios++/filterinputstreambuf.h"

#include "zipios++/zipiosexceptions.h"

namespace zipios
{

FilterInputStreambuf::FilterInputStreambuf( std::streambuf *inbuf, bool del_inbuf ) 
  : _inbuf( inbuf )
  , _del_inbuf( del_inbuf )
{
  if ( _inbuf == NULL )
  {
    throw InvalidStateException("FilterInputStreambuf was called with a null streambuf pointer");
  }
}


FilterInputStreambuf::~FilterInputStreambuf()
{
  if ( _del_inbuf )
  {
    delete _inbuf ;
  }
}


} // namespace
// vim: ts=2 sw=2 et
