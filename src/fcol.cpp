
#include "config.h"

#include <algorithm>
#include <string>
#include <vector>

#include "fcol.h"

namespace fcol {

using std::find_if ;

// FIXME: make InvalidStateException message customized for
// subclasses. maybe make an InvalidStateException factory ;-)

vector< ConstEntryPointer > FileCollection::entries() const {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to get entries from an invalid FileCollection" ) ;
  return vector< ConstEntryPointer > ( _entries.begin(), _entries.end() ) ;
}

ConstEntryPointer FileCollection::getEntry( const string &name, 
					   MatchPath matchpath ) const {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to get an entry from an invalid FileCollection" ) ;

  vector< EntryPointer >::const_iterator iter ;
  if ( matchpath == MATCH )
    iter = find_if( _entries.begin(), _entries.end(), FileEntry::MatchName( name ) ) ;
  else
    iter = find_if( _entries.begin(), _entries.end(), FileEntry::MatchFileName( name ) ) ;
  if ( iter == _entries.end() )
    return 0 ;
  else
    return *iter ; 
}

string FileCollection::getName() const {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to get the name of an invalid FileCollection" ) ;
  return _filename ;
}


int FileCollection::size() const {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to get size of an invalid FileCollection" ) ;
  return _entries.size() ;
}

FileCollection::~FileCollection() {
}


} // namespace

/** \file
    Implementation of FileCollection.
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
