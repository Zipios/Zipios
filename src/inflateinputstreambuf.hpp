#pragma once
#ifndef INFLATEINPUTSTREAMBUF_HPP
#define INFLATEINPUTSTREAMBUF_HPP

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
 * \brief Define zipios::InflateInputStreambuf to decompress files.
 *
 * The zipios::InflateInputStreambuf class defines a set of functions
 * that filter the data in the std::streambuf defined in the base
 * class and which may be compressed using the zlib library.
 */

#include "filterinputstreambuf.hpp"

#include "zipios++/zipios-config.hpp"

#include <vector>

#include <zlib.h>


namespace zipios
{


class InflateInputStreambuf : public FilterInputStreambuf
{
public:
                            InflateInputStreambuf(std::streambuf *inbuf, offset_t s_pos = -1);
                            InflateInputStreambuf(InflateInputStreambuf const& src) = delete;
    InflateInputStreambuf&  operator = (InflateInputStreambuf const& src) = delete;
    virtual                 ~InflateInputStreambuf();

    bool                    reset(offset_t stream_position = -1);

protected:
    virtual std::streambuf::int_type             underflow() override;

    /** \FIXME Consider design?
     */
    std::vector<char>       m_outvec;

private:
    std::vector<char>       m_invec;

    z_stream                m_zs;
    bool                    m_zs_initialized = false;
};


} // namespace

// vim: ts=4 sw=4 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

#endif
