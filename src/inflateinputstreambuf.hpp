#pragma
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
 * \brief Header file that defines InflateInputStreambuf.
 */

#include "filterinputstreambuf.hpp"

#include <vector>

#include <zlib.h>


namespace zipios
{


class InflateInputStreambuf : public FilterInputStreambuf
{
public:
    explicit    InflateInputStreambuf(std::streambuf *inbuf, int s_pos = -1);
                InflateInputStreambuf(InflateInputStreambuf const& src) = delete;
                InflateInputStreambuf const& operator = (InflateInputStreambuf const& src) = delete;
    virtual     ~InflateInputStreambuf();

    bool        reset(int stream_position = -1);

protected:
    virtual int underflow();

    // FIXME: reconsider design?
    int const           m_outvecsize = 1000; // FIXME: define a ZIPIOS_BUFSIZ
    std::vector<char>   m_outvec;

private:
    z_stream            m_zs;
    bool                m_zs_initialized = false;
    int const           m_invecsize = 1000; // FIXME: define a ZIPIOS_BUFSIZ
    std::vector<char>   m_invec;
};


} // namespace
// vim: ts=4 sw=4 et
