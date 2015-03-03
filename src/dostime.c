/*
  dostime.c - routines for converting UNIX time to MS-DOS time.

  Borrowed from Info-zip's unzip

  Copyright (C) 1999 Bryan Burns

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

/*

   Revision 1.3  2001/09/17 20:35:55  zlaski
   Resolve conflicts due to 2001-09-12 FSF import.

   Revision 1.2  2000/12/14 18:45:35  ghazi
   Warning fixes:

      * compress.c: Include stdlib.h and compress.h.
        (rcsid): Delete.
        (report_str_error): Make static.
        (ez_inflate_str): Delete unused variable.  Add parens in if-stmt.
        (hrd_inflate_str): Likewise.

      * compress.h (init_compression, end_compression, init_inflation,
        end_inflation): Prototype void arguments.

    * dostime.c (rcsid): Delete.

    * jargrep.c: Include ctype.h, stdlib.h, zlib.h and compress.h.
      Make functions static.  Cast ctype function argument to `unsigned
      char'.  Add parens in if-stmts.  Constify.
      (Usage): Change into a macro.
      (jargrep): Remove unused parameter.

    * jartool.c: Constify.  Add parens in if-stmts.  Align
      signed/unsigned char pointers in functions calls using casts.
      (rcsid): Delete.
      (list_jar): Fix printf format specifier.
      (usage): Chop long string into bits.  Reformat.

    * pushback.c (rcsid): Delete.

   Revision 1.1  2000/12/09 03:08:23  apbianco
   2000-12-08  Alexandre Petit-Bianco  <apbianco@cygnus.com>

           * fastjar: Imported.

   Revision 1.1.1.1  1999/12/06 03:09:12  toast
   initial checkin..



   Revision 1.6  1999/05/10 08:32:26  burnsbr
   added dos2unixtime

   Revision 1.5  1999/04/27 10:03:50  burnsbr
   configure support

   Revision 1.4  1999/04/26 21:55:19  burnsbr
   switched from sys/time.h to time.h for better portability

   Revision 1.3  1999/04/20 08:54:30  burnsbr
   added GPL comment

   Revision 1.2  1999/04/20 05:10:53  burnsbr
   added RCS tags

*/
#include "zipios++/dostime.h"

/*

 Copyright (C) 1990-1997 Mark Adler, Richard B. Wales, Jean-loup Gailly,
 Kai Uwe Rommel, Onno van der Linden and Igor Mandrichenko.
 Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as all of the original files are included,
 that it is not sold for profit, and that this copyright notice is retained.

*/


/** \brief Convert a DOS time to a Unix time
 *
 * Return the Unix time_t value (GMT/UTC time) for the DOS format (local)
 * time dostime, where dostime is a four byte value (date in most
 * significant word, time in least significant word), see dostime()
 * function.
 *
 * \param[in] dostime  A DOS time value as found in a zip file.
 *
 * \sa dostime()
 */
time_t dos2unixtime(unsigned long dostime)
{
    struct tm *t;         /* argument for mktime() */
    time_t const clock = time(NULL);

    t = localtime(&clock);
    t->tm_isdst = -1;     /* let mktime() determine if DST is in effect */
    /* Convert DOS time to UNIX time_t format */
    t->tm_sec  = (((int)dostime) <<  1) & 0x3e;
    t->tm_min  = (((int)dostime) >>  5) & 0x3f;
    t->tm_hour = (((int)dostime) >> 11) & 0x1f;
    t->tm_mday = (int)(dostime >> 16) & 0x1f;
    t->tm_mon  = ((int)(dostime >> 21) & 0x0f) - 1;
    t->tm_year = ((int)(dostime >> 25) & 0x7f) + 80;

    return mktime(t);
}


/* \brief Convert a broken up date to a DOS date.
 *
 * Convert the date y/n/d and time h:m:s to a four byte DOS date and
 * time (date in high two bytes, time in low two bytes allowing magnitude
 * comparison).
 *
 * \param[in] y  The year.
 * \param[in] n  The month.
 * \param[in] d  The day.
 * \param[in] h  The hour.
 * \param[in] m  The minute.
 * \param[in] s  The second.
 */
unsigned long dostime(int y, int n, int d, int h, int m, int s)
{
    return y < 1980 ? dostime(1980, 1, 1, 0, 0, 0) :
        (((unsigned long)y - 1980) << 25) | ((unsigned long)n << 21) |
        ((unsigned long)d << 16) | ((unsigned long)h << 11) |
        ((unsigned long)m << 5) | ((unsigned long)s >> 1);
}


/** \brief Convert a Unix date to a DOS date.
 *
 * This function return the Unix time_t converted in DOS format,
 * rounded up to the next even second.
 *
 * \param[in] t  A Unix time_t value.
 *
 * \return The Unix date in DOS format.
 */
unsigned long unix2dostime(time_t *t)
{
    time_t t_even;
    struct tm *s;         /* result of localtime() */

    t_even = (*t + 1) & (~1);     /* Round up to even seconds. */
    s = localtime(&t_even);       /* Use local time since MSDOS does. */
    return dostime(s->tm_year + 1900, s->tm_mon + 1, s->tm_mday,
                   s->tm_hour, s->tm_min, s->tm_sec);
}

// vim: ts=4 sw=4 et
