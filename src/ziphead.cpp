
#include "config.h"

#ifdef HAVE_STD_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif
#include <iterator>
#ifdef HAVE_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#endif
#include <string>

#include "fcol_common.h"
#include "ziphead.h"

namespace zipios {

using std::cerr ;
using std::endl ;
using std::ios ;
#ifdef HAVE_SSTREAM
using std::ostringstream ;
#else
using std::ostrstream ;
using std::ends ;
#endif

istream& operator>> ( istream &is, ZipLocalEntry &zlh         ) {
  zlh._valid = false ; // set to true upon successful completion.
  if ( ! is ) {
    cerr << "istream in error state prior to reading ZipLocalEntry" << endl ;
    return is ;
  }

  // Before reading anything we record the position in the stream
  // This is a field in the central directory entry, but not
  // in the local entry. After all, we know where we are, anyway.
  zlh.rel_offset_loc_head  = is.tellg() ;

  if ( zlh.signature != readUint32( is ) ) {
    // put stream in error state and return
    is.setstate ( ios::failbit ) ;
    return is ;
  }
  
  zlh.extract_version = readUint16( is ) ;
  zlh.gp_bitfield     = readUint16( is ) ;
  zlh.compress_method = readUint16( is ) ;
  zlh.last_mod_ftime  = readUint16( is ) ;
  zlh.last_mod_fdate  = readUint16( is ) ;
  zlh.crc_32          = readUint32( is ) ;
  zlh.compress_size   = readUint32( is ) ;
  zlh.uncompress_size = readUint32( is ) ;
  zlh.filename_len    = readUint16( is ) ;
  zlh.extra_field_len = readUint16( is ) ;

  // Read filename and extra_field
  readByteSeq( is, zlh.filename, zlh.filename_len ) ;
  readByteSeq( is, zlh.extra_field, zlh.extra_field_len ) ; 

  if ( is )
    zlh._valid = true ;
  return is ;
}


istream& operator>> ( istream &is, DataDescriptor &dd ) {
  return is ;
}


istream& operator>> ( istream &is, ZipCDirEntry &zcdh ) {
  zcdh._valid = false ; // set to true upon successful completion.
  if ( ! is ) {
    cerr << "istream in error state prior to reading ZipLocalEntry" << endl ;
    return is ;
  }

  if ( zcdh.signature != readUint32( is ) ) {
    // put stream in error state and return
    is.setstate ( ios::failbit ) ;
    return is ;
  }
  
  zcdh.writer_version       = readUint16( is ) ;
  zcdh.extract_version      = readUint16( is ) ;
  zcdh.gp_bitfield          = readUint16( is ) ;
  zcdh.compress_method      = readUint16( is ) ;
  zcdh.last_mod_ftime       = readUint16( is ) ;
  zcdh.last_mod_fdate       = readUint16( is ) ;
  zcdh.crc_32               = readUint32( is ) ;
  zcdh.compress_size        = readUint32( is ) ;
  zcdh.uncompress_size      = readUint32( is ) ;
  zcdh.filename_len         = readUint16( is ) ;
  zcdh.extra_field_len      = readUint16( is ) ;
  zcdh.file_comment_len     = readUint16( is ) ; 
  zcdh.disk_num_start       = readUint16( is ) ;
  zcdh.intern_file_attr     = readUint16( is ) ;
  zcdh.extern_file_attr     = readUint32( is ) ;
  zcdh.rel_offset_loc_head  = readUint32( is ) ;

  // Read filename and extra_field
  readByteSeq( is, zcdh.filename, zcdh.filename_len ) ;
  readByteSeq( is, zcdh.extra_field, zcdh.extra_field_len ) ; 
  readByteSeq( is, zcdh.file_comment, zcdh.file_comment_len ) ;

  if ( is )
    zcdh._valid = true ;
  return is ;
}

ostream &operator<< ( ostream &os, const ZipLocalEntry &zlh ) {
  os << "ZipLocalEntry:" << endl ;
  os << "Filename:           " << zlh.filename         << endl ;
  os << "Compression method: " << zlh.compress_method  << endl ;
  os << "Uncompressed size:  " << zlh.uncompress_size  << endl ;
  os << "Compressed size:    " << zlh.compress_size    << endl ;
  return os ;
}

ostream &operator<< ( ostream &os, const EndOfCentralDirectory &eocd ) {
  os << "Total entries : " << eocd.cdir_tot_entries << endl ;
  return os ;
}

bool operator== ( const ZipLocalEntry &zlh, const ZipCDirEntry &ze ) {
  // Not all fields need to be identical. Some of the information
  // may be put in a data descriptor that trails the compressed
  // data, according to the specs (The trailing data descriptor
  // can contain crc_32, compress_size and uncompress_size.)

  // Experience has shown that extra_field and extra_field_len
  // can differ too.

//    cerr << "----- BEGIN -----" << endl ;
//    cerr << ( zlh.extract_version == ze.extract_version     ) << endl ; 
//    cerr << ( zlh.gp_bitfield     == ze.gp_bitfield         ) << endl ; 
//    cerr << ( zlh.compress_method == ze.compress_method     ) << endl ; 
//    cerr << ( zlh.last_mod_ftime  == ze.last_mod_ftime      ) << endl ; 
//    cerr << ( zlh.last_mod_fdate  == ze.last_mod_fdate      ) << endl ; 

//    cerr << ( zlh.filename_len    == ze.filename_len        ) << endl ; 
  
//    cerr << ( zlh.filename        == ze.filename            ) << endl ; 
//    cerr << "----- END -----" << endl ;
  return ( zlh.extract_version == ze.extract_version     &&
	   zlh.gp_bitfield     == ze.gp_bitfield         &&
	   zlh.compress_method == ze.compress_method     &&
	   zlh.last_mod_ftime  == ze.last_mod_ftime      &&
	   zlh.last_mod_fdate  == ze.last_mod_fdate      &&
	   zlh.filename_len    == ze.filename_len        &&
	   
	   zlh.filename        == ze.filename               ) ;
}

//
// ZipLocalEntry methods
//


string ZipLocalEntry::getComment() const {
  return "" ; // No comment in a local entry
}

int ZipLocalEntry::getCompressedSize() const {
  return compress_size ;
}

int ZipLocalEntry::getCrc() const {
  return crc_32 ;
}

vector< unsigned char > ZipLocalEntry::getExtra() const {
  return extra_field ;
}

StorageMethod ZipLocalEntry::getMethod() const {
  return static_cast< StorageMethod >( compress_method ) ;
}

string ZipLocalEntry::getName() const {
  return filename ;
}

string ZipLocalEntry::getFileName() const {
  if ( isDirectory() )
    return string() ;
  string::size_type pos ;
  pos = filename.find_last_of( separator ) ;
  if ( pos != string::npos ) { // separator found!
    // isDirectory() check means pos should not be last, so pos+1 is ok 
    return string( filename, pos + 1 ) ;
  } else {
    return filename ;
  }
}

int ZipLocalEntry::getSize() const {
  return uncompress_size ;
}

int ZipLocalEntry::getTime() const {
  return last_mod_fdate << 16 + last_mod_ftime ; 
  // FIXME: what to do with this time date thing? (not only here?)
}

bool ZipLocalEntry::isValid() const {
  return _valid ;
}

bool ZipLocalEntry::isDirectory() const {
  assert( filename.size() != 0 ) ;
  return  filename[ filename.size() - 1 ] == separator ;
}


void ZipLocalEntry::setComment( const string &comment ) {
  // A local entry cannot hold a comment
}

void ZipLocalEntry::setCompressedSize( int size ) {
  compress_size = size ;
}

void ZipLocalEntry::setCrc( int crc ) {
  crc_32 = crc ;
}

void ZipLocalEntry::setExtra( const vector< unsigned char > &extra ) {
  extra_field = extra ;
  extra_field_len = extra_field.size() ;
}

void ZipLocalEntry::setMethod( StorageMethod method ) {
  compress_method = static_cast< uint16 >( method ) ;
}

void ZipLocalEntry::setName( const string &name ) {
  filename = name ;
  filename_len = filename.size() ;
}

void ZipLocalEntry::setSize( int size ) {
  uncompress_size = size ;
}

void ZipLocalEntry::setTime( int time ) {
  // FIXME: fix time setting here, and ind flist and elsewhere. Define the
  // date time semantics before mucking about - how surprising
}

string ZipLocalEntry::toString() const {
#ifdef HAVE_SSTREAM
  ostringstream sout ;
#else
  ostrstream sout ;
#endif
  sout << filename << " (" << uncompress_size << " bytes, " ;
  sout << compress_size << " bytes compressed)" ;
#ifndef HAVE_SSTREAM
  sout << ends ; // null terminate ostrstream
  sout.freeze( 0 ) ; // Deallocate the string when ostrstream is destroyed.
#endif
  return sout.str() ;
}

uint32 ZipLocalEntry::getLocalHeaderOffset() const {
  return rel_offset_loc_head ;
}

bool ZipLocalEntry::trailingDataDescriptor() const {
  // gp_bitfield bit 3 is one, if this entry uses a trailing data
  // descriptor to keep size, compressed size and crc-32
  // fields.
  if ( ( gp_bitfield & 4 ) == 1 )
    return true ;
  else
    return false ;
}

ZipLocalEntry *ZipLocalEntry::clone() const {
  return new ZipLocalEntry( *this ) ;
}


//
// ZipCDirEntry methods
//

string ZipCDirEntry::getComment() const {
  return file_comment ;
}


void ZipCDirEntry::setComment( const string &comment ) {
  file_comment = comment ;
  file_comment_len = file_comment.size() ;
}


string ZipCDirEntry::toString() const {
#ifdef HAVE_SSTREAM
  ostringstream sout ;
#else
  ostrstream sout ;
#endif
  sout << filename << " (" << uncompress_size << " bytes, " ;
  sout << compress_size << " bytes compressed)" ;
#ifndef HAVE_SSTREAM
  sout << ends ; // null terminate ostrstream
  sout.freeze( 0 ) ; // Deallocate the string when ostrstream is destroyed.
#endif

  return sout.str() ;
}


//
// EndOfCentralDirectory methods
//

bool EndOfCentralDirectory::read( vector<unsigned char> &buf, int pos ) {
  if ( ( buf.size() - pos < sizeof( uint32 ) )   || 
       ( ! checkSignature( &( buf[ pos ] ) ) )     )
    return false ;

  eocd_offset_from_end = buf.size() - pos ;
  pos += sizeof( uint32 ) ;
  disk_num         = ztohs( &( buf[ pos  ] ) ) ; pos += sizeof( uint16 ) ;
  cdir_disk_num    = ztohs( &( buf[ pos  ] ) ) ; pos += sizeof( uint16 ) ;
  cdir_entries     = ztohs( &( buf[ pos  ] ) ) ; pos += sizeof( uint16 ) ;
  cdir_tot_entries = ztohs( &( buf[ pos  ] ) ) ; pos += sizeof( uint16 ) ;
  cdir_size        = ztohl( &( buf[ pos  ] ) ) ; pos += sizeof( uint32 ) ;
  cdir_offset      = ztohl( &( buf[ pos  ] ) ) ; pos += sizeof( uint32 ) ;
  zip_comment_len  = ztohs( &( buf[ pos  ] ) ) ; pos += sizeof( uint16 ) ;
//    cerr << "Zip comment length = " << zip_comment_len << endl ;
//    cerr << "Length of remaining file = " << buf.size() - pos << endl ;

  return true ; // Dummy
}



} // namespace



/** \file
    Implementation of routines for reading the central directory and 
    local headers of a zip archive. 
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
