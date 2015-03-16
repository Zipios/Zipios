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
 * \brief Implementation of zipios::ZipInputStreambuf.
 *
 * This is the implementation of the Zip input std::streambuf class.
 */

#include "zipinputstreambuf.hpp"

#include "zipios++/zipiosexceptions.hpp"


namespace zipios
{


/** \brief An input stream buffer for Zip data.
 *
 * The ZipInputStreambuf class is a Zip input streambuf filter that
 * automatically decompresses input data that was compressed using
 * the zlib library.
 */


/** \brief Initialize a ZipInputStreambuf.
 *
 * This ZipInputStreambuf constructor initializes the buffer from the
 * user specified buffer.
 *
 * \param[in,out] inbuf  The streambuf to use for input.
 * \param[in] start_pos  A position to reset the inbuf to before reading.
 *                       Specify -1 to read from the current position.
 */
ZipInputStreambuf::ZipInputStreambuf(std::streambuf *inbuf, offset_t start_pos)
    : InflateInputStreambuf(inbuf, start_pos)
    //, m_open_entry(false) -- auto-init
    //, m_current_entry() -- auto-init
    //, m_data_start(0) -- auto-init
    //, m_remain(0) -- auto-init
{
    /** TODO:
     * Further the initialization of the buffer by calling the getNextEntry().
     * This is just plainly ugly and we want to change it ASAP.
     */
    getNextEntry();
}


/** \fn ZipInputStreambuf(ZipInputStreambuf const& src);
 * \brief The copy contructor is deleted.
 *
 * ZipInputStreambuf objects cannot be copied so the copy constructor
 * is deleted.
 *
 * \param[in] src  The source to copy.
 */


/** \fn ZipInputStreambuf& operator = (ZipInputStreambuf const& src):
 * \brief The copy assignment is deleted.
 *
 * ZipInputStreambuf objects cannot be copied so the copy assignment
 * is deleted.
 *
 * \param[in] src  The source to copy.
 */


/** \brief Clean up a ZipInputStreambuf object.
 *
 * The destructor ensures that all resources get released.
 */
ZipInputStreambuf::~ZipInputStreambuf()
{
}


/** \brief Close the current entry.
 *
 * This function closes the current entry, and positions the stream
 * read pointer at the beginning of the next entry (if there is one).
 */
void ZipInputStreambuf::closeEntry()
{
    if(!m_open_entry)
    {
        return;
    }
    m_open_entry = false;

    // check if we are positioned correctly, otherwise position us correctly
    std::streampos const position(m_inbuf->pubseekoff(0, std::ios::cur, std::ios::in));
    std::streampos const expected_position(m_data_start + m_current_entry.getCompressedSize());
    if(position != expected_position)
    {
        m_inbuf->pubseekoff(expected_position, std::ios::beg, std::ios::in);
    }
}


/** \brief Closes the stream buffer.
 *
 * This funtion closes the stream buffer.
 *
 * \note
 * At this time this function does nothing.
 */
void ZipInputStreambuf::close()
{
}


/** \brief Get the next entry.
 *
 * This function opens the next entry found in the Zip archive
 * and returns a shared pointer to a FileEntry object representing
 * the entry.
 *
 * \return A shared pointer to a FileEntry containing information about
 *         the now current entry.
 */
FileEntry::pointer_t ZipInputStreambuf::getNextEntry()
{
    if(m_open_entry)
    {
        closeEntry();
    }

    // read the zip local header
    std::istream is(m_inbuf); // istream does not destroy the streambuf.
    is.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);

    try
    {
        m_current_entry.read(is);
        if(m_current_entry.isValid())
        {
            m_data_start = m_inbuf->pubseekoff(0, std::ios::cur, std::ios::in);
            if(m_current_entry.getMethod() == StorageMethod::DEFLATED)
            {
                m_open_entry = true;
                reset() ; // reset inflatestream data structures
//std::cerr << "deflated" << std::endl;
            }
            else if(m_current_entry.getMethod() == StorageMethod::STORED)
            {
                m_open_entry = true;
                m_remain = m_current_entry.getSize();
                // Force underflow on first read:
                setg(&m_outvec[0], &m_outvec[0] + getBufferSize(), &m_outvec[0] + getBufferSize());
//std::cerr << "stored" << std::endl;
            }
            else
            {
                m_open_entry = false; // Unsupported compression format.
                throw FileCollectionException("Unsupported compression format");
            }
        }
    }
    catch(...)
    {
        /** \TODO
         * This is not valid, if not open we cannot access _curr_entry below
         */
        m_open_entry = false ;
    }

    if(m_current_entry.isValid() && m_current_entry.trailingDataDescriptor())
    {
        throw FileCollectionException("Trailing data descriptor in zip file not supported");
    }

    return m_current_entry.clone();
}


/** \brief Called when more data is required.
 *
 * The function ensures that at least one byte is available
 * in the input area by updating the pointers to the input area
 * and reading more data in from the input sequence if required.
 *
 * \return The value of that character on success or
 *         std::streambuf::traits_type::eof() on failure.
 */
std::streambuf::int_type ZipInputStreambuf::underflow()
{
    if(!m_open_entry)
    {
        // documentation says to return EOF if no data available
        return traits_type::eof();
    }

    if(m_current_entry.getMethod() == StorageMethod::DEFLATED)
    {
        return InflateInputStreambuf::underflow();
    }

    // Ok, we are stored, so we handle it ourselves.
    offset_t const num_b(std::min(m_remain, static_cast<offset_t>(getBufferSize())));
    std::streamsize const g(m_inbuf->sgetn(&m_outvec[0], num_b));
    setg(&m_outvec[0], &m_outvec[0], &m_outvec[0] + g);
    m_remain -= g;
    if(g > 0)
    {
        // we got some data, return it
        return traits_type::to_int_type(*gptr());
    }

    // documentation says to return EOF if no data available
    return traits_type::eof();
}


} // namespace
// vim: ts=4 sw=4 et
