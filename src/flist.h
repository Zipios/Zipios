#ifndef FLIST_H
#define FLIST_H

#include "config.h"

#if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM)
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <vector>

#include "fcoll.h"
#include "flistentry.h"

namespace zipios {

using std::ifstream ;

/** A FileList is a file collection, where the member files are
    specified in a text file. The text file has the following format:

    The text file may contain comments, filenames and a special
    ROOTDIR specification. There are two types of comments. One is
    ignored by the scanner, the other is recorded and stored with the
    filenames in the FileList object. The first works like shell-style
    # comments. The second type works in the same manner, but is
    preceeded by a : instead of a #, and the specified comment is
    associated. with the last specified file. If the : comment
    preceeds the first file specification it is associated with the
    FileList. The ROOTDIR specification is used like this

    ROOTDIR = dir

    where dir is a string to be prepended to all following file
    specifications.
*/

class FileList : public FileCollection {
public:
  /** FileList constructor. 
      @param name The name of a file containing a valid file collection
      description.
   */
  explicit FileList( const string &name ) ;
  /** FileList constructor. 
      @param is an istream from which a valid file collection description can 
      be read. 
   */
  explicit FileList( ifstream &is ) ;
  virtual void close() ;

  virtual istream *getInputStream( const ConstEntryPointer &entry ) ;
  virtual istream *getInputStream( const string &entry_name, 
				     MatchPath matchpath = MATCH ) ;

  /** Destructor. */
  virtual ~FileList() {}
private:

  bool init( istream &is ) ;
};

}

#endif

/** \file
    Header file that defines FileList.
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
