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
 * \brief Implementation of DeflateOutputStreambuf.
 */

#include "deflateoutputstreambuf.hpp"

#include "zipios++/zipiosexceptions.hpp"


namespace zipios
{


DeflateOutputStreambuf::DeflateOutputStreambuf(std::streambuf *outbuf, bool user_init)
    : FilterOutputStreambuf(outbuf)
    //, m_zs_initialized(false) -- auto-init
    //, m_invecsize(1000) -- auto-init
    , m_invec(m_invecsize)
    //, m_outvecsize(1000) -- auto-init
    , m_outvec(m_outvecsize)
    //, m_crc32(0) -- auto-init
    //, m_overflown_bytes(0) -- auto-init
{
    // NOTICE: It is important that this constructor and the methods it
    // calls does not do anything with the output streambuf _outbuf The
    // reason is that this class can be subclassed, and the subclass
    // should get a chance to write to the buffer first

    // zlib init:
    m_zs.zalloc = Z_NULL;
    m_zs.zfree  = Z_NULL;
    m_zs.opaque = Z_NULL;

    if(user_init && !init())
    {
        throw InvalidStateException("DeflateOutputStreambuf::init() failed initializing zlib.");
    }
}


DeflateOutputStreambuf::~DeflateOutputStreambuf()
{
    closeStream();
}


// This method is called in the constructor, so it must not write
// anything to the output streambuf _outbuf (see notice in
// constructor)
bool DeflateOutputStreambuf::init(int comp_level)
{
    static const int default_mem_level = 8;

    // _zs.next_in and avail_in must be set according to
    // zlib.h (inline doc).
    m_zs.next_in  = reinterpret_cast<unsigned char *>(&m_invec[0]);
    m_zs.avail_in = 0;

    m_zs.next_out  = reinterpret_cast<unsigned char *>(&m_outvec[0]);
    m_zs.avail_out = m_outvecsize;

    int err;
    if(m_zs_initialized)
    {
        // just reset it
        endDeflation();
        err = deflateReset(&m_zs);
        // FIXME: bug, for deflateReset we do not update the compression level
    }
    else
    {
        // init it
        err = deflateInit2(&m_zs, comp_level, Z_DEFLATED, -MAX_WBITS,
                default_mem_level, Z_DEFAULT_STRATEGY);
        /* windowBits is passed < 0 to tell that no zlib header should be
           written. */
        m_zs_initialized = true;
    }

    // streambuf init:
    setp(&m_invec[0], &m_invec[0] + m_invecsize);

    m_crc32 = crc32(0, Z_NULL, 0);
    m_overflown_bytes = 0;

    return err == Z_OK;
}


bool DeflateOutputStreambuf::closeStream()
{
    int err = Z_OK;
    if(m_zs_initialized)
    {
        endDeflation();
        err = deflateEnd(&m_zs);
        m_zs_initialized = false;
    }

    if(err == Z_OK)
    {
        return true;
    }

    std::cerr << "DeflateOutputStreambuf::closeStream(): deflateEnd failed";
#ifdef HAVE_ZERROR
    std::cerr << ": " << zError(err);
#endif
    std::cerr << std::endl;

    return false;
}


int DeflateOutputStreambuf::overflow(int c)
{
    m_zs.avail_in = pptr() - pbase();
    m_zs.next_in = reinterpret_cast<unsigned char *>(&m_invec[0]);

    m_crc32 = crc32(m_crc32, m_zs.next_in, m_zs.avail_in); // update crc32
    m_overflown_bytes += m_zs.avail_in;

    m_zs.next_out  = reinterpret_cast<unsigned char *>(&m_outvec[0]);
    m_zs.avail_out = m_outvecsize;

    // Deflate until _invec is empty.
    int err = Z_OK;
    while((m_zs.avail_in > 0 || m_zs.avail_out == 0) && err == Z_OK)
    {
        if(m_zs.avail_out == 0)
        {
            flushOutvec();
        }

        err = deflate(&m_zs, Z_NO_FLUSH);
    }

    flushOutvec();

    // Update 'put' pointers
    setp(&m_invec[0], &m_invec[0] + m_invecsize);

    if(err != Z_OK && err != Z_STREAM_END)
    {
        // Throw an exception to make istream set badbit
        OutputStringStream msgs ;
        msgs << "Deflation failed";
#ifdef HAVE_ZERROR
        msgs << ": " << zError(err);
#endif
        throw IOException(msgs.str());
    }

    if(c != EOF)
    {
        *pptr() = c;
        pbump(1);
    }

    return 0 ;
}

int DeflateOutputStreambuf::sync()
{
    // FIXME: Do something
    //    return overflow() ;
    return 0;
}


bool DeflateOutputStreambuf::flushOutvec()
{
    int const deflated_bytes = m_outvecsize - m_zs.avail_out;
    int const bc = m_outbuf->sputn(&m_outvec[0], deflated_bytes);

    m_zs.next_out = reinterpret_cast<unsigned char *>(&m_outvec[0]);
    m_zs.avail_out = m_outvecsize;

    return deflated_bytes == bc;
}


void DeflateOutputStreambuf::endDeflation()
{
    overflow();

    m_zs.next_out  = reinterpret_cast<unsigned char *>(&m_outvec[0]);
    m_zs.avail_out = m_outvecsize;

    // Deflate until _invec is empty.
    int err = Z_OK;

    while(err == Z_OK)
    {
        if(m_zs.avail_out == 0)
        {
            flushOutvec();
        }

        err = deflate(&m_zs, Z_FINISH);
    }

    flushOutvec();

    if(err != Z_STREAM_END)
    {
        std::cerr << "DeflateOutputStreambuf::endDeflation(): deflation failed:\n";
#ifdef HAVE_ZERROR
        std::cerr << ": " << zError(err);
#endif
        std::cerr << std::endl;
    }
}


} // namespace
// vim: ts=4 sw=4 et
