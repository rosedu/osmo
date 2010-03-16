
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

#ifndef _DATE_UTILS_H
#define _DATE_UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define JULIAN_GREGORIAN_YEAR   1582

enum {
    DATE_DD_MM_YYYY = 0,    /* do not change order */
    DATE_MM_DD_YYYY,
    DATE_YYYY_MM_DD,
    DATE_YYYY_DD_MM,
    DATE_NAME_DAY,
    DATE_FULL               /* last entry */
};

typedef struct {
    gint day;
    gint month;
    gint year;
} DATE;

gint        calculate_date_time_diff        (DATE date_from, TIME time_from, DATE date_to, TIME time_to, DATE *date_diff, TIME *time_diff);
guint       get_current_day                 (void);
guint       get_current_month               (void);
guint       get_current_year                (void);
guint       get_weekend_days                (guint month, guint year);
guint       get_day_of_the_year             (gint day, gint month, gint year);
guint       get_days_per_year               (gint year);
guint       get_week_of_the_year            (gint day, gint month, gint year);
guint       get_day_of_week                 (guint day, guint month, guint year);
guint       calc_moon_phase                 (gint year, gint month, gint day);
gchar*      get_moon_phase_name             (gint phase);
gchar*      month_to_name                   (gint month, gboolean short_name);
gchar*      day_to_name                     (gint day, gboolean short_name);
gchar*      current_date_text               (void);
gchar*      get_date_name                   (gint day, gint month, gint year);
gboolean    is_valid_date2                  (gint day, gint month, gint year);
guint       get_number_of_days_in_month     (gint month, gint year);
gboolean    is_leap_year                    (gint year);
gchar*      julian_to_str                   (guint32 julian_day, gint date_format);
guint32     str_to_julian                   (gchar *date_str, gint date_format);
guint32     date_to_julian                  (gint day, gint month, gint year);
guint       month_name_to_number            (gchar *month_str);
guint32     get_julian_for_today            (void);
void        parse_numeric_date              (gchar *date_str, gint *first, gint *second, gint *third);

#endif /* _DATE_UTILS_H */

