/*
  dostime.c - routines for converting UNIX time to MS-DOS time and vice versa.

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
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/** \file
 * \brief Functions to convert times between Unix and MS-DOS times.
 *
 * These functions make use of the old date and time format
 * defined to implement the FAT file system. This is defined
 * on the Microsoft website here:
 *
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms724247%28v=vs.85%29.aspx
 *
 * As a quick recap, we have:
 *
 * Date (actually we view those as bit 16 to 31)
 *
 * \code
 * 0-4    Day of the month (1-31)
 * 5-8    Month (1 = January, 2 = February, and so on)
 * 9-15   Year offset from 1980 (add 1980 to get actual year)
 * \endcode
 *
 * Time
 *
 * \code
 * 0-4    Second divided by 2
 * 5-10   Minute (0-59)
 * 11-15  Hour (0-23 on a 24-hour clock)
 * \endcode
 *
 * \note
 * This implementation uses C code.
 */

#include "dostime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** \brief Return the minimum possible value.
 *
 * This function returns the minimum DOS time that can be represented
 * in a dostime_t parameter.
 *
 * At this time we use a 32 bit field (like the Zip archive) so the
 * maximum is Dec 31, 2107 23:59:59.
 *
 * \note
 * To get the corresponding Unix time, use the dos2unixtime() as in:
 *
 * \code
 *      time_t min(dos2unixtime(mindostime()));
 * \endcode
 *
 * \return The smallest possible DOS time.
 */
dostime_t mindostime()
{
    // Jan 1, 1980 00:00:00 is the minimum
    return 0x00210000;
}


/** \brief Return the maximum possible value .
 *
 * This function returns the maximum DOS time that can be represented
 * in a dostime_t parameter.
 *
 * At this time we use a 32 bit field (like the Zip archive) so the
 * maximum is Dec 31, 2107 23:59:59.
 *
 * \note
 * To get the corresponding Unix time, use the dos2unixtime() as in:
 *
 * \code
 *      time_t max(dos2unixtime(maxdostime()));
 * \endcode
 *
 * \return The largest possible DOS time.
 */
dostime_t maxdostime()
{
    // Dec 31, 2107 23:59:59 is the maximum
    return 0xFF9FBF7D;
}


/** \brief Convert a DOS time to a Unix time
 *
 * This function returns the Unix time_t value (GMT/UTC time) from
 * the DOS format (local) time dostime, where dostime is a four
 * byte value (date in most significant word, time in least
 * significant word), see dostime() function.
 *
 * If the input DOS time is invalid, then the function returns -1.
 *
 * \note
 * If the dostime is not valid (one of the parameters is out of
 * range) then the function returns -1.
 *
 * \param[in] dostime  A DOS time value as found in a zip file.
 *
 * \return The DOS time converted to a Unix time or -1.
 *
 * \sa dostime()
 */
time_t dos2unixtime(dostime_t dostime)
{
    struct tm t;         /* argument for mktime() */

    memset(&t, 0, sizeof(t));

    t.tm_isdst = -1;     /* let mktime() determine if DST is in effect */
    /* Convert DOS time to UNIX time_t format */
    t.tm_sec  = (((int)dostime <<  1) & 0x3E);
    t.tm_min  = (((int)dostime >>  5) & 0x3F);
    t.tm_hour = (((int)dostime >> 11) & 0x1F);
    t.tm_mday = (((int)dostime >> 16) & 0x1F);
    t.tm_mon  = (((int)dostime >> 21) & 0x0F) - 1;
    t.tm_year = (((int)dostime >> 25) & 0x7F) + 80;

    if(t.tm_year < 80 || t.tm_year > 207
    || t.tm_mon  <  0 || t.tm_mon  >  11
    || t.tm_mday <  1 || t.tm_mday >  31 /** \FIXME Maximum for tm_mday depends on month/year. */
    || t.tm_hour <  0 || t.tm_hour >  23
    || t.tm_min  <  0 || t.tm_min  >  59
    || t.tm_sec  <  0 || t.tm_sec  >  59)
    {
        return -1;
    }

    // A full round trip between Unix date to DOS and back to Unix works
    // as is (without worry about the current timezone) because the DOS
    // format makes use of localdate() and that's 1 to 1 compatible with
    // mktime() which expects a local date too.
    return mktime(&t);
}


/* \brief Convert a broken up date to a DOS date.
 *
 * Convert the date y/n/d and time h:m:s to a four byte DOS date and
 * time (date in high two bytes, time in low two bytes allowing magnitude
 * comparison).
 *
 * The function returns zero if the month, day, hour, minute, second are
 * out of range.
 *
 * \todo
 * The dostime_t type is 32 bits, although if it were 64 bits we would
 * never have an overflow with the maximum. However, the Zip archive
 * format only supports 32 bits as far as I know.
 *
 * \param[in] year  The year.
 * \param[in] month  The month.
 * \param[in] day  The day.
 * \param[in] hour  The hour.
 * \param[in] minute  The minute.
 * \param[in] second  The second.
 *
 * \return The date parameters transformed in a DOS time value or zero if the
 *         date is considered invalid.
 */
dostime_t dostime(int year, int month, int day, int hour, int minute, int second)
{
    if(year < 1980 || year > 2107
    || month < 1 || month > 12
    || day < 1 || day > 31
    || hour < 0 || hour > 23
    || minute < 0 || minute > 59
    || second < 0 || second > 59)
    {
        return 0;
    }

    return (((dostime_t) year - 1980) << 25)
         | (((dostime_t) month      ) << 21)
         | (((dostime_t) day        ) << 16)
         | (((dostime_t) hour       ) << 11)
         | (((dostime_t) minute     ) <<  5)
         | (((dostime_t) second     ) >>  1);  // 1 every other second
}


/** \brief Convert a Unix date to a DOS date.
 *
 * This function return the Unix time_t converted in DOS format,
 * rounded up to the next even second.
 *
 * \param[in] unix_time  A Unix time_t value.
 *
 * \return The Unix date in DOS format unless it is out of range for
 *         a DOS time and date in which case zero (0) is returned.
 */
dostime_t unix2dostime(time_t unix_time)
{
    time_t even_time;
    struct tm *s;         /* result of localtime() */

    even_time = (unix_time + 1) & ~1;         /* Round up to even seconds. */
    s = localtime(&even_time);         /* Use local time since MSDOS does. */
    return dostime(s->tm_year + 1900, s->tm_mon + 1, s->tm_mday,
                   s->tm_hour, s->tm_min, s->tm_sec);
}


// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
