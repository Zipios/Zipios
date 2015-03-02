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
    Header file that defines ZipOutputStreambuf.
*/


#include "zipios++/deflateoutputstreambuf.h"

#include "zipios++/ziphead.h"


namespace zipios
{

/** ZipOutputStreambuf is a zip output streambuf filter.  */
class ZipOutputStreambuf : public DeflateOutputStreambuf
{
public:

  enum class CompressionLevels : int
  {
    NO_COMPRESSION      = Z_NO_COMPRESSION, 
    BEST_SPEED          = Z_BEST_SPEED,
    BEST_COMPRESSION    = Z_BEST_COMPRESSION,
    DEFAULT_COMPRESSION = Z_DEFAULT_COMPRESSION
  } ;

  /** ZipOutputStreambuf constructor. A newly constructed ZipOutputStreambuf
      is not ready to accept data, putNextEntry() must be invoked first.
      @param outbuf the streambuf to use for input.
      @param del_outbuf if true is specified outbuf will be deleted, when 
      the ZipOutputStreambuf is destructed.  */
  explicit ZipOutputStreambuf( std::streambuf *outbuf, bool del_outbuf = false ) ;

  /** Closes the current entry, and positions the stream read pointer at 
      the beginning of the next entry (if there is one). */
  void closeEntry() ;

  /** Calls finish. */
  void close() ;

  /** Closes the current entry (if one is open), then writes the Zip
      Central Directory Structure closing the ZipOutputStream. The
      output stream that the zip archive is being written to is not
      closed. */
  void finish() ;

  /** Begins writing the next entry.
      Opens the next entry in the zip archive and returns a const pointer to a 
      FileEntry object for the entry.
      @return a const FileEntry * containing information about the (now) current 
      entry. */
  void putNextEntry( ZipCDirEntry const& entry ) ;

  /** Sets the global comment for the Zip archive. */
  void setComment( std::string const& comment ) ;

  /** Sets the compression level to be used for subsequent entries. */
  void setLevel( int level ) ;
  void setLevel( CompressionLevels level ) ;

  /** Sets the compression method to be used. only STORED and DEFLATED are
      supported. */
  void setMethod( StorageMethod method ) ;

  /** Destructor. */
  virtual ~ZipOutputStreambuf() ;

protected:
  virtual int overflow( int c = EOF ) ;
  virtual int sync() ;

  void setEntryClosedState() ;
  void updateEntryHeaderInfo() ;

  // Should/could be moved to zipheadio.h ?!
  static void writeCentralDirectory( std::vector< ZipCDirEntry > const& entries, 
                                     EndOfCentralDirectory eocd,
                                     std::ostream& os ) ;



private:
  std::string _zip_comment ;
  std::vector< ZipCDirEntry > _entries ;
  bool _open_entry ;
  bool _open ;
  StorageMethod _method ;
  int _level ;
};


} // zipios namespace

// vim: ts=2 sw=2 et
