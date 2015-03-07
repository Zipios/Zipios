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
 * \brief Header file that defines FilePath.
 *
 * FilePath is used to manage paths and filenames to files and directories.
 */

#include "zipios++/zipios-config.hpp"

#include <string>


namespace zipios
{


class FilePath
{
public:
                    FilePath(std::string const& path = "", bool check_immediately = false);

                    operator std::string () const;
    FilePath&       operator = (std::string const& path);
    FilePath        operator + (FilePath const& name) const;
    FilePath        filename() const;
    bool            exists() const;
    bool            isRegular() const;
    bool            isDirectory() const;
    bool            isCharSpecial() const;
    bool            isBlockSpecial() const;
    bool            isSocket() const;
    bool            isFifo() const;

private:
    /** Prunes the trailing separator of a specified path. */
    void            pruneTrailingSeparator();
    void            check() const;

    mutable bool    m_checked;
    mutable bool    m_exists;
    mutable bool    m_is_reg;
    mutable bool    m_is_dir;
    mutable bool    m_is_char;
    mutable bool    m_is_block;
    mutable bool    m_is_socket;
    mutable bool    m_is_fifo;
    std::string     m_path;
};


} // namespace

// vim: ts=4 sw=4 et
