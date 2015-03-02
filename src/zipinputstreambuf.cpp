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
    Implementation of ZipInputStreambuf.
*/

#include "zipios++/zipinputstreambuf.h"

#include "zipios++/zipiosexceptions.h"


namespace zipios
{


ZipInputStreambuf::ZipInputStreambuf( std::streambuf *inbuf, int s_pos, bool del_inbuf ) 
  : InflateInputStreambuf( inbuf, s_pos, del_inbuf )
  , _open_entry( false ) 
{
  // TODO: this was added in our version, but it looked like it would
  //       skip an entry... so it probably is not required nor even wanted.
  //
  //ConstEntryPointer entry = getNextEntry() ;
  //
  //if ( ! entry->isValid() ) {
  //  ; // FIXME: throw something?
  //}
}

void ZipInputStreambuf::closeEntry()
{
  if ( ! _open_entry )
  {
    return ;
  }
  
  // check if we're positioned correctly, otherwise position us correctly
  int const position = _inbuf->pubseekoff(0, std::ios::cur, std::ios::in);
  if ( position != _data_start + static_cast< int >( _curr_entry.getCompressedSize() ) )
  {
    _inbuf->pubseekoff(_data_start + _curr_entry.getCompressedSize(), std::ios::beg, std::ios::in) ;
  }
}


void ZipInputStreambuf::close()
{
}


ConstEntryPointer ZipInputStreambuf::getNextEntry()
{
  if ( _open_entry )
  {
    closeEntry() ;
  }

  // read the zip local header
  std::istream is( _inbuf ) ; // istream does not destroy the streambuf.
  is.exceptions( std::ios::eofbit | std::ios::failbit | std::ios::badbit );

  try
  {
    is >> _curr_entry ;
    if ( _curr_entry.isValid() )
    {
      _data_start = _inbuf->pubseekoff(0, std::ios::cur, std::ios::in);
      if ( _curr_entry.getMethod() == StorageMethod::DEFLATED )
      {
        _open_entry = true ;
        reset() ; // reset inflatestream data structures 
//        cerr << "deflated" << endl ;
      }
      else if ( _curr_entry.getMethod() == StorageMethod::STORED )
      {
        _open_entry = true ;
        _remain = _curr_entry.getSize() ;
        // Force underflow on first read:
        setg( &( _outvec[ 0 ] ),
              &( _outvec[ 0 ] ) + _outvecsize,
              &( _outvec[ 0 ] ) + _outvecsize );
//        cerr << "stored" << endl ;
      }
      else
      {
        _open_entry = false ; // Unsupported compression format.
        throw FCollException( "Unsupported compression format" ) ;
      }
    }
  }
  catch (...)
  { // TODO: this is not valid, if not open we cannot access _curr_entry below
    _open_entry = false ;
  }

  if ( _curr_entry.isValid() && _curr_entry.trailingDataDescriptor() )
  {
    throw FCollException( "Trailing data descriptor in zip file not supported" ) ; 
  }

  return new ZipLocalEntry( _curr_entry ) ;
}


ZipInputStreambuf::~ZipInputStreambuf()
{
}


int ZipInputStreambuf::underflow()
{
  if ( ! _open_entry )
  {
    return EOF ; // traits_type::eof() 
  }

  if ( _curr_entry.getMethod() == StorageMethod::DEFLATED )
  {
    return InflateInputStreambuf::underflow() ;
  }

  // Ok, we are are stored, so we handle it ourselves.
  int num_b = std::min( _remain, _outvecsize ) ;
  int g = _inbuf->sgetn( &(_outvec[ 0 ] ) , num_b ) ;
  setg( &( _outvec[ 0 ] ),
	&( _outvec[ 0 ] ),
	&( _outvec[ 0 ] ) + g ) ;
  _remain -= g ;
  if ( g > 0 )
  {
    return static_cast< unsigned char >( *gptr() ) ;
  }

  return EOF ; // traits_type::eof() 
}


// FIXME: We need to check somew
//  
//    // gp_bitfield bit 3 is one, if the length of the zip entry
//    // is stored in a trailer.
//    if ( is->good  && ( _curr_entry.gp_bitfield & 4 ) != 1 ) <<--- this test is wrong! (i.e. it should be != 0 or != 4)
//      return true ;
//    else {
//      is->clear() ;
//      return false ;
//    }


} // namespace
// vim: ts=2 sw=2 et
