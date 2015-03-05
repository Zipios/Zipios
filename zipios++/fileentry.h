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
    Header file that defines FileEntry.
*/

#include "zipios++/meta-iostreams.h"

#include <ctime>
#include <memory>
#include <string>
#include <vector>


namespace zipios
{


/** \brief The types used with FileEntry::setMethod and FileEntry::getMethod.
 *
 * The current entries are the types supported by the zip format. The
 * numbering matches one to one the numbering used in the zip file format,
 * i.e. STORED is indicated by a 0 in the method field in a zip file and
 * so on.
 */
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

    virtual std::string         getComment() const = 0;
    virtual size_t              getCompressedSize() const;
    virtual crc32_t             getCrc() const;
    virtual buffer_t            getExtra() const;
    virtual StorageMethod       getMethod() const = 0;
    virtual std::string         getName() const;
    virtual std::string         getFileName() const;
    virtual size_t              getSize() const;
    virtual dostime_t           getTime() const = 0;
    virtual std::time_t         getUnixTime() const;
    bool                        hasCrc() const;
    //virtual int                 hashCode() const = 0;
    virtual bool                isDirectory() const;
    virtual bool                isValid() const;
    virtual void                setComment(std::string const& comment);

    /** Set the compressed size field of the entry.
      @param size value to set the compressed size field of the entry to.
     */
    virtual void setCompressedSize(uint32_t size) = 0;

    virtual void setCrc(crc32_t crc);

    /** Sets the extra field.
      @param extra the extra field is set to this value.
     */
    virtual void setExtra(std::vector<unsigned char> const& extra) = 0;

    /** Sets the storage method field for the entry.
      @param method the method field is set to the specified value.
     */
    virtual void setMethod(StorageMethod method) = 0;

    /** Sets the name field for the entry.
      @param name the name field is set to the specified value.
     */
    virtual void setName(std::string const& name);

    virtual void setSize(uint32_t size) = 0;

    /** Sets the time field for the entry.
      @param time set time field as is using this MSDOS date/time formatted value.
     */
    virtual void setTime(dostime_t time) = 0;

    /** Sets the time field in Unix time format for the entry.
      @param time the time field is set to the specified value.
     */
    virtual void setUnixTime(std::time_t time) = 0;

    /** Returns a human-readable string representation of the entry.
      @return a human-readable string representation of the entry.
     */
    virtual std::string toString() const = 0;

protected:
    std::string             m_filename;
    size_t                  m_uncompress_size = 0;
    uint32_t                m_crc_32 = 0;
    bool                    m_has_crc_32 = false;
    bool                    m_valid = false;
};


std::ostream& operator << (std::ostream &os, FileEntry const& entry);
std::ostream& operator << (std::ostream &os, FileEntry::pointer_t const& entry);


} // zipios namespace
// vim: ts=4 sw=4 et
