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
    Implementation of DirectoryCollection.
*/

#include "zipios++/dircoll.h"

#include "zipios++/zipiosexceptions.h"

#include "directory.h"

namespace zipios
{

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
DirectoryCollection::DirectoryCollection(std::string const& path,
                                         bool recursive,
                                         bool load_now)
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
    FileEntry::pointer_t ent(new DirEntry(name, "", m_filepath));
    if(ent->isValid())
    {
        return ent;
    }

    return 0;
}


DirectoryCollection::stream_pointer_t DirectoryCollection::getInputStream(FileEntry::pointer_t entry)
{
    mustBeValid();

    return getInputStream(entry->getName());
}


DirectoryCollection::stream_pointer_t DirectoryCollection::getInputStream(std::string const& entry_name, MatchPath matchpath)
{
    mustBeValid();

    if(matchpath != MatchPath::MATCH || m_entries_loaded)
    {
        loadEntries();

        FileEntry::pointer_t ent(getEntry(entry_name, matchpath));
        if(!ent)
        {
            return 0;
        }

        std::string real_path(m_filepath + entry_name);
        DirectoryCollection::stream_pointer_t p(new std::ifstream(real_path.c_str(), std::ios::in | std::ios::binary));
        return p;
    }

    // avoid loading entries if possible.
    std::string real_path(m_filepath + entry_name);
    DirectoryCollection::stream_pointer_t ifs(new std::ifstream(real_path.c_str(), std::ios::in | std::ios::binary));
    if(!*ifs)
    {
        return 0;
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

    const_cast< DirectoryCollection * >(this)->load(m_recursive);

    m_entries_loaded = true;
}


void DirectoryCollection::load(bool recursive, const FilePath &subdir)
{
    for(boost::filesystem::dir_it it(m_filepath + subdir); it != boost::filesystem::dir_it(); ++it)
    {
        // TBD: skipping "..." ?!?
        if(*it == "." || *it == ".." || *it == "...")
        {
            continue;
        }

        if(boost::filesystem::get<boost::filesystem::is_directory>(it) && recursive)
        {
            load(recursive, subdir + *it);
        }
        else
        {
            FileEntry::pointer_t ent(new BasicEntry(subdir + *it, "", m_filepath));
            m_entries.push_back(ent);
            ent->setSize(boost::filesystem::get<boost::filesystem::size>(it));
        }
    }
}


} // zipios namespace
// vim: ts=4 sw=4 et
