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


CollectionCollection *CollectionCollection::g_instance = nullptr;


CollectionCollection::CollectionCollection()
{
    m_valid = true; // we are valid even though we are empty!
}


CollectionCollection::CollectionCollection(CollectionCollection const& src)
    : FileCollection(src)
{
    m_collections.reserve( src.m_collections.size() ) ;
    for(auto it = src.m_collections.begin() ; it != src.m_collections.end(); ++it)
    {
        m_collections.push_back((*it)->clone());
    }
}


CollectionCollection const& CollectionCollection::operator = (CollectionCollection const& src)
{
    FileCollection::operator = (src);

    if(this != &src)
    {
        // Destroy current contents.
        for(auto it = m_collections.begin(); it != m_collections.end(); ++it)
        {
            delete *it;
        }

        //  Then copy src's content.
        m_collections.clear();
        m_collections.reserve(src.m_collections.size());
        for(auto it = src.m_collections.begin(); it != src.m_collections.end(); ++it)
        {
            m_collections.push_back((*it)->clone());
        }
    }

    return *this ;
}


CollectionCollection::~CollectionCollection()
{
    for(auto it = m_collections.begin(); it != m_collections.end(); ++it)
    {
        delete *it;
    }
}


CollectionCollection &CollectionCollection::inst()
{
    if(g_instance != 0)
    {
        return *g_instance;
    }

    return *(g_instance = new CollectionCollection);
}


bool CollectionCollection::addCollection(FileCollection const& collection)
{
    mustBeValid();

    if(this == &collection || !collection.isValid())
    {
        return false ;
    }

    m_collections.push_back(collection.clone());

    return true ;
}


bool CollectionCollection::addCollection(FileCollection *collection)
{
    mustBeValid();

    if(collection == nullptr || this == collection || !collection->isValid())
    {
        return false;
    }

    m_collections.push_back(collection);

    return true;
}


void CollectionCollection::close()
{
    m_valid = false;
}


ConstEntries CollectionCollection::entries() const
{
    mustBeValid();

    ConstEntries all_entries;
    for(auto it = m_collections.begin(); it != m_collections.end(); it++)
    {
        all_entries += (*it)->entries();
    }

    return all_entries;
}


ConstEntryPointer CollectionCollection::getEntry(std::string const& name,
                                                 MatchPath matchpath) const
{
    mustBeValid();

    // Returns the first matching entry.
    std::vector<FileCollection *>::const_iterator it;
    ConstEntryPointer cep;

    getEntry(name, cep, it, matchpath);

    return cep;
}


std::istream *CollectionCollection::getInputStream(ConstEntryPointer const& entry)
{
  mustBeValid();

  return getInputStream( entry->getName() ) ;
}


std::istream *CollectionCollection::getInputStream(std::string const& entry_name,
                                                   MatchPath matchpath)
{
    mustBeValid();

    std::vector< FileCollection * >::const_iterator it;
    ConstEntryPointer cep;

    getEntry(entry_name, cep, it, matchpath);

    return cep ? (*it)->getInputStream(entry_name) : nullptr;
}


int CollectionCollection::size() const
{
    mustBeValid();

    int sz(0);
    for(auto it = m_collections.begin() ; it != m_collections.end(); ++it)
    {
        sz += (*it)->size();
    }

    return sz;
}


FileCollection *CollectionCollection::clone() const
{
    return new CollectionCollection(*this);
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
void CollectionCollection::getEntry(std::string const& name,
                                    ConstEntryPointer& cep,
                                    std::vector<FileCollection *>::const_iterator& it,
                                    MatchPath matchpath) const
{
    for(it = m_collections.begin() ; it != m_collections.end() ; it++)
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
