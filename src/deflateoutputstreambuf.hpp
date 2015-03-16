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
 * \brief Header file that defines zipios::DeflateOutputStreambuf.
 *
 * This file is used to declare the zipios::DeflateOutputStreambuf class which
 * is used to compress data using zlib.
 *
 * The counter part is the class zipios::InflateInputStreambuf.
 */

#include "filteroutputstreambuf.hpp"

#include <cstdint>
#include <vector>

#include <zlib.h>


namespace zipios
{

class DeflateOutputStreambuf : public FilterOutputStreambuf
{
public:
    // zlib does not define a type for its compression level
    typedef int             CompressionLevel;

    static CompressionLevel const   NO_COMPRESSION      = Z_NO_COMPRESSION;
    static CompressionLevel const   BEST_SPEED          = Z_BEST_SPEED;
    static CompressionLevel const   BEST_COMPRESSION    = Z_BEST_COMPRESSION;
    static CompressionLevel const   DEFAULT_COMPRESSION = Z_DEFAULT_COMPRESSION;

                            DeflateOutputStreambuf(std::streambuf *outbuf, bool user_init = false);
    virtual                 ~DeflateOutputStreambuf();

    bool                    init(CompressionLevel comp_level = DEFAULT_COMPRESSION);
    bool                    closeStream();
    uint32_t                getCrc32() const;
    size_t                  getCount() const;

protected:
    virtual int             overflow(int c = EOF);
    virtual int             sync();
    bool                    flushOutvec();
    void                    endDeflation();

private:
    z_stream                m_zs;
    bool                    m_zs_initialized = false;

protected:
    std::vector<char>       m_invec;
    std::vector<char>       m_outvec;

    uint32_t                m_crc32 = 0;
    uint32_t                m_overflown_bytes = 0;
};


} // zipios namespace
// vim: ts=4 sw=4 et
