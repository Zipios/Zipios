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
    \anchor appendzip_anchor
    Source code to a small program appendzip that appends a zip 
    archive to another file. Run appendzip without arguments to
    get a helpful usage message.
*/

#include "zipios++/zipheadio.h"

#include <cstdlib>
#include <cstring>

// static variables
namespace
{

char *g_progname ;

}

void printUsage()
{
  std::cout << "Usage:  " << g_progname << " exe-file zipfile" << std::endl ;
  std::cout << "This tool appends a zipfile at the end of any other file." << std::endl;
  std::cout << "WARNING: this will not append the files of a zip file to another." << std::endl;
}


void exitUsage( int exit_code )
{
  printUsage() ;
  exit( exit_code ) ;
}


int main( int argc, char *argv[] )
{
  g_progname = argv[ 0 ] ;
  char *e(strrchr(g_progname, '/'));
  if(e)
  {
    g_progname = e + 1;
  }
  e = strrchr(g_progname, '\\');
  if(e)
  {
    g_progname = e + 1;
  }

  if ( argc != 3 )
  {
    exitUsage( 1 ) ;
  }
  
  std::ofstream exef( argv[ 1 ], std::ios::app | std::ios::binary ) ;
  if( ! exef )
  {
    std::cerr << g_progname << ":error: Unable to open " << argv[ 1 ] << " for writing" << std::endl ;
    exitUsage( 1 ) ;
  }

  std::ifstream zipf( argv[ 2 ], std::ios::in | std::ios::binary ) ;
  if( ! zipf )
  {
    std::cerr << g_progname << ":error: Unable to open " << argv[ 2 ] << " for reading." << std::endl ;
    exitUsage( 1 ) ;
  }

  // get eof pos (to become zip file starting position).
  uint32_t zip_start = exef.tellp() ;
  std::cout << "zip start will be at " << zip_start << std::endl ;

  // Append zip file to exe file

  exef << zipf.rdbuf() ;
  // write zipfile start offset to file
  zipios::writeUint32( zip_start, exef ) ;

  return 0;
}


// vim: ts=2 sw=2 et
