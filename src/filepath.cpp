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
 * Implementation of FilePath.
 */

#include "zipios++/filepath.h"

#include "zipios_common.h"

#include <sys/stat.h>

namespace zipios
{


FilePath::FilePath(std::string const& path, bool check_exists)
    : m_checked(false)
    , m_path(path)
{
    pruneTrailingSeparator();
    if(check_exists)
    {
        exists();
    }
}


/** \brief Read the file mode.
 *
 * This function sets m_checked to true, stat()'s the path, to see if
 * it exists and to determine what type of file it is. All the query
 * functions call check() before they test a flag to make sure it
 * is set appropriately.
 *
 * This means stat()'ing is deferred until it becomes necessary. But also
 * it is cached meaning that if the file changes in between we get the
 * old flags.
 */
void FilePath::check() const
{
    if(!m_checked)
    {
        m_checked     = true;
        m_exists      = false;

        m_is_reg      = false;
        m_is_dir      = false;
        m_is_char     = false;
        m_is_block    = false;
        m_is_socket   = false;
        m_is_fifo     = false;

        // TODO: under MS-Windows, we need to use _wstat()
        //       to make it work in Unicode (i.e. UTF-8
        //       to wchar_t then call _wstat()...)
        //       Also we want to use the 64 bit variant
        //       to make sure that we get a valid size
        struct stat buf;
        if(stat(m_path.c_str(), &buf) != -1)
        {
            m_exists    = true;
            m_is_reg    = S_ISREG (buf.st_mode);
            m_is_dir    = S_ISDIR (buf.st_mode);
            m_is_char   = S_ISCHR (buf.st_mode);
            m_is_block  = S_ISBLK (buf.st_mode);
            m_is_socket = S_ISSOCK(buf.st_mode);
            m_is_fifo   = S_ISFIFO(buf.st_mode);
        }
    }
}


FilePath& FilePath::operator = (std::string const& rhs)
{
    m_path = rhs;
    pruneTrailingSeparator();
    return *this;
}

void FilePath::pruneTrailingSeparator()
{
    while(m_path.size() > 0)
    {
        if(m_path[m_path.size() - 1] == g_separator)
        {
            m_path.erase(m_path.size() - 1); 
        }
    }
}

FilePath::operator std::string () const
{
    return m_path;
} 


FilePath FilePath::operator + (FilePath const& name) const
{
    if(m_path.size() > 0)
    {
        return m_path + g_separator + name.m_path;
    }

    return name.m_path;
}


FilePath FilePath::filename() const
{
    std::string::size_type pos;
    pos = m_path.find_last_of(g_separator);
    if(pos != std::string::npos)
    {
        return m_path.substr(pos + 1);
    }

    return m_path;
}


bool FilePath::exists() const
{
    check();
    return m_exists;
}


bool FilePath::isRegular() const
{
    check();
    return m_is_reg;
}


bool FilePath::isDirectory() const
{
    check();
    return m_is_dir;
}


bool FilePath::isCharSpecial() const
{
    check();
    return m_is_char;
}


bool FilePath::isBlockSpecial() const
{
    check();
    return m_is_block;
}


bool FilePath::isSocket() const
{
    check();
    return m_is_socket;
}


bool FilePath::isFifo() const
{
    check();
    return m_is_fifo;
}


} // namespace
// vim: ts=4 sw=4 et
