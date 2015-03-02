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
    Implementation of DirectoryCollection.
*/

#include "zipios++/dircoll.h"

#include "zipios++/zipiosexceptions.h"

#include "directory.h"

namespace zipios
{

DirectoryCollection::DirectoryCollection( std::string const& path,
                                          bool recursive, 
                                          bool load_now ) 
  : _entries_loaded( false )
  , _recursive     ( recursive )
  , _filepath      ( path      )
{
  _filename = _filepath ;
  _valid = _filepath.isDirectory() ;

  if( _valid && load_now )
  {
    loadEntries() ;
  }
}


void DirectoryCollection::close()
{
  _valid = false ;
}


ConstEntries DirectoryCollection::entries() const
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to use an invalid DirectoryCollection" ) ;
  }

  loadEntries() ;

  return FileCollection::entries() ;
}


ConstEntryPointer DirectoryCollection::getEntry( std::string const& name, 
                                                 MatchPath matchpath ) const
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to use an invalid DirectoryCollection" ) ;
  }

  if ( matchpath != MatchPath::MATCH || _entries_loaded )
  {
    loadEntries() ;
    return FileCollection::getEntry( name, matchpath ) ;
  }

  // avoid loading entries if possible.
  ConstEntryPointer ent ( new DirEntry( name, "", _filepath ) ) ;
  if ( ent->isValid() )
  {
    return ent ;
  }

  return 0 ;
}


std::istream *DirectoryCollection::getInputStream( ConstEntryPointer const& entry )
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to use an invalid DirectoryCollection" ) ;
  }

  return getInputStream( entry->getName() ) ;
}


std::istream *DirectoryCollection::getInputStream( std::string const& entry_name, 
                                                   MatchPath matchpath )
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to use an invalid DirectoryCollection" ) ;
  }

  if ( matchpath != MatchPath::MATCH || _entries_loaded )
  {
    loadEntries() ;

    ConstEntryPointer ent = getEntry( entry_name, matchpath ) ;
    if ( ! ent )
    {
      return 0 ;
    }

    std::string real_path( _filepath + entry_name ) ;
    return new std::ifstream( real_path.c_str(), std::ios::in | std::ios::binary ) ;
  }

  // avoid loading entries if possible.
  std::string real_path( _filepath + entry_name ) ;
  std::ifstream *ifs = new std::ifstream( real_path.c_str(), std::ios::in | std::ios::binary ) ;
  if( ! *ifs )
  {
    delete ifs ;
    return 0 ;
  }

  return ifs ;
}


int DirectoryCollection::size() const
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to use an invalid DirectoryCollection" ) ;
  }

  loadEntries() ;

  return _entries.size() ;
}


FileCollection *DirectoryCollection::clone() const
{
  return new DirectoryCollection( *this ) ;
}


DirectoryCollection::~DirectoryCollection()
{
}


void DirectoryCollection::loadEntries() const
{
  if( _entries_loaded )
  {
    return ;
  }

  const_cast< DirectoryCollection * >( this )->load( _recursive ) ;

  _entries_loaded = true ;
}


void DirectoryCollection::load( bool recursive, const FilePath &subdir )
{
  //using namespace boost::filesystem ;
  BasicEntry *ent ;
  for ( boost::filesystem::dir_it it( _filepath + subdir ) ; it != boost::filesystem::dir_it() ; ++it )
  {
    // TBD: skipping "..." ?!?
    if ( *it == "." || *it == ".." || *it == "..." )
    {
      continue ;
    }

    if ( boost::filesystem::get< boost::filesystem::is_directory >( it ) && recursive )
    {
      load( recursive, subdir + *it ) ;
    }
    else
    {
      _entries.push_back( ent = new BasicEntry( subdir + *it, "", _filepath ) ) ;
      ent->setSize( boost::filesystem::get< boost::filesystem::size >( it ) ) ;
    }
  }
}


} // zipios namespace
// vim: ts=2 sw=2 et
