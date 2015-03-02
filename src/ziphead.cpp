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
    Implementation of routines for reading the central directory and 
    local headers of a zip archive. 
*/

#include "zipios++/ziphead.h"

#include "zipios++/dostime.h"
#include "zipios++/zipheadio.h"
#include "zipios++/zipiosexceptions.h"

#include "zipios_common.h"


namespace zipios
{



bool operator == ( ZipLocalEntry const& zlh, ZipCDirEntry const& ze )
{
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

uint32_t const ZipLocalEntry::signature = 0x04034b50 ;



void ZipLocalEntry::setDefaultExtract()
{
  extract_version = 20 ; // version number
}


std::string ZipLocalEntry::getComment() const
{
  return "" ; // No comment in a local entry
}


uint32_t ZipLocalEntry::getCompressedSize() const
{
  return compress_size ;
}


uint32_t ZipLocalEntry::getCrc() const
{
  return crc_32 ;
}


std::vector< unsigned char > ZipLocalEntry::getExtra() const
{
  return extra_field ;
}


StorageMethod ZipLocalEntry::getMethod() const
{
  return static_cast< StorageMethod >( compress_method ) ;
}


std::string ZipLocalEntry::getName() const
{
  return filename ;
}


std::string ZipLocalEntry::getFileName() const
{
  if ( isDirectory() )
  {
    return std::string() ;
  }
  std::string::size_type const pos ( filename.find_last_of( separator ) );
  if ( pos != std::string::npos )
  {
    // separator found!
    // isDirectory() check means pos should not be last, so pos+1 is ok 
    return filename.substr( pos + 1 ) ;
  }

  return filename ;
}


uint32_t ZipLocalEntry::getSize() const
{
  return uncompress_size ;
}


int ZipLocalEntry::getTime() const
{
  return ( last_mod_fdate << 16 ) + last_mod_ftime ; 
  // FIXME: what to do with this time date thing? (not only here?)
}


std::time_t ZipLocalEntry::getUnixTime() const
{
    return dos2unixtime( getTime() );
}


bool ZipLocalEntry::isValid() const {
  return _valid ;
}


bool ZipLocalEntry::isDirectory() const
{
  if( filename.size() == 0 )
  {
    throw IOException( "directory filename cannot be empty" ) ;
  }

  return filename[ filename.size() - 1 ] == separator ;
}


void ZipLocalEntry::setComment( std::string const & )
{
  // A local entry cannot hold a comment
}


void ZipLocalEntry::setCompressedSize( uint32_t size )
{
  compress_size = size ;
}


void ZipLocalEntry::setCrc( uint32_t crc )
{
  crc_32 = crc ;
}


void ZipLocalEntry::setExtra( std::vector< unsigned char > const& extra )
{
  extra_field = extra ;
  extra_field_len = extra_field.size() ;
}


void ZipLocalEntry::setMethod( StorageMethod method )
{
  compress_method = static_cast< uint16_t >( method ) ;
}


void ZipLocalEntry::setName( std::string const& name )
{
  filename = name ;
  filename_len = filename.size() ;
}


void ZipLocalEntry::setSize( uint32_t size )
{
  uncompress_size = size ;
}


void ZipLocalEntry::setTime( int time )
{
  // FIXME: fix time setting here, and ind flist and elsewhere. Define the
  // date time semantics before mucking about - how surprising

  // Mark Donszelmann: added these lines to make zip work for winzip
  last_mod_fdate = (time >> 16) & 0x0000FFFF;
  last_mod_ftime = time & 0x0000FFFF;
}


void ZipLocalEntry::setUnixTime( std::time_t time )
{
    setTime( unix2dostime( &time ) );
}


std::string ZipLocalEntry::toString() const
{
  OutputStringStream sout ;
  sout << filename << " (" << uncompress_size << " bytes, " ;
  sout << compress_size << " bytes compressed)" ;
  return sout.str() ;
}


int ZipLocalEntry::getLocalHeaderSize() const
{
  return 30 + filename.size() + extra_field.size() ;
}


bool ZipLocalEntry::trailingDataDescriptor() const
{
  // gp_bitfield bit 3 is one, if this entry uses a trailing data
  // descriptor to keep size, compressed size and crc-32
  // fields.
  return ( gp_bitfield & 4 ) != 0 ;
}

FileEntry *ZipLocalEntry::clone() const
{
  return new ZipLocalEntry( *this ) ;
}


//
// ZipCDirEntry methods
//

uint32_t const ZipCDirEntry::signature = 0x02014b50 ;


void ZipCDirEntry::setDefaultWriter()
{
  writer_version = 0 ;
#ifdef WIN32
  writer_version |= static_cast< uint16_t >( 0 ) << 8 ; // Windows, DOS
#else
  writer_version |= static_cast< uint16_t >( 3 ) << 8 ; // Unix
#endif
  writer_version |= 20 ; // version number
}

std::string ZipCDirEntry::getComment() const
{
  return file_comment ;
}

uint32_t ZipCDirEntry::getLocalHeaderOffset() const
{
  return rel_offset_loc_head ;
}

void ZipCDirEntry::setLocalHeaderOffset( uint32_t offset )
{
  rel_offset_loc_head = offset ;
}


void ZipCDirEntry::setComment( std::string const& comment )
{
  file_comment = comment ;
  file_comment_len = file_comment.size() ;
}


std::string ZipCDirEntry::toString() const
{
  OutputStringStream sout ;
  sout << filename << " (" << uncompress_size << " bytes, " ;
  sout << compress_size << " bytes compressed)" ;
  return sout.str() ;
}


int ZipCDirEntry::getCDirHeaderSize() const
{
  return 46 + filename.size() + extra_field.size() + file_comment.size() ;
}


FileEntry *ZipCDirEntry::clone() const
{
  return new ZipCDirEntry( *this ) ;
}


//
// EndOfCentralDirectory methods
//

uint32_t const EndOfCentralDirectory::signature = 0x06054b50 ;


bool EndOfCentralDirectory::read( std::vector<unsigned char> const& buf, int pos )
{
  if ( ( buf.size() - pos < sizeof( uint32_t ) )   || 
       ( ! checkSignature( &( buf[ pos ] ) ) )     )
  {
    return false ;
  }

  eocd_offset_from_end = buf.size() - pos ;
  pos += sizeof( uint32_t ) ;
  disk_num         = ztohs( &( buf[ pos  ] ) ) ; pos += sizeof( uint16_t ) ;
  cdir_disk_num    = ztohs( &( buf[ pos  ] ) ) ; pos += sizeof( uint16_t ) ;
  cdir_entries     = ztohs( &( buf[ pos  ] ) ) ; pos += sizeof( uint16_t ) ;
  cdir_tot_entries = ztohs( &( buf[ pos  ] ) ) ; pos += sizeof( uint16_t ) ;
  cdir_size        = ztohl( &( buf[ pos  ] ) ) ; pos += sizeof( uint32_t ) ;
  cdir_offset      = ztohl( &( buf[ pos  ] ) ) ; pos += sizeof( uint32_t ) ;
  zip_comment_len  = ztohs( &( buf[ pos  ] ) ) ; pos += sizeof( uint16_t ) ;
//    cerr << "Zip comment length = " << zip_comment_len << endl ;
//    cerr << "Length of remaining file = " << buf.size() - pos << endl ;

  return true ;
}


bool EndOfCentralDirectory::checkSignature ( unsigned char const *buf ) const
{
//    cerr << "potential header: " << ztohl( buf ) << endl ;
  return checkSignature( ztohl( buf ) ) ;
}



} // zipios namespace

// vim: ts=2 sw=2 et
