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
 * \brief The implementation of the I/O to and from a Zip archive.
 *
 * Implementation of I/O functions for the header structures
 * defined in ziphead.h.
 */

#include "zipheadio.hpp"

#include "zipios++/zipiosexceptions.hpp"

#include "dostime.h"


namespace zipios
{



std::istream& operator >> (std::istream& is, ZipLocalEntry& zlh)
{
    zlh.m_valid = false ; // set to true upon successful completion.
    if(!is)
    {
        return is;
    }

    //    // Before reading anything we record the position in the stream
    //    // This is a field in the central directory entry, but not
    //    // in the local entry. After all, we know where we are, anyway.
    //    zlh.rel_offset_loc_head  = is.tellg() ;

    if(zlh.g_signature != readUint32(is))
    {
        // put stream in error state and return
        is.setstate(std::ios::failbit);
        return is;
    }

    zlh.m_extract_version   = readUint16(is);
    zlh.m_gp_bitfield       = readUint16(is);
    zlh.m_compress_method   = readUint16(is);
    uint16_t const last_mod_ftime = readUint16(is);    // FIXME can we use Uint32 instead?
    uint16_t const last_mod_fdate = readUint16(is);
    zlh.m_unix_time = dos2unixtime((last_mod_fdate << 16) + last_mod_ftime);
    zlh.m_crc_32            = readUint32(is);
    zlh.m_compressed_size   = readUint32(is);
    zlh.m_uncompressed_size = readUint32(is);
    zlh.m_filename_len      = readUint16(is);
    zlh.m_extra_field_len   = readUint16(is);

    // Read filename and extra_field
    readByteSeq(is, zlh.m_filename,    zlh.m_filename_len);
    readByteSeq(is, zlh.m_extra_field, zlh.m_extra_field_len);

    if(is)
    {
        zlh.m_valid = true;
    }

    return is;
}


std::istream& operator >> (std::istream& is, DataDescriptor&)
{
    return is;
}


std::istream& operator >> (std::istream& is, ZipCDirEntry& zcdh)
{
    zcdh.m_valid = false; // set to true upon successful completion.
    if(!is)
    {
        return is;
    }

    if(zcdh.g_signature != readUint32(is))
    {
        // put stream in error state and return
        is.setstate(std::ios::failbit);
        return is;
    }

    zcdh.m_writer_version       = readUint16(is);
    zcdh.m_extract_version      = readUint16(is);
    zcdh.m_gp_bitfield          = readUint16(is);
    zcdh.m_compress_method      = readUint16(is);
    uint16_t const last_mod_ftime = readUint16(is);    // FIXME can we use Uint32 instead?
    uint16_t const last_mod_fdate = readUint16(is);
    zcdh.m_unix_time = dos2unixtime((last_mod_fdate << 16) + last_mod_ftime);
    zcdh.m_crc_32               = readUint32(is);
    zcdh.m_compressed_size      = readUint32(is);
    zcdh.m_uncompressed_size    = readUint32(is);
    zcdh.m_filename_len         = readUint16(is);
    zcdh.m_extra_field_len      = readUint16(is);
    zcdh.m_file_comment_len     = readUint16(is);
    zcdh.m_disk_num_start       = readUint16(is);
    zcdh.m_intern_file_attr     = readUint16(is);
    zcdh.m_extern_file_attr     = readUint32(is);
    zcdh.m_rel_offset_loc_head  = readUint32(is);

    // Read filename and extra_field
    readByteSeq(is, zcdh.m_filename,     zcdh.m_filename_len);
    readByteSeq(is, zcdh.m_extra_field,  zcdh.m_extra_field_len);
    readByteSeq(is, zcdh.m_file_comment, zcdh.m_file_comment_len);

    if(is)
    {
        zcdh.m_valid = true;
    }

    return is;
}


std::ostream& operator << (std::ostream& os, ZipLocalEntry const& zlh)
{
    if(os)
    {
        if(zlh.m_compressed_size >= 0x100000000
        || zlh.m_uncompressed_size >= 0x100000000)
        {
            throw InvalidStateException("The size of this file is too large to fit in a zip archive.");
        }

        uint32_t const dostime(unix2dostime(&zlh.m_unix_time));

        writeUint32(zlh.g_signature                     , os);
        writeUint16(zlh.m_extract_version               , os);
        writeUint16(zlh.m_gp_bitfield                   , os);
        writeUint16(zlh.m_compress_method               , os);
        writeUint16(static_cast<uint16_t>(dostime)      , os); // FIXME can we use Uint32 instead?
        writeUint16(static_cast<uint16_t>(dostime >> 16), os);
        writeUint32(zlh.m_crc_32                        , os);
        writeUint32(zlh.m_compressed_size               , os);
        writeUint32(zlh.m_uncompressed_size             , os);
        writeUint16(zlh.m_filename_len                  , os);
        writeUint16(zlh.m_extra_field_len               , os);

        // Write filename and extra_field
        writeByteSeq(os, zlh.m_filename);
        writeByteSeq(os, zlh.m_extra_field);
    }

    return os;
}


std::ostream& operator << (std::ostream& os, ZipCDirEntry const& zcdh)
{
    if(os)
    {
        if(zcdh.m_compressed_size >= 0x100000000
        || zcdh.m_uncompressed_size >= 0x100000000)
        {
            throw InvalidStateException("The size of this file is too large to fit in a zip archive.");
        }

        uint32_t const dostime(unix2dostime(&zcdh.m_unix_time));

        writeUint32(zcdh.g_signature                    , os);
        writeUint16(zcdh.m_writer_version               , os);
        writeUint16(zcdh.m_extract_version              , os);
        writeUint16(zcdh.m_gp_bitfield                  , os);
        writeUint16(zcdh.m_compress_method              , os);
        writeUint16(static_cast<uint16_t>(dostime)      , os); // FIXME can we use Uint32 instead?
        writeUint16(static_cast<uint16_t>(dostime >> 16), os);
        writeUint32(zcdh.m_crc_32                       , os);
        writeUint32(zcdh.m_compressed_size              , os);
        writeUint32(zcdh.m_uncompressed_size            , os);
        writeUint16(zcdh.m_filename_len                 , os);
        writeUint16(zcdh.m_extra_field_len              , os);
        writeUint16(zcdh.m_file_comment_len             , os);
        writeUint16(zcdh.m_disk_num_start               , os);
        writeUint16(zcdh.m_intern_file_attr             , os);
        writeUint32(zcdh.m_extern_file_attr             , os);
        writeUint32(zcdh.m_rel_offset_loc_head          , os);

        // Write filename and extra_field
        writeByteSeq(os, zcdh.m_filename);
        writeByteSeq(os, zcdh.m_extra_field);
        writeByteSeq(os, zcdh.m_file_comment);
    }

    return os;
}


std::ostream& operator << (std::ostream &os, EndOfCentralDirectory const& eocd)
{
    if(os)
    {
        writeUint32(eocd.g_signature       , os);
        writeUint16(eocd.m_disk_num        , os);
        writeUint16(eocd.m_cdir_disk_num   , os);
        writeUint16(eocd.m_cdir_entries    , os);
        writeUint16(eocd.m_cdir_tot_entries, os);
        writeUint32(eocd.m_cdir_size       , os);
        writeUint32(eocd.m_cdir_offset     , os);
        writeUint16(eocd.m_zip_comment_len , os);

        writeByteSeq(os, eocd.m_zip_comment);
    }

    return os;
}



} // zipios namespace
// vim: ts=4 sw=4 et
