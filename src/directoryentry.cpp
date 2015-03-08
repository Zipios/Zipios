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
 * \brief Implementation of DirectoryEntry.
 */

#include "zipios++/directoryentry.hpp"

#include "zipios++/zipiosexceptions.hpp"

#include "zipios_common.hpp"


namespace zipios
{

/** \class DirectoryEntry
 * \brief A file entry that does not use compression.
 *
 * DirectoryEntry is a FileEntry that is suitable as a base class for
 * basic entries, that do not support any form of compression and
 * in most cadrd represent a file in a directtory.
 */


/** \brief Initialize a DirectoryEntry object.
 *
 * This constructor initializes a DirectoryEntry which represents a
 * file on disk. If the basepath + filename does not exist or is
 * neither a regular file or a directory, then this entry is created
 * but marked as invalid.
 *
 * \param[in] filename  The filename of the entry.
 * \param[in] comment  A comment for the entry.
 * \param[in] basepath  The base path to file.
 */
DirectoryEntry::DirectoryEntry(FilePath const& filename, std::string const& comment)
    : FileEntry(filename)
    , m_comment(comment)
{
    m_valid = m_filename.isRegular() || m_filename.isDirectory();
    if(m_valid)
    {
        m_uncompressed_size = m_filename.fileSize();
        m_unix_time = m_filename.lastModificationTime();
    }
}


/** \brief Create a copy of the DirectoryEntry.
 *
 * The clone function creates a copy of this DirectoryEntry object.
 *
 * In most cases, when a collection is copied, a clone of each
 * entry is created to avoid potential problems with sharing
 * the same object between various lists.
 *
 * \return A shared pointer of the new DirectoryEntry object.
 */
DirectoryEntry::pointer_t DirectoryEntry::clone() const
{
    return DirectoryEntry::pointer_t(new DirectoryEntry(*this));
}


DirectoryEntry::~DirectoryEntry()
{
}


std::string DirectoryEntry::getComment() const
{
    return m_comment;
}


void DirectoryEntry::setComment(std::string const& comment)
{
    m_comment = comment;
}


std::string DirectoryEntry::toString() const
{
    OutputStringStream sout;
    // TBD: shall we offer translation support for these?
    sout << static_cast<std::string>(m_filename) << " ("
         << m_uncompressed_size << " byte"
         << (m_uncompressed_size == 1 ? "" : "s") << ")";
    return sout.str();
}


} // zipios namespace
// vim: ts=4 sw=4 et
