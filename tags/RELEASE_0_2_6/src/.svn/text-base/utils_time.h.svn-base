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

#ifndef _UTILS_TIME_H
#define _UTILS_TIME_H

#include "gui.h"

typedef struct {
    gint hour;
    gint minute;
    gint second;
} TIME;

enum {
    TIME_HH_MM = 0,         /* don't change order */
    TIME_HH_MM_AMPM,
    TIME_HH_MM_SS,
    TIME_HH_MM_SS_AMPM,
    TIME_TIMEZONE           /* last entry */
};

TIME* utl_time_new         (void);
TIME* utl_time_new_hms     (gint hour, gint minute, gint second);
TIME* utl_time_new_seconds (gint seconds);
TIME* utl_time_new_now     (void);

void utl_time_set_hms     (TIME *time, gint hour, gint minute, gint second);
void utl_time_set_hour    (TIME *time, gint hour);
void utl_time_set_minute  (TIME *time, gint minute);
void utl_time_set_second  (TIME *time, gint second);
void utl_time_set_seconds (TIME *time, gint seconds);

void utl_time_get_hms     (TIME *time, gint *hour, gint *minute, gint *second);
gint utl_time_get_hour    (TIME *time);
gint utl_time_get_minute  (TIME *time);
gint utl_time_get_second  (TIME *time);
gint utl_time_get_seconds (TIME *time);

gint utl_time_add         (TIME *f_time, TIME *s_time);
gint utl_time_add_hours   (TIME *time, gint hours);
gint utl_time_add_minutes (TIME *time, gint minutes);
gint utl_time_add_seconds (TIME *time, gint seconds);

gint utl_time_subtract         (TIME *f_time, TIME *s_time);
gint utl_time_subtract_hours   (TIME *time, gint hours);
gint utl_time_subtract_minutes (TIME *time, gint minutes);
gint utl_time_subtract_seconds (TIME *time, gint seconds);

void utl_time_clamp           (TIME *time, TIME *min_time, TIME *max_time);
gint utl_time_compare         (TIME *f_time, TIME *s_time);
gint utl_time_seconds_between (TIME *f_time, TIME *s_time);

gchar* utl_time_print (TIME *time, gint time_format);

gboolean utl_time_valid         (TIME *time);
gboolean utl_time_valid_hms     (gint hour, gint minute, gint second);
gboolean utl_time_valid_hour    (gint hour);
gboolean utl_time_valid_minute  (gint minute);
gboolean utl_time_valid_second  (gint second);
gboolean utl_time_valid_seconds (gint seconds);

void utl_time_free (TIME *time);

#endif /* _UTILS_TIME_H */

