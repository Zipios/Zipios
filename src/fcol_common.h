#ifndef FCOL_COMMON_H
#define FCOL_COMMON_H

#include "config.h"

#include <vector>

namespace fcol {

using std::vector ;

static const char separator = '/' ;

template< class Type >
void operator+=( vector< Type > &v1, const vector< Type > &v2 ) {
  vector< Type >::const_iterator cit ;
  for ( cit = v2.begin() ; cit != v2.end() ; cit++ )
    v1.push_back( *cit ) ;
}

}

#endif

/** \file
    Header file that doesn't really contain much!
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
