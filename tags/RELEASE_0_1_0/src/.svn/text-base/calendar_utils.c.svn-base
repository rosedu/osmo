
/*
 * Osmo - a handy personal organizer
 *
 * Copyright (C) 2007 Tomasz Maka <pasp@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#include <stdio.h>
#include <string.h>
#include <glib/gprintf.h>
#include <glib.h>
#include <time.h>
#include <ctype.h>

#include "i18n.h"
#include "calendar.h"
#include "gui.h"
#include "calendar_utils.h"
#include "calendar_notes.h"
#include "options_prefs.h"


extern GtkWidget        *main_window;
extern gchar            tmpbuf[];

/*------------------------------------------------------------------------------*/

gint
get_current_day(void) {

struct tm   *timer;
time_t      tmm;

    tmm = time(NULL); 
    timer = localtime(&tmm);
    return timer->tm_mday;
}

/*------------------------------------------------------------------------------*/

gint
get_current_month(void) {

struct tm   *timer;
time_t      tmm;

    tmm = time(NULL); 
    timer = localtime(&tmm);
    return timer->tm_mon;
}

/*------------------------------------------------------------------------------*/

gint
get_current_year(void) {

struct tm   *timer;
time_t      tmm;

    tmm = time(NULL); 
    timer = localtime(&tmm);
    return (timer->tm_year + 1900);
}

/*------------------------------------------------------------------------------*/

gint
get_day_of_week (guint day, guint month, guint year) {

gint mm, yy, a, b, r;

    /* convert numbers to year and month used in Zeller's algorithm */

    mm = (month + 9 + 1)% 12 + 1;
    yy = year - mm / 11;

    a = yy % 100;        /* year of century */
    b = yy / 100;        /* previous century */

    r = (a + b) / 4 + day + a - 2 * b + (13 * mm - 1) / 5;

    return (((r % 7) + 7) % 7 + 1);
}

/*------------------------------------------------------------------------------*/

gint
get_weekend_days (guint month, guint year) {

gint i, n, p;

    n = 0;

    for (i = 1; i <= get_number_of_days_in_month(month, year); i++) {
        p = get_day_of_week(i, month, year);
        if(p == 1 || p == 7) {
            n++;
        }   
    }

    return n;
}

/*------------------------------------------------------------------------------*/

gchar* 
get_date_name(gint day, gint month, gint year) {

static gchar buffer[BUFFER_SIZE];
GDate *cdate;

    cdate = g_date_new();
    g_date_set_day (cdate, day);
    g_date_set_month (cdate, month+1);
    g_date_set_year (cdate, year);

    if (g_date_valid (cdate)) {
        g_date_strftime (buffer, BUFFER_SIZE-1, "%e %B %Y", cdate);     /* e.g. 1 August 1999 */
    }

    g_date_free(cdate);
    return buffer;
}

/*------------------------------------------------------------------------------
 * day: 1- 28, 29, 30 or 31
 * month: 0-11
*/

gboolean
is_valid_date(gint day, gint month, gint year) {

    if (year < JG_YEAR)
        return FALSE;
    if (month < 0 || month > 11)
        return FALSE;
    if (day < 1 || day > get_number_of_days_in_month(month, year))
        return FALSE;

    return TRUE;
}

/*------------------------------------------------------------------------------
 * hour: 0-11 or 0-23
 * minute: 0-59
 * second: 0-59
*/

gboolean
is_valid_time(gint hour, gint minute, gint second, gboolean mode24) {

    if ((hour < 0 || hour > 23) && mode24 == TRUE)
        return FALSE;
    else if ((hour < 0 || hour > 11) && mode24 == FALSE)
        return FALSE;
    if (minute < 0 || minute > 59)
        return FALSE;
    if (second < 0 || second > 59)
        return FALSE;
    
    return TRUE;
}

/*------------------------------------------------------------------------------*/

