
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
#include "time_utils.h"
#include "options_prefs.h"


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

gchar*
time_to_str(gint time_format) {

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

