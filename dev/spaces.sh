#!/bin/sh
#
# Generally this script is called using make as follow:
#
#    make -C YOUR_BUILD_DIR/zipios/ zipios_code_analysis
#
# License:
#      Zipios -- a small C++ library that provides easy access to .zip files.
#
#      Copyright (c) 2011-2019 Made to Order Software Corp.
#      contact@m2osw.com
#
#      This library is free software; you can redistribute it and/or
#      modify it under the terms of the GNU Lesser General Public
#      License as published by the Free Software Foundation; either
#      version 2.1 of the License, or (at your option) any later version.
#
#      This library is distributed in the hope that it will be useful,
#      but WITHOUT ANY WARRANTY; without even the implied warranty of
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#      Lesser General Public License for more details.
#
#      You should have received a copy of the GNU Lesser General Public
#      License along with this library; if not, write to the Free Software
#      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#

if test -z "$1"
then
    echo "$0: Usage: $0 <path to output>"
    exit 1;
fi

# List files that have tabs
grep -s '	' */* | grep -v Binary | sed -e 's/:.*//' | sort -u >$1/spaces.txt

# List files that have lines ending with spaces
grep -s ' $' */* | grep -v Binary | sed -e 's/:.*//' | sort -u >>$1/spaces.txt

# vim: ts=4 sw=4 et
