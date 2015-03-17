#pragma once
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
 * \brief Declare the zipios::ZipLocalEntry class used to handle Zip entries.
 *
 * This header file contains the zipios::ZipLocalEntry, which is used
 * to handle entries in a Zip archive.
 *
 * \sa zipios::ZipCDirEntry
 */

#include "zipios++/fileentry.hpp"


namespace zipios
{


class ZipLocalEntry : public FileEntry
{
public:
    // Zip file format version
    static uint16_t const       g_zip_format_version = 20; // 2.0

                                ZipLocalEntry(std::string const& filename = "", buffer_t const& extra_field = buffer_t());
    virtual pointer_t           clone() const override;
    virtual                     ~ZipLocalEntry() override;

    virtual size_t              getCompressedSize() const override;
    virtual buffer_t            getExtra() const override;
    virtual size_t              getHeaderSize() const override;
    virtual StorageMethod       getMethod() const override;
    virtual bool                isEqual(FileEntry const& file_entry) const override;
    virtual void                setCompressedSize(size_t size) override;
    virtual void                setCrc(crc32_t crc) override;
    virtual void                setExtra(buffer_t const& extra) override;
    virtual void                setMethod(StorageMethod method) override;
    virtual std::string         toString() const override;

    bool                        trailingDataDescriptor() const;

    virtual void                read(std::istream& is) override;
    virtual void                write(std::ostream& os) override;

protected:
    uint16_t                    m_extract_version = g_zip_format_version;
    uint16_t                    m_general_purpose_bitfield = 0;
    StorageMethod               m_compress_method = StorageMethod::STORED;
    size_t                      m_compressed_size = 0;
    buffer_t                    m_extra_field;
};


} // zipios namespace
// vim: ts=4 sw=4 et
