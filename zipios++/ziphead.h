#pragma once
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
    Header file containing classes and functions for reading the central
    directory and local header fields in a zip archive.
*/


#include "zipios++/fileentry.h"


namespace zipios
{



class ZipCDirEntry ;

/** A concrete implementation of the abstract FileEntry base class for
 * ZipFile entries, specifically for representing the information
 * present in the local headers of file entries in a zip file.
 */
class ZipLocalEntry : public FileEntry
{
  friend std::istream& operator >> ( std::istream& is, ZipLocalEntry& cdh ) ;
  friend std::ostream& operator << ( std::ostream& os, ZipLocalEntry const& zlh ) ;
  friend bool operator == ( ZipLocalEntry const& zlh, const ZipCDirEntry& ze ) ;

public:
  inline ZipLocalEntry( std::string const& _filename = "",
                        std::vector< unsigned char > const& _extra_field = std::vector< unsigned char >() )
    : gp_bitfield( 0 )
    , _valid( false )
  {
    setDefaultExtract() ;
    setName( _filename ) ;
    setExtra( _extra_field ) ; 
  }

  void setDefaultExtract() ;
  inline ZipLocalEntry& operator = ( ZipLocalEntry const& src ) ;
  virtual std::string getComment() const ;
  virtual uint32_t getCompressedSize() const ;
  virtual uint32_t getCrc() const ;
  virtual std::vector< unsigned char > getExtra() const ;
  virtual StorageMethod getMethod() const ;
  virtual std::string getName() const ;
  virtual std::string getFileName() const ;
  virtual uint32_t getSize() const ;
  virtual int getTime() const ;
  virtual std::time_t getUnixTime() const ;
  virtual bool isValid() const ;

  virtual bool isDirectory() const ;

  virtual void setComment( std::string const& comment ) ;
  virtual void setCompressedSize( uint32_t size ) ;
  virtual void setCrc( uint32_t crc ) ;
  virtual void setExtra( std::vector< unsigned char > const& extra ) ;
  virtual void setMethod( StorageMethod method ) ;
  virtual void setName( std::string const& name ) ;
  virtual void setSize( uint32_t size ) ;
  virtual void setTime( int time ) ;
  virtual void setUnixTime( std::time_t time ) ;

  virtual std::string toString() const ;

  int getLocalHeaderSize() const ;

  bool trailingDataDescriptor() const ;

  virtual FileEntry *clone() const ;

  virtual ~ZipLocalEntry() {}

protected:
  static const uint32_t signature ;
  uint16_t extract_version ;
  uint16_t gp_bitfield     ;
  uint16_t compress_method ;
  uint16_t last_mod_ftime  ;
  uint16_t last_mod_fdate  ;
  uint32_t crc_32          ;
  uint32_t compress_size   ;
  uint32_t uncompress_size ;
  uint16_t filename_len    ;
  uint16_t extra_field_len ;

  std::string filename ;
  std::vector< unsigned char > extra_field ; 

  bool _valid ;
};


/** A struct containing fields for the entries in a zip file data
 descriptor, that trails the compressed data in files that were
 created by streaming, ie where the zip compressor cannot seek back
 to the local header and store the data. */
struct DataDescriptor {
  uint32_t crc_32          ;
  uint32_t compress_size   ;
  uint32_t uncompress_size ;
};

/** Specialization of ZipLocalEntry, that add fields for storing the
    extra information, that is only present in the entries in the zip
    central directory and not in the local entry headers. */
class ZipCDirEntry : public ZipLocalEntry
{
  friend std::istream& operator >> ( std::istream& is, ZipCDirEntry& zcdh ) ;
  friend std::ostream& operator << ( std::ostream& os, ZipCDirEntry const& zcdh ) ;
  friend bool operator == ( ZipLocalEntry const& zlh, ZipCDirEntry const& ze ) ;

public:

  inline ZipCDirEntry( std::string const& _filename = "",
                       std::string const& _file_comment = "",
                       std::vector< unsigned char > const& _extra_field = std::vector< unsigned char >() ) 
    : ZipLocalEntry   ( _filename, _extra_field )
    , disk_num_start  ( 0x0 )
    , intern_file_attr( 0x0 )
    , extern_file_attr( 0x81B40000 ) 
    // FIXME: I don't understand the external mapping, simply
    // copied value for a file with -rw-rw-r-- permissions
    // compressed with info-zip
  { 
    setComment( _file_comment ) ;
    setDefaultWriter() ;
  } 

  void setDefaultWriter() ;

  inline ZipCDirEntry& operator = ( ZipCDirEntry const& rhs ) ;
  virtual std::string toString() const ;

  virtual std::string getComment() const ;

