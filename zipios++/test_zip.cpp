
#include "zipios++/zipios-config.h"

#if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM)
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <memory>
#include <stdlib.h>

#include "zipios++/zipfile.h"
#include "zipios++/zipinputstream.h"

using namespace zipios ;

using std::cerr ;
using std::cout ;
using std::endl ;
using std::auto_ptr ;
using std::ofstream ;

void entryToFile( const string &ent_name, istream &is, const string &outfile,
		  bool cerr_report = true ) ;

int main() {
  try {
    const string name_zipfile( "test.zip"              ) ;
    const string name_entry1 ( "file1.txt"             ) ;
    const string name_entry2 ( "file2.txt"             ) ;
    const string name_entry3 ( "file3.txt"             ) ;
    const string unzip_ext   ( "unzipped"              ) ;
    const string name_uz1    ( name_entry1 + unzip_ext ) ;
    const string name_uz2    ( name_entry2 + unzip_ext ) ;
    const string name_uz3    ( name_entry3 + unzip_ext ) ;
    const string diffcmd     ( "diff -sq "             ) ;
    const string unzipcmd    ( "unzip -oq "             ) ;

    ZipFile rzf( name_zipfile ) ;
    ZipFile zf( rzf ) ; // Test copy constructor
//      ZipFile zf( name_zipfile ) ;

    vector< ConstEntryPointer > entries = zf.entries() ;
    cout << "\nEntries (" << zf.size() <<  "):\n" ;
    vector< ConstEntryPointer >::iterator it ;
    for( it = entries.begin() ; it != entries.end() ; it++)
      cout << *(*it) << endl ;
    cout << "\n" ;
    
    
    // Unzip second entry
    ConstEntryPointer ent = zf.getEntry( name_entry2, FileCollection::IGNORE ) ;
    if ( ent != 0 ) {
      auto_ptr< istream > is( zf.getInputStream( ent ) ) ;
      entryToFile( name_entry2, *is, name_uz2 ) ;
    }
    
    // Unzip first entry
    ent = zf.getEntry( name_entry1, FileCollection::IGNORE ) ;
    if ( ent != 0 ) {
      auto_ptr< istream > is( zf.getInputStream( ent ) ) ;
      entryToFile( name_entry1, *is, name_uz1 ) ;
    }

    // Unzip third entry, by use of ZipInputStream alone
    ZipInputStream zf2( name_zipfile ) ;
    zf2.getNextEntry() ;
    zf2.getNextEntry() ;
    entryToFile( name_entry3, zf2, name_uz3 ) ;
    
    cerr << "Unzipping entries using 'unzip' to get references to 'diff' against :\n" ;
    system( string( unzipcmd + name_zipfile + " " + name_entry1 + " " + 
		    name_entry2 + " " + name_entry3 ).c_str() ) ;
    cerr << "\nOutput from " << diffcmd << " :\n" ;
    system( string( diffcmd + name_uz1 + " " + name_entry1 ).c_str() ) ;
    system( string( diffcmd + name_uz2 + " " + name_entry2 ).c_str() ) ;
    system( string( diffcmd + name_uz3 + " " + name_entry3 ).c_str() ) ;

    // Do things that are supposed to fail here
    // FIXME: well... do it.

    cout << "end of main()" << endl ;
    
    return 0 ;
  }
  catch( exception &excp ) {
    cerr << "Exception caught in main() :" << endl ;
    cerr << excp.what() << endl ;
  }
}


void entryToFile( const string &ent_name, istream &is, const string &outfile,
		  bool cerr_report ) {
  ofstream ofs( outfile.c_str() ) ;

  cout << "writing " << ent_name << " to " << outfile << endl ;

  ofs << is.rdbuf() ;
  if ( cerr_report ) {
    cerr << "Stream state: "  ;
    cerr << "good() = " << is.good() << ",\t" ;
    cerr << "fail() = " << is.fail() << ",\t" ;
    cerr << "bad()  = " << is.bad()  << ",\t" ;
    cerr << "eof()  = " << is.eof()  << endl << endl;
  }
  ofs.close() ;
}
   

/** \file
    \anchor test_zip_anchor
    Source code to a small program that tests the functionality of Zipios++.
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
