#ifndef BASICENTRY_H
#define BASICENTRY_H

#include "config.h"

#if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM)
#include <iostream>
#else
#include <iostream.h>
#endif
#include <string>

#include "fcollexceptions.h"
#include "fileentry.h"

namespace zipios {

/** BasicEntry is a FileEntry that is suitable as a base class for
    basic entries, that e.g. do not support any form of compression */
class BasicEntry : public FileEntry {
public:
  /** Constructor.
      @param filename the filename of the entry.
      @param comment a comment for the entry.
   */
  explicit BasicEntry( const string &filename, const string &comment,
		       const string &basepath = "" ) ;
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
  
  //     virtual int hashCode() const ;
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
  
  virtual BasicEntry *clone() const ;

  virtual ~BasicEntry() ;
protected:
  inline string fullPath() const ;
  string _filename ;
  string _comment ;
  int _size ;
  bool _valid ;
  string _basepath ;

  void setError( const string &msg ) ;
};

}
#endif

/** \file
    Header file that defines BasicEntry.
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
