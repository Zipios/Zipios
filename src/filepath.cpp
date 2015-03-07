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
 * \brief Implementation of FilePath.
 */

#include "zipios++/filepath.hpp"

#include "zipios_common.hpp"

#include <sys/stat.h>


namespace zipios
{


/** \class FilePath
 * \brief Handle a file path and name and its statistics.
 *
 * The FilePath class represents a path to a file or directory name.
 * FilePath has member functions to check if the file path is a valid
 * file system entity, and to check what kind of file system entity
 * it is, e.g. is it a file, a directory, a pipe, etc.
 */


/** \brief Initialize a FilePath object.
 *
 * The constructor saves the path and if \p check_exists is true, read
 * the file statistics, especially the st_mode.
 *
 * \param[in] path  A string representation of the path.
 * \param[in] check_immediately  If call exists() as well.
 *
 * \sa exists()
 * \sa pruneTrailingSeparator()
 */
FilePath::FilePath(std::string const& path, bool check_immediately)
    : m_checked(false)
    , m_path(path)
{
    pruneTrailingSeparator();
    if(check_immediately)
    {
        check();
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


/** \brief Replace the path with a new path.
 *
 * This function replaces the internal path of this FilePath with
 * the new specified path.
 *
 * \param[in] path  The new path to save in this object.
 *
 * \return A reference to this object.
 */
FilePath& FilePath::operator = (std::string const& path)
{
    m_path = path;
    pruneTrailingSeparator();
    return *this;
}


/** \brief Prune the trailing separator if present.
 *
 * This function ensures that the FilePath does NOT end with a separator.
 *
 * \warning
 * At this time the path is not canonicalized properly. We expect path
 * to not include double separators one after another. However, passing
 * such a path to the FilePath will keep it as is.
 */
void FilePath::pruneTrailingSeparator()
{
    if(m_path.size() > 0)
    {
        if(m_path[m_path.size() - 1] == g_separator)
        {
            m_path.erase(m_path.size() - 1);
        }
    }
}


/** \brief Retrieve the path.
 *
 * This operator can be used to retrieve a copy of the path.
 *
 * \return The m_path string returned as is (i.e. the whole path).
 */
FilePath::operator std::string () const
{
    return m_path;
}


/** \brief Append the a child name to this path.
 *
 * This function concatenates two FilePath objects and returns
 * another FilePath.
 *
 * A file separator is inserted between both names if appropriate.
 *
 * \warning
 * Note that the function allows you to append two full paths,
 * or even a relative path (left) to a full path (right), which
 * may result in a new path that is not quite sensible.
 *
 * \param[in] rhs  The right hand side.
 */
FilePath FilePath::operator + (FilePath const& rhs) const
{
    if(m_path.empty())
    {
        return rhs;
    }

    if(rhs.m_path.empty())
    {
        return *this;
    }

    if(rhs.m_path[0] == g_separator)
    {
        return m_path + rhs.m_path;
    }

    return m_path + g_separator + rhs.m_path;
}


/** \brief Retrieve the basename.
 *
 * This function returns the filename part of the FilePath
 * object by pruning the path off.
 *
 * \return Return the basename of this FilePath filename.
 */
FilePath FilePath::filename() const
{
    std::string::size_type const pos(m_path.find_last_of(g_separator));
    if(pos != std::string::npos)
    {
        return m_path.substr(pos + 1);
    }

    return m_path;
}


/** \brief Check whether the file exists.
 *
 * This function calls check() and then returns true if the file
 * exists on disk.
 *
 * \return true If the path is a valid file system entity.
 */
bool FilePath::exists() const
{
    check();
    return m_exists;
}


/** \brief Check whether the file is a regular file.
 *
 * This function returns true if the file exists and is a
 * regular file.
 *
 * \return true if the path is a regular file.
 */
bool FilePath::isRegular() const
{
    check();
    return m_is_reg;
}


/** \brief Check whether the file is a directory.
 *
 * This function returns true if the file exists and is a
 * directory.
 *
 * \return true if the path is a directory.
 */
bool FilePath::isDirectory() const
{
    check();
    return m_is_dir;
}


/** \brief Check whether the file is a character special file.
 *
 * This function returns true if the file exists and is a
 * character special file.
 *
 * \return true if the path is character special (a character device file).
 */
bool FilePath::isCharSpecial() const
{
    check();
    return m_is_char;
}


/** \brief Check whether the file is a block special file.
 *
 * This function returns true if the file exists and is a
 * block special file.
 *
 * \return true if the path is block special (a block device file).
 */
bool FilePath::isBlockSpecial() const
{
    check();
    return m_is_block;
}


/** \brief Check whether the file is a socket.
 *
 * This function returns true if the file exists and is a
 * socket file.
 *
 * \return true if the path is a socket.
 */
bool FilePath::isSocket() const
{
    check();
    return m_is_socket;
}


/** \brief Check whether the file is a pipe.
 *
 * This function returns true if the file exists and is a
 * pipe file.
 *
 * \return true if the path is a FIFO.
 */
bool FilePath::isFifo() const
{
    check();
    return m_is_fifo;
}


} // namespace
// vim: ts=4 sw=4 et
