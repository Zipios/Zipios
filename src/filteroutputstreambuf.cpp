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
    Implementation of FilterOutputStreambuf.
*/

#include "zipios++/filteroutputstreambuf.h"

#include "zipios++/zipiosexceptions.h"


namespace zipios
{


FilterOutputStreambuf::FilterOutputStreambuf( std::streambuf *outbuf, bool del_outbuf ) 
  : _outbuf( outbuf)
  , _del_outbuf( del_outbuf )
{
  if ( ! _outbuf )
  {
    throw InvalidStateException("FilterOutputStreambuf was called with a null streambuf pointer");
  }
}


FilterOutputStreambuf::~FilterOutputStreambuf()
{
  if ( _del_outbuf )
  {
    delete _outbuf ;
  }
}


} // zipios namespace
// vim: ts=2 sw=2 et
