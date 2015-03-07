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
 * \brief Header file that defines FileEntry.
 *
 * The FileEntry class is an interface defining a virtual representation
 * to a file. File entries are found in various file collections.
 *
 * \sa FileCollection
 */

#include "zipios++/meta-iostreams.hpp"

#include <ctime>
#include <memory>
#include <string>
#include <vector>


namespace zipios
{


enum class StorageMethod : uint8_t
{
    STORED = 0,
    SHRUNK,
    REDUCED1,
    REDUCED2,
    REDUCED3,
    REDUCED4,
    IMPLODED,
    RESERVED,
    DEFLATED
};



class FileEntry
{
public:
    typedef std::shared_ptr<FileEntry>      pointer_t;
    typedef std::vector<pointer_t>          vector_t;
    typedef std::vector<unsigned char>      buffer_t;
    typedef uint32_t                        crc32_t;
    typedef uint32_t                        dostime_t;

                                FileEntry(std::string const& filename);
    virtual pointer_t           clone() const = 0;
    virtual                     ~FileEntry();

    virtual std::string         getComment() const;
    virtual size_t              getCompressedSize() const;
    virtual crc32_t             getCrc() const;
    virtual buffer_t            getExtra() const;
    virtual StorageMethod       getMethod() const;
    virtual std::string         getName() const;
    virtual std::string         getFileName() const;
    virtual size_t              getSize() const;
    virtual dostime_t           getTime() const;
    virtual std::time_t         getUnixTime() const;
    bool                        hasCrc() const;
    virtual bool                isDirectory() const;
    virtual bool                isValid() const;
    virtual void                setComment(std::string const& comment);
    virtual void                setCompressedSize(size_t size);
    virtual void                setCrc(crc32_t crc);
    virtual void                setExtra(buffer_t const& extra);
    virtual void                setMethod(StorageMethod method);
    virtual void                setName(std::string const& name);
    virtual void                setSize(size_t size);
    virtual void                setTime(dostime_t time);
    virtual void                setUnixTime(std::time_t time);
    virtual std::string         toString() const = 0;

protected:
    std::string             m_filename;
    size_t                  m_uncompressed_size = 0;
    time_t                  m_unix_time = 0;
    uint32_t                m_crc_32 = 0;
    bool                    m_has_crc_32 = false;
    bool                    m_valid = false;
};


std::ostream& operator << (std::ostream &os, FileEntry const& entry);


} // zipios namespace
// vim: ts=4 sw=4 et
