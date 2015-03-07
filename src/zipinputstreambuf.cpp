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
 * \brief Implementation of ZipInputStreambuf.
 */

#include "zipios++/zipinputstreambuf.hpp"

#include "zipios++/zipiosexceptions.hpp"


namespace zipios
{


ZipInputStreambuf::ZipInputStreambuf(std::streambuf *inbuf, int s_pos)
    : InflateInputStreambuf(inbuf, s_pos)
    //, m_open_entry(false) -- auto-init
{
    // TODO: this was added in our version, but it looked like it would
    //       skip an entry... so it probably is not required nor even wanted.
    //
    //ConstEntryPointer entry = getNextEntry() ;
    //
    //if ( ! entry->isValid() ) {
    //  ; // FIXME: throw something?
    //}
}

void ZipInputStreambuf::closeEntry()
{
    if(!m_open_entry)
    {
        return;
    }

    // check if we're positioned correctly, otherwise position us correctly
    int const position(m_inbuf->pubseekoff(0, std::ios::cur, std::ios::in));
    if(position != m_data_start + static_cast<int>(m_curr_entry.getCompressedSize()))
    {
        m_inbuf->pubseekoff(m_data_start + m_curr_entry.getCompressedSize(), std::ios::beg, std::ios::in);
    }
}


void ZipInputStreambuf::close()
{
}


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
        is >> m_curr_entry;
        if(m_curr_entry.isValid())
        {
            m_data_start = m_inbuf->pubseekoff(0, std::ios::cur, std::ios::in);
            if(m_curr_entry.getMethod() == StorageMethod::DEFLATED)
            {
                m_open_entry = true;
                reset() ; // reset inflatestream data structures
                //        cerr << "deflated" << endl ;
            }
            else if(m_curr_entry.getMethod() == StorageMethod::STORED)
            {
                m_open_entry = true;
                m_remain = m_curr_entry.getSize();
                // Force underflow on first read:
                setg(&m_outvec[0], &m_outvec[0] + m_outvecsize, &m_outvec[0] + m_outvecsize);
                // std::cerr << "stored" << std::endl;
            }
            else
            {
                m_open_entry = false; // Unsupported compression format.
                throw FCollException("Unsupported compression format");
            }
        }
    }
    catch(...)
    {
        // TODO: this is not valid, if not open we cannot access _curr_entry below
        m_open_entry = false ;
    }

    if(m_curr_entry.isValid() && m_curr_entry.trailingDataDescriptor())
    {
        throw FCollException("Trailing data descriptor in zip file not supported");
    }

    return FileEntry::pointer_t(new ZipLocalEntry(m_curr_entry));
}


ZipInputStreambuf::~ZipInputStreambuf()
{
}


int ZipInputStreambuf::underflow()
{
    if(!m_open_entry)
    {
        return EOF ; // traits_type::eof()
    }

    if(m_curr_entry.getMethod() == StorageMethod::DEFLATED)
    {
        return InflateInputStreambuf::underflow() ;
    }

    // Ok, we are stored, so we handle it ourselves.
    int num_b(std::min(m_remain, m_outvecsize));
    int const g(m_inbuf->sgetn(&m_outvec[0], num_b));
    setg(&m_outvec[0], &m_outvec[0], &m_outvec[0] + g);
    m_remain -= g;
    if(g > 0)
    {
        return static_cast<unsigned char>(*gptr());
    }

    return EOF; // traits_type::eof()
}


// FIXME: We need to check somew
//
//    // gp_bitfield bit 3 is one, if the length of the zip entry
//    // is stored in a trailer.
//    if ( is->good  && ( _curr_entry.gp_bitfield & 4 ) != 1 ) <<--- this test is wrong! (i.e. it should be != 0 or != 4)
//      return true ;
//    else {
//      is->clear() ;
//      return false ;
//    }


} // namespace
// vim: ts=4 sw=4 et
