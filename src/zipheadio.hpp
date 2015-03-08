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
    Header file that defines I/O functions for the header structures
    defined in ziphead.h.
*/

#include "zipios++/ziphead.hpp"


namespace zipios
{

// byte order conversion functions.
// ztohs (zip-to-host-short)
#ifdef MY_BIG_ENDIAN

inline uint16_t ztohs ( unsigned char const *buf )
{
    return (static_cast<uint16_t>(buf[0]) << 8) +
           (static_cast<uint16_t>(buf[1])     );
}


// ztohl (zip-to-host-long)
inline uint32_t ztohl ( unsigned char const *buf )
{
    return (static_cast<uint32_t>(buf[0]) << 24) +
           (static_cast<uint32_t>(buf[1]) << 16) +
           (static_cast<uint32_t>(buf[2]) << 8 ) +
           (static_cast<uint32_t>(buf[3])      );
}

#else

inline uint16_t ztohs(unsigned char const *buf)
{
    return (static_cast<uint16_t>(buf[1]) << 8) +
           (static_cast<uint16_t>(buf[0])     );
}


// ztohl (zip-to-host-long)
inline uint32_t ztohl(unsigned char const *buf)
{
    uint32_t out;
    out = ( static_cast< uint32_t >( buf[ 3 ] ) << 24 ) +
          ( static_cast< uint32_t >( buf[ 2 ] ) << 16 ) +
          ( static_cast< uint32_t >( buf[ 1 ] ) << 8  ) +
          ( static_cast< uint32_t >( buf[ 0 ] )       )  ;

    //    std::cerr << "buf : " << static_cast< int >( buf[ 0 ] ) ;
    //    std::cerr << " "      << static_cast< int >( buf[ 1 ] ) ;
    //    std::cerr << " "      << static_cast< int >( buf[ 2 ] ) ;
    //    std::cerr << " "      << static_cast< int >( buf[ 3 ] ) << std::endl ;
    //    std::cerr << "uint32_t " << out << std::endl ;
    return out;
}


#endif

// htozl (host-to-zip-long)
inline uint32_t htozl(unsigned char const *buf)
{
    return ztohl(buf);
}


// htozs (host-to-zip-short)
inline uint16_t htozs(unsigned char const *buf)
{
    return ztohs(buf);
}


inline uint32_t readUint32(std::istream& is)
{
    int const buf_len(sizeof(uint32_t));
    unsigned char buf[buf_len];
    int rsf(0);
    while(rsf < buf_len)
    {
        is.read(reinterpret_cast<char *>(buf) + rsf, buf_len - rsf);
        rsf += is.gcount();
    }
    return ztohl(buf);
}


inline void writeUint32 ( uint32_t const host_val, std::ostream& os )
{
    uint32_t const val = htozl( reinterpret_cast< unsigned char const * >( &host_val ) ) ;
    os.write( reinterpret_cast< char const * >( &val ), sizeof( uint32_t ) ) ;
}


inline uint16_t readUint16(std::istream& is)
{
    int const buf_len(sizeof(uint16_t));
    unsigned char buf[buf_len];
    int rsf(0);
    while(rsf < buf_len)
    {
        is.read(reinterpret_cast<char *>(buf) + rsf, buf_len - rsf);
        rsf += is.gcount();
    }
    return ztohs(buf);
}


inline void writeUint16 ( uint16_t const host_val, std::ostream& os )
{
    uint16_t val = htozl( reinterpret_cast< unsigned char const * >( &host_val ) ) ;
    os.write( reinterpret_cast< char const * >( &val ), sizeof( uint16_t ) ) ;
}


inline void readByteSeq(std::istream& is, std::string& con, int const count)
{
    std::vector<char> buf;
    buf.resize(count + 1);
    int rsf = 0;
    while(rsf < count && is)
    {
        is.read(&buf[0] + rsf, count - rsf);
        rsf += is.gcount();
    }
    buf[count] = '\0';

    con = &buf[0];
}


inline void writeByteSeq( std::ostream& os, const std::string& con )
{
    os << con;
}


inline void readByteSeq(std::istream& is, unsigned char *buf, int const count)
{
    int rsf(0);
    while(rsf < count && is)
    {
        is.read(reinterpret_cast<char *>(buf) + rsf, count - rsf);
        rsf += is.gcount();
    }
}


inline void writeByteSeq ( std::ostream &os, unsigned char const *buf, int count )
{
    os.rdbuf()->sputn( reinterpret_cast< char const * >( buf ), count ) ;
}


inline void readByteSeq ( std::istream &is, std::vector < unsigned char >& vec, int const count )
{
    std::vector<unsigned char> buf;
    buf.resize(count);
    int rsf = 0 ;
    while ( rsf < count && is )
    {
        is.read ( reinterpret_cast< char * >( &buf[0] ) + rsf, count - rsf ) ;
        rsf += is.gcount() ;
    }

    vec.insert ( vec.end (), buf.begin(), buf.end() ) ;
}


inline void writeByteSeq(std::ostream& os, std::vector <unsigned char> const& vec)
{
    os.rdbuf()->sputn(reinterpret_cast<char const *>(&vec[0]), vec.size());
}


//std::istream& operator >> (std::istream& is, ZipLocalEntry& zlh);
//std::istream& operator >> (std::istream& is, DataDescriptor& dd);
//std::istream& operator >> (std::istream& is, ZipCDirEntry& zcdh);
//std::istream& operator >> (std::istream& is, EndOfCentralDirectory& eocd);

//std::ostream &operator << (std::ostream& os, ZipLocalEntry const& zlh);
//std::ostream &operator << (std::ostream& os, ZipCDirEntry const& zcdh);
//std::ostream &operator << (std::ostream& os, EndOfCentralDirectory const& eocd);


} // zipios namespace
// vim: ts=4 sw=4 et
