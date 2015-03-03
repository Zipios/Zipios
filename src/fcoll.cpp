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
    Implementation of FileCollection.
*/

#include "zipios++/fcoll.h"

#include "zipios++/zipiosexceptions.h"

#include <algorithm>

namespace zipios
{


FileCollection::FileCollection(FileCollection const& src)
    : m_filename(src.m_filename)
    , m_valid(src.m_valid)
{
    m_entries.reserve(src.m_entries.size());
    for(auto it = src.m_entries.begin(); it != src.m_entries.end(); ++it)
    {
        m_entries.push_back((*it)->clone());
    }
}


FileCollection const& FileCollection::operator = (FileCollection const& src)
{
    if(this != &src)
    {
        m_filename = src.m_filename;
        m_valid = src.m_valid;

        m_entries.clear() ;
        m_entries.reserve( src.m_entries.size() ) ;

        for(auto it(src.m_entries.begin()); it != src.m_entries.end(); ++it)
        {
            m_entries.push_back((*it)->clone());
        }
    }

    return *this ;
}


/** FileCollection destructor. */
FileCollection::~FileCollection()
{
}



ConstEntries FileCollection::entries() const
{
    mustBeValid();

    // The constructor below is not in all vector impl. (not those
    // without member templates)
    // ConstEntries ( _entries.begin(), _entries.end() ) ;
    // Instead of using that we copy the vector manually
    ConstEntries cep_vec;
    cep_vec.reserve(m_entries.size());
    for(auto cit = m_entries.begin(); cit != m_entries.end(); ++cit)
    {
        cep_vec.push_back(*cit);
    }

    return cep_vec;
}


ConstEntryPointer FileCollection::getEntry(std::string const& name,
                                           MatchPath matchpath) const
{
    mustBeValid();

    Entries::const_iterator iter;
    if(matchpath == MatchPath::MATCH)
    {
        iter = std::find_if(m_entries.begin(), m_entries.end(), FileEntry::MatchName(name));
    }
    else
    {
        iter = std::find_if(m_entries.begin(), m_entries.end(), FileEntry::MatchFileName(name));
    }

    return iter == m_entries.end() ? EntryPointer() : *iter;
}


std::string FileCollection::getName() const
{
    mustBeValid();
    return m_filename;
}


int FileCollection::size() const
{
    mustBeValid();
    return m_entries.size();
}


void FileCollection::mustBeValid() const
{
    if(!m_valid)
    {
        throw InvalidStateException("Attempted to access an invalid CollectionCollection");
    }
}



} // zipios namespace
// vim: ts=4 sw=4 et
