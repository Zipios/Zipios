
#ifdef HAVE_STD_IOSTREAM
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <vector>

#include <sys/stat.h>

#include "dircoll.h"

namespace zipios {

using std::cerr ;
using std::endl ;
using std::vector ;

DirectoryCollection::DirectoryCollection( const string &path ) {
  // Check path is actually a directory
  if ( isDir( path ) ) {
    _filename = path ;
    _valid = true ;
  } else {
    _valid = false ;
  }
}

void DirectoryCollection::close() {
  _valid = false ;
}


vector< ConstEntryPointer > DirectoryCollection::entries() const {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to use an invalid DirectoryCollection" ) ;

  cerr << "DirectoryCollection::entries not implemented yet" << endl ;

  // FIXME: Return the real thing.
  return vector< ConstEntryPointer >() ; 
}


ConstEntryPointer
DirectoryCollection::getEntry( const string &name, 
			       MatchPath matchpath = MATCH ) const {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to use an invalid DirectoryCollection" ) ;

  if ( matchpath != MATCH ) {
    cerr << 
      "DirectoryCollection::getEntry not implemented for matchpath = IGNORE" 
	 << endl ;
    return 0 ;
  }

  return new DirEntry( name, "", _filename ) ;
}


istream *DirectoryCollection::getInputStream( const ConstEntryPointer &entry ) {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to use an invalid DirectoryCollection" ) ;

  return getInputStream( entry->getName() ) ;
}


istream *DirectoryCollection::getInputStream( const string &entry_name, 
					      MatchPath matchpath = MATCH ) {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to use an invalid DirectoryCollection" ) ;

  if ( matchpath != MATCH ) {
    cerr << 
      "DirectoryCollection::getInputStream not implemented for matchpath = IGNORE" 
	 << endl ;
    return 0 ;
  }

  string real_path( _filename + entry_name ) ;
  ifstream *ifs = new ifstream( real_path.c_str() ) ;
  if( ! *ifs ) {
    delete ifs ;
    return 0 ;
  } else 
    return ifs ;  
}


int DirectoryCollection::size() const {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to use an invalid DirectoryCollection" ) ;
  // FIXME: implement this method.
}


DirectoryCollection::~DirectoryCollection() {}


bool DirectoryCollection::isDir( const string &path ) const {
  struct stat buf ;
  // If stat fails something is wrong, and we
  // may as well return false.
  if( stat( path.c_str(), &buf ) != 0 ) 
    return false ;
  else
    return S_ISDIR( buf.st_mode ) ;
}


} // namespace

/** \file
    Implementation of DirectoryCollection.
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
