#!/bin/sh
#
# Generally this script is called using make as follow:
#
#    make -C YOUR_BUILD_DIR/zipios/ zipios_code_analysis
#
# License:
#      Zipios -- a small C++ library that provides easy access to .zip files.
#      Copyright (c) 2011-2019 Made to Order Software Corp.
#      contact@m2osw.com
#
#      This program is free software; you can redistribute it and/or modify
#      it under the terms of the GNU General Public License as published by
#      the Free Software Foundation; either version 2 of the License, or
#      (at your option) any later version.
#
#      This program is distributed in the hope that it will be useful,
#      but WITHOUT ANY WARRANTY; without even the implied warranty of
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#      GNU General Public License for more details.
#
#      You should have received a copy of the GNU General Public License
#      along with this program; if not, write to the Free Software
#      Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
#

if test -z "$1"
then
    echo "$0: Usage: $0 <path to output>"
    exit 1;
fi

OUTPUT=$1/todo.txt

echo "TODO: entries that need to be fixed before release 2.0" >$OUTPUT
find . -type f -exec grep TODO {} \; | wc -l >>$OUTPUT

echo "FIXME: things that probably need to be fixed (was used by previous owners)" >>$OUTPUT
find . -type f -exec grep FIXME {} \; | wc -l >>$OUTPUT

echo "XXX: entries that are likely to be addressed quickly" >>$OUTPUT
find . -type f -exec grep XXX {} \; | wc -l >>$OUTPUT

echo "TBD: questions that need testing to be answered" >>$OUTPUT
find . -type f -exec grep TBD {} \; | wc -l >>$OUTPUT

echo "todo: long term, nice to have things defined in Doxygen" >>$OUTPUT
find . -type f -exec grep "todo:\|\\todo" {} \; | wc -l >>$OUTPUT

# vim: ts=4 sw=4 et
