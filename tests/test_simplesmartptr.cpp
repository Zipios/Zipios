
#include "zipios++/zipios-config.h"

#include "zipios++/meta-iostreams.h"
#include <vector>


#include "zipios++/simplesmartptr.h"

using namespace zipios ;

using std::cerr ;
using std::cout ;
using std::endl ;
using std::auto_ptr ;
using std::ofstream ;
using std::vector ;

class Bogus {
protected:
  friend SimpleSmartPointer< Bogus > ;
  friend SimpleSmartPointer< const Bogus > ;

  void           ref() const { _refcount.ref() ;          }
  unsigned int unref() const { return _refcount.unref() ; }
  ReferenceCount< Bogus > _refcount ;
};

typedef SimpleSmartPointer< Bogus > SPBogus ;

int main() {
  Bogus *p = new Bogus ;
  SPBogus sp1( p ) ;
  SPBogus sp2 ;
  sp2 = sp1 ;

  SPBogus sp3 ;
  sp3 = p ;

  cerr << " p         = " << p         << endl ;
  cerr << " sp1.get() = " << sp1.get() << endl ;
  cerr << " sp2.get() = " << sp2.get() << endl ;
  cerr << " sp3.get() = " << sp3.get() << endl ;

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
