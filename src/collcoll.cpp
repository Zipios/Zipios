
#include "config.h"

#ifdef HAVE_STD_IOSTREAM
#include <fstream>
#else
#include <fstream.h>
#endif

#include "collcoll.h"
#include "fcoll_common.h"

namespace zipios {

using std::ifstream ;

CollectionCollection *CollectionCollection::_inst = 0 ;


CollectionCollection::CollectionCollection() {
  _valid = true ; // we're valid even though we are empty!
}


bool CollectionCollection::addCollection( FileCollection &collection ) {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to add a FileCollection to an invalid CollectionCollection" ) ;
  if ( this == &collection )
    return false ;
  _collections.push_back( &collection ) ;
  return true ;
}


void CollectionCollection::close() {
  _valid = false ;
}


vector< ConstEntryPointer > CollectionCollection::entries() const {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to get entries from an invalid CollectionCollection" ) ;

  vector< ConstEntryPointer > all_entries ;
  vector< FileCollection * >::const_iterator it ;
  for ( it = _collections.begin() ; it != _collections.end() ; it++ )
    all_entries += (*it)->entries() ;
  return all_entries ;
}


ConstEntryPointer CollectionCollection::getEntry( const string &name, 
						  MatchPath matchpath ) const {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to get an entry from an invalid CollectionCollection" ) ;
  // Returns the first matching entry.
  vector< FileCollection * >::const_iterator it ;
  ConstEntryPointer cep ;
  for ( it = _collections.begin() ; it != _collections.end() ; it++ ) {
    cep = (*it)->getEntry( name, matchpath ) ;
    if ( cep )
      break ;
  }
  return cep ;
}


istream *CollectionCollection::getInputStream( const ConstEntryPointer &entry ) {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to get an input stream from an invalid CollectionCollection" ) ;

  return getInputStream( entry->getName() ) ;
}


istream *CollectionCollection::getInputStream( const string &entry_name, 
					       MatchPath matchpath ) {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to get an input stream from an invalid CollectionCollection" ) ;

  ConstEntryPointer ent = getEntry( entry_name, matchpath ) ;
  
  if ( ent == 0 )
    return 0 ;
  else
    return new ifstream( fullPath( ent->getName() ).c_str() ) ;
  
}


int CollectionCollection::size() const {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to get the size of an invalid CollectionCollection" ) ;
  int sz = 0 ;
  vector< FileCollection * >::const_iterator it ;
  for ( it = _collections.begin() ; it != _collections.end() ; it++ )
    sz += (*it)->size() ;
  return sz ;
}

//
// Protected methods
//
string CollectionCollection::fullPath( const string &name ) const {
  return _filename + name ;
}

} // namespace

/** \file
    Implementation of CollectionCollection.
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
