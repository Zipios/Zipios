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
 * Header file that defines DeflateOutputStreambuf.
 */

#include "filteroutputstreambuf.hpp"

#include <cstdint>
#include <vector>

#include <zlib.h>


namespace zipios
{


/** DeflateOutputStreambuf is an output stream filter, that deflates
    the data that is written to it before it passes it on to the
    output stream it is attached to. Deflation/Inflation is a
    compression/decompression method used in gzip and zip. The zlib
    library is used to perform the actual deflation, this class only
    wraps the functionality in an output stream filter. */
class DeflateOutputStreambuf : public FilterOutputStreambuf
{
public:

    /** DeflateOutputStreambuf constructor.
     * @param outbuf the streambuf to use for output.
     * @param user_init If false user must invoke init() before writing any data.
     * (ZipOutputStreambuf needs to do this)
     * @param del_outbuf if true is specified outbuf will be deleted, when
     * the DeflateOutputStreambuf is destructed.
     */
    explicit DeflateOutputStreambuf(std::streambuf *outbuf,
                                    bool user_init = false);

    /** Destructor. */
    virtual ~DeflateOutputStreambuf();

    bool init(int comp_level = 6);

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

protected: // FIXME: reconsider design?
    int const               m_invecsize = 1000;
    std::vector<char>       m_invec;
    int const               m_outvecsize = 1000;
    std::vector<char>       m_outvec;

    uint32_t                m_crc32 = 0;
    uint32_t                m_overflown_bytes = 0;
};


} // zipios namespace
// vim: ts=4 sw=4 et
