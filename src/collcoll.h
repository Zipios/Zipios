#ifndef COLLCOLL_H
#define COLLCOLL_H

#include "config.h"

#include <string>
#include <vector>

#include "fcoll.h"

namespace zipios {

using std::string ;

/** \anchor collcoll_anchor
    CollectionCollection is a FileCollection that consists of an
    arbitrary number of FileCollections. With a CollectionCollection
    the user can use multiple FileCollections transparently, making it
    easy for a program to keep some of its files in a zip archive and
    others stored in ordinary files.
*/
class CollectionCollection : public FileCollection {
public:
  /** \anchor collcoll_inst_anchor
      This static method provides a singleton instance of a CollectionCollection.
      The instance is instantiated the first time the method is called.
      @return A pointer to a singleton CollectionCollection instance.
   */
  static inline CollectionCollection *inst() ;

  /** Constructor.
   */
  explicit CollectionCollection() ;

  /** \anchor collcoll_addcoll_anchor
      Adds a collection.
      @param collection The collection to add.
   */
  bool addCollection( FileCollection &collection ) ;

  virtual void close() ;

  virtual vector< ConstEntryPointer > entries() const ;

  virtual ConstEntryPointer getEntry( const string &name, 
				      MatchPath matchpath = MATCH ) const ;

  virtual istream *getInputStream( const ConstEntryPointer &entry ) ;

  virtual istream *getInputStream( const string &entry_name, 
				   MatchPath matchpath = MATCH ) ;

  /** Returns the number in entries in all collections kept by
      the CollectionCollection object */
  virtual int size() const ;

protected:
  inline string fullPath( const string &name ) const ;

  vector< FileCollection * > _collections ;
private:
  static CollectionCollection *_inst ;
};


/** Shortcut name for a CollectionCollection. If the static method
inst is used, it is often used a lot, so it's handy with a short name for
CollectionCollection */
typedef CollectionCollection CCol ;


// Inline methods

CollectionCollection *CollectionCollection::inst() {
  if( _inst != 0 )
    return _inst ;
  else
    return _inst = new CollectionCollection ;
}

} // namespace



#endif

/** \file
    Header file that defines CollectionCollection.
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
