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

    /** \brief Initialize a ZipOutputStream object.
     *
     * This constructor saves the specified output stream internally and
     * makes use of it to write any data that is saved in the Zip archive.
     *
     * \param[in,out] os  The stream where the compressed data is written.
     */
    explicit ZipOutputStream(std::ostream& os);

    /** \brief ZipOutputStream constructor.
     *
     * Create an output stream that will be saved to a file.
     *
     * \param[in] filename  Name of the file to write the zip archive to.
     */
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

    /** \brief Begins writing the next entry.
     */
    void putNextEntry(FileEntry::pointer_t entry);

    /** \brief Begins writing the next entry.
     */
    void putNextEntry(std::string const& entryName);

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
