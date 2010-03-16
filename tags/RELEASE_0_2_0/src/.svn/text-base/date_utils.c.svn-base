
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
#include "calendar_notes.h"
#include "options_prefs.h"
#include "time_utils.h"
#include "date_utils.h"
#include "calendar_widget.h"


/*------------------------------------------------------------------------------*/

gint
calculate_date_time_diff (DATE date_from, TIME time_from, DATE date_to, TIME time_to,
                          DATE *date_diff, TIME *time_diff)
{
gint days;

    days = (gint)(date_to_julian (date_to.day, date_to.month, date_to.year)
                - date_to_julian (date_from.day, date_from.month, date_from.year));

    if (days < 0 || (days == 0 &&
            time_from.hour * 3600 + time_from.minute * 60 + time_from.second >
            time_to.hour * 3600 + time_to.minute * 60 + time_to.second))
        return -1;

    time_diff->second = time_to.second - time_from.second;
    if (time_diff->second < 0) {
        time_to.minute--;
        time_diff->second += 60;
    }

    time_diff->minute = time_to.minute - time_from.minute;
    if (time_diff->minute < 0) {
        time_to.hour--;
        time_diff->minute += 60;
    }

    time_diff->hour = time_to.hour - time_from.hour;
    if (time_diff->hour < 0) {
        date_to.day--;
        days--;
        time_diff->hour += 24;
    }

    date_diff->day = date_to.day - date_from.day;
    if (date_diff->day < 0) {
        if (date_to.month == 0) {
            date_to.year--;
            date_to.month = 11;
        } else {
            date_to.month--;
        }
        date_diff->day += (gint)get_number_of_days_in_month (date_to.month, date_to.year);
    }

    date_diff->month = date_to.month - date_from.month;
    if (date_diff->month < 0) {
        date_to.year--;
        date_diff->month += 12;
    }

    date_diff->year = date_to.year - date_from.year;

    return days;
}

/*------------------------------------------------------------------------------*/

guint
get_current_day(void) {
    return get_tm_struct()->tm_mday;
}

/*------------------------------------------------------------------------------*/

guint
get_current_month(void) {
    return get_tm_struct()->tm_mon;
}

/*------------------------------------------------------------------------------*/

guint
get_current_year(void) {
    return (get_tm_struct()->tm_year + 1900);
}

/*------------------------------------------------------------------------------*/

