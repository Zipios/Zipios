#ifndef META_IOSTREAMS_H
#define META_IOSTREAMS_H

// Includes the different iostream libraries

#include "zipios++/zipios-config.h"

#if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM)
#include <iostream>
#include <fstream>
#include <sstream>
#include <strstream>

// #if IOSTREAMS_IN_NAMESPACE_STD
using std::cerr ;
using std::cout ;
using std::endl ;
using std::ends ;
using std::ifstream ;
using std::ios ;
using std::istream ;
using std::ofstream ;
using std::ostream ;
using std::ostringstream ;
using std::ostrstream ;
using std::streambuf ;
using std::streampos ;
// #endif

#else
#include <iostream.h>
#include <fstream.h>
#include <strstream>
#endif

#endif
