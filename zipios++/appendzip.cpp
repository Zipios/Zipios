#include "zipios++/zipios-config.h"

#if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM)
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif

#include "ziphead.h"

using namespace std ;
using namespace zipios ;

char *_progname ;

void printUsage() ;
void exitUsage( int exit_code ) ;

int main( int argc, char *argv[] ) {
  _progname = argv[ 0 ] ;
  if ( argc != 3 ) 
    exitUsage( 1 ) ;
  
  ofstream  exef( argv[ 1 ], ios::app ) ;
  if( ! exef ) {
    cout << "Error: Unable to open " << argv[ 1 ] << " for writing" << endl ;
    exitUsage( 1 ) ;
  }

  ifstream  zipf( argv[ 2 ] ) ;
  if( ! zipf ) {
    cout << "Error: Unable to open " << argv[ 2 ] << " for reading." << endl ;
    exitUsage( 1 ) ;
  }

  // get eof pos (to become zip file starting position).
  uint32 zip_start = exef.tellp() ;
  cout << "zip start will be at " << zip_start << endl ;

  // Append zip file to exe file

  exef << zipf.rdbuf() ;
  // write zipfile start offset to file
  writeUint32( zip_start, exef ) ;

  exef.close() ;
  zipf.close() ;
}


void printUsage() {
  cout << "Usage:  " << _progname << " exe-file zipfile" << endl ;
}

void exitUsage( int exit_code ) {
  printUsage() ;
  exit( exit_code ) ;
}
