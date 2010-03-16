
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

#ifndef _CALENDAR_H
#define _CALENDAR_H

#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf/gdk-pixdata.h>

#define NOTES_NAME          "osmo_calendar_notes"
#define NOTES_FILENAME      "calendar_notes.xml"

struct note {
    guint day;
    guint month;
    guint year;
    gchar *note;
};

enum {
    PREVIOUS_DAY = 0,
    TODAY,
    NEXT_DAY,
    PREVIOUS_MONTH,
    NEXT_MONTH,
    PREVIOUS_WEEK,
    NEXT_WEEK,
    PREVIOUS_YEAR,
    NEXT_YEAR
};

void        jump_to_date                    (guint jday, guint jmonth, guint jyear);
void        set_today                       (gboolean update);
void        gui_create_calendar             (GtkWidget *notebook);
void        calendar_update_note            (guint uday, guint umonth, guint uyear);
void        calendar_clear_text_cb          (GtkWidget *widget, gpointer data);
void        calendar_create_jumpto_window   (void);
gchar *     get_mark_symbol                 (gint idx);

void        calendar_btn_prev_day           (void);
void        calendar_btn_next_day           (void);
void        calendar_btn_prev_week          (void);
void        calendar_btn_next_week          (void);
void        calendar_btn_prev_month         (void);
void        calendar_btn_next_month         (void);
void        calendar_btn_prev_year          (void);
void        calendar_btn_next_year          (void);

#endif /* _CALENDAR_H */


