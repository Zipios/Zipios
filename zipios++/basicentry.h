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
    Header file that defines BasicEntry.
*/

#include "zipios++/fileentry.h"
#include "zipios++/filepath.h"


namespace zipios
{

/** \brief A file entry that does not use compression.
 *
 * BasicEntry is a FileEntry that is suitable as a base class for
 * basic entries, that e.g. do not support any form of compression
 */
class BasicEntry : public FileEntry
{
public:
  /** Constructor.
   *
   * \param[in] filename  The filename of the entry.
   * \param[in] comment  A comment for the entry.
   * \param[in] basepath  The base path to file.
   */
  explicit BasicEntry( std::string const& filename,
                       std::string const& comment,
                       FilePath const& basepath = FilePath() ) ;

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
  
  //     virtual int hashCode() const ;
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

  virtual FileEntry *clone() const ;

  virtual ~BasicEntry() ;

protected:
  std::string _filename ;
  std::string _comment ;
  int _size ;
  bool _valid ;
  FilePath _basepath ;
};

} // zipios namespace

// vim: ts=2 sw=2 et
