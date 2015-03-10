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
 * \brief Implementation of zipios::DirectoryCollection.
 *
 * This file includes the implementation of the zipios::DirectoryCollection
 * class, which is used to read a directory from disk and create
 * a set of zipios::DirectoryEntry objects.
 */

#include "zipios++/directorycollection.hpp"

#include "zipios++/zipiosexceptions.hpp"

#include "directory.hpp"

#include <fstream>


namespace zipios
{

/** \class DirectoryCollection
 * \brief A collection generated from reading a directory.
 *
 * The DirectoryCollection class is a FileCollection that obtains
 * its entries from a directory on disk.
 */


/** \brief Initialize a DirectoryCollection object.
 *
 * The default Constructor creates an empty directory collection.
 */
DirectoryCollection::DirectoryCollection()
    //: m_entries_loaded(false) -- auto-init
    //, m_recursive(true) -- auto-init
    //, m_filepath("") -- auto-init
{
}


/** \brief Constructor.
 *
 * Create a directory which represents a collection of files.
 *
 * \param[in] path  A directory path. If the name is not a valid
 *                  directory the created DirectoryCollection is
 *                  marked as being invalid.
 * \param[in] recursive  Whether to load all the files found in
 *                       sub-direcotries.
 * \param[in] load_now  Load directory into memory now if true.
 *                      Otherwise it will be read when it is first
 *                      needed.
 */
DirectoryCollection::DirectoryCollection(std::string const& path, bool recursive, bool load_now)
    //: m_entries_loaded(false) -- auto-init
    : m_recursive(recursive)
    , m_filepath(path)
{
    m_filename = m_filepath;
    m_valid = m_filepath.isDirectory();

    if(m_valid && load_now)
    {
        loadEntries();
    }
}


/** \brief Clean up a DirectoryCollection object.
 *
 * The destructor ensures that the object is properly cleaned up.
 */
DirectoryCollection::~DirectoryCollection()
{
}


void DirectoryCollection::close()
{
    m_valid = false;
}


FileEntry::vector_t DirectoryCollection::entries() const
{
    mustBeValid();

    loadEntries();

    return FileCollection::entries();
}


FileEntry::pointer_t DirectoryCollection::getEntry(std::string const& name, MatchPath matchpath) const
{
    mustBeValid();

    if(matchpath != MatchPath::MATCH || m_entries_loaded)
    {
        loadEntries();
        return FileCollection::getEntry(name, matchpath);
    }

    // avoid loading entries if possible.
    FileEntry::pointer_t ent(new DirectoryEntry(m_filepath + name, ""));
    if(ent->isValid())
    {
        return ent;
    }

    return FileEntry::pointer_t();
}


/** \brief Retrieve pointer to an istream.
 *
 * This function returns a shared pointer to an istream defined from
 * the named entry, which is expected to be available in this collection.
 *
 * The function returns a NULL pointer if no FileEntry can be found from
 * the specified name or the FileEntry is marked as invalid.
 *
 * The returned istream represents a file on disk, although the filename
 * must exist in the collection or it will be ignored. A filename that
 * represents a directory cannot return an input stream and thus an error
 * is returned in that case.
 *
 * \param[in] entry_name  The name of the file to search in the collection.
 * \param[in] matchpath  Whether the full path or just the filename is matched.
 *
 * \return A shared pointer to an open istream for the specified entry.
 */
DirectoryCollection::stream_pointer_t DirectoryCollection::getInputStream(std::string const& entry_name, MatchPath matchpath)
{
    mustBeValid();

    // avoid loading entries if possible.
    if(matchpath != MatchPath::MATCH || m_entries_loaded)
    {
        loadEntries();

        FileEntry::pointer_t ent(getEntry(entry_name, matchpath));
        if(!ent || ent->isDirectory())
        {
            return DirectoryCollection::stream_pointer_t();
        }

        std::string const real_path(m_filepath + entry_name);
        DirectoryCollection::stream_pointer_t p(new std::ifstream(real_path.c_str(), std::ios::in | std::ios::binary));
        return p;
    }

    std::string const real_path(m_filepath + entry_name);
    DirectoryCollection::stream_pointer_t ifs(new std::ifstream(real_path.c_str(), std::ios::in | std::ios::binary));
    if(!*ifs)
    {
        return DirectoryCollection::stream_pointer_t();
    }

    return ifs;
}


size_t DirectoryCollection::size() const
{
    mustBeValid();

    loadEntries();

    return m_entries.size();
}


FileCollection::pointer_t DirectoryCollection::clone() const
{
    return FileCollection::pointer_t(new DirectoryCollection(*this));
}


void DirectoryCollection::loadEntries() const
{
    if(m_entries_loaded)
    {
        return;
    }

    const_cast<DirectoryCollection *>(this)->load(m_recursive);

    m_entries_loaded = true;
}


void DirectoryCollection::load(bool recursive, FilePath const& subdir)
{
    for(boost::filesystem::dir_it it(m_filepath + subdir); it != boost::filesystem::dir_it(); ++it)
    {
        if(*it == "." || *it == "..")
        {
            continue;
        }

        if(boost::filesystem::get<boost::filesystem::is_directory>(it))
        {
            /** \TODO
             * Should we be adding directories to the collection?
             */
            if(recursive)
            {
                load(recursive, subdir + *it);
            }
            //else -- ignore directories because DirectoryEntry() does not support them!?
        }
        else
        {
            FileEntry::pointer_t ent(new DirectoryEntry(m_filepath + subdir + *it, ""));
            m_entries.push_back(ent);
        }
    }
}


} // zipios namespace
// vim: ts=4 sw=4 et
