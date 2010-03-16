
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

#ifndef _TIME_UTILS_H
#define _TIME_UTILS_H

enum {
    TIME_HH_MM = 0,         /* don't change order */
    TIME_HH_MM_AMPM,
    TIME_HH_MM_SS,
    TIME_HH_MM_SS_AMPM,
    TIME_TIMEZONE           /* last entry */
};

enum {
    TIME_24 = 0,
    TIME_12
};

typedef struct {
    gint hour;
    gint minute;
    gint second;
} TIME;

gint            get_seconds_for_today   (void);
gint            time_to_seconds         (TIME *time);
TIME*           seconds_to_time         (gint seconds);
struct tm *     get_tm_struct           (void);
gchar*          current_time_to_str     (gint time_format);
gchar*          time_to_str             (TIME *time, gint time_format);
gboolean        is_valid_time           (TIME *time);
gboolean        is_valid_time2          (gint hour, gint minute, gint second, gboolean mode24);
gint            get_current_hour        (void);
gint            get_current_minute      (void);
gint            get_current_second      (void);
gboolean        time_handler            (GUI *appGUI);

#endif /* _TIME_UTILS_H */

