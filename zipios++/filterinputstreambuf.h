#ifndef IFILTSTREAMBUF_H
#define IFILTSTREAMBUF_H

#include "zipios++/zipios-config.h"

// This includes the right streambuf header file
#if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM)
#include <iostream>
#else
#include <iostream.h>
#endif

namespace zipios {

using std::streambuf ;

/** An input streambuf filter is a streambuf that filters the input it
 gets from the streambuf it is attached to. FilterInputStreambuf is a base class to
 derive input streambuf filters from. */
class FilterInputStreambuf : public streambuf {
public:
  /** Constructor.
      @param inbuf the streambuf to use for input.
      @param del_inbuf if true is specified inbuf will be deleted, when 
      the FilterInputStreambuf is destructed.
  */
  explicit FilterInputStreambuf( streambuf *inbuf, bool del_inbuf = false ) ;
  /** Destructor. */
  virtual ~FilterInputStreambuf() ;

protected:
  int _s_pos ; // Position in this streambuf - FIXME: is this used?
  streambuf *_inbuf ;
  bool _del_inbuf ;
};


} // namespace 


#endif

/** \file
    Header file that defines FilterInputStreambuf.
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
