
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

#include "calendar_utils.h"
#include "i18n.h"
#include "gui.h"
#include "options_prefs.h"
#include "calendar.h"
#include "calendar_notes.h"
#include "calendar_widget.h"


/*------------------------------------------------------------------------------*/

gboolean
is_date_in_the_past (guint32 julian, gint seconds, guint32 ac_julian, gint ac_seconds)
{
	if (seconds <= ac_seconds || ac_seconds < 0) ac_julian++;

	return ((julian < ac_julian) ? TRUE : FALSE);
}

/*------------------------------------------------------------------------------*/

guint
get_days_in_month (GDate *cdate)
{
	return g_date_get_days_in_month (g_date_get_month (cdate), g_date_get_year (cdate));
}

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
        date_diff->day += g_date_get_days_in_month (date_to.month + 1, date_to.year);
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

void
sync_cal_date_with_gdate (GUI *appGUI)
{
	appGUI->cal->day = g_date_get_day (appGUI->cal->date);
	appGUI->cal->month = g_date_get_month (appGUI->cal->date) - 1;
	appGUI->cal->year = g_date_get_year (appGUI->cal->date);
}

/*------------------------------------------------------------------------------*/

guint
get_days_in_selected_month (GUI *appGUI)
{
	return g_date_get_days_in_month (g_date_get_month (appGUI->cal->date), g_date_get_year (appGUI->cal->date));
}

/*------------------------------------------------------------------------------*/

guint
get_current_day (void) {
    return get_tm_struct()->tm_mday;
}

/*------------------------------------------------------------------------------*/

guint
get_current_month (void) {
    return get_tm_struct()->tm_mon;
}

/*------------------------------------------------------------------------------*/

guint
get_current_year (void) {
    return (get_tm_struct()->tm_year + 1900);
}

/*------------------------------------------------------------------------------*/

guint
get_gdate_current_day (void)
{
GDate *cdate;
guint current_day;

	cdate = g_date_new ();
	g_return_val_if_fail (cdate != NULL, 0);

	g_date_set_time_t (cdate, time (NULL));
	current_day = g_date_get_day (cdate);
	g_date_free (cdate);

	return current_day;
}

/*------------------------------------------------------------------------------*/

guint
get_gdate_current_month (void)
{
GDate *cdate;
guint current_month;

	cdate = g_date_new ();
	g_return_val_if_fail (cdate != NULL, 0);

	g_date_set_time_t (cdate, time (NULL));
	current_month = g_date_get_month (cdate);
	g_date_free (cdate);

	return current_month;
}

/*------------------------------------------------------------------------------*/

guint
get_gdate_current_year (void)
{
GDate *cdate;
guint current_year;

	cdate = g_date_new ();
	g_return_val_if_fail (cdate != NULL, 0);

	g_date_set_time_t (cdate, time (NULL));
	current_year = g_date_get_year (cdate);
	g_date_free (cdate);

	return current_year;
}

/*------------------------------------------------------------------------------*/

guint32
get_gdate_current_julian (void)
{
GDate *cdate;
guint32 current_julian;

	cdate = g_date_new ();
	g_return_val_if_fail (cdate != NULL, 0);

	g_date_set_time_t (cdate, time (NULL));
	current_julian = g_date_get_julian (cdate);
	g_date_free (cdate);

	return current_julian;
}

/*------------------------------------------------------------------------------*/

gchar *
get_day_name (guint day, gboolean short_name)
{
static gchar tmpbuf[BUFFER_SIZE];
GDate *cdate;

	cdate = g_date_new_dmy (day, 1, 2007);
	g_return_val_if_fail (cdate != NULL, tmpbuf);

	g_date_strftime (tmpbuf, BUFFER_SIZE, short_name ? "%a" : "%A", cdate);
	g_date_free (cdate);

	return tmpbuf;
}

/*------------------------------------------------------------------------------*/

guint
get_weekend_days (guint month, guint year)
{
guint i, wdays, days, day;
GDate *cdate;

	cdate = g_date_new_dmy (1, month, year);
	g_return_val_if_fail (cdate != NULL, 0);

	days = g_date_get_days_in_month (month, year);
	for (wdays = 0, i = 1; i <= days; i++) {
		g_date_set_day (cdate, i);
		day = g_date_get_weekday (cdate);
		if (day == G_DATE_SATURDAY || day == G_DATE_SUNDAY) {
			wdays++;
		}
	}

    return wdays;
}

/*------------------------------------------------------------------------------*/

gchar *
get_julianday_name (gint julian_day)
{
static gchar tmpbuf[BUFFER_SIZE];
GDate *cdate;

	cdate = g_date_new_julian (julian_day);
	g_return_val_if_fail (cdate != NULL, tmpbuf);

	g_date_strftime (tmpbuf, BUFFER_SIZE, "%A", cdate);
	g_date_free (cdate);

	return tmpbuf;
}

/*------------------------------------------------------------------------------*/

