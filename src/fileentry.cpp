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
 * \brief Implementation of zipios::FileEntry.
 *
 * This file includes the implementation of the Fzipios::ileEntry.
 * Many of the functions in zipios::FileEntry are pure virtual so
 * only their documentation appears here.
 *
 * zipios::FileEntry is used to build collections of files.
 */

#include "zipios++/fileentry.hpp"

#include "dostime.h"
#include "zipios++/zipiosexceptions.hpp"

#include "zipios_common.hpp"


namespace zipios
{

/** \enum StorageMethod
 * \brief The types used with FileEntry::setMethod and FileEntry::getMethod.
 *
 * The current entries are the types supported by the zip format. The
 * numbering matches one to one the numbering used in the zip file format,
 * i.e. STORED is indicated by a 0 in the method field in a zip file and
 * so on.
 *
 * The zipios library only support STORED and DEFLATED at this time.
 */


/** \class FileEntry
 * \brief A FileEntry represents an entry in a FileCollection.
 *
 * The interface is a copy of the ZipEntry interface from the java.util.zip
 * package. The name has been changed to FileEntry, as FileCollection
 * is a more general abstraction, that covers other types of file
 * collections than just zip files.
 *
 * \note
 * The hashCode() supported in Java is not included as we do not have an
 * equivalent in this library.
 */


/** \brief Initialize a FileEntry object.
 *
 * This funciton initializes a FileEntry object. By default you may define
 * the filename of the FileEntry object.
 *
 * In case of an on disk directory, the filename should be the path to the
 * file that can be read from disk, otherwise the FileEntry object will be
 * viewed as invalid. There is, otherwise, no restriction to the filename.
 *
 * \param[in] filename  The file entry filename.
 */
FileEntry::FileEntry(FilePath const& filename)
    : m_filename(filename)
    //, m_uncompressed_size(0) -- auto-init
    //, m_unix_time(0) -- auto-init
    //, m_entry_offset(0) -- auto-init
    //, m_crc_32(0) -- auto-init
    //, m_has_crc_32(false) -- auto-init
    //, m_valid(false) -- auto-init
{
}


/** \fn FileEntry::pointer_t FileEntry::clone() const;
 * \brief Create a clone of a file entry.
 *
 * This function creates a heap allocated clone of the object
 * this method is called for.
 *
 * Note that the object is expected to overload this function in
 * order to create a clone of the correct type.
 *
 * \return A smart pointer to the clone.
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


/** \brief Retrieve the comment of the file entry.
 *
 * This function returns the comment of this entry.
 *
 * If the entry was not assigned a comment, this function returns
 * an empty string.
 *
 * Note that certain types of file entries cannot be assigned a
 * comment and thus this method will always return an empty string
 * for those.
 *
 * \return The comment associated with this entry, if there is one.
 */
std::string FileEntry::getComment() const
{
    return "";
}


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




/** \brief Return the CRC of the entry.
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
    /** \FIXME
     * Should we throw if m_has_crc_32 is false?
     */
    return m_crc_32;
}


/** \brief Get the offset of this entry in a Zip archive.
 *
 * This function retrieves the offset at which this FileEntry
 * resides in the Zip archive it is attached to.
 *
 * Note that in case of a Zip archive embedded in another file,
 * the offset is virtual (relative to the start of the Zip archive
 * in the larger file.)
 *
 * \return The position in the Zip archive.
 */
std::streampos FileEntry::getEntryOffset() const
{
    return m_entry_offset;
}


/** \brief Some extra data to be stored along the entry.
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


/** \brief Retrieve the size of the header.
 *
 * This function determines the size of the Zip archive header necessary
 * for that file.
 *
 * By default the function returns zero meaning that no header is defined.
 *
 * \return The size of the header in bytes.
 */
size_t FileEntry::getHeaderSize() const
{
    return 0;
}


/** \brief Return the method used to create this entry.
 *
 * This function returns the method used to store the entry in
 * the FileCollection it is attached to.
 *
 * \return the storage method used to store the entry in a collection.
 *
 * \sa StorageMethod
 * \sa setMethod()
 */
StorageMethod FileEntry::getMethod() const
{
    return StorageMethod::STORED;
}




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
 * \warning
 * The function returns the last segment of the filename whether it is
 * a regular file or a directory so one can search for a directory with
 * the MATCH or IGNORE search options.
 *
 * \return The filename of the entry.
 */
std::string FileEntry::getFileName() const
{
    return m_filename.filename();
}


/** \brief Retrieve the size of the file when uncompressed.
 *
 * This function returns the uncompressed size of the entry data.
 *
 * \return Returns the uncompressed size of the entry.
 */
size_t FileEntry::getSize() const
{
    return m_uncompressed_size;
}


/** \brief Get the MS-DOS date/time of this entry.
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
FileEntry::dostime_t FileEntry::getTime() const
{
    return unix2dostime(m_unix_time);
}


/** \brief Get the Unix date/time of this entry.
 *
 * This function returns the date and time of the entry in Unix
 * date/time format (see time()).
 *
 * \note
 * The FileEntry object saves the time as a Unix time_t value,
 * however, the Zip file format uses the DOS time format. So
 * for a Zip file, the precision of the date is to the next
 * even second. Yet, this function may return a value which
 * is odd when the time comes from a file on disk.
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
    return m_unix_time;
}


/** \brief Check whether the CRC32 was defined.
 *
 * This function returns true if the setCrc() function was called earlier
 * with a valid CRC32 and the FileEntry implementation supports a CRC (i.e.
 * a DirectoryEntry does not have a CRC).
 *
 * \return true if a CRC32 is defined in this class.
 */
