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
    \anchor example_zip_anchor
    source code to a small program that demonstrates the central elements
    of Zipios++.
*/

#include "zipios++/zipfile.h"

#include <memory>

int main()
{
  try
  {
    std::cout << "Instantiating a ZipFile" << std::endl ;
    zipios::ZipFile zf( "test.zip" ) ;
    
    std::cout << "list length : " << zf.size() << std::endl ;
    
    zipios::ConstEntries entries ;
    entries = zf.entries() ;
    
    
    for( auto it = entries.begin() ; it != entries.end() ; ++it)
    {
      std::cout << *(*it) << std::endl ;
    }

    zipios::ConstEntryPointer ent = zf.getEntry( "file2.txt", zipios::FileCollection::MatchPath::IGNORE ) ;
    if ( ent )
    {
      std::auto_ptr< std::istream > is( zf.getInputStream( ent ) ) ;
      
      std::cout << "Contents of entry, " << ent->getName() << " :" << std::endl ;
      
      std::cout << is->rdbuf() ;
    }
    std::cout << "end of main()" << std::endl ;
    
    return 0 ;
  }
  catch( std::exception const& excp )
  {
    std::cerr << "Exception caught in main() :" << std::endl ;
    std::cerr << excp.what() << std::endl ;
  }
  return -1;
}

