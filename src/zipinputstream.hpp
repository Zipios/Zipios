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
 * \brief Define zipios::ZipInputStream.
 *
 * This file declares the zipios::ZipInputStream class.
 *
 * The class is used to read data from a Zip archive, data that may
 * have been compressed using the zlib library.
 */

#include "zipinputstreambuf.hpp"


namespace zipios
{



/** \brief The ZipInputStream to read data from a Zip archive.
 *
 * ZipInputStream is an istream that gets it's input from a zip file. The
 * interface approximates the interface of the Java
 * ZipInputStream.
 */
class ZipInputStream : public std::istream
{
public:

    /** \brief Crate a ZipInputStream from a stream.
     *
     * This function expects an already open input stream which
     * represents a ZIP archive. The file must be seekable.
     *
     * The input file must remain open until the ZipInputStream object
     * gets destroyed.
     *
     * \param[in,out] is  An istream from which the compressed zip archive
     *                    can be read.
     * \param[in] pos   Position to reposition the istream to before reading.
     */
    explicit ZipInputStream(std::istream& is, std::streampos pos = 0);

    /** \brief Create a ZipInputStream from a file.
     *
     * This constructor creates a ZIP file by loading from filename.
     *
     * \param[in] filename  The name of a valid zip file.
     * \param[in] pos position to reposition the istream to before reading.
     */
    explicit ZipInputStream(std::string const& filename, std::streampos pos = 0);
    ZipInputStream(ZipInputStream const& src) = delete;
    ZipInputStream const& operator = (ZipInputStream const& src) = delete;

    /** \brief Clean up the input stream.
     *
     * The destructor ensures that all resources used by the class get
     * released.
     */
    virtual ~ZipInputStream();

    int available();

    /** Closes the current entry, and positions the stream read pointer at
      the beginning of the next entry (if there is one). */
    void closeEntry();

    /** Closes the istream. */
    void                            close();

    //ZipLocalEntry *createZipCDirEntry(const string &name);

    /** \brief Get the next entry from a Zip archive.
     *
     * This function opens the next entry in the zip archive and returns
     * a pointer to a FileEntry object for the entry. For new instances
     * this method has to be called once before you can read from the
     * first entry.
     *
     * \return A list constant FileEntry pointers containing information
     *         about the (now) current entry.
     */
    FileEntry::pointer_t                getNextEntry();

private:
    std::unique_ptr<std::ifstream>      m_ifs;
    std::unique_ptr<ZipInputStreambuf>  m_izf;
};


} // zipios namespace
// vim: ts=4 sw=4 et
