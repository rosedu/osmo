
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

#ifndef _CALENDAR_ICAL_H
#define _CALENDAR_ICAL_H

#include <stdio.h>
#include <stdlib.h>

#include <config.h>

#include "time_utils.h"
#include "date_utils.h"

#ifdef HAVE_ICAL

#define ICAL_NAME               "osmo_ical_files"
#define ICAL_ENTRIES_FILENAME   "ical_files.xml"

struct ics_entry {
    DATE date;
    gchar *summary;
    gchar *description;
};

struct ics_file {
    gchar *filename;
    GSList *entries_list;
};

enum {     
    I_COLUMN_DATE = 0,
    I_COLUMN_SUMMARY,
    I_COLUMN_FONT_WEIGHT,
    ICAL_EVENTS_NUM_COLUMNS
};

void        calendar_display_ics    (DATE *date, GUI *appGUI);

gboolean    ics_initialize_timezone (void);
void        ics_check_if_valid      (GUI *appGUI);
void        ics_calendar_refresh    (GUI *appGUI);

void        ical_events_browser     (GUI *appGUI);

void        read_ical_entries       (GUI *appGUI);
void        write_ical_entries      (GUI *appGUI);

#endif  /* HAVE_ICAL */

#endif /* _CALENDAR_ICAL_H */

