
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

#include "gui.h"

#define TASKS_NAME                  "osmo_tasks"
#define TASKS_ENTRIES_NAME          "tasks_entries"
#define TASKS_CATEGORY_ENTRIES_NAME "category_entries"
#define TASKS_ENTRIES_FILENAME      "tasks_entries.xml"

#define MAX_SUMMARY_SIZE            512     /* max summary string size */
#define MAX_DATE_SIZE               12      /* max data string size */

void    gui_create_tasks                        (GUI *appGUI);
void    update_tasks_number                     (GUI *appGUI);
gint    get_priority_index                      (gchar *text);
gchar*  get_priority_text                       (gint index);
void    show_tasks_desc_panel                   (gboolean enable, GUI *appGUI);
void    tasks_fill_fields                       (GtkWidget *due_date_entry, GtkWidget *summary_entry, GtkWidget *desc_entry, GUI *appGUI);
void    tasks_select_first_position_in_list     (GUI *appGUI);
void    apply_task_attributes                   (GUI *appGUI);
void    refresh_due_dates                       (GUI *appGUI);
void    add_item_to_list                        (gboolean done, guint32 due_date_julian, guint32 start_date_julian, gchar *priority, gchar *category, gchar *summary, gchar *desc, GUI *appGUI);

#endif /* _TASKS_H */