  virtual void setComment( std::string const& comment ) ;

  virtual uint32_t getLocalHeaderOffset() const ;
  virtual void   setLocalHeaderOffset( uint32_t offset ) ;

  int getCDirHeaderSize() const ;

  virtual FileEntry *clone() const ;

  virtual ~ZipCDirEntry() {}

private:
  static const uint32_t signature ;
  uint16_t writer_version      ;

  uint16_t file_comment_len    ; 
  uint16_t disk_num_start      ;
  uint16_t intern_file_attr    ;
  uint32_t extern_file_attr    ;

  uint32_t rel_offset_loc_head ;

  std::string file_comment ;
};

/** The end of the Central directory structure. This structure is
    stored in the end of the zipfile, and contains information about
    the zipfile, including the position of the start of the central
    directory. */
class EndOfCentralDirectory
{
  friend std::ostream& operator << ( std::ostream& os, EndOfCentralDirectory const& eocd ) ;

public:
  explicit EndOfCentralDirectory( std::string const& _zip_comment = "",
          uint16_t _disk_num = 0, uint16_t _cdir_disk_num = 0,
          uint16_t _cdir_entries = 0,
          uint16_t _cdir_tot_entries = 0,
          uint32_t _cdir_size = 0, uint32_t _cdir_offset = 0 )
    : disk_num         ( _disk_num           )
    , cdir_disk_num    ( _cdir_disk_num      )
    , cdir_entries     ( _cdir_entries       )
    , cdir_tot_entries ( _cdir_tot_entries   )
    , cdir_size        ( _cdir_size          )
    , cdir_offset      ( _cdir_offset        )
    , zip_comment_len  ( _zip_comment.size() )
    , zip_comment      ( _zip_comment        )
  {
  }

  uint32_t    offset() const              { return cdir_offset ;          }
  uint16_t    totalCount() const          { return cdir_tot_entries ;     }
  void setCDirSize( uint32_t size )       { cdir_size = size ;            }
  void setOffset( uint32_t new_offset )   { cdir_offset = new_offset ;    }

  void setTotalCount( uint16_t c )        { cdir_entries = c ; cdir_tot_entries = c ; }
  int  eocdOffSetFromEnd() const          { return eocd_offset_from_end ; }
  bool read( std::vector<unsigned char> const& buf, int pos ) ;

private:
  static uint32_t const signature;
  uint16_t disk_num         ;
  uint16_t cdir_disk_num    ;
  uint16_t cdir_entries     ;
  uint16_t cdir_tot_entries ;
  uint32_t cdir_size        ;
  uint32_t cdir_offset      ;
  uint16_t zip_comment_len  ;

  std::streampos eocd_offset_from_end ; // Not a Zip defined field
  std::string zip_comment;
  bool checkSignature( unsigned char const *buf ) const ;
  inline bool checkSignature( uint32_t sig ) const ;
};


bool operator == ( ZipLocalEntry const& zlh, ZipCDirEntry const& ze ) ;


inline bool operator == ( ZipCDirEntry const& ze, ZipLocalEntry const& zlh )
{
  return zlh == ze ;
}


inline bool operator != ( ZipLocalEntry const& zlh, ZipCDirEntry const& ze )
{
  return ! ( zlh == ze ) ;
}


inline bool operator != ( ZipCDirEntry const& ze, ZipLocalEntry const& zlh )
{
  return ! ( zlh == ze ) ;
}

// Inline member functions

ZipCDirEntry& ZipCDirEntry::operator = ( ZipCDirEntry const& src )
{
  writer_version      = src.writer_version      ;
  extract_version     = src.extract_version     ;
  gp_bitfield         = src.gp_bitfield         ;
  compress_method     = src.compress_method     ;
  last_mod_ftime      = src.last_mod_ftime      ;
  last_mod_fdate      = src.last_mod_fdate      ;
  crc_32              = src.crc_32              ;
  compress_size       = src.compress_size       ; 
  uncompress_size     = src.uncompress_size     ;
  filename_len        = src.filename_len        ;
  extra_field_len     = src.extra_field_len     ;
  file_comment_len    = src.file_comment_len    ; 
  disk_num_start      = src.disk_num_start      ;
  intern_file_attr    = src.intern_file_attr    ;
  extern_file_attr    = src.extern_file_attr    ;
  rel_offset_loc_head = src.rel_offset_loc_head ;

  filename     = src.filename     ;
  extra_field  = src.extra_field  ; 
  file_comment = src.file_comment ;

  return *this ;
}


bool EndOfCentralDirectory::checkSignature ( uint32_t sig ) const
{
  return signature == sig ;
}


} // zipios namespace

// vim: ts=2 sw=2 et