guint
get_weekend_days (guint month, guint year) {

gint i, n, p;

    n = 0;

    for (i = 1; i <= get_number_of_days_in_month(month-1, year); i++) {
        p = day_of_week(year, month, i);
        if(p == 6 || p == 7) {
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
is_valid_date2(gint day, gint month, gint year) {

    if (year < JULIAN_GREGORIAN_YEAR)
        return FALSE;
    if (month < 0 || month > 11)
        return FALSE;
    if (day < 1 || day > get_number_of_days_in_month(month, year))
        return FALSE;

    return TRUE;
}

/*------------------------------------------------------------------------------*/

guint
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

guint
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

guint
get_week_of_the_year(gint day, gint month, gint year) {

    return (((get_day_of_the_year(day, month, year)-1) / 7) + 1);
}

/*------------------------------------------------------------------------------*/

guint
get_days_per_year(gint year) {

    if (is_leap_year(year) == TRUE)
        return 366;
    else
        return 365;
}

/*------------------------------------------------------------------------------*/

gboolean
is_leap_year(gint year) {

    return((((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0));
}

/*------------------------------------------------------------------------------*/
/*  This routine has been taken from http://www.voidware.com/moon_phase.htm
    calculates the moon phase (0-7), accurate to 1 segment: 0 = > new moon, 4 => full moon.
*/

guint
calc_moon_phase(gint myear, gint mmonth, gint mday) {

gint b, c, e;
gdouble jd;

    if (mmonth < 3) {
        myear--;
        mmonth += 12;
    }
    ++mmonth;
    c = 365.25 * myear;
    e = 30.6 * mmonth;
    jd = c + e + mday - 694039.09;  /* jd is total days elapsed */
    jd /= 29.53;                    /* divide by the moon cycle (29.53 days) */
    b = jd;                         /* int(jd) -> b, take integer part of jd */
    jd -= b;                        /* subtract integer part to leave fractional part of original jd */
    b = jd * 8 + 0.5;               /* scale fraction from 0-8 and round by adding 0.5 */
    b = b & 7;                      /* 0 and 8 are the same so turn 8 into 0 */

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
gchar *date_format_str[] = {
    "%d-%m-%Y", "%m-%d-%Y", "%Y-%m-%d", "%Y-%d-%m", "%e %B", "%e %B %Y"
};

    strcpy(buffer, _("No date"));

    if (g_date_valid_julian(julian_day)) {
        cdate = g_date_new_julian (julian_day);

        if (cdate != NULL) {
            if (g_date_valid (cdate)) {
                if (date_format < DATE_DD_MM_YYYY || date_format > DATE_FULL) {
                    date_format = DATE_DD_MM_YYYY;
                }
                g_date_strftime (buffer, BUFFER_SIZE-1, date_format_str[date_format], cdate);
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

guint
month_name_to_number (gchar *month_str) {

GDate *cdate;
gint i;
gboolean found;
char tmpbuf[BUFFER_SIZE];

    found = FALSE;

    cdate = g_date_new_dmy (1, 1, 1);

    if (cdate != NULL) {
        for (i = 1; i < 13; i++) {
            g_date_set_month (cdate, i);
            g_date_strftime (tmpbuf, BUFFER_SIZE-1, "%B", cdate);
            if (!strcmp(month_str, tmpbuf)) {
                found = TRUE;
                break;
            }
        }
        g_date_free(cdate);
    }

    if (found == TRUE) {
        return i;
    } else {
        return -1;
    }
}

/*------------------------------------------------------------------------------*/

void
parse_numeric_date (gchar *date_str, gint *first, gint *second, gint *third) {

gint i;
gchar *date, *token;

    date = g_strdup(date_str);

    token = strtok (date, " -");
    i = 0;
    while (token != NULL && i != 3) {
        if (i == 0) {
            *first = atoi(token);
        } else if (i == 1) {
            *second = atoi(token);
        } else if (i == 2) {
            *third = atoi(token);
        }
        token = strtok (NULL, " -");
        i++;
    }

    g_free(date);
}

/*------------------------------------------------------------------------------*/

guint32
str_to_julian(gchar *date_str, gint date_format) {

gint day, month, year, i;
gchar *token;

    day = month = year = 1;

    switch (date_format) {
        case DATE_DD_MM_YYYY:
            parse_numeric_date (date_str, &day, &month, &year);
            break;
        case DATE_MM_DD_YYYY:
            parse_numeric_date (date_str, &month, &day, &year);
            break;
        case DATE_YYYY_MM_DD:
            parse_numeric_date (date_str, &year, &month, &day);
            break;
        case DATE_YYYY_DD_MM:
            parse_numeric_date (date_str, &year, &day, &month);
            break;
        case DATE_NAME_DAY:
        case DATE_FULL:
            token = strtok (date_str, " -");
            i = 0;
            while (token != NULL && i != 3) {
                if (i == 0) {
                    day = atoi(token);
                } else if (i == 1) {
                    month = month_name_to_number(token);
                } else if (i == 2 && date_format == DATE_FULL) {
                    year = atoi(token);
                }
                token = strtok (NULL, " -");
                i++;
            }
            break;
    };

    if (g_date_valid_day (day) == TRUE && g_date_valid_month (month-1) == TRUE && g_date_valid_year (year) == TRUE) {
        return date_to_julian(day, month-1, year);
    } else {
        return 0;
    }
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

    timer = get_tm_struct();

    return date_to_julian(timer->tm_mday, timer->tm_mon, timer->tm_year + 1900);
}

/*------------------------------------------------------------------------------*/

gchar *
get_date_time_str (guint32 date, gint time) {

static gchar buffer[BUFFER_SIZE];
TIME *s_time;

    s_time = seconds_to_time (time);

    sprintf(buffer, "%s, %s", julian_to_str(date, config.date_format), time_to_str(s_time, config.time_format));

    g_free(s_time);

    return buffer;
}

/*------------------------------------------------------------------------------*/

gchar *
get_chinese_year_name (gint year) {

static gchar buffer[BUFFER_SIZE];
gint n;

gchar *animals[] = {
    N_("Rat"), N_("Ox"), N_("Tiger"), N_("Hare"), N_("Dragon"), N_("Snake"),
    N_("Horse"), N_("Sheep"), N_("Monkey"), N_("Fowl"), N_("Dog"), N_("Pig")
};

    n = (year - 3) % 12;
    if (n == 0) n = 12;
    --n;
    strncpy (buffer, gettext(animals[n]), BUFFER_SIZE-1);

    return buffer;
}

/*------------------------------------------------------------------------------*/

gchar *
get_zodiac_name (gint day, gint month) {

static gchar buffer[BUFFER_SIZE];

    strncpy (buffer, _("Unknown"), BUFFER_SIZE-1);

    if ((month == 1 && day > 20) || (month == 2 && day < 20)) {
        strncpy (buffer, _("Aquarius"), BUFFER_SIZE-1);
    }else if ((month == 2 && day > 18) || (month == 3 && day < 21)) {
        strncpy (buffer, _("Pisces"), BUFFER_SIZE-1);
    }else if ((month == 3 && day > 20) || (month == 4 && day < 21)) {
        strncpy (buffer, _("Aries"), BUFFER_SIZE-1);
    }else if ((month == 4 && day > 20) || (month == 5 && day < 22)) {
        strncpy (buffer, _("Taurus"), BUFFER_SIZE-1);
    }else if ((month == 5 && day > 21) || (month == 6 && day < 22)) {
        strncpy (buffer, _("Gemini"), BUFFER_SIZE-1);
    }else if ((month == 6 && day > 21) || (month == 7 && day < 24)) {
        strncpy (buffer, _("Cancer"), BUFFER_SIZE-1);
    }else if ((month == 7 && day > 23) || (month == 8 && day < 24)) {
        strncpy (buffer, _("Leo"), BUFFER_SIZE-1);
    }else if ((month == 8 && day > 23) || (month == 9 && day < 24)) {
        strncpy (buffer, _("Virgo"), BUFFER_SIZE-1);
    }else if ((month == 9 && day > 23) || (month == 10 && day < 24)) {
        strncpy (buffer, _("Libra"), BUFFER_SIZE-1);
    }else if ((month == 10 && day > 23) || (month == 11 && day < 23)) {
        strncpy (buffer, _("Scorpio"), BUFFER_SIZE-1);
    }else if ((month == 11 && day > 22) || (month == 12 && day < 23)) {
        strncpy (buffer, _("Sagittarius"), BUFFER_SIZE-1);
    }else if ((month == 12 && day > 22) || (month == 1 && day < 21)) {
        strncpy (buffer, _("Capricorn"), BUFFER_SIZE-1);
    }

    return buffer;
}

/*------------------------------------------------------------------------------*/

