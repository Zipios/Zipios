
#include "config.h"

#ifdef HAVE_STD_IOSTREAM
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif

#include "backbuffer.h"
#include "fcol.h"
#include "zipfile.h"
#include "zipinputstream.h"


namespace zipios {

//
// Public
//


ZipFile::ZipFile( const string &name /* , ios::open_mode mode */ ) 
  : _zipfile      ( name.c_str() /* , mode */ )
{
  _filename = name ;
  
  init () ;
}

ZipFile::~ZipFile() {
  close() ;
}

void ZipFile::close() {
  _valid = false ;

  // FIXME: make sure it's not a problem to close an ifstream, that may not
  // have been opened. ie. and ifstream created by calling the default constructor
  
  // We actually don't need to keep the file open all the time.
  _zipfile.close() ;
}

istream *ZipFile::getInputStream( const ConstEntryPointer &entry ) {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to use an invalid FileCollection" ) ;
  return getInputStream( entry->getName() ) ;
}

istream *ZipFile::getInputStream( const string &entry_name, 
				  MatchPath matchpath = MATCH ) {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to use an invalid ZipFile" ) ;

  ConstEntryPointer ent = getEntry( entry_name, matchpath ) ;
  
  if ( ent == 0 )
    return 0 ;
  else
    return new ZipInputStream( _filename,	
			   static_cast< const ZipCDirEntry * >( ent.get() )->
			   getLocalHeaderOffset() ) ;
}



//
// Private
//

bool ZipFile::init() {

  // Check stream error state
  if ( ! _zipfile ) {
    setError ( "Error reading from file" ) ;
    return false ;
  }
  
  _valid = readCentralDirectory() ;

  return _valid ;
}


bool ZipFile::readCentralDirectory () {
  // Find and read eocd. 
  if ( ! readEndOfCentralDirectory() ) {
    cerr << "Could not find eocd" << endl ;
  }
//    cerr << "FOUND HEADER" << endl ;
  // Position read pointer to start of first entry in central dir.
  _zipfile.seekg( _eocd.offset(), ios::beg ) ;

  int entry_num = 0 ;
  _entries.resize ( _eocd.totalCount() ) ;
  while ( ( entry_num < _eocd.totalCount() ) ) {
    ZipCDirEntry *ent = new ZipCDirEntry ; 
    _entries[ entry_num ] = ent ;
    _zipfile >>  *ent ;
    if ( ! _zipfile ) {
      cerr << "BAD STREAM STATE" << endl ;
      if ( _zipfile.eof()  ) { return false ; } else
      if ( _zipfile.bad()  ) { return false ; } else
      if ( _zipfile.fail() ) { return false ; }
    }
    ++entry_num ;
  }
  // Consistency check. eocd should start here
  int pos = _zipfile.tellg() ;
  _zipfile.seekg( 0, ios::end ) ;
  int remaining = static_cast< int >( _zipfile.tellg() ) - pos ;
  if ( remaining != _eocd.eocdOffSetFromEnd() ) {
    cerr << "Zip file consistency error." << endl ;
    cerr << "  End-of-central-directory record starts at -" ;
    cerr << _eocd.eocdOffSetFromEnd() << " from eof" << endl ;
    cerr << "  It's expected to follow last file header at -" ;
    cerr << remaining << endl ;
    return false ;
  }
  // Consistency check 2, are local headers consistent with
  // cd headers
  if ( ! confirmLocalHeaders() ) {
    cerr << "Local headers are inconsistent with central directory entries" << endl ;
    return false ;
  }
  
  return true ;
}


bool ZipFile::readEndOfCentralDirectory () {
  BackBuffer bb( _zipfile ) ;
  int read_p = -1 ;
  bool found = false ;
  while ( ! found ) {
    if ( read_p < 0 )
      if ( ! bb.readChunk ( read_p ) ) {
	found = false ;
	break ;
      }
    if ( _eocd.read( bb, read_p ) ) {
      found = true ;
      break ;
    }
//      cerr << "bb : " << static_cast< int >( bb[ read_p ] ) << endl ;
    --read_p ;
//      cerr << read_p << endl ;
  }

  return found ;
}

bool ZipFile::confirmLocalHeaders() {
  vector< EntryPointer >::const_iterator it ;
  ZipCDirEntry *ent ;
  int inconsistencies = 0 ;
  ZipLocalEntry zlh ;
  for ( it = _entries.begin() ; it != _entries.end() ; it++ ) {
    ent = static_cast< ZipCDirEntry * >( (*it).get()  ) ;
    _zipfile.seekg( ent->getLocalHeaderOffset(), ios::beg ) ;
    _zipfile >> zlh ;
//      cerr << zlh ;
    if ( ! _zipfile || zlh != *ent ) {
      inconsistencies++ ;
      _zipfile.clear() ;
    }
  }
  return ! inconsistencies ;
}

void ZipFile::setError ( string error_str ) {
  _valid = false ;
#ifdef _USE_EXCEPTIONS
    throw  error_str ; // define exception class instead.
#else
    cerr << error_str << endl ; // define operator<< for exception class if such a class replaces string
#endif
}


}

/** \file
    The implementation of ZipFile.
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
