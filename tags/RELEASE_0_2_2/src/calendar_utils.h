
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

#ifndef _CALENDAR_UTILS_H
#define _CALENDAR_UTILS_H

#include "gui.h"

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

gint        get_seconds_for_today           (void);
gint        time_to_seconds                 (TIME *time);
TIME*       seconds_to_time                 (gint seconds);
struct tm * get_tm_struct                   (void);
gchar*      current_time_to_str             (gint time_format);
gchar*      time_to_str                     (TIME *time, gint time_format);
gint        get_current_hour                (void);
gint        get_current_minute              (void);
gint        get_current_second              (void);

gboolean    is_date_in_the_past             (guint32 julian, gint seconds, guint32 ac_julian, gint ac_seconds);
guint       get_days_in_month               (GDate *cdate);
gint        calculate_date_time_diff        (DATE date_from, TIME time_from, DATE date_to, TIME time_to, DATE *date_diff, TIME *time_diff);
void        sync_cal_date_with_gdate        (GUI *appGUI);
guint       get_days_in_selected_month      (GUI *appGUI);
guint       get_current_day                 (void);
guint       get_current_month               (void);
guint       get_current_year                (void);
guint       get_weekend_days                (guint month, guint year);
guint       get_days_per_year               (guint year);
guint       get_day_of_week                 (guint day, guint month, guint year);
guint       calc_moon_phase                 (gint year, gint month, gint day);
gchar*      get_day_name                    (guint day, gboolean short_name);
gchar*      get_julianday_name              (gint julian_day);
gchar*      get_moon_phase_name             (gint phase);
gchar*      current_date_text               (void);
gchar*      get_date_name                   (guint day, guint month, guint year);
gchar*      calendar_get_date_name          (GDate *cdate);
gchar*      julian_to_str                   (guint32 julian_day, gint date_format);
guint32     str_to_julian                   (gchar *date_str, gint date_format);
guint32     date_to_julian                  (guint day, guint month, guint year);
DATE*       julian_to_date                  (guint32 julian_day);
gint        julian_to_year                  (guint32 julian_day);
guint       month_name_to_number            (gchar *month_str);
guint32     get_julian_for_today            (void);
gchar*      get_current_date_distance_str   (guint32 date);
gchar*      get_date_time_str               (guint32 date, gint time);
gchar*      get_date_time_full_str          (guint32 date, gint time);
void        parse_numeric_date              (gchar *date_str, gint *first, gint *second, gint *third);
gchar*      get_zodiac_name                 (guint day, guint month);
gchar*      get_chinese_year_name           (guint year);
guint       get_gdate_current_day           (void);
guint       get_gdate_current_month         (void);
guint       get_gdate_current_year          (void);
guint32     get_gdate_current_julian        (void);

#endif /* _CALENDAR_UTILS_H */

