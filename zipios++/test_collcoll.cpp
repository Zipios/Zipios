

#include "zipios++/zipios-config.h"

#if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM)
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <memory>

#include "zipios++/dircoll.h"
#include "zipios++/zipfile.h"
#include "zipios++/collcoll.h"

using namespace zipios ;

using std::cerr ;
using std::cout ;
using std::endl ;
using std::auto_ptr ;

int main() {
  try {
  
    cout << "Instantiating a DirectoryCollection" << endl ;
    DirectoryCollection dircoll( "." ) ;

    cout << "Instantiating a ZipFile" << endl ;
    ZipFile zipfile( "test.zip" ) ;
    
    cout << "Instantiating a CollectionCollection" << endl ;
    CollectionCollection collcoll_orig ;

    cout << "Adding the zip file and directory collection to the collection collection" 
	 << endl ;
    if ( ! collcoll_orig.addCollection( zipfile ) ) {
      cerr << "Failed to add the zip file" << endl ;
      return 1 ;
    }
    if ( ! collcoll_orig.addCollection( dircoll ) ) {
      cerr << "Failed to add the zip file" << endl ;
      return 1 ;
    }

    CollectionCollection collcoll( collcoll_orig ) ; // Test copy constructor
//      if ( ! collcoll.addCollection( new ZipFile( "test.zip" ) ) ) {
//        cerr << "Failed to add the zip file" << endl ;
//        return 1 ;
//      }
//      if ( ! collcoll.addCollection( new DirectoryCollection( "." ) ) ) {
//        cerr << "Failed to add the zip file" << endl ;
//        return 1 ;
//      }

//      cout << "list length : " << collcoll.size() << endl ;
    
//      vector< ConstEntryPointer > entries ;
//      entries = collcoll.entries() ;
    
    
//      vector< ConstEntryPointer >::iterator it ;
//      for( it = entries.begin() ; it != entries.end() ; it++)
//        cout << *(*it) << endl ;
    
    ConstEntryPointer ent = collcoll.getEntry( "file2.txt" ) ;
    if ( ent != 0 ) {
      auto_ptr< istream > is( collcoll.getInputStream( ent ) ) ;
      
      cout << "Contents of entry, " << ent->getName() << " :" << endl ;
      
      cout << is->rdbuf() ;
    }

    ent = collcoll.getEntry( "flistentry.cpp" ) ;
    if ( ent != 0 ) {
      auto_ptr< istream > is( collcoll.getInputStream( ent ) ) ;
      
      cout << "Contents of entry, " << ent->getName() << " :" << endl ;
      
      cout << is->rdbuf() ;
    }
    cout << "end of main()" << endl ;
    
    return 0 ;
  }
  catch( exception &excp ) {
    cerr << "Exception caught in main() :" << endl ;
    cerr << excp.what() << endl ;
  }
}

/** \file
    \anchor example_zip_anchor
    source code to a small program that demonstrates the central elements
    of Zipios++.
*/

/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000  Thomas Søndergaard
  
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
