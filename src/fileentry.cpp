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
 * \brief Implementation of FileEntry.
 *
 * This file includes the implementation of the FileEntry. Many of
 * the functions in FileEntry are pure virtual so they do not appear
 * here. Yet their documentation is here to avoid clutering the
 * header file.
 */

#include "zipios++/fileentry.h"

#include "zipios++/dostime.h"
#include "zipios++/filepath.h"
#include "zipios++/zipiosexceptions.h"

#include "zipios_common.h"


namespace zipios
{

/** \class FileEntry
 * \brief A FileEntry represents an entry in a FileCollection.
 *
 * The interface is a copy of the ZipEntry interface from the java.util.zip
 * package. The name has been changed to FileEntry, as FileCollection
 * is a more general abstraction, that covers other types of file
 * collections than just zip files.
 */


/** \brief Save the CRC of the entry.
 *
 * This funciton savees the CRC field in this FileEntry field.
 *
 * \param crc value to set the crc field to.
 */
FileEntry::FileEntry(std::string const& filename)
    : m_filename(filename)
    //, m_uncompress_size(0) -- auto-init
    //, m_crc_32(0) -- auto-init
    //, m_has_crc_32(false) -- auto-init
    //, m_valid(false) -- auto-init
{
}


/** \brief Create a clone of a file entry.
 *
 * This function creates a heap allocated clone of the object
 * this method is called for.
 *
 * Note that the object is expected to overload this function in
 * order to create a clone of the correct type.
 *
 * \return A heap allocated copy of the object this method is called for.
 */


/** \brief Clean up a FileEntry object.
 *
 * The destructor is defined as it has to be virtual.
 *
 * It will eventually clean up resources used by the FileEntry class.
 */
FileEntry::~FileEntry()
{
}


/** \func std::string getComment() const;
 * \brief Retrieve the comment of the file entry.
 *
 * This function returns the comment of thse entry.
 *
 * If the entry was not assigned a comment, this function returns
 * an empty string.
 *
 * \return The comment associated with this entry, if there is one.
 */


/** \brief Retrive the size of the file when compressed.
 *
 * This function returns the compressed size of the entry. If the
 * entry is not stored in a compressed format, the uncompressed
 * size is returned.
 *
 * \return The compressed size of the entry.
 */
size_t FileEntry::getCompressedSize() const
{
    return getSize();
}




/** \func uint32_t getCrc() const;
 * \brief Return the CRC of the entry.
 *
 * This function returns the CRC 32 of this entry, if it has one.
 *
 * The CRC is set only after the file is compressed so it may
 * not always be available. The hasCrc() function can be used
 * to know whether it was set before.
 *
 * \return The CRC32 for the entry, if it has one.
 */
FileEntry::crc32_t FileEntry::getCrc() const
{
    // FIXME: should we throw if m_has_crc_32 is false?
    return m_crc_32;
}


/** \func FileEntry::buffer_t getExtra() const;
 * \brief Some extra data to be stored along the entry.
 *
 * This function returns a vector of bytes of extra data that are
 * stored with the entry.
 *
 * \return A buffer_t of extra bytes that are associated with this entry.
 */
FileEntry::buffer_t FileEntry::getExtra() const
{
    return buffer_t();
}




/** \func StorageMethod getMethod();
 * \brief Return the method used to create this entry.
 *
 * This function returns the method used to store the entry in
 * the FileCollection it is attached to.
 *
 * \return the storage method used to store the entry in a collection.
 *
 * \sa StorageMethod
 */


/** \brief Return the filename of the entry.
 *
 * The function returns the full filename of the entry, including
 * a path if the entry is stored in a sub-folder.
 *
 * \return The filename of the entry including its path.
 */
std::string FileEntry::getName() const
{
    return m_filename;
}


/** \brief Return the basename of this entry.
 *
 * This function returns the filename only of the entry.
 *
 * The function uses a FilePath object to remove the path from
 * the full filename.
 *
 * \return The filename of the entry.
 */
std::string FileEntry::getFileName() const
{
    if(isDirectory())
    {
        return std::string();
    }

    return FilePath(m_filename).filename();
}


/** \func uint32_t getTime() const
 * \brief Get the MS-DOS date/time of this entry.
 *
 * This function returns the date and time of the entry in MSDOS
 * date/time format.
 *
 * \note
 * An MS-DOS date is limited to 127 years starting on 1980.
 * So it will be over after Dec 31, 2107.
 *
 * \return The date and time of the entry in MS-DOS format.
 */


/** \brief Retrieve the size of the file when uncompressed.
 *
 * This function returns the uncompressed size of the entry data.
 *
 * \return Returns the uncompressed size of the entry.
 */
size_t FileEntry::getSize() const
{
    return m_uncompress_size;
}


/** \func std::time_t getUnixTime() const;
 * \brief Get the Unix date/time of this entry.
 *
 * This function returns the date and time of the entry in Unix
 * date/time format (see time()).
 *
 * \note
 * Unless you have an old 32 bit system that defines time_t
 * as a 32 bit value, a Unix date can be considered infinite.
 * Otherwise it is limited to some time in 2068.
 *
 * \return The date and time of the entry as a time_t value.
 */
std::time_t FileEntry::getUnixTime() const
{
    return dos2unixtime(getTime());
}




/** \brief Check whether the filename represents a directory.
 *
 * This function checks the last character of the filename, if it
 * is a separator ('/') then the function returns true meaning
 * that the file represents a directory.
 *
 * \return true if the entry represents a directory.
 */
bool FileEntry::isDirectory() const
{
    if(m_filename.empty())
    {
        throw IOException("FileEntry filename cannot be empty if it is to represent a directory.");
    }

    return m_filename.back() == g_separator;
}


/** \brief Check whether this entry is valid.
 *
 * Any method or operator that initializes a FileEntry may set a
 * flag that specifies whether the file entry is valid or not. If
 * it is not this method returns false.
 *
 * \return true if the FileEntry has been parsed succesfully.
 */
bool FileEntry::isValid() const
{
    return m_valid;
}


/** \brief Set the comment field for the FileEntry.
 *
 * This function sets the comment of this FileEntry. Note that
 * all implementations of the FileEntry may not include support
 * for a comment. In that case this function does nothing.
 *
 * \param[in] comment  A string with the new comment.
 */
void FileEntry::setComment(std::string const &)
{
    // By default, no comment!
}




/** \brief Save the CRC of the entry.
 *
 * This funciton savees the CRC field in this FileEntry field.
 *
 * \param crc value to set the crc field to.
 */
void FileEntry::setCrc(crc32_t crc)
{
    static_cast<void>(crc);
}


/**   Sets the size field for the entry.
 * \param size the size field is set to this value.
 */
void FileEntry::setName(std::string const& name)
{
    m_filename = name;
}


/** \brief Output an entry as a string to a stream.
 *
 * This function transforms the FileEntry into a string and prints
 * the result to the specified output stream.
 *
 * \param[in,out] os  The output stream.
 * \param[in] entry  The entry to print out.
 *
 * \return A reference to the output stream.
 */
std::ostream& operator << (std::ostream& os, FileEntry const& entry)
{
    os << entry.toString();
    return os;
}


/** \brief Output an entry from its pointer.
 *
 * To make it easier, we offer an ostream << operator to print out a
 * FileEntry from a pointer_t of such.
 *
 * The one limitation is that you would have to reinterpret cast your
 * pointer to (void *) if you wanted to print the actual pointer.
 *
 * \param[in,out] os  The output stream.
 * \param[in] entry_ptr  The entry pointer to print out.
 *
 * \return A reference to the output stream.
 */
std::ostream& operator << (std::ostream &os, FileEntry::pointer_t const& entry_ptr)
{
    os << entry_ptr->toString();
    return os;
}


} // namespace
// vim: ts=4 sw=4 et
