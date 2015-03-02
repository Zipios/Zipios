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
    Implementation of ZipOutputStreambuf.
*/

#include "zipios++/zipoutputstreambuf.h"

#include "zipios++/zipiosexceptions.h"


namespace zipios
{

ZipOutputStreambuf::ZipOutputStreambuf( std::streambuf *outbuf, bool del_outbuf )
  : DeflateOutputStreambuf( outbuf, false, del_outbuf )
  , _open_entry( false )
  , _open      ( true )
  , _method    ( StorageMethod::DEFLATED )
  , _level     ( 6 )
{
}


void ZipOutputStreambuf::closeEntry()
{
  if ( ! _open_entry )
  {
    return ;
  }

  closeStream() ;

  updateEntryHeaderInfo() ;
  setEntryClosedState( ) ;
}


void ZipOutputStreambuf::close()
{
  finish() ;
}


void ZipOutputStreambuf::finish()
{
  if( ! _open )
  {
    return ;
  }

  closeEntry() ;
  std::ostream os( _outbuf ) ;
  writeCentralDirectory( _entries, EndOfCentralDirectory( _zip_comment), os ) ;
  _open = false ;
}


ZipOutputStreambuf::~ZipOutputStreambuf()
{
  finish() ;
}


void ZipOutputStreambuf::putNextEntry( ZipCDirEntry const& entry )
{
  if ( _open_entry )
  {
    closeEntry() ;
  }

  if ( ! init( _level ) )
  {
    std::cerr << "ZipOutputStreambuf::putNextEntry(): init() failed!\n" ;
  }

  _entries.push_back( entry ) ;
  ZipCDirEntry& ent = _entries.back() ;

  std::ostream os( _outbuf ) ;

  // Update entry header info
  ent.setLocalHeaderOffset( os.tellp() ) ;
  ent.setMethod( _method ) ;
  
  os << static_cast< ZipLocalEntry >( ent ) ;

  _open_entry = true ;
}


void ZipOutputStreambuf::setComment( std::string const& comment )
{
  _zip_comment = comment ;
}


void ZipOutputStreambuf::setLevel( int level )
{
  _level = level ;
}


void ZipOutputStreambuf::setLevel( CompressionLevels level )
{
  _level = static_cast<int>(level) ;
}


void ZipOutputStreambuf::setMethod( StorageMethod method )
{
  _method = method ;
  if( method == StorageMethod::STORED )
  {
    setLevel( CompressionLevels::NO_COMPRESSION ) ;
  }
  else if ( method == StorageMethod::DEFLATED )
  {
    if( _level == static_cast<int>(CompressionLevels::NO_COMPRESSION) )
    {
      setLevel( CompressionLevels::DEFAULT_COMPRESSION ) ; 
    }
  }
  else 
  {
    throw FCollException( "Specified compression method not supported" ) ;
  }
}

//
// Protected and private methods
//

int ZipOutputStreambuf::overflow( int c )
{
  return DeflateOutputStreambuf::overflow( c ) ;
//    // FIXME: implement
  
//    cout << "ZipOutputStreambuf::overflow() not implemented yet!\n" ;
//    return EOF ;
}



int ZipOutputStreambuf::sync()
{
  return DeflateOutputStreambuf::sync() ;
//    // FIXME: implement
//    cout << "ZipOutputStreambuf::sync() not implemented yet!\n" ;
//    return EOF ;
}



void ZipOutputStreambuf::setEntryClosedState()
{
  _open_entry = false ;
  // FIXME: update put pointers to trigger overflow on write. overflow
  // should then return EOF while _open_entry is false.
}


void ZipOutputStreambuf::updateEntryHeaderInfo()
{
  if ( ! _open_entry )
  {
    return ;
  }

  std::ostream os( _outbuf ) ;
  int const curr_pos = os.tellp() ;
  
  // update fields in _entries.back()
  ZipCDirEntry &entry = _entries.back() ;
  entry.setSize( getCount() ) ;
  entry.setCrc( getCrc32() ) ;
  entry.setCompressedSize( curr_pos - entry.getLocalHeaderOffset() - entry.getLocalHeaderSize() ) ;

#if 0
  // Mark Donszelmann: added current date and time
  time_t ltime;
  time( &ltime );
  struct tm *now;
  now = localtime( &ltime );
  int dosTime = (now->tm_year - 80) << 25 | (now->tm_mon + 1) << 21 | now->tm_mday << 16 |
              now->tm_hour << 11 | now->tm_min << 5 | now->tm_sec >> 1;
  entry.setTime( dosTime );
#else
  // Mon Mar 24 19:10:46 PDT 2014 (RDB)
  // Use std time_t, and get the local time. Use new setUnixTime method.
  //
  entry.setUnixTime( std::time( nullptr ) );
#endif

  // write ZipLocalEntry header to header position
  os.seekp( entry.getLocalHeaderOffset() ) ;
  os << static_cast< ZipLocalEntry >( entry ) ;
  os.seekp( curr_pos ) ;
}


void ZipOutputStreambuf::writeCentralDirectory( std::vector< ZipCDirEntry > const& entries, 
                                                EndOfCentralDirectory eocd, 
                                                std::ostream &os )
{
  int cdir_start = os.tellp() ;
  std::vector< ZipCDirEntry >::const_iterator it ;
  int cdir_size = 0 ;

  for ( it = entries.begin() ; it != entries.end() ; ++it )
  {
    os << *it ;
    cdir_size += it->getCDirHeaderSize() ;
  }
  eocd.setOffset( cdir_start ) ;
  eocd.setCDirSize( cdir_size ) ;
  eocd.setTotalCount( entries.size() ) ;
  os << eocd ;
}


} // zipios namespace
// vim: ts=2 sw=2 et