gint
get_number_of_days_in_month(gint month, gint year) {

    gint days_in_month[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    if (month == 1 && is_leap_year(year) == TRUE) /* february in leap year ? */
        return days_in_month[month] + 1;
    else
        return days_in_month[month];
}

/*------------------------------------------------------------------------------*/

gint
get_day_of_the_year(gint day, gint month, gint year) {

gint sum, m;

    sum = 0;
    m = 0;

    while(m < month) {
        sum += get_number_of_days_in_month(m, year);
        m++;
    }
    sum += day;

    return sum;
}

/*------------------------------------------------------------------------------*/

gint
get_week_of_the_year(gint day, gint month, gint year) {

    return (((get_day_of_the_year(day, month, year)-1) / 7) + 1);
}

/*------------------------------------------------------------------------------*/

gint
get_days_per_year(gint year) {

    if (is_leap_year(year) == TRUE)
        return 366;
    else
        return 365;
}

/*------------------------------------------------------------------------------*/

gboolean
is_leap_year(gint year) {
  
    if ((year % 100) == 0) 
        return (year % 400) == 0 ? TRUE : FALSE;

    return (year % 4) == 0 ? TRUE : FALSE;
}

/*------------------------------------------------------------------------------*/
/* This routine has been taken from http://www.voidware.com/moon_phase.htm */

gint
calc_moon_phase(gint myear, gint mmonth, gint mday) {

/*
      calculates the moon phase (0-7), accurate to 1 segment.
      0 = > new moon.
      4 => full moon.
*/

gint b, c, e;
gdouble jd;

    if (mmonth < 3) {
        myear--;
        mmonth += 12;
    }
    ++mmonth;
    c = 365.25*myear;
    e = 30.6*mmonth;
    jd = c+e+mday-694039.09; /* jd is total days elapsed */
    jd /= 29.53;             /* divide by the moon cycle (29.53 days) */
    b = jd;                  /* int(jd) -> b, take integer part of jd */
    jd -= b;		         /* subtract integer part to leave fractional part of original jd */
    b = jd*8 + 0.5;	         /* scale fraction from 0-8 and round by adding 0.5 */
    b = b & 7;		         /* 0 and 8 are the same so turn 8 into 0 */

    return b;
}

/*------------------------------------------------------------------------------*/

gchar*
get_moon_phase_name(gint phase) {

const gchar *phase_names[] = {
    N_("New Moon"), N_("Waxing Crescent Moon"), N_("Quarter Moon"), N_("Waxing Gibbous Moon"),
    N_("Full Moon"), N_("Waning Gibbous Moon"), N_("Last Quarter Moon"), N_("Waning Crescent Moon")
};

    return (gchar *)gettext(phase_names[phase]);
}

/*------------------------------------------------------------------------------*/

gchar* 
julian_to_str(guint32 julian_day, gint date_format) {

static gchar buffer[BUFFER_SIZE];
GDate *cdate;
gint i;

    strcpy(buffer, _("No date"));

    if (g_date_valid_julian(julian_day)) {
        cdate = g_date_new_julian (julian_day);

        if (cdate != NULL) {

            if (g_date_valid (cdate)) {
                switch (date_format) {
                    case DATE_DD_MM_YYYY:
                        g_date_strftime (buffer, BUFFER_SIZE-1, "%d-%m-%Y", cdate);
                        break;
                    case DATE_MM_DD_YYYY:
                        g_date_strftime (buffer, BUFFER_SIZE-1, "%m-%d-%Y", cdate);
                        break;
                    case DATE_YYYY_MM_DD:
                        g_date_strftime (buffer, BUFFER_SIZE-1, "%Y-%m-%d", cdate);
                        break;
                    case DATE_YYYY_DD_MM:
                        g_date_strftime (buffer, BUFFER_SIZE-1, "%Y-%d-%m", cdate);
                        break;
                    case DATE_NAME_DAY:
                        g_date_strftime (buffer, BUFFER_SIZE-1, "%e %B", cdate);        /* e.g. 1 August */
                        break;
                    case DATE_FULL:
                        g_date_strftime (buffer, BUFFER_SIZE-1, "%e %B %Y", cdate);     /* e.g. 1 August 1999 */
                        break;
                };
            }

            g_date_free(cdate);
        }
    }

    if (buffer[0] == ' ') {
        for(i=1; buffer[i]; i++) buffer[i-1] = buffer[i];
        buffer[i-1] = '\0';
    }
    return buffer;
}

/*------------------------------------------------------------------------------*/

guint32 
date_to_julian(gint day, gint month, gint year) {

GDate *cdate;
guint32 julian;

    cdate = g_date_new();

    if (cdate != NULL) {
        g_date_set_day (cdate, day);
        g_date_set_month (cdate, month+1);
        g_date_set_year (cdate, year);

        if (g_date_valid (cdate)) {
            julian = g_date_get_julian (cdate);
            g_date_free(cdate);
            return julian;
        }

        g_date_free(cdate);
    }
    return 0;
}

/*------------------------------------------------------------------------------*/

guint32 
get_julian_for_today(void) {

struct tm   *timer;
time_t      tmm;

    tmm = time(NULL); 
    timer = localtime(&tmm);

    return date_to_julian(timer->tm_mday, timer->tm_mon, timer->tm_year + 1900);
}

/*------------------------------------------------------------------------------*/


