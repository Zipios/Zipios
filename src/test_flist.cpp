
#include "config.h"

#ifdef HAVE_STD_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif
#include <memory>

#include "flist.h"

using namespace zipios ;

using std::cout ;
using std::cerr ;
using std::endl ;

int main() {
  try {
    
    cout << "Instantiating a FileList" << endl ;
    FileList fl( "/home/thomas/src/testdata/list.txt" ) ;
    
    cout << "list length : " << fl.size() << endl ;
    
    vector< const FileEntry * > entries ;
    entries = fl.entries() ;
    
    
    vector< const FileEntry * >::iterator it ;
    for( it = entries.begin() ; it != entries.end() ; it++)
      cout << *(*it) << endl ;
    
    const FileEntry  *ent = fl.getEntry( "fil2.txt", FileCollection::IGNORE ) ;
    if ( ent != 0 ) {
      istream *is = fl.getInputStream( ent ) ;
      const int buflen = 2 ;
      char buf[ buflen ] ;
      
      cout << "Contents of first entry, " << ent->getName() << " :" << endl ;
      
      is->read( buf, buflen - 1 ) ;
      while ( is && ! is->eof() ) {
	buf[ is->gcount() ] = '\0' ;
	cout << buf ;
	is->read( buf, buflen - 1 ) ;
      }
      delete is ;
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
    Source for a test program for testing FileList.
*/

/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000  Thomas Søndergaard
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
