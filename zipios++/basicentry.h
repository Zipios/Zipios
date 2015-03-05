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
    Header file that defines BasicEntry.
*/

#include "zipios++/fileentry.h"
#include "zipios++/filepath.h"


namespace zipios
{

class BasicEntry : public FileEntry
{
public:
    explicit                BasicEntry(std::string const& filename,
                                       std::string const& comment,
                                       FilePath const& basepath = FilePath());
    virtual pointer_t       clone() const;
    virtual                 ~BasicEntry();

    virtual std::string     getComment() const;
    virtual StorageMethod   getMethod() const;
    virtual dostime_t       getTime() const;

    virtual void            setComment(std::string const& comment);
    virtual void            setCompressedSize(uint32_t size);
    virtual void            setExtra(std::vector<unsigned char> const& extra);
    virtual void            setMethod(StorageMethod method);
    virtual void            setName(std::string const& name);
    virtual void            setSize(uint32_t size);
    virtual void            setTime(dostime_t time);
    virtual void            setUnixTime(std::time_t time);

    virtual std::string     toString() const;


protected:
    std::string             m_comment;
    FilePath                m_basepath;
};

} // zipios namespace

// vim: ts=4 sw=4 et
