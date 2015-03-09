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
        m_uncompressed_size = m_filename.isDirectory() ? 0 : m_filename.fileSize();
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


/** \brief Clean up a DirectoryEntry object.
 *
 * The destructor is defined as it has to be virtual.
 *
 * It will eventually clean up resources used by the DirectoryEntry class.
 */
DirectoryEntry::~DirectoryEntry()
{
}


/** \brief Retrieve the comment of the file entry.
 *
 * This function returns the comment of this entry.
 *
 * If the entry was not assigned a comment, this function returns
 * an empty string.
 *
 * This implementation of FileEntry has a comment.
 *
 * \return The comment associated with this entry, if there is one.
 */
std::string DirectoryEntry::getComment() const
{
    return m_comment;
}


/** \brief Compare two file entries for equality.
 *
 * This function compares most of the fields between two file
 * entries to see whether they are equal or not.
 *
 * \note
 * This function calls the base class isEqual() and also verifies
 * that the object comments are equal.
 *
 * \param[in] file_entry  The file entry to compare this against.
 *
 * \return true if both FileEntry objects are considered equal.
 */
bool DirectoryEntry::isEqual(FileEntry const& file_entry) const
{
    DirectoryEntry const * const de(dynamic_cast<DirectoryEntry const * const>(&file_entry));
    if(!de)
    {
        return false;
    }
    return FileEntry::isEqual(file_entry)
        && m_comment == de->m_comment;
}


/** \brief Set the comment field for the DirectoryEntry.
 *
 * This function sets the comment of this DirectoryEntry.
 * This implementation actually keeps track of comments.
 *
 * \param[in] comment  A string with the new comment.
 */
void DirectoryEntry::setComment(std::string const& comment)
{
    m_comment = comment;
}


/** \brief Returns a human-readable string representation of the entry.
 *
 * This function transforms the basic information of the entry in a
 * string. Note that most of the information is lost as the function
 * is likely to only display the filename and the size of the file,
 * nothing more.
 *
 * \return A human-readable string representation of the entry.
 */
std::string DirectoryEntry::toString() const
{
    OutputStringStream sout;
    // TBD: shall we offer translation support for these?
    sout << static_cast<std::string>(m_filename) << " (";
    if(isDirectory())
    {
        sout << "directory";
    }
    else
    {
        sout << m_uncompressed_size << " byte"
             << (m_uncompressed_size == 1 ? "" : "s");
    }
    sout << ")";
    return sout.str();
}


} // zipios namespace
// vim: ts=4 sw=4 et
