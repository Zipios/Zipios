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
 * \brief Implementation of zipios::FilterOutputStreambuf.
 *
 * This file includes the functions of the output stream buffer
 * filter.
 */

#include "filteroutputstreambuf.hpp"

#include "zipios++/zipiosexceptions.hpp"


namespace zipios
{


/** \class FilterOutputStreambuf
 * \brief A base class to develop output stream filters.
 *
 * An output stream buffer filter is an std::streambuf that filters the
 * output it gets from the std::streambuf it is attached to.
 *
 * FilterOutputStreambuf is a base class to derive input streambuf
 * filters from.
 */


/** \brief Initialize your filter output stream buffer.
 *
 * This constructor initializes the filter by saving the outbuf buffer
 * pointer in it.
 *
 * \param[in] outbuf  The streambuf to pass the filtered data on to.
 */
FilterOutputStreambuf::FilterOutputStreambuf(std::streambuf *outbuf)
    : m_outbuf(outbuf)
{
    if(!m_outbuf)
    {
        throw InvalidStateException("FilterOutputStreambuf was called with a null streambuf pointer");
    }
}


/** \brief Clean up the object.
 *
 * At this time the destructor does nothing.
 *
 * The former version would eventually delete the m_outbuf pointer. However,
 * here we have decided that this class did not own that pointer and thus
 * should not have such permissions.
 */
FilterOutputStreambuf::~FilterOutputStreambuf()
{
}


} // zipios namespace
// vim: ts=4 sw=4 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:
