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
    Implementation of CollectionCollection.
*/

#include "zipios++/collcoll.h"

#include "zipios++/zipiosexceptions.h"

#include "zipios_common.h"


namespace zipios
{


CollectionCollection::CollectionCollection()
{
    m_valid = true; // we are valid even though we are empty!
}


CollectionCollection::CollectionCollection(CollectionCollection const& src)
    : FileCollection(src)
{
    m_collections.reserve(src.m_collections.size());
    for(auto it = src.m_collections.begin(); it != src.m_collections.end(); ++it)
    {
        m_collections.push_back((*it)->clone());
    }
}


CollectionCollection const& CollectionCollection::operator = (CollectionCollection const& rhs)
{
    FileCollection::operator = (rhs);

    if(this != &rhs)
    {
        m_collections.clear();
        m_collections.reserve(rhs.m_collections.size());
        for(auto it = rhs.m_collections.begin(); it != rhs.m_collections.end(); ++it)
        {
            m_collections.push_back((*it)->clone());
        }
    }

    return *this ;
}


FileCollection::pointer_t CollectionCollection::clone() const
{
    return FileCollection::pointer_t(new CollectionCollection(*this));
}


CollectionCollection::~CollectionCollection()
{
}


/** \anchor collcoll_addcoll_anchor
 * Adds a collection.
 * @param collection The collection to add.
 * @return true if the collection was added succesfully and
 * the added collection is valid.
 */
bool CollectionCollection::addCollection(FileCollection const& collection)
{
    mustBeValid();

    // TODO: the first test below is to avoid adding ourselves to
    //       ourselves; the current test is quite weak since A
    //       could include B which itself includes A and we
    //       would not know...
    if(this == &collection || !collection.isValid())
    {
        return false;
    }

    m_collections.push_back(collection.clone());

    return true;
}


/** Adds the collection pointed to by collection. The CollectionCollection
  will call delete on the pointer when it is destructed, so the caller
  should make absolutely sure to only pass in a collection created with new
  and be sure to leave it alone after adding it. If the collection is not
  added false is returned and the caller remains responsible for the
  collection pointed to by collection.
  @param collection A pointer to the collection to add.
  @return true if the collection was added succesfully and
  the added collection is valid. */
bool CollectionCollection::addCollection(FileCollection::pointer_t collection)
{
    mustBeValid();

    return addCollection(*collection);
}


void CollectionCollection::close()
{
    m_valid = false;
}


FileEntry::vector_t CollectionCollection::entries() const
{
    mustBeValid();

    FileEntry::vector_t all_entries;
    for(auto it = m_collections.begin(); it != m_collections.end(); it++)
    {
        all_entries += (*it)->entries();
    }

    return all_entries;
}


FileEntry::pointer_t CollectionCollection::getEntry(std::string const& name, MatchPath matchpath) const
{
    mustBeValid();

    // Returns the first matching entry.
    FileCollection::vector_t::const_iterator it;
    FileEntry::pointer_t cep;

    getEntry(name, cep, it, matchpath);

    return cep;
}


CollectionCollection::stream_pointer_t CollectionCollection::getInputStream(FileEntry::pointer_t entry)
{
  mustBeValid();

  return getInputStream(entry->getName());
}


CollectionCollection::stream_pointer_t CollectionCollection::getInputStream(std::string const& entry_name, MatchPath matchpath)
{
    mustBeValid();

    FileCollection::vector_t::const_iterator it;
    FileEntry::pointer_t cep;

    getEntry(entry_name, cep, it, matchpath);

    return cep ? (*it)->getInputStream(entry_name) : nullptr;
}


size_t CollectionCollection::size() const
{
    mustBeValid();

    size_t sz(0);
    for(auto it = m_collections.begin(); it != m_collections.end(); ++it)
    {
        sz += (*it)->size();
    }

    return sz;
}


//
// Protected member functions
//

/** \brief Seach for an entry.
 *
 * This function searchs for an entry that match the given name.
 * If that entry exists, the \p it parameter will be pointing
 * to it.
 *
 * The \p cep parameter is also set to the object found.
 *
 * \param[in] name  The name of the entry to search.
 * \param[out] cep  The pointer to the entry found.
 * \param[out] it  The iterator pointing to the entry in this collection.
 * \param[in] matchpath  How the name of the entry is compared with \p name.
 */
void CollectionCollection::getEntry(std::string const& name, FileEntry::pointer_t& cep, FileCollection::vector_t::const_iterator& it, MatchPath matchpath) const
{
    for(it = m_collections.begin(); it != m_collections.end(); it++)
    {
        cep = (*it)->getEntry(name, matchpath);
        if(cep)
        {
            break;
        }
    }
    cep = nullptr;
}


} // zipios namespace
// vim: ts=4 sw=4 et
