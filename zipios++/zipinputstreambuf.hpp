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
 * \brief Header file that defines ZipInputStreambuf.
 */

#include "zipios++/inflateinputstreambuf.hpp"

#include "zipios++/ziphead.hpp"


namespace zipios
{


/** ZipInputStreambuf is a zip input streambuf filter.
 */
class ZipInputStreambuf : public InflateInputStreambuf
{
public:
    /** ZipInputStreambuf constructor.
      @param inbuf the streambuf to use for input.
      @param s_pos a position to reset the inbuf to before reading. Specify
      -1 to read from the current position.
      @param del_inbuf if true is specified inbuf will be deleted, when
      the ZipInputStreambuf is destructed.
     */
    explicit ZipInputStreambuf(std::streambuf *inbuf, int s_pos = -1);

    /** Copy-constructor is private to prevent copying. */
    ZipInputStreambuf(ZipInputStreambuf const& src) = delete;

    /** Copy-assignment operator is private to prevent copying.  */
    ZipInputStreambuf const& operator = (ZipInputStreambuf const& src) = delete;

    /** Destructor. */
    virtual ~ZipInputStreambuf();

    /** Closes the current entry, and positions the stream read pointer at
      the beginning of the next entry (if there is one). */
    void closeEntry();

    /** Closes the streambuf. */
    void close();

    /** Opens the next entry in the zip archive and returns a const pointer to a
      FileEntry object for the entry.
      @return a const FileEntry * containing information about the (now) current
      entry.
     */
    FileEntry::pointer_t    getNextEntry();

protected:
    virtual int             underflow();

private:
    bool                    m_open_entry = false;
    ZipLocalEntry           m_curr_entry;
    // TODO: require initialization
    int                     m_data_start; // Do not forget entry header has a length too.
    int                     m_remain;     // For STORED entry only. the number of bytes that
                                          // has not been put in the m_outvec yet.
};


} // namespace
// vim: ts=4 sw=4 et
