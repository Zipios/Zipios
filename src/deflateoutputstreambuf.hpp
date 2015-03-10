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

/** \brief A class to handle stream deflate on the fly.
 *
 * DeflateOutputStreambuf is an output stream filter, that deflates
 * the data that is written to it before it passes it on to the
 * output stream it is attached to. Deflation/Inflation is a
 * compression/decompression method used in gzip and zip. The zlib
 * library is used to perform the actual deflation, this class only
 * wraps the functionality in an output stream filter.
 */
class DeflateOutputStreambuf : public FilterOutputStreambuf
{
public:
    // zlib does not define a type for its compression level
    typedef int             CompressionLevel;

    static CompressionLevel const   NO_COMPRESSION      = Z_NO_COMPRESSION;
    static CompressionLevel const   BEST_SPEED          = Z_BEST_SPEED;
    static CompressionLevel const   BEST_COMPRESSION    = Z_BEST_COMPRESSION;
    static CompressionLevel const   DEFAULT_COMPRESSION = Z_DEFAULT_COMPRESSION;

    /** DeflateOutputStreambuf constructor.
     * @param outbuf the streambuf to use for output.
     * @param user_init If false user must invoke init() before writing any data.
     * (ZipOutputStreambuf needs to do this)
     */
    explicit DeflateOutputStreambuf(std::streambuf *outbuf, bool user_init = false);

    /** Destructor. */
    virtual ~DeflateOutputStreambuf();

    bool init(CompressionLevel comp_level = DEFAULT_COMPRESSION);

    bool closeStream();

    /** Returns the CRC32 for the current stream. The returned value is
      the CRC for the data that has been compressed already (due to a
      call to overflow()). As DeflateOutputStreambuf may buffer an
      arbitrary amount of bytes until closeStream() has been invoked,
      the returned value is not very useful before closeStream() has
      been called. */
    uint32_t getCrc32() const
    {
        return m_crc32 ;
    }

    /** Returns the number of bytes written to the streambuf, that has
      been processed from the input buffer by the compressor. After
      closeStream() has been called this number is the total number of
      bytes written to the stream. */
    uint32_t getCount() const
    {
        return m_overflown_bytes;
    }

protected:
    virtual int overflow(int c = EOF);
    virtual int sync();

    /** Flushes _outvec and updates _zs.next_out and _zs.avail_out. */
    bool flushOutvec();

    /** Flushes the remaining data in the zlib buffers, after which the
      only possible operations are deflateEnd() or deflateReset(). */
    void endDeflation();

private:
    z_stream                m_zs;
    bool                    m_zs_initialized = false;

protected: /** FIXME: consider design? Also the size of 1,000 ... hmmm.... */
    int const               m_invecsize = 1000;
    std::vector<char>       m_invec;
    int const               m_outvecsize = 1000;
    std::vector<char>       m_outvec;

    uint32_t                m_crc32 = 0;
    uint32_t                m_overflown_bytes = 0;
};


} // zipios namespace
// vim: ts=4 sw=4 et