gchar *
get_date_name (guint day, guint month, guint year) {

static gchar tmpbuf[BUFFER_SIZE];
GDate *cdate;

	cdate = g_date_new_dmy (day, month, year);
	g_return_val_if_fail (cdate != NULL, tmpbuf);

	g_date_strftime (tmpbuf, BUFFER_SIZE, "%e %B %Y", cdate);     /* e.g. 1 August 1999 */
	g_date_free (cdate);

	return tmpbuf;
}

/*------------------------------------------------------------------------------*/

gchar *
calendar_get_date_name (GDate *cdate)
{
static gchar tmpbuf[BUFFER_SIZE];

	g_date_strftime (tmpbuf, BUFFER_SIZE, "%e %B %Y", cdate);     /* e.g. 1 August 1999 */
	return tmpbuf;
}

/*------------------------------------------------------------------------------*/

guint
get_days_per_year (guint year)
{
	return (g_date_is_leap_year (year) ? 366 : 365);
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
julian_to_str (guint32 julian_day, gint date_format)
{
static gchar buffer[BUFFER_SIZE];
GDate *cdate;
gint i;
gchar *date_format_str[] = {
    "%d-%m-%Y", "%m-%d-%Y", "%Y-%m-%d", "%Y-%d-%m", "%e %B", "%e %B %Y"
};

	if (g_date_valid_julian (julian_day)) {
		cdate = g_date_new_julian (julian_day);
		g_return_val_if_fail (cdate != NULL, buffer);

		if (date_format < DATE_DD_MM_YYYY || date_format > DATE_FULL) {
			date_format = DATE_DD_MM_YYYY;
		}
		g_date_strftime (buffer, BUFFER_SIZE, date_format_str[date_format], cdate);
		g_date_free (cdate);

		if (buffer[0] == ' ') {
			for (i = 1; buffer[i]; i++) buffer[i-1] = buffer[i];
			buffer[i-1] = '\0';
		}
	} else {
		g_strlcpy (buffer, _("No date"), BUFFER_SIZE);
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
	g_return_val_if_fail (cdate != NULL, -1);

	for (i = G_DATE_JANUARY; i <= G_DATE_DECEMBER; i++) {
		g_date_set_month (cdate, i);
		g_date_strftime (tmpbuf, BUFFER_SIZE, "%B", cdate);
		if (!strcmp (month_str, tmpbuf)) {
			found = TRUE;
			break;
		}
	}
	g_date_free (cdate);
	return (found == TRUE ? i : -1);
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

    if (g_date_valid_dmy (day, month, year) == TRUE) {
        return date_to_julian (day, month-1, year);
    } else {
        return 0;
    }
}

/*------------------------------------------------------------------------------*/

guint32
date_to_julian (guint day, guint month, guint year)
{
GDate *cdate;
guint32 julian;

	cdate = g_date_new_dmy (day, month + 1, year);
	g_return_val_if_fail (cdate != NULL, 0);
	g_return_val_if_fail (g_date_valid (cdate), 0);

	julian = g_date_get_julian (cdate);
	g_date_free (cdate);
	return julian;
}

/*------------------------------------------------------------------------------*/

DATE*
julian_to_date (guint32 julian_day)
{
DATE *date;
GDate *cdate;
    
    date = g_new0 (DATE, 1);
    if (date == NULL) {
        return date;
    }

    if (g_date_valid_julian(julian_day)) {
        cdate = g_date_new_julian (julian_day);

        if (cdate != NULL) {
            if (g_date_valid (cdate)) {
                date->year = g_date_get_year (cdate);
                date->month = g_date_get_month (cdate);
                date->day = g_date_get_day (cdate);
            }
            g_date_free(cdate);
        }

        return date;    /* you have to free DATE struct after use! */

    } else {
        return NULL;
    }
}

/*------------------------------------------------------------------------------*/

gint
julian_to_year(guint32 julian_day) {

DATE *date;
gint year;

    date = julian_to_date (julian_day);
    year = 0;

    if (date != NULL) {
        year = date->year;
        g_free(date);
    }

    return year;
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
get_current_date_distance_str (guint32 date)
{
static gchar buffer[BUFFER_SIZE];

	gint d = date - get_julian_for_today ();
	d = (d < 0) ? -d : d;
	g_snprintf (buffer, BUFFER_SIZE, "%d", d);

	return buffer;
}

/*------------------------------------------------------------------------------*/

gchar *
get_date_time_str (guint32 date, gint time)
{
static gchar buffer[BUFFER_SIZE];
TIME *s_time;

	s_time = seconds_to_time (time);
	g_snprintf (buffer, BUFFER_SIZE, "%s, %s",
	            julian_to_str (date, config.date_format), time_to_str (s_time, config.time_format));
	g_free (s_time);

	return buffer;
}

/*------------------------------------------------------------------------------*/

gchar *
get_date_time_full_str (guint32 date, gint time)
{
static gchar buffer[BUFFER_SIZE];
TIME *s_time;

	if (time >= 0) {
		s_time = seconds_to_time (time);
		g_snprintf (buffer, BUFFER_SIZE, "%s, %s",
		            julian_to_str (date, config.date_format), time_to_str (s_time, TIME_HH_MM));
		g_free (s_time);
	} else {
		g_snprintf (buffer, BUFFER_SIZE, "%s", julian_to_str (date, config.date_format));
	}

	return buffer;
}

/*------------------------------------------------------------------------------*/

gchar *
get_chinese_year_name (guint year)
{
gchar *animals[] = {
	N_("Rat"), N_("Ox"), N_("Tiger"), N_("Hare"), N_("Dragon"), N_("Snake"),
	N_("Horse"), N_("Sheep"), N_("Monkey"), N_("Fowl"), N_("Dog"), N_("Pig")
};

static gchar buffer[BUFFER_SIZE];
gint n;

	n = (year - 3) % 12;
	if (n == 0) n = 12;
	n--;
	g_strlcpy (buffer, gettext (animals[n]), BUFFER_SIZE);

	return buffer;
}

/*------------------------------------------------------------------------------*/

gchar *
get_zodiac_name (guint day, guint month)
{
gchar *zodiac[] = {
	N_("Unknown"), N_("Aquarius"), N_("Pisces"), N_("Aries"), N_("Taurus"), N_("Gemini"), N_("Cancer"),
	N_("Leo"), N_("Virgo"), N_("Libra"), N_("Scorpio"), N_("Sagittarius"), N_("Capricorn")
};

guint dtable[13] = { 0, 20, 19, 21, 21, 22, 23, 23, 24, 23, 24, 23, 22 };

static gchar buffer[BUFFER_SIZE];
guint i;

	g_strlcpy (buffer, gettext (zodiac[0]), BUFFER_SIZE);
	g_return_val_if_fail (month > 0 && month <= 12, buffer);

	if (day >= dtable[month]) {
		i = month;
	} else {
		i = (month == 1 ? 12 : month - 1);
	}
	g_strlcpy (buffer, gettext (zodiac[i]), BUFFER_SIZE);

	return buffer;
}

/*------------------------------------------------------------------------------*/

TIME*
seconds_to_time (gint seconds)
{
TIME *time;

    if (seconds < 0) {
        return (void *) -1;
    }

    time = g_new0 (TIME, 1);
    g_return_val_if_fail (time != NULL, time);

    time->second = seconds % 60;
    seconds /= 60;

    time->minute = seconds % 60;
    seconds /= 60;

    time->hour = seconds % 24;

    return time;    /* you have to free TIME struct after use! */
}

/*------------------------------------------------------------------------------*/

gint
time_to_seconds (TIME *time)
{
    return time->hour * 3600 + time->minute * 60 + time->second;
}

/*------------------------------------------------------------------------------*/

gint
get_seconds_for_today (void) {

TIME time;
struct tm *tm_today;

    tm_today = get_tm_struct();
    time.hour = tm_today->tm_hour;
    time.minute = tm_today->tm_min;
    time.second = tm_today->tm_sec;

    return time_to_seconds (&time);
}

/*------------------------------------------------------------------------------*/

struct tm *
get_tm_struct(void) {

time_t      tmm;

    tmm = time(NULL);
    return localtime(&tmm);
}

/*------------------------------------------------------------------------------*/

gint
get_current_hour(void) {
    return get_tm_struct()->tm_hour;
}

/*------------------------------------------------------------------------------*/

gint
get_current_minute(void) {
    return get_tm_struct()->tm_min;
}

/*------------------------------------------------------------------------------*/

gint
get_current_second(void) {
    return get_tm_struct()->tm_sec;
}

/*------------------------------------------------------------------------------*/

gchar*
current_time_to_str(gint time_format) {

static gchar buffer[BUFFER_SIZE];
const struct tm *timer;

gchar *time_format_str[] = {
    "%R", "%I:%M %p", "%T", "%r", "%Z"
};

    timer = get_tm_struct();

    g_strlcpy (buffer, "empty", BUFFER_SIZE);

    if (time_format < TIME_HH_MM || time_format > TIME_TIMEZONE) {
        time_format = TIME_HH_MM;
    }

    strftime (buffer, BUFFER_SIZE-1, time_format_str[time_format], timer);
    return buffer;
}

/*------------------------------------------------------------------------------*/

gchar*
time_to_str(TIME *time, gint time_format) {

static gchar buffer[BUFFER_SIZE];
struct tm timer;

gchar *time_format_str[] = {
    "%R", "%I:%M %p", "%T", "%r", "%Z"
};

    timer.tm_hour = time->hour;
    timer.tm_min = time->minute;
    timer.tm_sec = time->second;

    g_strlcpy (buffer, "empty", BUFFER_SIZE);

    if (time_format < TIME_HH_MM || time_format > TIME_TIMEZONE) {
        time_format = TIME_HH_MM;
    }

    strftime (buffer, BUFFER_SIZE-1, time_format_str[time_format], &timer);
    return buffer;
}

/*------------------------------------------------------------------------------*/


