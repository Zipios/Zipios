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
 * \brief Implementation of zip header handling.
 *
 * Implementation of routines for reading the central directory and
 * local headers of a zip archive.
 */

#include "ziphead.hpp"

#include "zipios++/zipiosexceptions.hpp"

#include "dostime.h"
#include "endofcentraldirectory.hpp"
#include "zipheadio.hpp"
#include "zipios_common.hpp"


namespace zipios
{


//friend std::istream& operator >> (std::istream& is, ZipLocalEntry& cdh);
//friend std::ostream& operator << (std::ostream& os, ZipLocalEntry const& zlh);
//friend bool operator == (ZipLocalEntry const& zlh, const ZipCDirEntry& ze);

//friend std::istream&        operator >> ( std::istream& is, ZipCDirEntry& zcdh ) ;
//friend std::ostream&        operator << ( std::ostream& os, ZipCDirEntry const& zcdh ) ;
//friend bool                 operator == ( ZipLocalEntry const& zlh, ZipCDirEntry const& ze ) ;

//friend std::ostream& operator << (std::ostream& os, EndOfCentralDirectory const& eocd);


uint32_t const ZipCDirEntry::g_signature          = 0x02014b50;
uint32_t const ZipLocalEntry::g_signature         = 0x04034b50;
uint32_t const EndOfCentralDirectory::g_signature = 0x06054b50;


bool ZipLocalEntry::operator == (ZipCDirEntry const& ze) const
{
    // Not all fields need to be identical. Some of the information
    // may be put in a data descriptor that trails the compressed
    // data, according to the specs (The trailing data descriptor
    // can contain crc_32, compressed_size and uncompressed_size.)

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

    return m_extract_version == ze.m_extract_version     &&
           m_gp_bitfield     == ze.m_gp_bitfield         &&
           m_compress_method == ze.m_compress_method     &&
           //m_last_mod_ftime  == ze.m_last_mod_ftime      &&
           //m_last_mod_fdate  == ze.m_last_mod_fdate      &&
           m_unix_time       == ze.m_unix_time           &&
           m_filename        == ze.m_filename;
}

//
// ZipLocalEntry methods
//




void ZipLocalEntry::setDefaultExtract()
{
    // TODO: use a static const for that 20
    m_extract_version = 20; // version number
}


size_t ZipLocalEntry::getCompressedSize() const
{
    return m_compressed_size;
}


ZipLocalEntry::buffer_t ZipLocalEntry::getExtra() const
{
    return m_extra_field;
}


StorageMethod ZipLocalEntry::getMethod() const
{
  return static_cast<StorageMethod>(m_compress_method);
}






//ZipLocalEntry::dostime_t ZipLocalEntry::getTime() const
//{
//  return (m_last_mod_fdate << 16) + m_last_mod_ftime;
//}


void ZipLocalEntry::setCompressedSize(size_t size)
{
    m_compressed_size = size;
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


//void ZipLocalEntry::setTime(dostime_t time)
//{
//  // FIXME: fix time setting here, and ind flist and elsewhere. Define the
//  // date time semantics before mucking about - how surprising
//
//  // Mark Donszelmann: added these lines to make zip work for winzip
//  m_last_mod_fdate = (time >> 16) & 0x0000FFFF;
//  m_last_mod_ftime = time & 0x0000FFFF;
//}


std::string ZipLocalEntry::toString() const
{
    OutputStringStream sout;
    sout << m_filename
         << " (" << m_uncompressed_size << " bytes, "
         << m_compressed_size << " bytes compressed)";
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
    sout << m_filename << " (" << m_uncompressed_size << " bytes, ";
    sout << m_compressed_size << " bytes compressed)";
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


void ZipLocalEntry::read(std::istream& is)
{
    m_valid = false ; // set to true upon successful completion.
    if(is)
    {
        //    // Before reading anything we record the position in the stream
        //    // This is a field in the central directory entry, but not
        //    // in the local entry. After all, we know where we are, anyway.
        //    zlh.rel_offset_loc_head  = is.tellg() ;

        if(g_signature != readUint32(is))
        {
            // put stream in error state and return
            is.setstate(std::ios::failbit);
        }
        else
        {
            m_extract_version   = readUint16(is);
            m_gp_bitfield       = readUint16(is);
            m_compress_method   = readUint16(is);
            m_unix_time         = dos2unixtime(readUint32(is));
            m_crc_32            = readUint32(is);
            m_compressed_size   = readUint32(is);
            m_uncompressed_size = readUint32(is);
            m_filename_len      = readUint16(is);
            m_extra_field_len   = readUint16(is);

            // Read filename and extra_field
            std::string str;
            readByteSeq(is, str, m_filename_len);
            m_filename = FilePath(str);
            readByteSeq(is, m_extra_field, m_extra_field_len);

            if(is)
            {
                m_valid = true;
            }
        }
    }
}


//void DataDescriptor::read(std::istream& is)
//{
//    static_cast<void>(is);
//}


void ZipCDirEntry::read(std::istream& is)
{
    m_valid = false; // set to true upon successful completion.
    if(!is)
    {
        return;
    }

    if(g_signature != readUint32(is))
    {
        // put stream in error state and return
        is.setstate(std::ios::failbit);
        return;
    }

    m_writer_version       = readUint16(is);
    m_extract_version      = readUint16(is);
    m_gp_bitfield          = readUint16(is);
    m_compress_method      = readUint16(is);
    m_unix_time            = dos2unixtime(readUint32(is));
    m_crc_32               = readUint32(is);
    m_compressed_size      = readUint32(is);
    m_uncompressed_size    = readUint32(is);
    m_filename_len         = readUint16(is);
    m_extra_field_len      = readUint16(is);
    m_file_comment_len     = readUint16(is);
    m_disk_num_start       = readUint16(is);
    m_intern_file_attr     = readUint16(is);
    m_extern_file_attr     = readUint32(is);
    m_rel_offset_loc_head  = readUint32(is);

    // Read filename and extra_field
    std::string str;
    readByteSeq(is, str, m_filename_len);
    m_filename = FilePath(str);
    readByteSeq(is, m_extra_field,  m_extra_field_len);
    readByteSeq(is, m_file_comment, m_file_comment_len);

    if(is)
    {
        m_valid = true;
    }
}


void ZipLocalEntry::write(std::ostream& os)
{
    if(os)
    {
        if(m_compressed_size >= 0x100000000
        || m_uncompressed_size >= 0x100000000)
        {
            throw InvalidStateException("The size of this file is too large to fit in a zip archive.");
        }

        writeUint32(g_signature                     , os);
        writeUint16(m_extract_version               , os);
        writeUint16(m_gp_bitfield                   , os);
        writeUint16(m_compress_method               , os);
        writeUint32(unix2dostime(m_unix_time)       , os);
        writeUint32(m_crc_32                        , os);
        writeUint32(m_compressed_size               , os);
        writeUint32(m_uncompressed_size             , os);
        writeUint16(m_filename_len                  , os);
        writeUint16(m_extra_field_len               , os);

        // Write filename and extra_field
        writeByteSeq(os, m_filename);
        writeByteSeq(os, m_extra_field);
    }
}


void ZipCDirEntry::write(std::ostream& os)
{
    if(os)
    {
        if(m_compressed_size >= 0x100000000
        || m_uncompressed_size >= 0x100000000)
        {
            throw InvalidStateException("The size of this file is too large to fit in a zip archive.");
        }

        writeUint32(g_signature                    , os);
        writeUint16(m_writer_version               , os);
        writeUint16(m_extract_version              , os);
        writeUint16(m_gp_bitfield                  , os);
        writeUint16(m_compress_method              , os);
        writeUint32(unix2dostime(m_unix_time)      , os);
        writeUint32(m_crc_32                       , os);
        writeUint32(m_compressed_size              , os);
        writeUint32(m_uncompressed_size            , os);
        writeUint16(m_filename_len                 , os);
        writeUint16(m_extra_field_len              , os);
        writeUint16(m_file_comment_len             , os);
        writeUint16(m_disk_num_start               , os);
        writeUint16(m_intern_file_attr             , os);
        writeUint32(m_extern_file_attr             , os);
        writeUint32(m_rel_offset_loc_head          , os);

        // Write filename and extra_field
        writeByteSeq(os, m_filename);
        writeByteSeq(os, m_extra_field);
        writeByteSeq(os, m_file_comment);
    }
}


void EndOfCentralDirectory::write(std::ostream& os)
{
    if(os)
    {
        writeUint32(g_signature       , os);
        writeUint16(m_disk_num        , os);
        writeUint16(m_cdir_disk_num   , os);
        writeUint16(m_cdir_entries    , os);
        writeUint16(m_cdir_tot_entries, os);
        writeUint32(m_cdir_size       , os);
        writeUint32(m_cdir_offset     , os);
        writeUint16(m_zip_comment_len , os);

        writeByteSeq(os, m_zip_comment);
    }
}



} // zipios namespace
// vim: ts=4 sw=4 et
