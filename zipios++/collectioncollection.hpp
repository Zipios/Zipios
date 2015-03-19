#pragma once
#ifndef ZIPIOS_COLLECTIONCOLLECTION_HPP
#define ZIPIOS_COLLECTIONCOLLECTION_HPP

/*
  Zipios++ - a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015  Made to Order Software Corporation

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
 * \brief Define the zipios::CollectionCollection class.
 *
 * This zipios::CollectionCollection class can be used to manage
 * a collection of collections with a simple interface.
 */

#include "zipios++/filecollection.hpp"


namespace zipios
{


class CollectionCollection : public FileCollection
{
public:
    explicit                        CollectionCollection();
                                    CollectionCollection(CollectionCollection const& src);
    virtual pointer_t               clone() const override;
    CollectionCollection&           operator = (CollectionCollection const& src);
    virtual                         ~CollectionCollection() override;

    bool                            addCollection(FileCollection const& collection);
    bool                            addCollection(FileCollection::pointer_t collection);
    virtual void                    close() override;
    virtual FileEntry::vector_t     entries() const override;
    virtual FileEntry::pointer_t    getEntry(std::string const& name, MatchPath matchpath = MatchPath::MATCH) const override;
    virtual stream_pointer_t        getInputStream(std::string const& entry_name, MatchPath matchpath = MatchPath::MATCH) override;
    virtual size_t                  size() const override;
    virtual void                    mustBeValid() const;

protected:
    vector_t                        m_collections;
};


} // zipios namespace

// vim: ts=4 sw=4 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

#endif
