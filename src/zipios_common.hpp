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
 * Header file containing miscellaneous small functions and variables.
 */

#include "zipios++/zipios-config.hpp"

#include <vector>


namespace zipios
{


extern char const g_separator;


/** \brief Contatenate two vectors together.
 *
 * This function appends vector v2 to vector v1 using a push_back()
 * of all the elements of v2.
 *
 * \param[in,out] v1  The vector which receives a copy of v2.
 * \param[in]  v2  The vector to concatenate at the end of v1.
 */
template<class Type>
void operator += (std::vector<Type>& v1, std::vector<Type> const& v2)
{
    // make sure these are not the same vector or the insert()
    // is not unlikely to fail badly
    if(&v1 != &v2)
    {
        v1.reserve(v1.size() + v2.size());
        v1.insert(v1.end(), v2.begin(), v2.end());
    }
    //for ( std::vector< Type >::const_iterator cit = v2.begin() ; cit != v2.end() ; ++cit )
    //{
    //  v1.push_back( *cit ) ;
    //}
}


} // zipios namespace
// vim: ts=4 sw=4 et
