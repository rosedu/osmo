
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

#include "gui.h"

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

void        gui_create_calendar                     (GtkWidget *notebook, GUI *appGUI);
void        jump_to_date                            (guint jday, guint jmonth, guint jyear, GUI *appGUI);
void        set_today                               (gboolean update, GUI *appGUI);
void        calendar_update_date                    (guint day, guint month, guint year, GUI *appGUI);
void        calendar_update_note                    (guint uday, guint umonth, guint uyear, gchar *color, GUI *appGUI);
void        calendar_clear_text_cb                  (GtkWidget *widget, gpointer data);
void        enable_disable_note_buttons             (GUI *appGUI);
gchar *     get_mark_symbol                         (gint idx);
gchar *     calendar_get_note_text                  (GUI *appGUI);
void        calendar_create_color_selector_window   (gboolean window_pos, GUI *appGUI);
void        calendar_create_popup_menu              (GtkWidget *menu, GUI *appGUI);
gboolean    check_tasks_contacts                    (guint tc_day, guint tc_month, guint tc_year, GUI *appGUI);
void        set_day_info                            (guint dday, guint dmonth, guint dyear, GUI *appGUI);
void        calendar_refresh_marks                  (GUI *appGUI);

void        calendar_btn_prev_day                   (GUI *appGUI);
void        calendar_btn_next_day                   (GUI *appGUI);
void        calendar_btn_prev_week                  (GUI *appGUI);
void        calendar_btn_next_week                  (GUI *appGUI);
void        calendar_btn_prev_month                 (GUI *appGUI);
void        calendar_btn_next_month                 (GUI *appGUI);
void        calendar_btn_prev_year                  (GUI *appGUI);
void        calendar_btn_next_year                  (GUI *appGUI);

#endif /* _CALENDAR_H */