bool FileEntry::hasCrc() const
{
    return m_has_crc_32;
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
    return m_filename.isDirectory();
}


/** \brief Compare two file entries for equality.
 *
 * This function compares most of the fields between two file
 * entries to see whether they are equal or not.
 *
 * \param[in] file_entry  The file entry to compare this against.
 *
 * \return true if both FileEntry objects are considered equal.
 */
bool FileEntry::isEqual(FileEntry const& file_entry) const
{
    return m_filename          == file_entry.m_filename
        && m_uncompressed_size == file_entry.m_uncompressed_size
        && m_unix_time         == file_entry.m_unix_time
        && m_crc_32            == file_entry.m_crc_32
        && m_has_crc_32        == file_entry.m_has_crc_32
        && m_valid             == file_entry.m_valid;
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
void FileEntry::setComment(std::string const& comment)
{
    // By default, no comment!
    static_cast<void>(comment);
}


/** \brief Set the size when the file is compressed.
 *
 * This function saves the compressed size of the entry in this object.
 *
 * By default the compressed size is viewed as the same as the
 * uncompressed size (i.e. as if STORED was used for the compression
 * method.)
 *
 * \param[in] size  Value to set the compressed size field of the entry to.
 */
void FileEntry::setCompressedSize(size_t size)
{
    static_cast<void>(size);
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


/** \brief Defines the position of the entry in a Zip archive.
 *
 * This function defines the position of the FileEntry in a
 * Zip archive. By default the position is set to zero.
 *
 * The offset is generally read from a Zip directory entry.
 *
 * When used to seek in a file, the FileCollection will add
 * the start offset defined in the VirtualSeeker. In other words
 * this is the position in the Zip archive itself, not the final
 * position in the file you are reading the archive from.
 *
 * \param[in] offset  The new archive entry offset.
 */
void FileEntry::setEntryOffset(std::streampos offset)
{
    m_entry_offset = offset;
}


/** \brief Set the extra field buffer.
 *
 * This function is used to set the extra field.
 *
 * Only one type of file entry supports an extra field buffer.
 * The others do nothing when this function is called.
 *
 * \param[in] extra  The extra field is set to this value.
 */
void FileEntry::setExtra(buffer_t const& extra)
{
    static_cast<void>(extra);
}


/** \brief Sets the storage method field for the entry.
 *
 * This function sets the method with which the file data is to
 * be compressed.
 *
 * The method is ignored in a file entry which cannot be compressed.
 * (or more precisly, the method is forced as STORED.)
 *
 * \param[in] method  The method field is set to the specified value.
 */
void FileEntry::setMethod(StorageMethod method)
{
    static_cast<void>(method);
}


/** \brief Sets the size field for the entry.
 *
 * This function is used to save the size of this file on disk
 * when uncompressed.
 *
 * \param[in] size  The size field is set to this value.
 */
void FileEntry::setSize(size_t size)
{
    m_uncompressed_size = size;
}


/** \brief Set the FileEntry time using a DOS time.
 *
 * This function saves the specified \p dostime value as the last modification
 * date and time of this entry. This is generally used when reading that information
 * from a Zip archive. Otherwise you probably want to use the setUnixTime()
 * instead since it is one to one compatible with the value handle by time(),
 * stat(), and other OS functions.
 *
 * \param[in] dostime  Set time field as is using this MSDOS date/time value.
 */
void FileEntry::setTime(dostime_t dostime)
{
    setUnixTime(dos2unixtime(dostime));
}


/** \brief Sets the time field in Unix time format for the entry.
 *
 * This function is used to set the last modification time of this
 * entry. In most cases this comes from the stat structure field
 * named st_mtime. If you are creating a file directly in memory,
 * you may use the return value of <code>time(nullptr);</code>.
 *
 * \param[in] time  The time field is set to the specified value.
 */
void FileEntry::setUnixTime(std::time_t time)
{
    m_unix_time = time;
}


/** \fn std::string FileEntry::toString() const;
 * \brief Returns a human-readable string representation of the entry.
 *
 * This function transforms the basic information of the entry in a
 * string. Note that most of the information is lost as the function
 * is likely to only display the filename and the size of the file,
 * nothing more.
 *
 * \return A human-readable string representation of the entry.
 */


/** \brief Read this FileEntry from the input stream.
 *
 * This function is called when the FileEntry should be initialized from
 * the specified input stream.
 *
 * \exception IOException
 * The default implementation raise an IOException error because there is
 * no reading the FileEntry from anywhere.
 *
 * \param[in,out] is  The input stream.
 */
void FileEntry::read(std::istream& is)
{
    static_cast<void>(is);
    throw IOException("FileEntry::read(): read not available with this type of FileEntry.");
}


/** \brief Write this FileEntry to the output stream.
 *
 * This function is called when the FileEntry should be saved in the
 * specified output stream.
 *
 * \exception IOException
 * The default implementation raise an IOException error because there is
 * no writing the FileEntry anywhere.
 *
 * \param[in,out] os  The output stream.
 */
void FileEntry::write(std::ostream& os)
{
    static_cast<void>(os);
    throw IOException("FileEntry::write(): write not available with this type of FileEntry.");
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


} // namespace
// vim: ts=4 sw=4 et
