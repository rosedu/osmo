/*
 * Osmo - a handy personal organizer
 *
 * Copyright (C) 2007 Tomasz Maka <pasp@users.sourceforge.net>
 *           (C) 2007 Piotr Maka <silloz@users.sourceforge.net>
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

#include "utils_time.h"

/*------------------------------------------------------------------------------*/

TIME *
utl_time_new (void)
{
	TIME *time = NULL;

	time = g_new (TIME, 1);

	return time;
}

/*------------------------------------------------------------------------------*/

TIME *
utl_time_new_hms (gint hour, gint minute, gint second)
{
	TIME *time = NULL;
	g_return_val_if_fail (utl_time_valid_hms (hour, minute, second), NULL);

	time = g_new (TIME, 1);
	utl_time_set_hms (time, hour, minute, second);

	return time;
}

/*------------------------------------------------------------------------------*/

TIME *
utl_time_new_seconds (gint seconds)
{
	TIME *time = NULL;
	g_return_val_if_fail (utl_time_valid_seconds (seconds), NULL);

	time = g_new (TIME, 1);
	utl_time_set_seconds (time, seconds);

	return time;
}

/*------------------------------------------------------------------------------*/

TIME *
utl_time_new_now (void)
{
	TIME *t = NULL;
	struct tm *ptm;
	time_t tmm;

	tmm = time (NULL);
	ptm = localtime (&tmm);

	t = g_new (TIME, 1);
	utl_time_set_hms (t, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

	return t;
}

/*------------------------------------------------------------------------------*/

void
utl_time_set_hms (TIME *time, gint hour, gint minute, gint second)
{
	g_return_if_fail (time != NULL);
	g_return_if_fail (utl_time_valid_hms (hour, minute, second));

	time->hour = hour;
	time->minute = minute;
	time->second = second;
}

/*------------------------------------------------------------------------------*/

void
utl_time_set_hour (TIME *time, gint hour)
{
	g_return_if_fail (time != NULL);
	g_return_if_fail (utl_time_valid_hour (hour));

	time->hour = hour;
}

/*------------------------------------------------------------------------------*/

void
utl_time_set_minute (TIME *time, gint minute)
{
	g_return_if_fail (time != NULL);
	g_return_if_fail (utl_time_valid_minute (minute));

	time->minute = minute;
}

/*------------------------------------------------------------------------------*/

void
utl_time_set_second (TIME *time, gint second)
{
	g_return_if_fail (time != NULL);
	g_return_if_fail (utl_time_valid_second (second));

	time->second = second;
}

/*------------------------------------------------------------------------------*/

void
utl_time_set_seconds (TIME *time, gint seconds)
{
	g_return_if_fail (time != NULL);
	g_return_if_fail (utl_time_valid_seconds (seconds));

	time->hour = seconds / 3600;
	seconds %= 3600;

	time->minute = seconds / 60;
	seconds %= 60;

	time->second = seconds;
}

/*------------------------------------------------------------------------------*/

void
utl_time_get_hms (TIME *time, gint *hour, gint *minute, gint *second)
{
	g_return_if_fail (utl_time_valid (time));

	*hour = time->hour;
	*minute = time->minute;
	*second = time->second;
}

/*------------------------------------------------------------------------------*/

gint
utl_time_get_hour (TIME *time)
{
	g_return_val_if_fail (utl_time_valid (time), 0);

	return time->hour;
}

/*------------------------------------------------------------------------------*/

gint
utl_time_get_minute (TIME *time)
{
	g_return_val_if_fail (utl_time_valid (time), 0);

	return time->minute;
}

/*------------------------------------------------------------------------------*/

gint
utl_time_get_second (TIME *time)
{
	g_return_val_if_fail (utl_time_valid (time), 0);

	return time->second;
}

/*------------------------------------------------------------------------------*/

gint
utl_time_get_seconds (TIME *time)
{
	g_return_val_if_fail (utl_time_valid (time), 0);

	return time->hour * 3600 + time->minute * 60 + time->second;
}

/*------------------------------------------------------------------------------*/

gint
utl_time_add (TIME *f_time, TIME *s_time)
{
	gint days;

	g_return_val_if_fail (utl_time_valid (s_time), 0);

	days = utl_time_add_hours (f_time, s_time->hour);
	days += utl_time_add_minutes (f_time, s_time->minute);
	days += utl_time_add_seconds (f_time, s_time->second);

	return days;
}

/*------------------------------------------------------------------------------*/

gint
utl_time_add_hours (TIME *time, gint hours)
{
	gint days;

	hours += utl_time_get_hour (time);

	days = hours / 24;
	utl_time_set_hour (time, hours % 24);
	
	return days;
}

/*------------------------------------------------------------------------------*/

gint
utl_time_add_minutes (TIME *time, gint minutes)
{
	gint hours;

	minutes += utl_time_get_minute (time);

	hours = minutes / 60;
	utl_time_set_minute (time, minutes % 60);

	return (hours > 0 ? utl_time_add_hours (time, hours) : 0);
}

/*------------------------------------------------------------------------------*/

gint
utl_time_add_seconds (TIME *time, gint seconds)
{
	gint minutes;

	seconds += utl_time_get_second (time);

	minutes = seconds / 60;
	utl_time_set_second (time, seconds % 60);

	return (minutes > 0 ? utl_time_add_minutes (time, minutes) : 0);
}

/*------------------------------------------------------------------------------*/

gint
utl_time_subtract (TIME *f_time, TIME *s_time)
{
	gint days;

	g_return_val_if_fail (utl_time_valid (s_time), 0);

	days = utl_time_subtract_hours (f_time, s_time->hour);
	days += utl_time_subtract_minutes (f_time, s_time->minute);
	days += utl_time_subtract_seconds (f_time, s_time->second);

	return days;
}

/*------------------------------------------------------------------------------*/

gint
utl_time_subtract_hours (TIME *time, gint hours)
{
	gint days, h;

	days = hours / 24;
	hours %= 24;

	h = utl_time_get_hour (time);

	if (h < hours) {
		h += 24;
		days++;
	}

	utl_time_set_hour (time, h - hours);

	return days;
}

/*------------------------------------------------------------------------------*/

gint
utl_time_subtract_minutes (TIME *time, gint minutes)
{
	gint hours, m;

	hours = minutes / 60;
	minutes %= 60;

	m = utl_time_get_minute (time);

	if (m < minutes) {
		m += 60;
		hours++;
	}

	utl_time_set_minute (time, m - minutes);

	return (hours > 0 ? utl_time_subtract_hours (time, hours) : 0);
}

/*------------------------------------------------------------------------------*/

gint
utl_time_subtract_seconds (TIME *time, gint seconds)
{
	gint minutes, s;

	minutes = seconds / 60;
	seconds %= 60;

	s = utl_time_get_second (time);

	if (s < seconds) {
		s += 60;
		minutes++;
	}

	utl_time_set_second (time, s - seconds);

	return (minutes > 0 ? utl_time_subtract_minutes (time, minutes) : 0);
}

/*------------------------------------------------------------------------------*/
/*
 * If time is prior to min_time, sets time equal to min_time. If time falls after max_time,
 * sets time equal to max_time. Otherwise, time is unchanged.
 * Either of min_time and max_time may be NULL. All non-NULL times must be valid.
 * time:     a TIME to clamp
 * min_time: minimum accepted value for time
 * max_time: maximum accepted value for time
 */

void
utl_time_clamp (TIME *time, TIME *min_time, TIME *max_time)
{
	gint t, min, max;

	t = utl_time_get_seconds (time);

	if (min_time != NULL) {

		min = utl_time_get_seconds (min_time);

		if (t < min)
			utl_time_set_seconds (time, min);

	}

	if (max_time != NULL) {

		max = utl_time_get_seconds (max_time);

		if (t > max)
			utl_time_set_seconds (time, max);

	}
}

/*------------------------------------------------------------------------------*/
/*
 * Returns :
 *  0 for equal, less than zero if f_time is less than s_time,
 *  greater than zero if f_time is greater than s_time.
 */

gint
utl_time_compare (TIME *f_time, TIME *s_time)
{
	gint fs, ss;

	fs = utl_time_get_seconds (f_time);
	ss = utl_time_get_seconds (s_time);

	return (fs >= ss ? 1 : fs == ss ? 0 : -1);
}

/*------------------------------------------------------------------------------*/

gint
utl_time_seconds_between (TIME *f_time, TIME *s_time)
{
	gint fs, ss;

	fs = utl_time_get_seconds (f_time);
	ss = utl_time_get_seconds (s_time);

	return (fs > ss ? fs - ss : ss - fs);
}

/*------------------------------------------------------------------------------*/

gchar *
utl_time_print (TIME *time, gint time_format)
{
	gchar *time_format_str[] = {
	    "%R", "%I:%M %p", "%T", "%r", "%Z"
	};
	gchar *buffer = NULL;
	struct tm timer;

	buffer = g_malloc (BUFFER_SIZE);
	g_return_val_if_fail (buffer != NULL, NULL);

    g_strlcpy (buffer, "empty", BUFFER_SIZE);

    if (time_format < TIME_HH_MM || time_format > TIME_TIMEZONE) {
        time_format = TIME_HH_MM;
    }

    utl_time_get_hms (time, &(timer.tm_hour), &(timer.tm_min), &(timer.tm_sec));
    strftime (buffer, BUFFER_SIZE-1, time_format_str[time_format], &timer);

    return buffer;
}

/*------------------------------------------------------------------------------*/

gboolean
utl_time_valid (TIME *time)
{
	g_return_val_if_fail (time != NULL, FALSE);

	return (utl_time_valid_hms (time->hour, time->minute, time->second));
}

/*------------------------------------------------------------------------------*/

gboolean
utl_time_valid_hms (gint hour, gint minute, gint second)
{
	return (utl_time_valid_hour (hour) &&
	        utl_time_valid_minute (minute) &&
	        utl_time_valid_second (second));
}

/*------------------------------------------------------------------------------*/

gboolean
utl_time_valid_hour (gint hour)
{
	return (hour >= 0 && hour < 24);
}

/*------------------------------------------------------------------------------*/

gboolean
utl_time_valid_minute (gint minute)
{
	return (minute >= 0 && minute < 60);
}

/*------------------------------------------------------------------------------*/

gboolean
utl_time_valid_second (gint second)
{
	return (second >= 0 && second < 60);
}

/*------------------------------------------------------------------------------*/

gboolean
utl_time_valid_seconds (gint seconds)
{
	return (seconds >= 0 && seconds < 86400);
}

/*------------------------------------------------------------------------------*/

void
utl_time_free (TIME *time)
{
	g_return_if_fail (time != NULL);

	g_free (time);
	time = NULL;
}

/*------------------------------------------------------------------------------*/

