
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
#include "time_utils.h"
#include "date_utils.h"
#include "options_prefs.h"


/*------------------------------------------------------------------------------*/

TIME*
seconds_to_time (gint seconds)
{
TIME *time;

    if (seconds < 0) {
        return (void *) -1;
    }

    time = g_new0 (TIME, 1);
    if (time == NULL) {
        return time;
    }

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

gboolean
is_valid_time (TIME *time) {

    if (time->hour < 0 || time->hour > 23)
        return FALSE;

    if (time->minute < 0 || time->minute > 59)
        return FALSE;

    if (time->second < 0 || time->second > 59)
        return FALSE;

    return TRUE;
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

/*------------------------------------------------------------------------------
 * hour: 0-11 or 0-23
 * minute: 0-59
 * second: 0-59
*/

gboolean
is_valid_time2(gint hour, gint minute, gint second, gboolean mode24) {

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

gchar*
current_time_to_str(gint time_format) {

static gchar buffer[BUFFER_SIZE];
const struct tm *timer;

gchar *time_format_str[] = {
    "%R", "%I:%M %p", "%T", "%r", "%Z"
};

    timer = get_tm_struct();

    strcpy(buffer, "empty");

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

    strcpy(buffer, "empty");

    if (time_format < TIME_HH_MM || time_format > TIME_TIMEZONE) {
        time_format = TIME_HH_MM;
    }
    
    strftime (buffer, BUFFER_SIZE-1, time_format_str[time_format], &timer);
    return buffer;
}

/*------------------------------------------------------------------------------*/

gboolean 
time_handler (GUI *appGUI) {

gchar tmpbuf[BUFFER_SIZE];


    /* display current time in calendar day info */

    if (config.time_format == TIME_24) {
        sprintf(tmpbuf, "<tt>%s</tt>", current_time_to_str(TIME_HH_MM_SS));
    } else {
        sprintf(tmpbuf, "<tt>%s</tt>", current_time_to_str(TIME_HH_MM_SS_AMPM));
    }
    gtk_label_set_markup (GTK_LABEL (appGUI->cal->time_label), tmpbuf);

	return TRUE;
}

/*------------------------------------------------------------------------------*/

