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


#include "zipios++/collcoll.h"

#include "zipios++/dircoll.h"
#include "zipios++/zipfile.h"

#include <memory>


int main()
{
  try
  {
    std::cout << "Instantiating a DirectoryCollection" << std::endl ;
    zipios::DirectoryCollection dircoll( "." ) ;

    std::cout << "Instantiating a ZipFile" << std::endl ;
    zipios::ZipFile zipfile( "test.zip" ) ;

    std::cout << "Instantiating a CollectionCollection" << std::endl ;
    zipios::CollectionCollection collcoll_orig ;

    std::cout << "Adding the zip file and directory collection to the collection collection" << std::endl ;
    if ( ! collcoll_orig.addCollection( zipfile ) )
    {
      std::cerr << "Failed to add the zip file" << std::endl ;
      return 1 ;
    }
    if ( ! collcoll_orig.addCollection( dircoll ) )
    {
      std::cerr << "Failed to add the zip file" << std::endl ;
      return 1 ;
    }

    zipios::CollectionCollection collcoll( collcoll_orig ) ; // Test copy constructor
    zipios::CColl::inst() = collcoll ; // test copy-assignment and Singleton instance inst().

//      if ( ! collcoll.addCollection( new ZipFile( "test.zip" ) ) ) {
//        std::cerr << "Failed to add the zip file" << std::endl ;
//        return 1 ;
//      }
//      if ( ! collcoll.addCollection( new DirectoryCollection( "." ) ) ) {
//        std::cerr << "Failed to add the zip file" << std::endl ;
//        return 1 ;
//      }

//      std::cout << "list length : " << collcoll.size() << std::endl ;

//      ConstEntries entries ;
//      entries = collcoll.entries() ;


//      ConstEntries::iterator it ;
//      for( it = entries.begin() ; it != entries.end() ; it++)
//        std::cout << *(*it) << std::endl ;

    zipios::ConstEntryPointer ent = zipios::CColl::inst().getEntry( "file2.txt" ) ;
    if ( ent )
    {
      std::auto_ptr< std::istream > is( zipios::CColl::inst().getInputStream( ent ) ) ;

      std::cout << "Contents of entry, " << ent->getName() << " :" << std::endl ;

      std::cout << is->rdbuf() ;
    }

    ent = zipios::CColl::inst().getEntry( "flistentry.cpp" ) ;
    if ( ent )
    {
      std::auto_ptr< std::istream > is( zipios::CColl::inst().getInputStream( ent ) ) ;

      std::cout << "Contents of entry, " << ent->getName() << " :" << std::endl ;

      std::cout << is->rdbuf() ;
    }
    std::cout << "end of main()" << std::endl ;
  }
  catch( std::exception const& e )
  {
    std::cerr << "Exception caught in main() :" << std::endl ;
    std::cerr << e.what() << std::endl ;
    return 1;
  }

  return 0 ;
}

// vim: ts=2 sw=2 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:
