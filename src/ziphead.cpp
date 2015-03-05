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
 * Implementation of routines for reading the central directory and
 * local headers of a zip archive.
 */

#include "zipios++/ziphead.h"

#include "zipios++/dostime.h"
#include "zipios++/zipheadio.h"
#include "zipios++/zipiosexceptions.h"

#include "zipios_common.h"


namespace zipios
{

uint32_t const ZipCDirEntry::g_signature          = 0x02014b50;
uint32_t const ZipLocalEntry::g_signature         = 0x04034b50;
uint32_t const EndOfCentralDirectory::g_signature = 0x06054b50;


bool operator == (ZipLocalEntry const& zlh, ZipCDirEntry const& ze)
{
    // Not all fields need to be identical. Some of the information
    // may be put in a data descriptor that trails the compressed
    // data, according to the specs (The trailing data descriptor
    // can contain crc_32, compress_size and uncompress_size.)

    // Experience has shown that extra_field and extra_field_len
    // can differ too.

//    std::cerr << "----- BEGIN -----" << std::endl ;
//    std::cerr << ( zlh.extract_version == ze.extract_version     ) << std::endl ;
//    std::cerr << ( zlh.gp_bitfield     == ze.gp_bitfield         ) << std::endl ;
//    std::cerr << ( zlh.compress_method == ze.compress_method     ) << std::endl ;
//    std::cerr << ( zlh.last_mod_ftime  == ze.last_mod_ftime      ) << std::endl ;
//    std::cerr << ( zlh.last_mod_fdate  == ze.last_mod_fdate      ) << std::endl ;

//    std::cerr << ( zlh.filename_len    == ze.filename_len        ) << std::endl ;

//    std::cerr << ( zlh.filename        == ze.filename            ) << std::endl ;
//    std::cerr << "----- END -----" << std::endl ;

    return zlh.m_extract_version == ze.m_extract_version     &&
           zlh.m_gp_bitfield     == ze.m_gp_bitfield         &&
           zlh.m_compress_method == ze.m_compress_method     &&
           zlh.m_last_mod_ftime  == ze.m_last_mod_ftime      &&
           zlh.m_last_mod_fdate  == ze.m_last_mod_fdate      &&
           zlh.m_filename        == ze.m_filename;
}

//
// ZipLocalEntry methods
//




void ZipLocalEntry::setDefaultExtract()
{
    // TODO: use a static const for that 20
    m_extract_version = 20; // version number
}


std::string ZipLocalEntry::getComment() const
{
    return ""; // No comment in a local entry
}


size_t ZipLocalEntry::getCompressedSize() const
{
    return m_compress_size;
}


ZipLocalEntry::buffer_t ZipLocalEntry::getExtra() const
{
    return m_extra_field;
}


StorageMethod ZipLocalEntry::getMethod() const
{
  return static_cast<StorageMethod>(m_compress_method);
}






ZipLocalEntry::dostime_t ZipLocalEntry::getTime() const
{
  return (m_last_mod_fdate << 16) + m_last_mod_ftime;
}


void ZipLocalEntry::setCompressedSize(uint32_t size)
{
    m_compress_size = size;
}


void ZipLocalEntry::setCrc(uint32_t crc)
{
    m_crc_32 = crc;
    m_has_crc_32 = true;
}


void ZipLocalEntry::setExtra(std::vector<unsigned char> const& extra)
{
    m_extra_field = extra;
    m_extra_field_len = m_extra_field.size();
}


void ZipLocalEntry::setMethod(StorageMethod method)
{
    m_compress_method = static_cast<uint16_t>(method);
}


void ZipLocalEntry::setSize(uint32_t size)
{
    m_uncompress_size = size;
}


void ZipLocalEntry::setTime(dostime_t time)
{
  // FIXME: fix time setting here, and ind flist and elsewhere. Define the
  // date time semantics before mucking about - how surprising

  // Mark Donszelmann: added these lines to make zip work for winzip
  m_last_mod_fdate = (time >> 16) & 0x0000FFFF;
  m_last_mod_ftime = time & 0x0000FFFF;
}


void ZipLocalEntry::setUnixTime(std::time_t time)
{
    setTime(unix2dostime(&time));
}


std::string ZipLocalEntry::toString() const
{
    OutputStringStream sout;
    sout << m_filename << " (" << m_uncompress_size << " bytes, ";
    sout << m_compress_size << " bytes compressed)";
    return sout.str();
}


int ZipLocalEntry::getLocalHeaderSize() const
{
    return 30 + m_filename.size() + m_extra_field.size();
}


bool ZipLocalEntry::trailingDataDescriptor() const
{
    // gp_bitfield bit 3 is one, if this entry uses a trailing data
    // descriptor to keep size, compressed size and crc-32
    // fields.
    return (m_gp_bitfield & 4) != 0;
}

FileEntry::pointer_t ZipLocalEntry::clone() const
{
    return FileEntry::pointer_t(new ZipLocalEntry(*this));
}


//
// ZipCDirEntry methods
//



void ZipCDirEntry::setDefaultWriter()
{
    m_writer_version = 0 ;
#ifdef WIN32
    m_writer_version |= static_cast<uint16_t>(0) << 8 ; // Windows, DOS
#else
    m_writer_version |= static_cast<uint16_t>(3) << 8 ; // Unix
#endif
    m_writer_version |= 20 ; // version number
}

std::string ZipCDirEntry::getComment() const
{
    return m_file_comment;
}

uint32_t ZipCDirEntry::getLocalHeaderOffset() const
{
    return m_rel_offset_loc_head;
}

void ZipCDirEntry::setLocalHeaderOffset(uint32_t offset)
{
    m_rel_offset_loc_head = offset;
}


void ZipCDirEntry::setComment(std::string const& comment)
{
    m_file_comment = comment;
    m_file_comment_len = m_file_comment.size();
}


std::string ZipCDirEntry::toString() const
{
    OutputStringStream sout;
    sout << m_filename << " (" << m_uncompress_size << " bytes, ";
    sout << m_compress_size << " bytes compressed)";
    return sout.str();
}


int ZipCDirEntry::getCDirHeaderSize() const
{
    return 46 + m_filename.size() + m_extra_field.size() + m_file_comment.size() ;
}


FileEntry::pointer_t ZipCDirEntry::clone() const
{
    return FileEntry::pointer_t(new ZipCDirEntry(*this));
}


//
// EndOfCentralDirectory methods
//



bool EndOfCentralDirectory::read(std::vector<unsigned char> const& buf, int pos)
{
    if((buf.size() - pos < sizeof(uint32_t)) || !checkSignature(&buf[pos]))
    {
        return false;
    }

    m_eocd_offset_from_end = buf.size() - pos;
    pos += sizeof(uint32_t);
    m_disk_num         = ztohs(&buf[pos]); pos += sizeof(uint16_t);
    m_cdir_disk_num    = ztohs(&buf[pos]); pos += sizeof(uint16_t);
    m_cdir_entries     = ztohs(&buf[pos]); pos += sizeof(uint16_t);
    m_cdir_tot_entries = ztohs(&buf[pos]); pos += sizeof(uint16_t);
    m_cdir_size        = ztohl(&buf[pos]); pos += sizeof(uint32_t);
    m_cdir_offset      = ztohl(&buf[pos]); pos += sizeof(uint32_t);
    m_zip_comment_len  = ztohs(&buf[pos]); pos += sizeof(uint16_t);
    //    cerr << "Zip comment length = " << zip_comment_len << endl ;
    //    cerr << "Length of remaining file = " << buf.size() - pos << endl ;

    return true;
}


bool EndOfCentralDirectory::checkSignature(unsigned char const *buf) const
{
    //cerr << "potential header: " << ztohl(buf) << endl;
    return checkSignature(ztohl(buf));
}


bool EndOfCentralDirectory::checkSignature(uint32_t sig) const
{
    return g_signature == sig;
}


} // zipios namespace

// vim: ts=4 sw=4 et
