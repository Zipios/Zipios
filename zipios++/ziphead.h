#ifndef ZIPHEAD_H
#define ZIPHEAD_H

#include "zipios++/zipios-config.h"

#if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM)
#include <iostream>
#else
#include <iostream.h>
#endif
#include <string>
#include <vector>

#include "zipios++/fileentry.h"

namespace zipios {

using std::streampos ;

typedef unsigned short uint16 ;
typedef unsigned long  uint32 ;

class ZipCDirEntry ;

/** A concrete implementation of the abstract FileEntry base class for
 ZipFile entries, specifically for representing the information
 present in the local headers of file entries in a zip file. */
class ZipLocalEntry : public FileEntry {
  friend istream& operator>> ( istream &is, ZipLocalEntry &zcdh ) ;
  friend ostream &operator<< ( ostream &os, const ZipLocalEntry &zlh ) ;
  friend bool operator== ( const ZipLocalEntry &zlh, const ZipCDirEntry &ze ) ;
public:
  inline ZipLocalEntry() : signature( 0x04034b50 ), _valid( false ) {}
  inline ZipLocalEntry &operator=( const class ZipLocalEntry &src ) ;
  virtual string getComment() const ;
  virtual int getCompressedSize() const ;
  virtual int getCrc() const ;
  virtual vector< unsigned char > getExtra() const ;
  virtual StorageMethod getMethod() const ;
  virtual string getName() const ;
  virtual string getFileName() const ;
  virtual int getSize() const ;
  virtual int getTime() const ;
  virtual bool isValid() const ;
  
  virtual bool isDirectory() const ;
  
  virtual void setComment( const string &comment ) ;
  virtual void setCompressedSize( int size ) ;
  virtual void setCrc( int crc ) ;
  virtual void setExtra( const vector< unsigned char > &extra ) ;
  virtual void setMethod( StorageMethod method ) ;
  virtual void setName( const string &name ) ;
  virtual void setSize( int size ) ;
  virtual void setTime( int time ) ;
  
  virtual string toString() const ;

  virtual uint32 getLocalHeaderOffset() const ;

  bool trailingDataDescriptor() const ;

  virtual ZipLocalEntry *clone() const ;

  virtual ~ZipLocalEntry() {}
protected:
  uint32 signature ;
  uint16 extract_version ;
  uint16 gp_bitfield     ;
  uint16 compress_method ;
  uint16 last_mod_ftime  ;
  uint16 last_mod_fdate  ;
  uint32 crc_32          ;
  uint32 compress_size   ;
  uint32 uncompress_size ;
  uint16 filename_len    ;
  uint16 extra_field_len ;

  string filename ;
  vector< unsigned char > extra_field ; 

  // This field is an element in a ZipCDirEntry
  // in a zipfile, but not in a local entry. But
  // we can set it in the local entry, anyway, because
  // we know where we are, when we read the entry.
  uint32 rel_offset_loc_head ;

  bool _valid ;
};

/** A struct containing fields for the entries in a zip file data
 descriptor, that trails the compressed data in files that were
 created by streaming, ie. where the zip compressor cannot seek back
 to the local header and store the data. */
struct DataDescriptor {
  uint32 crc_32          ;
  uint32 compress_size   ;
  uint32 uncompress_size ;
};

/** Specialization of ZipLocalEntry, that add fields for storing the
    extra information, that is only present in the entries in the zip
    central directory and not in the local entry headers. */
class ZipCDirEntry : public ZipLocalEntry {
friend istream& operator>> ( istream &is, ZipCDirEntry &zcdh ) ;
friend bool operator== ( const ZipLocalEntry &zlh, const ZipCDirEntry &ze ) ;
public:
  inline ZipCDirEntry() { signature = 0x02014b50 ; } 
  inline ZipCDirEntry &operator=( const class ZipCDirEntry &src ) ;
  virtual string toString() const ;

  virtual string getComment() const ;

  virtual void setComment( const string &comment ) ;
  virtual ~ZipCDirEntry() {}
private:
  uint16 writer_version      ;

  uint16 file_comment_len    ; 
  uint16 disk_num_start      ;
  uint16 intern_file_attr    ;
  uint32 extern_file_attr    ;

  string file_comment ;
};

/** The end of the Central directory structure. This structure is
    stored in the end of the zipfile, and contains information about
    the zipfile, including the position of the start of the central
    directory. */
class EndOfCentralDirectory {
  friend ostream &operator<< ( ostream &os, const EndOfCentralDirectory &eocd ) ;
public:
  uint32    offset() const          { return cdir_offset ;          }
  uint16    totalCount() const      { return cdir_tot_entries ;     }
  int  eocdOffSetFromEnd() const { return eocd_offset_from_end ; }
  bool read( vector<unsigned char> &buf, int pos ) ;
private:
  static const uint32 signature = 0x06054b50 ;
  uint16 disk_num         ;
  uint16 cdir_disk_num    ;
  uint16 cdir_entries     ;
  uint16 cdir_tot_entries ;
  uint32 cdir_size        ;
  uint32 cdir_offset      ;
  uint16 zip_comment_len  ;
  
