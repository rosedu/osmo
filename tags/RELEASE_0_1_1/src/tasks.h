
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

#ifndef _TASKS_H
#define _TASKS_H

#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf/gdk-pixdata.h>


#define TASKS_NAME                  "osmo_tasks"
#define TASKS_ENTRIES_NAME          "tasks_entries"
#define TASKS_CATEGORY_ENTRIES_NAME "category_entries"
#define TASKS_ENTRIES_FILENAME      "tasks_entries.xml"

#define MAX_SUMMARY_SIZE            512     /* max summary string size */
#define MAX_DATE_SIZE               12      /* max data string size */

enum {
    COLUMN_DONE = 0,
    COLUMN_DUE_DATE,
    COLUMN_DUE_DATE_JULIAN,
    COLUMN_START_DATE_JULIAN,
    COLUMN_PRIORITY,
    COLUMN_CATEGORY,
    COLUMN_SUMMARY,
    COLUMN_DESCRIPTION,
    COLUMN_COLOR,
    TASKS_NUM_COLUMNS
};

void    gui_create_tasks                        (GtkWidget *notebook);
void    add_item_to_list                        (gboolean done, guint32 due_date_julian, guint32 start_date_julian, gchar *priority, gchar *category, gchar *summary, gchar *desc);
void    fill_fields                             (GtkWidget *due_date_entry, GtkWidget *summary_entry, GtkWidget *desc_entry);
void    show_tasks_desc_panel                   (gboolean enable);
void    apply_date_colors                       (void);
void    create_category_combobox                (GtkComboBox *combo_box, GtkListStore *store, gboolean none);
void    tasks_select_first_position_in_list     (void);
gint    get_combobox_items                      (GtkComboBox *combo_box);
void    update_n_items                          (void);
void    refresh_due_dates                       (void);

#endif /* _TASKS_H */

