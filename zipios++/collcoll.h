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
    /** Constructor.
     */
    explicit                        CollectionCollection();

    /** Copy constructor. */
                                    CollectionCollection(CollectionCollection const& src);
    virtual pointer_t               clone() const;
    virtual                         ~CollectionCollection();

    /** Copy assignment operator. */
    const CollectionCollection& operator = (CollectionCollection const& src);

    bool                            addCollection(FileCollection const& collection);

    bool                            addCollection(FileCollection::pointer_t collection);

    virtual void                    close();
    virtual FileEntry::vector_t     entries() const;
    virtual FileEntry::pointer_t    getEntry(std::string const& name, MatchPath matchpath = MatchPath::MATCH) const;
    virtual stream_pointer_t        getInputStream(FileEntry::pointer_t entry);
    virtual stream_pointer_t        getInputStream(std::string const& entry_name, MatchPath matchpath = MatchPath::MATCH);

    /** Returns the number in entries in all collections kept by
     * the CollectionCollection object
     */
    virtual size_t                  size() const;

protected:
    void getEntry(std::string const& name, FileEntry::pointer_t& cep, vector_t::const_iterator& it, MatchPath matchpath = MatchPath::MATCH) const;

    vector_t                        m_collections;
};






} // namespace

// vim: ts=4 sw=4 et
