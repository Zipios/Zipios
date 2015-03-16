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
 * \brief Declaration of the zipios::ZipCentralDirectoryEntry, which represents a
 *        directory Zip archive entry.
 *
 * This header file contains the zipios::ZipCentralDirectoryEntry class used
 * to read the central directory fields found in a zip archive.
 * It can also be used to write a central directory to an output
 * Zip archive.
 */

#include "ziplocalentry.hpp"


namespace zipios
{


class ZipCentralDirectoryEntry : public ZipLocalEntry
{
public:
                                ZipCentralDirectoryEntry(std::string const& filename = "", std::string const& file_comment = "", buffer_t const& extra_field = buffer_t());
    virtual pointer_t           clone() const override;
    virtual                     ~ZipCentralDirectoryEntry() override;

    virtual std::string         getComment() const override;
    virtual size_t              getHeaderSize() const override;
    virtual void                setComment(std::string const& comment) override;
    virtual std::string         toString() const override;

    virtual void                read(std::istream& is) override;
    virtual void                write(std::ostream& os) override;

private:
    std::string                 m_file_comment;
};


} // zipios namespace
// vim: ts=4 sw=4 et
