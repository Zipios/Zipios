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
    Implementation of CollectionCollection.
*/

#include "zipios++/collcoll.h"

#include "zipios++/zipiosexceptions.h"

#include "zipios_common.h"


namespace zipios
{


CollectionCollection *CollectionCollection::_inst = 0 ;


CollectionCollection::CollectionCollection()
{
  _valid = true ; // we are valid even though we are empty!
}


bool CollectionCollection::addCollection( FileCollection const& collection )
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to add a FileCollection to an invalid CollectionCollection" ) ;
  }

  if ( this == &collection || ! collection.isValid()  )
  {
    return false ;
  }

  _collections.push_back( collection.clone() ) ;

  return true ;
}


bool CollectionCollection::addCollection( FileCollection *collection )
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to add a FileCollection to an invalid CollectionCollection" ) ;
  }

  if ( collection == 0 || this == collection || ! collection->isValid() )
  {
    return false ;
  }

  _collections.push_back( collection ) ;

  return true ;
}


void CollectionCollection::close()
{
  _valid = false ;
}


ConstEntries CollectionCollection::entries() const
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to get entries from an invalid CollectionCollection" ) ;
  }

  ConstEntries all_entries ;
  for ( std::vector< FileCollection * >::const_iterator it = _collections.begin() ; it != _collections.end() ; it++ )
  {
    all_entries += (*it)->entries() ;
  }

  return all_entries ;
}


ConstEntryPointer CollectionCollection::getEntry( std::string const& name, 
                                                  MatchPath matchpath ) const
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to get an entry from an invalid CollectionCollection" ) ;
  }

  // Returns the first matching entry.
  std::vector< FileCollection * >::const_iterator it ;
  ConstEntryPointer cep ;

  getEntry( name, cep, it, matchpath ) ; 

  return cep ;
}


std::istream *CollectionCollection::getInputStream( ConstEntryPointer const& entry )
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to get an input stream from an invalid CollectionCollection" ) ;
  }

  return getInputStream( entry->getName() ) ;
}


std::istream *CollectionCollection::getInputStream( std::string const& entry_name, 
                                               MatchPath matchpath )
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to get an input stream from an invalid CollectionCollection" ) ;
  }

  std::vector< FileCollection * >::const_iterator it ;
  ConstEntryPointer cep ;

  getEntry( entry_name, cep, it, matchpath ) ; 

  return cep ? (*it)->getInputStream( entry_name ) : 0 ;
}


int CollectionCollection::size() const
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to get the size of an invalid CollectionCollection" ) ;
  }

  int sz = 0 ;
  std::vector< FileCollection * >::const_iterator it ;
  for ( it = _collections.begin() ; it != _collections.end() ; it++ )
  {
    sz += (*it)->size() ;
  }

  return sz ;
}


FileCollection *CollectionCollection::clone() const
{
  return new CollectionCollection( *this ) ;
}


CollectionCollection::~CollectionCollection()
{
  std::vector< FileCollection * >::iterator it ;
  for ( it = _collections.begin() ; it != _collections.end() ; ++it )
  {
    delete *it ;
  }
}


//
// Protected member functions
//

void CollectionCollection::getEntry( std::string const& name,
                                     ConstEntryPointer& cep, 
                                     std::vector< FileCollection * >::const_iterator& it, 
                                     MatchPath matchpath ) const
{
  // Returns the first matching entry.
  cep = 0 ;
  for ( it = _collections.begin() ; it != _collections.end() ; it++ )
  {
    cep = (*it)->getEntry( name, matchpath ) ;
    if ( cep )
    {
      break ;
    }
  }
}


} // zipios namespace
// vim: ts=2 sw=2 et
