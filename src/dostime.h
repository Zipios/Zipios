#pragma once
/*
  dostime.h - routines for converting UNIX time to MS-DOS time.

  Various Copyrights:

  First written by Mark Adler, Richard B. Wales, Jean-loup Gailly,
  Kai Uwe Rommel, Onno van der Linden and Igor Mandrichenko (1990-1997).
  Tweaked further by Bryan Burns (1999).
  Redesigned with full error checks by Alexis Wilke (2015).

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <time.h>
#include <inttypes.h>


#ifdef __cplusplus
extern "C"
{
#endif


typedef uint32_t  dostime_t;

dostime_t   mindostime();
dostime_t   maxdostime();
time_t      dos2unixtime(dostime_t dostime);
dostime_t   dostime(int year, int month, int day, int hour, int minute, int second);
dostime_t   unix2dostime(time_t const *unix_time);


#ifdef __cplusplus
}
#endif
// vim: ts=2 sw=2 et
