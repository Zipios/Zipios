#ifndef DIRCOLL_H
#define DIRCOLL_H

#include "config.h"


#include "fcoll.h"
#include "basicentry.h"

namespace zipios {

/** DirEntry is a BasicEntry. */
typedef BasicEntry DirEntry ;

/** \anchor dircol_anchor
    DirectoryCollection is a FileCollection that obtains its entries
    from a directory. */
class DirectoryCollection : public FileCollection {
public:
  /** Default Constructor. */
  explicit DirectoryCollection() {}
  /** Constructor.
      @param path A directory path name. If the name is not a valid
      directory the created DirectoryCollection will be invalid.
  */
  explicit DirectoryCollection( const string &path ) ;
  virtual void close() ;
  virtual vector< ConstEntryPointer > entries() const ;
  virtual ConstEntryPointer getEntry( const string &name, 
				     MatchPath matchpath = MATCH ) const ;
  virtual istream *getInputStream( const ConstEntryPointer &entry ) ;
  virtual istream *getInputStream( const string &entry_name, 
				     MatchPath matchpath = MATCH ) ;
  virtual int size() const ;

  /** Destructor. */
  virtual ~DirectoryCollection() ;
protected:
  bool isDir( const string &path ) const ;
 
};
 
} // namespace

#endif

/** \file
    Header file that defines DirectoryCollection.
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
