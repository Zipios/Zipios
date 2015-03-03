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
    Header file that defines CollectionCollection.
*/

#include "zipios++/fcoll.h"


namespace zipios
{


/** \anchor collcoll_anchor
    CollectionCollection is a FileCollection that consists of an
    arbitrary number of FileCollections. With a CollectionCollection
    the user can use multiple FileCollections transparently, making it
    easy for a program to keep some of its files in a zip archive and
    others stored in ordinary files. CollectionCollection can be used
    to create a simple virtual filesystem, where all collections are
    mounted in /. If more than one collection contain a file with
    the same path only the one in the first added collection will
    be accessible.
*/
class CollectionCollection : public FileCollection
{
public:
    /** \anchor collcoll_inst_anchor
     * This static method provides a singleton instance of a
     * CollectionCollection.
     *
     * The instance is instantiated the first time the method is called.
     * @return A pointer to a singleton CollectionCollection instance.
     */
    static CollectionCollection& inst();

    /** Constructor.
     */
    explicit CollectionCollection();

    /** Copy constructor. */
    CollectionCollection(CollectionCollection const& src);

    virtual ~CollectionCollection();

    /** Copy assignment operator. */
    const CollectionCollection& operator = (CollectionCollection const& src);

    /** \anchor collcoll_addcoll_anchor
     * Adds a collection.
     * @param collection The collection to add.
     * @return true if the collection was added succesfully and
     * the added collection is valid.
     */
    bool addCollection(FileCollection const& collection);

    /** Adds the collection pointed to by collection. The CollectionCollection
      will call delete on the pointer when it is destructed, so the caller
      should make absolutely sure to only pass in a collection created with new
      and be sure to leave it alone after adding it. If the collection is not
      added false is returned and the caller remains responsible for the
      collection pointed to by collection.
      @param collection A pointer to the collection to add.
      @return true if the collection was added succesfully and
      the added collection is valid. */
    bool addCollection( FileCollection *collection ) ;

    virtual void close() ;

    virtual ConstEntries entries() const ;

    virtual ConstEntryPointer getEntry( std::string const& name,
            MatchPath matchpath = MatchPath::MATCH ) const ;

    virtual std::istream *getInputStream( ConstEntryPointer const& entry ) ;

    virtual std::istream *getInputStream( std::string const& entry_name,
                                          MatchPath matchpath = MatchPath::MATCH ) ;

    /** Returns the number in entries in all collections kept by
     *  the CollectionCollection object
     */
    virtual int size() const ;

    virtual FileCollection *clone() const ;

protected:
    void getEntry(std::string const& name,
                  ConstEntryPointer& cep,
                  std::vector< FileCollection * >::const_iterator& it,
                  MatchPath matchpath = MatchPath::MATCH) const;

    std::vector<FileCollection *>   m_collections;

private:
    static CollectionCollection *   g_instance;
};


/** Shortcut name for a CollectionCollection. If the static method
 * inst is used, it is often used a lot, so it's handy with a short name for
 * CollectionCollection
 */
typedef CollectionCollection CColl;




} // namespace

// vim: ts=4 sw=4 et
