#ifndef FLISTSCANNER_H
#define FLISTSCANNER_H

#include "config.h"

namespace zipios {

// #define straddle namespaces !!

#define S_KW_ROOTDIR_EQ     2
#define S_STRING            3
#define S_COMMENT           4
#define S_IGNORE_COMMENT    5
#define S_NEWLINE           6
#define S_UNRECOGNIZED      7

#undef  yyFlexLexer
#define yyFlexLexer FListScanner
#include <FlexLexer.h>

/** \class FListScanner FListScanner is a flex c++ scanner class used
by FListScanner to help parse a FileList specification. */

} // namespace

#endif

/** \file 
    Header file that includes the FlexLexer.h to define FListScanner.
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
