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
 * \brief Define the zipios::ZipOutputStreambuf class.
 *
 * This class is used to save files in a Zip archive.
 */

#include "deflateoutputstreambuf.hpp"
#include "zipcdirentry.hpp"


namespace zipios
{


class ZipOutputStreambuf : public DeflateOutputStreambuf
{
public:
    explicit                ZipOutputStreambuf(std::streambuf *outbuf);
    virtual                 ~ZipOutputStreambuf();

    void                    closeEntry();
    void                    close();
    void                    finish();
    void                    putNextEntry(FileEntry::pointer_t entry);
    void                    setComment(std::string const& comment);
    void                    setLevel(CompressionLevel level);
    void                    setMethod(StorageMethod method);

protected:
    virtual int             overflow(int c = EOF);
    virtual int             sync();
    void                    setEntryClosedState();
    void                    updateEntryHeaderInfo();

private:
    std::string             m_zip_comment;
    FileEntry::vector_t     m_entries;
    bool                    m_open_entry = false;
    bool                    m_open = true;
    StorageMethod           m_method = StorageMethod::DEFLATED;
    int                     m_level = 6;
};


} // zipios namespace
// vim: ts=4 sw=4 et
