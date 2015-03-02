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
    Header file that defines DirectoryCollection.
*/

#include "zipios++/fcoll.h"
#include "zipios++/basicentry.h"

namespace zipios
{

/** DirEntry is a BasicEntry. */
typedef BasicEntry DirEntry ;

/** \anchor dircol_anchor
 * DirectoryCollection is a FileCollection that obtains its entries
 * from a directory.
 */
class DirectoryCollection : public FileCollection
{
public:

  /** Default Constructor. */
  explicit DirectoryCollection() 
    : _entries_loaded( false )
    , _recursive( true )
  {
  }

  /** Constructor.
   * @param path A directory path name. If the name is not a valid
   * directory the created DirectoryCollection will be invalid.
   * @param load_now Load directory into memory now. Otherwise it will
   * be done when it is first needed.
   */
  explicit DirectoryCollection( std::string const& path, 
                                bool recursive = true,
                                bool load_now = false ) ;

  /* Default Copy constructor and copy assignment operator are sufficient. */

  virtual void close() ;

  virtual ConstEntries entries() const ;

  virtual ConstEntryPointer getEntry( std::string const& name, 
                                      MatchPath matchpath = MatchPath::MATCH ) const ;

  virtual std::istream *getInputStream( ConstEntryPointer const& entry ) ;

  virtual std::istream *getInputStream( std::string const& entry_name, 
                                   MatchPath matchpath = MatchPath::MATCH ) ;

  virtual int size() const ;

  virtual FileCollection *clone() const ;

  /** Destructor. */
  virtual ~DirectoryCollection() ;

protected:
  mutable bool _entries_loaded ;
  bool _recursive ; // recurse into subdirs.
  FilePath _filepath ;

  void loadEntries() const ;
  void load( bool recursive, FilePath const& subdir = FilePath() ) ;
};
 
} // namespace

// vim: ts=2 sw=2 et
