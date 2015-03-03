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

#include "zipios++/zipios-config.h"

#include <string>


namespace zipios
{


/** FilePath represents a path to a file or directory name. FilePath has
 * member functions to check if the file path is a valid file system entity,
 * and to check what kind of file system entity it is, e.g. is it a file, a 
 * directory, a pipe etc.
 */
class FilePath
{
public:
    /** Constructor.
     * @param path A string representation of the path.
     * @param check_exists If true is specified the constructor will
     * check the existence and type of the path immidiately, instead of
     * deferring that task until it is needed.
     */
    FilePath(std::string const& path = "", bool check_exists = false);

    FilePath& operator = (std::string const& rhs);

    operator std::string () const;

    /** Concatenates FilePath objects. A file separator is inserted
      if appropriate. */
    FilePath operator + (FilePath const& name) const;

    /** Returns filename of the FilePath object by pruning the path
      off. */
    FilePath filename() const;

    /** @return true If the path is a valid file system entity. */
    bool exists() const;

    /** @return true if the path is a regular file. */
    bool isRegular() const;

    /** @return true if the path is a directory. */
    bool isDirectory() const;

    /** @return true if the path is character special (a character
      device file).  */
    bool isCharSpecial() const;

    /** @return true if the path is block special (a block device
      file). */
    bool isBlockSpecial() const;

    /** @return true if the path is a socket. */
    bool isSocket() const;

    /** @return true if the path is a Fifo (a pipe). */
    bool isFifo() const;

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
