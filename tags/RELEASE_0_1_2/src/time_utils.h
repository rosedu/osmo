
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

struct tm * get_tm_struct                   (void);
gchar*      time_to_str                     (gint time_format);
gboolean    is_valid_time                   (gint hour, gint minute, gint second, gboolean mode24);
gint        get_current_hour                (void);
gint        get_current_minute              (void);
gint        get_current_second              (void);

#endif /* _TIME_UTILS_H */

