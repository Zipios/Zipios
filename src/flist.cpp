
#include "config.h"

#ifdef HAVE_STD_IOSTREAM
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <string>
#include <vector>
#ifdef HAVE_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#endif

#include "fcoll_common.h"
#include "flistscanner.h"
#include "flist.h"

namespace zipios {

using std::ifstream ;
#ifdef HAVE_SSTREAM
using std::ostringstream ;
#else
using std::ostrstream ;
using std::ends ;
#endif

//
// Public definitions
//

FileList::FileList( const string &name ) {
  _filename = name ;
  _valid = false ;
  ifstream list_file( name.c_str() ) ;
  
  if ( ! list_file ) {
#ifdef HAVE_SSTREAM
    ostringstream err_msg ;
#else
    ostrstream err_msg ;
#endif
    err_msg << "Could not open file " << _filename << " for reading" ;
#ifndef HAVE_SSTREAM
    err_msg << ends ; // null terminate ostrstream
    err_msg.freeze( 0 ) ; // Deallocate the string when ostrstream is destroyed.
#endif
    throw IOException ( err_msg.str() ) ;
  }
  init( list_file ) ;

}


FileList::FileList( ifstream &is ) {
  _valid = false ;
  init( is ) ;
}


void FileList::close () {
  // Does not need to do anything for FileList.
}

istream *FileList::getInputStream( const ConstEntryPointer &entry ) {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to use an invalid FileCollection" ) ;
  return getInputStream( entry->getName() ) ;
}

istream *FileList::getInputStream( const string &entry_name, 
				   MatchPath matchpath = MATCH ) {
  if ( ! _valid )
    throw InvalidStateException( "Attempt to use an invalid FileList" ) ;
  return new ifstream( entry_name.c_str() ) ;

  ConstEntryPointer ent = getEntry( entry_name, matchpath ) ;
  
  if ( ent == 0 )
    return 0 ;
  else
    return new ifstream( ent->getName().c_str() ) ;
}

//
// Protected definitions
//

//
// Private definitions
//

bool FileList::init( istream &is ) {
  if ( ! is ) {
    _valid = false ;
    throw IOException() ;
  }
#ifdef HAVE_SSTREAM
  ostringstream out_ss ;
#else
  ostrstream out_ss ;
#endif
  FListScanner fls ( &is, &out_ss ) ;
  
  string rootdir ;
  string global_comment ;

  string filename ;
  string filecomment ;
  int files = 0 ;
  int token ;
  bool success = true ;

  token = fls.yylex();
  while ( token && success ) {
    switch ( token ) {

    case S_KW_ROOTDIR_EQ :
      token = fls.yylex() ;
      if ( token == S_STRING ) {
	rootdir = fls.YYText() ;
	if ( rootdir[ rootdir.length()-1 ] != separator )
	  rootdir += separator ;
	token = fls.yylex() ;
      } else {
	success = false ;
	throw fcollException( "Parse error. 'ROOTDIR =' not followed by path" ) ;
      }
      break ;

    case S_STRING :
      if ( files > 0 )
	_entries.push_back( new FListEntry( filename, filecomment ) ) ;
      filename = rootdir + fls.YYText() ;
      filecomment = "" ;
      ++files ;

      token = fls.yylex() ;
      break ;

    case S_COMMENT :
      if ( files == 0 )
	global_comment.append( fls.YYText() ) ;
      else
	filecomment.append( fls.YYText() ) ;
      token = fls.yylex() ;
      break;

    case S_IGNORE_COMMENT :
    case S_NEWLINE :
      token = fls.yylex() ;
      break ;

    default :
      success = false ;
      throw fcollException( "Unrecognized token in input stream" ) ;
      break ;
    }
  }
  if ( ( files > 0 ) && success )
    _entries.push_back( new FListEntry( filename, filecomment ) ) ;

  // Traverse _entries to make sure they're all valid.
  vector< EntryPointer >::iterator iter ;

  _valid = true ;
  for ( iter = _entries.begin() ; iter != _entries.end() ; iter++ )
    if ( ! (*iter)->isValid() ) {
      _valid = false ;
#ifdef HAVE_SSTREAM
      ostringstream err_msg ;
#else
      ostrstream err_msg ;
#endif
      err_msg << "Listed entry " << (*iter)->getName() << " is not valid" ;
#ifndef HAVE_SSTREAM
      err_msg << ends ; // null terminate ostrstream
      err_msg.freeze( 0 ) ; // Deallocate the string when ostrstream is destroyed.
#endif
      throw fcollException( err_msg.str() ) ;
    }

  return success ;
}


} // namespace

/** \file
    Implementation of FileList.
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
