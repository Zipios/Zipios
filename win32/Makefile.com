#
# Common configuration settings for Zipios
#

#
# If zlib is not in standard compiler paths, define it here
#

#ZLIB_INC = ..
#ZLIB_LIB = ..
ZLIB_INC=d:\zlib-1.1.4
ZLIB_LIB=d:\zlib-1.1.4\WIN32-VC

#
# If cppunit is not in standard compiler paths, define it here
# 
#CPPUNIT_INC = ..
#CPPUNIT_LIB = ..
CPPUNIT_INC=d:\cppunit-1.9.11\include
CPPUNIT_LIB=d:\cppunit-1.9.11\WIN32-VC

################## DO NOT EDIT BELOW THIS LINE #############################

#
# Locations
#

TOP_DIR = ..
INC_DIR = $(TOP_DIR)\zipios++
SRC_DIR = $(TOP_DIR)\src
LIB_DIR = $(TOP_DIR)\lib
TST_DIR = $(TOP_DIR)\tests
BLD_DIR = $(TOP_DIR)\win32

###########################################################################

#
# Zipios targets
#

ZIPIOS_LIB = $(LIB_DIR)\zipios_s.lib

ZIPIOS_DLL = $(LIB_DIR)\zipios_d.dll

###########################################################################

#
# Configuration
#

CONFIG_HDR = $(INC_DIR)\zipios-config.h

W32_FILES = $(CONFIG_HDR)

configure: $(W32_FILES)

.SUFFIXES: .w32 .h

.w32.h:
	@type $*.w32 > $@
	@echo *****************
	@echo Zipios configured
	@echo *****************

###########################################################################

#
# Misc
#

tidy:
	@if exist *~ del *~

###########################################################################

#  Zipios++ - a small C++ library that provides easy access to .zip files.
#  Copyright (C) 2000  1. Thomas Søndergaard 2. Kevin Shea

#  Written by Kevin Shea
  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2 of the License, or (at your option) any later version.
  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

############################################################################
