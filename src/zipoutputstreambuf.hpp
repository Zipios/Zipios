#pragma once
#ifndef ZIPOUTPUTSTREAMBUF_HPP
#define ZIPOUTPUTSTREAMBUF_HPP

/*
  Zipios – a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015-2017  Made to Order Software Corporation

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/** \file
 * \brief Define the zipios::ZipOutputStreambuf class.
 *
 * This class is used to save files in a Zip archive.
 */

#include "deflateoutputstreambuf.hpp"

#include "zipios/fileentry.hpp"


namespace zipios
{


class ZIPIOSDLL_API ZipOutputStreambuf : public DeflateOutputStreambuf
{
public:
                                ZipOutputStreambuf(std::streambuf *outbuf);
                                ZipOutputStreambuf(ZipOutputStreambuf const & src) = delete;
    ZipOutputStreambuf &        operator = (ZipOutputStreambuf const & rhs) = delete;
    virtual                     ~ZipOutputStreambuf();

    void                        closeEntry();
    void                        close();
    void                        finish();
    void                        putNextEntry(FileEntry::pointer_t entry);
    void                        setComment(std::string const& comment);

protected:
    virtual int                 overflow(int c = EOF) override;
    virtual int                 sync() override;

private:
    void                        setEntryClosedState();
    void                        updateEntryHeaderInfo();

    std::string                 m_zip_comment;
    FileEntry::vector_t         m_entries;
    FileEntry::CompressionLevel m_compression_level = FileEntry::COMPRESSION_LEVEL_DEFAULT;
    bool                        m_open_entry = false;
    bool                        m_open = true;
};


} // zipios namespace

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
#endif
