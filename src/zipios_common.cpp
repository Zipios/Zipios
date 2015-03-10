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
 * \brief Implementation file containing miscellaneous small functions.
 */

#include "zipios_common.hpp"

#include "zipios++/zipiosexceptions.hpp"


namespace zipios
{


char const g_separator = '/';


void zipRead(std::istream& is, uint32_t& value)
{
    unsigned char buf[sizeof(value)];

    if(!is.read(reinterpret_cast<char *>(buf), sizeof(value)))
    {
        throw IOException("an I/O error while reading zip archive data from file.");
    }
    if(is.gcount() != sizeof(value))
    {
        throw IOException("EOF or an I/O error while reading zip archive data from file."); // LCOV_EXCL_LINE
    }

    // zip data is always in little endian
    value = (buf[0] <<  0)
          | (buf[1] <<  8)
          | (buf[2] << 16)
          | (buf[3] << 24);
}


void zipRead(std::istream& is, uint16_t& value)
{
    unsigned char buf[sizeof(value)];

    if(!is.read(reinterpret_cast<char *>(buf), sizeof(value)))
    {
        throw IOException("an I/O error while reading zip archive data from file.");
    }
    if(is.gcount() != sizeof(value))
    {
        throw IOException("EOF or an I/O error while reading zip archive data from file."); // LCOV_EXCL_LINE
    }

    // zip data is always in little endian
    value = (buf[0] <<  0)
          | (buf[1] <<  8);
}


void zipRead(std::istream& is, uint8_t&  value)
{
    unsigned char buf[sizeof(value)];

    if(!is.read(reinterpret_cast<char *>(buf), sizeof(value)))
    {
        throw IOException("an I/O error while reading zip archive data from file.");
    }
    if(is.gcount() != sizeof(value))
    {
        throw IOException("EOF or an I/O error while reading zip archive data from file."); // LCOV_EXCL_LINE
    }

    // zip data is always in little endian
    value = buf[0];
}


void zipRead(std::istream& is, buffer_t& buffer, ssize_t const count)
{
    buffer.resize(count);
    if(!is.read(reinterpret_cast<char *>(&buffer[0]), count))
    {
        throw IOException("an I/O error while reading zip archive data from file.");
    }
    if(is.gcount() != count)
    {
        throw IOException("EOF or an I/O error while reading zip archive data from file."); // LCOV_EXCL_LINE
    }
}


void zipRead(std::istream& is, std::string& str, ssize_t const count)
{
    str.resize(count);
    if(!is.read(reinterpret_cast<char *>(&str[0]), count))
    {
        throw IOException("an I/O error while reading zip archive data from file.");
    }
    if(is.gcount() != count)
    {
        throw IOException("EOF or an I/O error while reading zip archive data from file."); // LCOV_EXCL_LINE
    }
}


void zipRead(buffer_t const& is, size_t& pos, uint32_t& value)
{
    if(pos + sizeof(value) > is.size())
    {
        throw IOException("EOF reached while reading zip archive data from file.");
    }

    value = (is[pos + 0] <<  0)
          | (is[pos + 1] <<  8)
          | (is[pos + 2] << 16)
          | (is[pos + 3] << 24);

    pos += sizeof(value);
}


void zipRead(buffer_t const& is, size_t& pos, uint16_t& value)
{
    if(pos + sizeof(value) > is.size())
    {
        throw IOException("EOF reached while reading zip archive data from file.");
    }

    value = (is[pos + 0] <<  0)
          | (is[pos + 1] <<  8);

    pos += sizeof(value);
}


void zipRead(buffer_t const& is, size_t& pos, uint8_t& value)
{
    if(pos + sizeof(value) > is.size())
    {
        throw IOException("EOF reached while reading zip archive data from file.");
    }

    value = is[pos];

    pos += sizeof(value);
}


void zipRead(buffer_t const& is, size_t& pos, buffer_t& buffer, ssize_t const count)
{
    if(pos + count > is.size())
    {
        throw IOException("EOF reached while reading zip archive data from file.");
    }

    buffer.clear();
    buffer.insert(buffer.begin(), is.begin() + pos, is.begin() + pos + count);

    pos += count;
}


void zipRead(buffer_t const& is, size_t& pos, std::string& str, ssize_t const count)
{
    if(pos + count > is.size())
    {
        throw IOException("EOF reached while reading zip archive data from file.");
    }

    str.clear();
    str.insert(str.begin(), is.begin() + pos, is.begin() + pos + count);

    pos += count;
}


void zipWrite(std::ostream& os, uint32_t const& value)
{
    char buf[sizeof(value)];

    buf[0] = value >>  0;
    buf[1] = value >>  8;
    buf[2] = value >> 16;
    buf[3] = value >> 24;

    if(!os.write(buf, sizeof(value)))
    {
        throw IOException("an I/O error occurred while writing to a zip archive file.");
    }
}


void zipWrite(std::ostream& os, uint16_t const& value)
{
    char buf[sizeof(value)];

    buf[0] = value >>  0;
    buf[1] = value >>  8;

    if(!os.write(buf, sizeof(value)))
    {
        throw IOException("an I/O error occurred while writing to a zip archive file.");
    }
}


void zipWrite(std::ostream& os, uint8_t const& value)
{
    char buf[sizeof(value)];

    buf[0] = value;

    if(!os.write(buf, sizeof(value)))
    {
        throw IOException("an I/O error occurred while writing to a zip archive file.");
    }
}


void zipWrite(std::ostream& os, buffer_t const& buffer)
{
    if(!os.write(reinterpret_cast<char const *>(&buffer[0]), buffer.size()))
    {
        throw IOException("an I/O error occurred while writing to a zip archive file.");
    }
}


void zipWrite(std::ostream& os, std::string const& str)
{
    if(!os.write(&str[0], str.length()))
    {
        throw IOException("an I/O error occurred while writing to a zip archive file.");
    }
}


} // zipios namespace
// vim: ts=4 sw=4 et
