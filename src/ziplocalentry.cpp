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
 * \brief Implementation of the zipios::ZipLocalEntry class.
 *
 * This file is the implementation of the zipios::ZipLocalEntry class
 * which handles zipios::FileEntry's found in Zip archives.
 */

#include "ziplocalentry.hpp"

#include "zipios++/zipiosexceptions.hpp"

#include "dostime.h"
#include "zipios_common.hpp"


namespace zipios
{


namespace
{

/** \brief The signature of a local entry.
 *
 * This value represents the signature of a Zip archive block defining
 * a ZipLocalEntry.
 *
 * \code
 * "PK 3.4"
 * \endcode
 */
uint32_t const      g_signature = 0x04034b50;


/** \brief ZipLocalEntry Header
 *
 * This structure shows how the header of the ZipLocalEntry is defined.
 * Note that the file name and extra field have a variable size which
 * is defined in two 16 bit values just before they appear.
 *
 * The filename cannot be empty, however, the extra field can (and
 * usually is).
 *
 * \note
 * This structure is NOT used directly only for its sizeof() and
 * documentation because that way zipios can work on little and big
 * endians without the need to know the endianess of your computer.
 */
struct ZipLocalEntryHeader
{
    uint32_t            m_signature;
    uint16_t            m_extract_version;
    uint16_t            m_gp_bitfield;
    uint16_t            m_compress_method;
    uint32_t            m_dostime;
    uint32_t            m_crc_32;
    uint32_t            m_compressed_size;
    uint32_t            m_uncompressed_size;
    uint16_t            m_filename_len;
    uint16_t            m_extra_field_len;
    //uint8_t             m_filename[m_filename_len];
    //uint8_t             m_extra_field[m_extra_field_len];
};


} // no name namespace


/** \class ZipLocalEntry
 * \brief An implementation of the FileEntry for Zip archives.
 *
 * A concrete implementation of the abstract FileEntry base class for
 * ZipFile entries, specifically for representing the information
 * present in the local headers of file entries in a zip file.
 */



ZipLocalEntry::ZipLocalEntry(std::string const& filename, buffer_t const& extra_field)
    : FileEntry(filename)
    //, m_extract_version(g_zip_format_version) -- auto-init
    //, m_gp_bitfield(0) -- auto-init
    //, m_compress_method(StorageMethod::STORED) -- auto-init
    //, m_compressed_size(0) -- auto-init
    //, m_extra_field() -- see below, beneficiate from the size check by using the setExtra() function
{
    setExtra(extra_field);
}


/** \brief Create a clone of a ZipLocalEntry object.
 *
 * This function allocates a new ZipLocalEntry on the heap and returns
 * a copy of this ZipLocalEntry object in it.
 *
 * \return A new ZipLocalEntry which is a clone of this ZipLocalEntry object.
 */
FileEntry::pointer_t ZipLocalEntry::clone() const
{
    return FileEntry::pointer_t(new ZipLocalEntry(*this));
}


/** \brief Clean up a ZipLocalEntry object.
 *
 * This function ensures proper clean up of a ZipLocationEntry object.
 */
ZipLocalEntry::~ZipLocalEntry()
{
}


/** \brief Compare two file entries for equality.
 *
 * This function compares most of the fields between two file
 * entries to see whether they are equal or not.
 *
 * \note
 * This function calls the base class isEqual() and also verifies
 * that the ZipLocalEntry fields are equal.
 *
 * \note
 * This function is also used to compare ZipCDirEntry since none
 * of the additional field participate in the comparison.
 *
 * \param[in] file_entry  The file entry to compare this against.
 *
 * \return true if both FileEntry objects are considered equal.
 */
bool ZipLocalEntry::isEqual(FileEntry const& file_entry) const
{
    ZipLocalEntry const * const ze(dynamic_cast<ZipLocalEntry const * const>(&file_entry));
    if(!ze)
    {
        return false;
    }
    return FileEntry::isEqual(file_entry)
        && m_extract_version == ze->m_extract_version
        && m_gp_bitfield     == ze->m_gp_bitfield
        && m_compress_method == ze->m_compress_method;
        //&& m_compressed_size == ze->m_compressed_size -- ignore in comparison
        //&& m_extra_field     == ze->m_extra_field -- ignored in comparison
}


size_t ZipLocalEntry::getCompressedSize() const
{
    return m_compressed_size;
}


ZipLocalEntry::buffer_t ZipLocalEntry::getExtra() const
{
    return m_extra_field;
}


/** \brief Retrieve the size of the header.
 *
 * This function dynamically determines the size of the Zip archive
 * header necessary for this FileEntry.
 *
 * This function returns the size of the Zip archive header.
 *
 * \return The size of the header in bytes.
 */
size_t ZipLocalEntry::getHeaderSize() const
{
    return sizeof(ZipLocalEntryHeader) + m_filename.length() + m_extra_field.size();
}


StorageMethod ZipLocalEntry::getMethod() const
{
  return m_compress_method;
}


void ZipLocalEntry::setCompressedSize(size_t size)
{
    m_compressed_size = size;
}


void ZipLocalEntry::setCrc(uint32_t crc)
{
    m_crc_32 = crc;
    m_has_crc_32 = true;
}


void ZipLocalEntry::setExtra(buffer_t const& extra)
{
    // TBD: use a max. of 0xFFFE to avoid problems with 64 bit headers
    if(extra.size() >= 0xFFFE)
    {
        throw InvalidException("ZipLocalEntry::setExtra(): trying to setup an extra buffer of more than 64Kb, maximum size is 0xFFFE.");
    }
    m_extra_field = extra;
}


void ZipLocalEntry::setMethod(StorageMethod method)
{
    m_compress_method = method;
}


std::string ZipLocalEntry::toString() const
{
    OutputStringStream sout;
    sout << m_filename
         << " (" << m_uncompressed_size << " bytes, "
         << m_compressed_size << " bytes compressed)";
    return sout.str();
}


bool ZipLocalEntry::trailingDataDescriptor() const
{
    // gp_bitfield bit 3 is one, if this entry uses a trailing data
    // descriptor to keep size, compressed size and crc-32
    // fields.
    return (m_gp_bitfield & 4) != 0;
}


void ZipLocalEntry::read(std::istream& is)
{
    m_valid = false; // set to true upon successful completion.

    //    // Before reading anything we record the position in the stream
    //    // This is a field in the central directory entry, but not
    //    // in the local entry. After all, we know where we are, anyway.
    //    zlh.rel_offset_loc_head  = is.tellg() ;

    uint32_t signature;
    zipRead(is, signature);                             // 32
    if(g_signature != signature)
    {
        // put stream in error state and return
        is.setstate(std::ios::failbit);
        throw IOException("ZipLocalEntry::read() expected a signature and got some other data");
    }

    uint16_t compress_method(0);
    uint32_t dostime(0);
    uint32_t compressed_size(0);
    uint32_t uncompressed_size(0);
    uint16_t filename_len(0);
    uint16_t extra_field_len(0);
    std::string filename;

    // See the ZipLocalEntryHeader for more details
    zipRead(is, m_extract_version);                 // 16
    zipRead(is, m_gp_bitfield);                     // 16
    zipRead(is, compress_method);                   // 16
    zipRead(is, dostime);                           // 32
    zipRead(is, m_crc_32);                          // 32
    zipRead(is, compressed_size);                   // 32
    zipRead(is, uncompressed_size);                 // 32
    zipRead(is, filename_len);                      // 16
    zipRead(is, extra_field_len);                   // 16
    zipRead(is, filename, filename_len);            // string
    zipRead(is, m_extra_field, extra_field_len);    // buffer
    /** \TODO add support for zip64, some of those parameters
     *        may be 0xFFFFF...FFFF in which case the 64 bit
     *        header should be read
     */

    m_compress_method = static_cast<StorageMethod>(compress_method);
    m_unix_time = dos2unixtime(dostime);
    m_compressed_size = compressed_size;
    m_uncompressed_size = uncompressed_size;
    m_filename = FilePath(filename);

    // the zipRead() should throw if the input is invalid...
    if(is)
    {
        m_valid = true;
    }
}


/** \brief Write a ZipLocalEntry to \p os.
 *
 * This function writes this ZipLocalEntry header to the specified
 * output stream.
 *
 * \exception IOException
 * If an error occurs while writing to the output stream, the function
 * throws an IOException.
 *
 * \param[in] os  The output stream where the ZipLocalEntry is written.
 */
void ZipLocalEntry::write(std::ostream& os)
{
    if(m_filename.length()  > 0x10000
    || m_extra_field.size() > 0x10000)
    {
        throw InvalidStateException("ZipLocalEntry::write(): file name or extra field too large to save in a Zip file.");
    }

    /** TODO: add support for 64 bit zip archive
     */
// Solaris defines _ILP32 for 32 bit platforms
#if !defined(_ILP32)
    if(m_compressed_size   >= 0x100000000UL
    || m_uncompressed_size >= 0x100000000UL)
    {
        throw InvalidStateException("The size of this file is too large to fit in a zip archive.");
    }
#endif

    uint16_t compress_method(static_cast<uint8_t>(m_compress_method));
    uint32_t dostime(unix2dostime(m_unix_time));
    uint32_t compressed_size(m_compressed_size);
    uint32_t uncompressed_size(m_uncompressed_size);
    uint16_t filename_len(m_filename.length());
    uint16_t extra_field_len(m_extra_field.size());

    // See the ZipLocalEntryHeader for more details
    zipWrite(os, g_signature);          // 32
    zipWrite(os, m_extract_version);    // 16
    zipWrite(os, m_gp_bitfield);        // 16
    zipWrite(os, compress_method);      // 16
    zipWrite(os, dostime);              // 32
    zipWrite(os, m_crc_32);             // 32
    zipWrite(os, compressed_size);      // 32
    zipWrite(os, uncompressed_size);    // 32
    zipWrite(os, filename_len);         // 16
    zipWrite(os, extra_field_len);      // 16
    zipWrite(os, m_filename);           // string
    zipWrite(os, m_extra_field);        // buffer
}


} // zipios namespace
// vim: ts=4 sw=4 et
