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
    \anchor test_zip_anchor
    Source code to a small program that tests the functionality of Zipios++.
*/

#include "zipios++/zipfile.h"
#include "zipios++/zipinputstream.h"

#include <memory>


void entryToFile( std::string const&, std::istream &is,
                  std::string const& outfile,
                  bool cerr_report )
{
  std::ofstream ofs( outfile.c_str(), std::ios::out | std::ios::binary ) ;

//    cout << "writing " << ent_name << " to " << outfile << endl ;

  ofs << is.rdbuf() ;
  if ( cerr_report )
  {
    std::cerr << "Stream state: "  ;
    std::cerr << "good() = " << is.good() << ",\t" ;
    std::cerr << "fail() = " << is.fail() << ",\t" ;
    std::cerr << "bad()  = " << is.bad()  << ",\t" ;
    std::cerr << "eof()  = " << is.eof()  << std::endl << std::endl;
  }
  ofs.close() ;
}



int main()
{
  try
  {
    std::string const name_zipfile( "test.zip"              ) ;
    std::string const name_entry1 ( "file1.txt"             ) ;
    std::string const name_entry2 ( "file2.txt"             ) ;
    std::string const name_entry3 ( "file3.txt"             ) ;
    std::string const unzip_ext   ( "unzipped"              ) ;
    std::string const name_uz1    ( name_entry1 + unzip_ext ) ;
    std::string const name_uz2    ( name_entry2 + unzip_ext ) ;
    std::string const name_uz3    ( name_entry3 + unzip_ext ) ;
    std::string const diffcmd     ( "diff -q "              ) ;
    std::string const unzipcmd    ( "unzip -oq "            ) ;

    zipios::ZipFile rzf( name_zipfile ) ;
    zipios::ZipFile zf( rzf ) ; // Test copy constructor
//      ZipFile zf( name_zipfile ) ;

    zipios::ConstEntries entries = zf.entries() ;
//      cout << "\nEntries (" << zf.size() <<  "):\n" ;
//      ConstEntries::iterator it ;
//      for( it = entries.begin() ; it != entries.end() ; it++)
//        cout << *(*it) << endl ;
//      cout << "\n" ;


    // Unzip second entry
    zipios::ConstEntryPointer ent = zf.getEntry( name_entry2, zipios::FileCollection::MatchPath::IGNORE ) ;
    if ( ent )
    {
      std::auto_ptr< std::istream > is( zf.getInputStream( ent ) ) ;
      entryToFile( name_entry2, *is, name_uz2, false ) ;
    }

    // Unzip first entry
    ent = zf.getEntry( name_entry1, zipios::FileCollection::MatchPath::IGNORE ) ;
    if ( ent )
    {
      std::auto_ptr< std::istream > is( zf.getInputStream( ent ) ) ;
      entryToFile( name_entry1, *is, name_uz1, false ) ;
    }

    // Unzip third entry, by use of ZipInputStream alone
    zipios::ZipInputStream zf2( name_zipfile ) ;
    zf2.getNextEntry() ;
    zf2.getNextEntry() ;
    entryToFile( name_entry3, zf2, name_uz3, false ) ;

//      cerr << "Unzipping entries using 'unzip' to get references to 'diff' against :\n" ;
    if(system( ( unzipcmd + name_zipfile + " " + name_entry1 + " " +
                    name_entry2 + " " + name_entry3 ).c_str() ))
    {
      return 1;
    }

//      cerr << "\nOutput from " << diffcmd << " :\n" ;

    // Fail if any one of the fails
    return system( ( diffcmd + name_uz1 + " " + name_entry1 ).c_str() ) +
           system( ( diffcmd + name_uz2 + " " + name_entry2 ).c_str() ) +
           system( ( diffcmd + name_uz3 + " " + name_entry3 ).c_str() ) ;
  }
  catch( std::exception const& excp )
  {
    std::cerr << "Exception caught in main() :" << std::endl ;
    std::cerr << excp.what() << std::endl ;
  }
  return -1;
}




// vim: ts=2 sw=2 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:
