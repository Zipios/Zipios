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
 * \brief The implementation of ZipFile.
 */


#include "zipios++/zipfile.h"

#include "zipios++/zipiosexceptions.h"

#include "zipios++/zipinputstream.h"


#include "backbuffer.h"

namespace zipios
{

//
// Public
//

ZipFile ZipFile::openEmbeddedZipFile( std::string const& name )
{
  // open zipfile, read 4 last bytes close file
  // create ZipFile object.
  std::ifstream ifs( name.c_str(), std::ios::in | std::ios::binary ) ;
  ifs.seekg( -4, std::ios::end ) ;
  uint32_t const start_offset( readUint32( ifs ) );
  ifs.close() ;
  return ZipFile( name, start_offset, 4 ) ;
}


ZipFile::ZipFile( std::string const& name , int s_off, int e_off )
  : _vs( s_off, e_off )
{
  _filename = name ;
  std::ifstream _zipfile( _filename.c_str(), std::ios::in | std::ios::binary ) ;
  init( _zipfile ) ;
}


FileCollection *ZipFile::clone() const
{
  return new ZipFile( *this ) ;
}


ZipFile::~ZipFile()
{
  close() ;
}


void ZipFile::close()
{
  _valid = false ;
}


std::istream *ZipFile::getInputStream( ConstEntryPointer const& entry )
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to use an invalid FileCollection" ) ;
  }

  return getInputStream( entry->getName() ) ;
}


std::istream *ZipFile::getInputStream( std::string const& entry_name,
                                       MatchPath matchpath )
{
  if ( ! _valid )
  {
    throw InvalidStateException( "Attempt to use an invalid ZipFile" );
  }

  ConstEntryPointer ent = getEntry( entry_name, matchpath ) ;
  if ( ! ent )
  {
    return 0 ;
  }

  ZipInputStream *zis( new ZipInputStream( _filename,
      static_cast< ZipCDirEntry const * >( ent.get() )->getLocalHeaderOffset()
                                                       + _vs.startOffset() ) ) ;
  //
  // Wed Mar 19 18:16:34 PDT 2014 (RDB)
  // This was causing a basic_ios::clear exception.
  // Not sure why we would need to do this here!
  //zis->getNextEntry();
  return zis;
}


//
// Private
//

bool ZipFile::init( std::istream& _zipfile )
{
  // Check stream error state
  if ( ! _zipfile )
  {
    _valid = false ;
    // was returning false in previous versions...
    throw IOException( "Error reading from file" ) ;
  }

  _valid = readCentralDirectory( _zipfile ) ;

  return _valid ;
}


bool ZipFile::readCentralDirectory ( std::istream& _zipfile )
{
  // Find and read eocd.
  if ( ! readEndOfCentralDirectory( _zipfile ) )
  {
    throw FCollException( "Unable to find zip structure: End-of-central-directory" ) ;
  }

  // Position read pointer to start of first entry in central dir.
  _vs.vseekg( _zipfile, _eocd.offset(), std::ios::beg ) ;

  int entry_num = 0 ;
  // Giving the default argument in the next line to keep Visual C++ quiet
  _entries.resize ( _eocd.totalCount(), 0 ) ;
  while ( ( entry_num < _eocd.totalCount() ) )
  {
    ZipCDirEntry *ent = new ZipCDirEntry ;
    _entries[ entry_num ] = ent ;
    _zipfile >> *ent ;
    if ( ! _zipfile )
    {
      if ( _zipfile.bad()  )
      {
        throw IOException( "Error reading zip file while reading zip file central directory" ) ;
      }
      else if ( _zipfile.fail() )
      {
        throw FCollException( "Zip file consistency problem. Failure while reading zip file central directory" ) ;
      }
      else if ( _zipfile.eof()  )
      {
        throw IOException( "Premature end of file while reading zip file central directory" ) ;
      }
    }
    ++entry_num ;
  }

  // Consistency check. eocd should start here

  int const pos = _vs.vtellg( _zipfile ) ;
  _vs.vseekg( _zipfile, 0, std::ios::end ) ;
  int const remaining = static_cast< int >( _vs.vtellg( _zipfile ) ) - pos ;
  if ( remaining != _eocd.eocdOffSetFromEnd() )
  {
    throw FCollException( "Zip file consistency problem. Zip file data fields are inconsistent with zip file layout" ) ;
  }

  // Consistency check 2, are local headers consistent with
  // cd headers
  if ( ! confirmLocalHeaders( _zipfile ) )
  {
    throw FCollException( "Zip file consistency problem. Zip file data fields are inconsistent with zip file layout" ) ;
  }

  return true ;
}


bool ZipFile::readEndOfCentralDirectory ( std::istream& _zipfile )
{
  BackBuffer bb( _zipfile, _vs ) ;
  int read_p = -1 ;
  bool found = false ;
  while ( ! found )
  {
    if ( read_p < 0 )
    {
      if ( ! bb.readChunk ( read_p ) )
      {
        found = false ;
        break ;
      }
    }
    if ( _eocd.read( bb, read_p ) )
    {
      found = true ;
      break ;
    }
    --read_p ;
  }

  return found ;
}

bool ZipFile::confirmLocalHeaders( std::istream& _zipfile )
{
  int inconsistencies = 0 ;
  ZipLocalEntry zlh ;
  for ( Entries::const_iterator it = _entries.begin() ; it != _entries.end() ; it++ )
  {
    ZipCDirEntry *ent = static_cast< ZipCDirEntry * >( (*it).get() ) ;
    _vs.vseekg( _zipfile, ent->getLocalHeaderOffset(), std::ios::beg ) ;
    _zipfile >> zlh ;
    if ( ! _zipfile || zlh != *ent )
    {
      inconsistencies++ ;
      _zipfile.clear() ;
    }
  }

  return ! inconsistencies ;
}



} // zipios namespace

// vim: ts=2 sw=2 et
