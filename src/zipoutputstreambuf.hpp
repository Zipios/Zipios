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
 * \brief Header file that defines ZipOutputStreambuf.
 */

#include "deflateoutputstreambuf.hpp"
#include "ziphead.hpp"


namespace zipios
{


class ZipOutputStreambuf : public DeflateOutputStreambuf
{
public:
    // zlib does not define a type for its compression level
    typedef int             CompressionLevel;

    static CompressionLevel const   NO_COMPRESSION      = Z_NO_COMPRESSION;
    static CompressionLevel const   BEST_SPEED          = Z_BEST_SPEED;
    static CompressionLevel const   BEST_COMPRESSION    = Z_BEST_COMPRESSION;
    static CompressionLevel const   DEFAULT_COMPRESSION = Z_DEFAULT_COMPRESSION;

    explicit                ZipOutputStreambuf(std::streambuf *outbuf);
    virtual                 ~ZipOutputStreambuf();

    void                    closeEntry();
    void                    close();
    void                    finish();
    void                    putNextEntry(ZipCDirEntry const& entry);
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
    ZipCDirEntry::vector_t  m_entries;
    bool                    m_open_entry = false;
    bool                    m_open = true;
    StorageMethod           m_method = StorageMethod::DEFLATED;
    int                     m_level = 6;
};


} // zipios namespace
// vim: ts=4 sw=4 et
