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
 * \brief Implementation of BasicEntry.
 */

#include "zipios++/basicentry.h"

#include "zipios++/zipiosexceptions.h"

#include "zipios_common.h"


namespace zipios
{

/** \class BasicEntry
 * \brief A file entry that does not use compression.
 *
 * BasicEntry is a FileEntry that is suitable as a base class for
 * basic entries, that e.g. do not support any form of compression
 */


/** Constructor.
 *
 * \param[in] filename  The filename of the entry.
 * \param[in] comment  A comment for the entry.
 * \param[in] basepath  The base path to file.
 */
BasicEntry::BasicEntry(std::string const& filename,
                       std::string const& comment,
                       FilePath const& basepath)
    : m_filename(filename)
    , m_comment(comment)
    //, m_size(0) -- auto-init
    //, m_valid(false) -- auto-init
    , m_basepath(basepath)
{
    std::string const full_path(m_basepath + m_filename);
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if (!is)
    {
        m_valid = false;
    }
    else
    {
        is.seekg(0, std::ios::end);
        m_size = is.tellg();
        is.close();
        m_valid = true;
    }
}


BasicEntry::~BasicEntry()
{
}


std::string BasicEntry::getComment() const
{
    return m_comment;
}


uint32_t BasicEntry::getCompressedSize() const
{
    return getSize();
}


uint32_t BasicEntry::getCrc() const
{
    return 0;
}


std::vector<unsigned char> BasicEntry::getExtra() const
{
    return std::vector<unsigned char>();
}


StorageMethod BasicEntry::getMethod() const
{
    return StorageMethod::STORED;
}

std::string BasicEntry::getName() const
{
    return m_filename;
}


std::string BasicEntry::getFileName() const
{
    if(isDirectory())
    {
        return std::string();
    }

    std::string::size_type const pos(m_filename.find_last_of(g_separator));
    if(pos != std::string::npos) // separator found!
    {
        // isDirectory() check means pos should not be last, so pos+1 is ok
        return m_filename.substr(pos + 1);
    }

    return m_filename;
}

uint32_t BasicEntry::getSize() const
{
    return m_size;
}


int BasicEntry::getTime() const
{
    return 0; // FIXME later
}


std::time_t BasicEntry::getUnixTime() const
{
    return 0; // Mon Mar 24 18:33:19 PDT 2014 (RDB)
}


bool BasicEntry::isValid() const
{
    return m_valid;
}


//     virtual int hashCode() const {}


bool BasicEntry::isDirectory() const
{
    if(m_filename.size() == 0)
    {
        throw IOException("directory filename cannot be empty");
    }

    return m_filename[m_filename.size() - 1] == g_separator;
}


void BasicEntry::setComment(std::string const& comment)
{
    m_comment = comment;
}


void BasicEntry::setCompressedSize(uint32_t)
{
}


void BasicEntry::setCrc(uint32_t)
{
}


void BasicEntry::setExtra(std::vector<unsigned char> const&)
{
}


void BasicEntry::setMethod(StorageMethod)
{
}


void BasicEntry::setName(std::string const& name)
{
    m_filename = name;
}


void BasicEntry::setSize(uint32_t size)
{
    m_size = size;
}


void BasicEntry::setTime(int)
{
}


void BasicEntry::setUnixTime(std::time_t)
{
}


std::string BasicEntry::toString() const
{
    OutputStringStream sout;
    sout << m_filename << " (" << m_size << " bytes)";
    return sout.str();
}


FileEntry *BasicEntry::clone() const
{
    return new BasicEntry(*this);
}


} // zipios namespace

// vim: ts=4 sw=4 et