  streampos eocd_offset_from_end ; // Not a Zip defined field
  string zip_comment;
  inline bool checkSignature( unsigned char *buf ) const ;
  inline bool checkSignature( uint32 sig ) const ;
};


// byte order conversion functions. 
// ztohs (zip-to-host-short)
#ifdef MY_BIG_ENDIAN
inline uint16 ztohs ( unsigned char *buf ) {
  uint16 out ;
//    *( reinterpret_cast<unsigned char *>( &out )     ) = *( buf  + 1 );
//    *( reinterpret_cast<unsigned char *>( &out ) + 1 ) = *( buf      );
  out = ( static_cast< uint16 >( buf[ 0 ] ) << 8  ) + 
        ( static_cast< uint16 >( buf[ 1 ] )       )  ; 

  return out;
}

// ztohl (zip-to-host-long)
inline uint32 ztohl ( unsigned char *buf ) {
  uint32 out;
  out = ( static_cast< uint32 >( buf[ 0 ] ) << 24 ) +  
        ( static_cast< uint32 >( buf[ 1 ] ) << 16 ) + 
        ( static_cast< uint32 >( buf[ 2 ] ) << 8  ) + 
        ( static_cast< uint32 >( buf[ 3 ] )       )  ; 
  
  return out;
}

#else

inline uint16 ztohs ( unsigned char *buf ) {
  uint16 out ;
  out = ( static_cast< uint16 >( buf[ 1 ] ) << 8  ) + 
        ( static_cast< uint16 >( buf[ 0 ] )       )  ; 
  return out;
}

// ztohl (zip-to-host-long)
inline uint32 ztohl ( unsigned char *buf ) {
  uint32 out;
  out = ( static_cast< uint32 >( buf[ 3 ] ) << 24 ) +  
        ( static_cast< uint32 >( buf[ 2 ] ) << 16 ) + 
        ( static_cast< uint32 >( buf[ 1 ] ) << 8  ) + 
        ( static_cast< uint32 >( buf[ 0 ] )       )  ; 
//    cerr << "buf : " << static_cast< int >( buf[ 0 ] ) ;
//    cerr << " "      << static_cast< int >( buf[ 1 ] ) ;
//    cerr << " "      << static_cast< int >( buf[ 2 ] ) ;
//    cerr << " "      << static_cast< int >( buf[ 3 ] ) << endl ;
//    cerr << "uint32 " << out << endl ;
  return out;
}
#endif

inline uint32 readUint32 ( istream &is ) {
  static const int buf_len = sizeof ( uint32 ) ;
  unsigned char buf [ buf_len ] ;
  int rsf = 0 ;
  while ( rsf < buf_len ) {
    is.read ( reinterpret_cast< char * >( buf ) + rsf, buf_len - rsf ) ;
    rsf += is.gcount () ;
  }
  return  ztohl ( buf ) ;
}

inline uint16 readUint16 ( istream &is ) {
  static const int buf_len = sizeof ( uint16 ) ;
  unsigned char buf [ buf_len ] ;
  int rsf = 0 ;
  while ( rsf < buf_len ) {
    is.read ( reinterpret_cast< char * >( buf ) + rsf, buf_len - rsf ) ;
    rsf += is.gcount () ;
  }
  return  ztohs ( buf ) ;
}

inline void readByteSeq ( istream &is, string &con, int count ) {
  char *buf = new char [ count + 1 ] ;
  int rsf = 0 ;
  while ( rsf < count && is ) {
    is.read ( buf + rsf, count - rsf ) ;
    rsf += is.gcount() ;
  }
  buf [ count ] = '\0' ;

  con = buf ;
  delete [] buf ;
}

inline void readByteSeq ( istream &is, unsigned char *buf, int count ) {
  int rsf = 0 ;
  while ( rsf < count && is ) {
    is.read ( reinterpret_cast< char * >( buf ) + rsf, count - rsf ) ;
    rsf += is.gcount() ;
  }
}

inline void readByteSeq ( istream &is, vector < unsigned char > &vec, int count ) {
  unsigned char *buf = new unsigned char [ count ] ;
  int rsf = 0 ;
  while ( rsf < count && is ) {
    is.read ( reinterpret_cast< char * >( buf ) + rsf, count - rsf ) ;
    rsf += is.gcount() ;
  }
  
  vec.insert ( vec.end (), buf, buf + count ) ;
  delete [] buf ;
}

istream& operator>> ( istream &is, ZipLocalEntry &zlh         ) ;
istream& operator>> ( istream &is, DataDescriptor &dd          ) ;
istream& operator>> ( istream &is, ZipCDirEntry &zcdh           ) ;
//  istream& operator>> ( istream &is, EndOfCentralDirectory &eocd ) ;

ostream &operator<< ( ostream &os, const ZipLocalEntry &zlh ) ;
ostream &operator<< ( ostream &os, const EndOfCentralDirectory &eocd ) ;

bool operator== ( const ZipLocalEntry &zlh, const ZipCDirEntry &ze ) ;
inline bool operator== ( const ZipCDirEntry &ze, const ZipLocalEntry &zlh ) {
  return zlh == ze ;
}
inline bool operator!= ( const ZipLocalEntry &zlh, const ZipCDirEntry &ze ) {
  return ! ( zlh == ze ) ;
}
inline bool operator!= ( const ZipCDirEntry &ze, const ZipLocalEntry &zlh ) {
  return ! ( zlh == ze ) ;
}

// Inline member functions

ZipCDirEntry &ZipCDirEntry::operator=( const class ZipCDirEntry &src ) {
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

bool EndOfCentralDirectory::checkSignature ( unsigned char *buf ) const {
//    cerr << "potential header: " << ztohl( buf ) << endl ;
  return checkSignature( ztohl( buf ) ) ;
}

bool EndOfCentralDirectory::checkSignature ( uint32 sig ) const {
  return signature == sig ;
}


} // namespace

#endif


/** \file
    Header file containing classes and functions for reading the central
    directory and local header fields in a zip archive.
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
