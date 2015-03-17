#pragma once
#ifndef ZIPIOS_ZIPFILE_HPP
#define  ZIPIOS_ZIPFILE_HPP

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
 * \brief Define the zipios::ZipFile class.
 *
 * This file defines the main class of the Zipios++ library. The
 * zipios::ZipFile class is the one used to read or write
 * Zip archives.
 *
 * Note that a Zip archive is a zipios::FileCollection. When reading,
 * you get zipios::FileEntry objects that are defined internally
 * such as the zipios::ZipCDirEntry. When writing, you get
 * zipios::FileEntry objects from a zipios::DirectoryCollection.
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
    virtual pointer_t           clone() const override;
    virtual                     ~ZipFile() override;

    virtual stream_pointer_t    getInputStream(std::string const& entry_name, MatchPath matchpath = MatchPath::MATCH) override;
    static void                 saveCollectionToArchive(std::ostream& os, FileCollection const& collection);

private:
    VirtualSeeker           	m_vs;
};


} // zipios namespace

// vim: ts=4 sw=4 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

#endif
