#pragma once
#ifndef ZIPOUTPUTSTREAM_HPP
#define ZIPOUTPUTSTREAM_HPP

/*
  Zipios++ - a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015  Made to Order Software Corporation

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
 * \brief Define the zipios::ZipOutputStream class.
 *
 * This file includes the class declaration of the zipios::ZipOutputStream
 * which is used to compress data being saved in a Zip archive.
 */

#include "zipoutputstreambuf.hpp"


namespace zipios
{

class ZipOutputStreambuf;

/** \brief A ZipOutputStream to allow for data to be compressed zlib.
 *
 * ZipOutputStream is an ostream that writes the output to a zip file. The
 * interface approximates the interface of the Java ZipOutputStream.
 */
class ZipOutputStream : public std::ostream
{
public:

    explicit ZipOutputStream(std::string const& filename);

    /** Destructor. */
    virtual ~ZipOutputStream();

    /** Closes the current entry updates its header with the relevant
      size information and positions the stream write pointer for the
      next entry header. Puts the stream in EOF state. Call
      putNextEntry() to clear the EOF stream state flag. */
    void closeEntry();

    /** Calls finish and if the ZipOutputStream was created with a
      filename as a parameter that file is closed as well. If the
      ZipOutputStream was created with an ostream as its first
      parameter nothing but the call to finish happens. */
    void close();

    /** Closes the current entry (if one is open), then writes the Zip
      Central Directory Structure closing the ZipOutputStream. The
      output stream that the zip archive is being written to is not
      closed. */
    void finish();

    void putNextEntry(FileEntry::pointer_t entry);

    /** Sets the global comment for the Zip archive. */
    void setComment(std::string const& comment);

    /** Sets the compression level to be used for subsequent entries. */
    void setLevel(int level);

    /** Sets the compression method to be used. only STORED and DEFLATED are
      supported. */
    void setMethod(StorageMethod method);

private:
    std::unique_ptr<std::ofstream>      m_ofs;
    std::unique_ptr<ZipOutputStreambuf> m_ozf;
};


} // zipios namespace

// vim: ts=4 sw=4 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

#endif
