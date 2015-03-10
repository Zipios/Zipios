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
 * \brief Implementation of zipios::InflateInputStreambuf.
 *
 * This file defines the various functions found in the
 * zipios::InflateInputStreambuf class. It filters a stream buffer
 * to decompress data that was compressed using the zlib library.
 */

#include "inflateinputstreambuf.hpp"

#include "zipios++/zipiosexceptions.hpp"

#include "zipios_common.hpp"


namespace zipios
{

/** \class InflateInputStreambuf
 *
 * The InflateInputStreambuf class is an input stream filter, that
 * inflates the input from the attached input stream.
 *
 * Deflation/Inflation is a compression/decompression method used
 * in gzip and zip. The zlib library is used to perform the actual
 * inflation, this class only wraps the functionality in an input
 * stream filter.
 *
 * \todo
 * Add support for bzip2 compression.
 */



/** \brief Initialize a InflateInputStreambuf.
 *
 * The constructor initializes the various stream buffers and
 * setup the stream start position using the \p start_pos
 * parameter.
 *
 * Data will be inflated (decompressed using zlib) before being
 * returned.
 *
 * \param[in,out] inbuf  The streambuf to use for input.
 * \param[in] start_pos  A position to reset the inbuf to before reading. Specify
 *                       -1 to not change the position.
 */
InflateInputStreambuf::InflateInputStreambuf(std::streambuf *inbuf, offset_t start_pos)
    : FilterInputStreambuf(inbuf)
    //, m_outvecsize(1000) -- auto-init
    , m_outvec(m_outvecsize)
    //, m_zs_initialized(false) -- auto-init
    //, m_invecsize(1000) -- auto-init
    , m_invec(m_invecsize)
{
    // NOTICE: It is important that this constructor and the methods it
    // calls doesn't do anything with the input streambuf inbuf, other
    // than repositioning it to the specified position. The reason is
    // that this class can be subclassed, and the subclass should get a
    // chance to read from the buffer first)

    // zlib init:
    m_zs.zalloc = Z_NULL;
    m_zs.zfree  = Z_NULL;
    m_zs.opaque = Z_NULL;

    reset(start_pos);
    // We are not checking the return value of reset() and throwing
    // an exception in case of an error, because we cannot catch the exception
    // in the constructors of subclasses with all compilers.
}


/** \brief Clean up the InflateInputStreambuf object.
 *
 * The destructor makes sure all allocated resources get cleaned up.
 */
InflateInputStreambuf::~InflateInputStreambuf()
{
    // Dealloc z_stream stuff
    int const err(inflateEnd(&m_zs));
    if(err != Z_OK)
    {
        std::cerr << "InflateInputStreambuf::~InflateInputStreambuf(): inflateEnd() failed"
                  << ": " << zError(err)
                  << std::endl;
    }
}


/** \brief Called when more data is required.
 *
 * The function ensures that at least one byte is available
 * in the input area by updating the pointers to the input area
 * and reading more data in from the input sequence if required.
 *
 * This function actually passes the data through the zlib library
 * to decompress it.
 *
 * \return The value of that character on success or
 *         std::streambuf::traits_type::eof() on failure.
 */
std::streambuf::int_type InflateInputStreambuf::underflow()
{
    // If not underflow don't fill buffer
    if(gptr() < egptr())
    {
        return traits_type::to_int_type(*gptr());
    }

    // Prepare _outvec and get array pointers
    m_zs.avail_out = m_outvecsize;
    m_zs.next_out = reinterpret_cast<unsigned char *>(&m_outvec[0]);

    // Inflate until _outvec is full
    // eof (or I/O prob) on _inbuf will break out of loop too.
    int err(Z_OK);
    while(m_zs.avail_out > 0 && err == Z_OK)
    {
        if(m_zs.avail_in == 0)
        {
            // fill m_invec
            std::streamsize const bc(m_inbuf->sgetn(&m_invec[0], m_invecsize));
            /** \FIXME
             * Add I/O error handling while inflating data from a file.
             */
            m_zs.next_in = reinterpret_cast<unsigned char *>(&m_invec[0]);
            m_zs.avail_in = bc;
            // If we could not read any new data (bc == 0) and inflate is not
            // done it will return Z_BUF_ERROR and thus breaks out of the
            // loop. This means we do not have to respond to the situation
            // where we cannot read more bytes here.
        }

        err = inflate(&m_zs, Z_NO_FLUSH);
    }
    // Normally the number of inflated bytes will be the
    // full length of the output buffer, but if we can't read
    // more input from the _inbuf streambuf, we end up with
    // less.
    offset_t const inflated_bytes = m_outvecsize - m_zs.avail_out;
    setg(&m_outvec[0], &m_outvec[0], &m_outvec[0] + inflated_bytes);

    /** \FIXME
     * Look at the error returned from inflate here, if there is
     * some way to report it to the InflateInputStreambuf user.
     * Until I find out I'll just print a warning to stdout.
     * This at least throws, we probably want to create a log
     * mechanism that the end user can connect to with a callback.
     */
    if(err != Z_OK && err != Z_STREAM_END)
    {
        OutputStringStream msgs;
        msgs << "InflateInputStreambuf: inflate failed";
#if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM) && defined (HAVE_ZERROR)
        msgs << ": " << zError(err);
#endif
        // Throw an exception to make istream set badbit
        throw IOException(msgs.str());
    }

    if(inflated_bytes > 0)
    {
        return traits_type::to_int_type(*gptr());
    }

    return traits_type::eof();
}



/** \brief
 *
 * Resets the zlib stream and purges input and output buffers.
 * repositions the input streambuf at stream_position.
 * @param stream_position a position to reset the inbuf to before reading. Specify
 * -1 to read from the current position.
 *
 * This method is called in the constructor, so it must not
 * read anything from the input streambuf _inbuf (see notice in constructor)
 */
bool InflateInputStreambuf::reset(offset_t stream_position)
{
    if(stream_position >= 0)
    {
        // reposition m_inbuf
        m_inbuf->pubseekpos(stream_position);
    }

    // m_zs.next_in and avail_in must be set according to
    // zlib.h (inline doc).
    m_zs.next_in = reinterpret_cast<unsigned char *>(&m_invec[0]);
    m_zs.avail_in = 0;

    int err(0);
    if(m_zs_initialized)
    {
        // just reset it
        err = inflateReset(&m_zs);
    }
    else
    {
        // initialize it
        err = inflateInit2(&m_zs, -MAX_WBITS);
        /* windowBits is passed < 0 to tell that there is no zlib header.
           Note that in this case inflate *requires* an extra "dummy" byte
           after the compressed stream in order to complete decompression
           and return Z_STREAM_END.  We always have an extra "dummy" byte,
           because there is always some trailing data after the compressed
           data (either the next entry or the central directory.  */
        m_zs_initialized = true;
    }

    // streambuf init:
    // The important thing here, is that
    // - the pointers are not NULL (which would mean unbuffered)
    // - and that gptr() is not less than  egptr() (so we trigger underflow
    //   the first time data is read).
    setg(&m_outvec[0], &m_outvec[0] + m_outvecsize, &m_outvec[0] + m_outvecsize);

    return err == Z_OK;
}


} // zipios namespace
// vim: ts=4 sw=4 et
