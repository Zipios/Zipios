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
 * \brief Header file that defines ZipFile.
 *
 * This file defines the main class to use to read or write a Zip archive.
 */

#include "zipios++/filecollection.hpp"
#include "zipios++/virtualseeker.hpp"


namespace zipios
{


class ZipFile : public FileCollection
{
public:
    static pointer_t            openEmbeddedZipFile(std::string const& name);

                                ZipFile();
                                ZipFile(std::string const& filename, offset_t s_off = 0, offset_t e_off = 0);
    virtual pointer_t           clone() const;
    virtual                     ~ZipFile();

    virtual void                close();
    virtual stream_pointer_t    getInputStream(std::string const& entry_name, MatchPath matchpath = MatchPath::MATCH);

    static void                 saveCollectionToArchive(std::string const& filename, FileCollection const& collection);

private:
    bool                        init(std::istream& zipfile);
    bool                        readCentralDirectory(std::istream& zipfile);
    bool                        confirmLocalHeaders(std::istream& zipfile);

    VirtualSeeker           	m_vs;
};


} // zipios namespace
// vim: ts=4 sw=4 et
