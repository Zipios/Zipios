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

#include "zipios++/basicentry.hpp"

#include "zipios++/zipiosexceptions.hpp"

#include "zipios_common.hpp"


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
    : FileEntry(filename)
    , m_comment(comment)
    //, m_uncompressed_size(0) -- auto-init
    //, m_valid(false) -- auto-init
    , m_basepath(basepath)
{
    // TODO: convert this open + close in a stat() instead so we get mtime too
    std::string const full_path(m_basepath + m_filename);
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if(!is)
    {
        m_valid = false;
    }
    else
    {
        is.seekg(0, std::ios::end);
        m_uncompressed_size = is.tellg();
        is.close();
        m_valid = true;
    }
}


/** \brief Create a copy of the BasicEntry.
 *
 * The clone function creates a copy of this BasicEntry object.
 *
 * In most cases, when a collection is copied, a clone of each
 * entry is created to avoid potential problems with sharing
 * the same object between various lists.
 *
 * \return A shared pointer of the new BasicEntry object.
 */
BasicEntry::pointer_t BasicEntry::clone() const
{
    return BasicEntry::pointer_t(new BasicEntry(*this));
}


BasicEntry::~BasicEntry()
{
}


std::string BasicEntry::getComment() const
{
    return m_comment;
}


void BasicEntry::setComment(std::string const& comment)
{
    m_comment = comment;
}


std::string BasicEntry::toString() const
{
    OutputStringStream sout;
    sout << m_filename << " (" << m_uncompressed_size << " bytes)";
    return sout.str();
}


} // zipios namespace
// vim: ts=4 sw=4 et
